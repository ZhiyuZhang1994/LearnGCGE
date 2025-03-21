#include "mmloader.h"
PetscErrorCode MatCreateFromMTX(Mat *A, const char *filein, PetscBool aijonly) {
    MM_typecode matcode;
    FILE *file;
    // M: 行数, N: 列数
    PetscInt M, N, ninput;
    // ia: 行索引数组, ja: 列索引数组, val: 非零元素数组
    PetscInt *ia, *ja;
    // nz: 非零元素数, ownz: 每行的非零元素数
    PetscInt i, j, nz, *rownz;
    // val: 非零元素数组
    PetscScalar *val;
    PetscBool sametype, symmetric = PETSC_FALSE, skew = PETSC_FALSE;

    /* Read in matrix */
    PetscFunctionBeginUser;
    PetscCall(PetscFOpen(PETSC_COMM_SELF, filein, "r", &file));
    PetscCheck(mm_read_banner(file, &matcode) == 0, PETSC_COMM_SELF, PETSC_ERR_ARG_WRONG, "Could not process Matrix Market banner.");
    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */
    PetscCheck(mm_is_matrix(matcode) && mm_is_sparse(matcode) && (mm_is_real(matcode) || mm_is_integer(matcode)), PETSC_COMM_SELF, PETSC_ERR_ARG_WRONG, "Input must be a sparse real or integer matrix. Market Market type: [%s]", mm_typecode_to_str(matcode));

    if (mm_is_symmetric(matcode)) symmetric = PETSC_TRUE;
    if (mm_is_skew(matcode)) skew = PETSC_TRUE;

    /* Find out size of sparse matrix .... */
    PetscCheck(mm_read_mtx_crd_size(file, &M, &N, &nz) == 0, PETSC_COMM_SELF, PETSC_ERR_ARG_WRONG, "Size of sparse matrix is wrong.");

    /* Reserve memory for matrices */
    PetscCall(PetscMalloc4(nz, &ia, nz, &ja, nz, &val, M, &rownz));
    for (i = 0; i < M; i++) rownz[i] = 0;

    /* NOTE: when reading in doubles, ANSI C requires the use of the "l"  */
    /*   specifier as in "%lg", "%lf", "%le", otherwise errors will occur */
    /*  (ANSI C X3.159-1989, Sec. 4.9.6.2, p. 136 lines 13-15)            */
    for (i = 0; i < nz; i++) {
        ninput = fscanf(file, "%d %d %lg\n", &ia[i], &ja[i], &val[i]);
        PetscCheck(ninput >= 3, PETSC_COMM_SELF, PETSC_ERR_FILE_UNEXPECTED, "Badly formatted input file");
        ia[i]--;
        ja[i]--;                              /* adjust from 1-based to 0-based */
        if ((symmetric && aijonly) || skew) { /* transpose */
            rownz[ia[i]]++;
            if (ja[i] != ia[i]) rownz[ja[i]]++;
        } else {
            if (symmetric)
                rownz[ja[i]]++;
            else
                rownz[ia[i]]++;
        }
    }
    PetscCall(PetscFClose(PETSC_COMM_SELF, file));

    /* Create, preallocate, and then assemble the matrix */
    PetscCall(MatCreate(PETSC_COMM_SELF, A));
    PetscCall(MatSetSizes(*A, PETSC_DECIDE, PETSC_DECIDE, M, N));

    if (symmetric && !aijonly) {
        // MATSEQSBAIJ或MATMPISBAIJ 设置后，只需要填充将上三角矩阵，petsc自动补齐下三角矩阵
        PetscCall(MatSetType(*A, MATSEQSBAIJ));
        PetscCall(MatSetFromOptions(*A));
        /**
         * @brief 为 MPI 对称块对角稀疏矩阵（MPI Symmetric Block AIJ Matrix） 预分配内存
         * @param Mat 要预分配内存的矩阵对象
         * @param bs 块大小（Block Size），即矩阵的每个块的行数和列数。
         * @param PetscInt 对角块每行的非零块数的估计值。如果不知道具体值，可以传入 PETSC_DEFAULT
         * @param const PetscInt[] 对角块每行的非零块数的精确数组。如果不知道具体值，可以传入 NULL。
         * @param PetscInt 非对角块每行的非零块数的估计值。如果不知道具体值，可以传入 PETSC_DEFAULT。
         * @param const PetscInt[] 非对角块每行的非零块数的精确数组。如果不知道具体值，可以传入 NULL。
         * 
         */
        // PetscCall(MatMPISBAIJSetPreallocation (*A, 1, PETSC_DEFAULT, NULL, PETSC_DEFAULT, NULL));
        PetscCall(MatSeqSBAIJSetPreallocation(*A, 1, 0, rownz));
        PetscCall(PetscObjectTypeCompare((PetscObject)*A, MATSEQSBAIJ, &sametype));
        PetscCheck(sametype, PETSC_COMM_SELF, PETSC_ERR_ARG_WRONG, "Only AIJ and SBAIJ are supported. Your mattype is not supported");
    }
    /* Add values to upper triangular part for some cases */
    if (symmetric && !aijonly) {
        printf("zzy2 symmetric: %d, aijonly: %d\n", symmetric, aijonly);
        /* MatrixMarket matrix stores symm matrix in lower triangular part. Take its transpose */
        for (j = 0; j < nz; j++) PetscCall(MatSetValues(*A, 1, &ja[j], 1, &ia[j], &val[j], INSERT_VALUES));
    }
    PetscCall(MatAssemblyBegin(*A, MAT_FINAL_ASSEMBLY));
    PetscCall(MatAssemblyEnd(*A, MAT_FINAL_ASSEMBLY));
    PetscCall(PetscFree4(ia, ja, val, rownz));
    PetscFunctionReturn(PETSC_SUCCESS);
}
