# src
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/app SRC_APP)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRCS)

# this project
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${MPI_C_INCLUDE_PATH}
    ${PETSC_INCLUDE_DIR} # PETSc 头文件路径
    ${SLEPC_INCLUDE_DIR} # SLEPc 头文件路径
)
# 设置库文件搜索路径
link_directories(
    ${PETSC_LIB_DIR}
    ${SLEPC_LIB_DIR}
)

# shared library
add_library(GCGE STATIC
    ${SRC_APP}
    ${SRCS}
)

# 查找 OpenMP 包
find_package(OpenMP REQUIRED)

# 如果找到 OpenMP，将其链接到目标
if(OpenMP_FOUND)
    message(STATUS "OpenMP found, enabling support.")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")  # 添加 OpenMP 编译选项
endif()

find_package(OpenBLAS REQUIRED)

target_link_libraries(GCGE PRIVATE 
    "${SLEPC_DIR}/lib/libslepc.a"     # SLEPC需要放在PETSC之前
    "${PETSC_DIR}/lib/libpetsc.a"     # PETSC # todo: 这样写存在链接错误，为什么
    dmumps              # petsc支持mumps，新增的库
    mumps_common
    pord
    scalapack
    gfortran            # GNU Fortran 运行时库
    stdc++              # C++ 标准库
    quadmath            # 高精度数学库（Fortran 相关）
    ${MPI_C_LIBRARIES}
    MPI::MPI_C          # MPI C 库， # todo: 采用${MPI_C_LIBRARIES}会失败，为什么
    OpenBLAS::OpenBLAS  # OpenBLAS
    OpenMP::OpenMP_C    # OpenMP C 库, todo: 是否要包含OpenMP::OpenMP_CXX
)

if(UNIX)
    target_link_libraries(GCGE PRIVATE 
        m                   # petsc需要的库
        #mpifort            # 新编译mpich --enable-fortran=all FC=gfortran
        #mpi
        gcc_s
    )
endif()

if(WIN32)
    target_link_libraries(GCGE PRIVATE 
        regex               # 正则表达式库
        gdi32               # Windows GDI（图形设备接口）
        user32              # Windows 用户界面库
        advapi32            # Windows 高级 API
        kernel32            # Windows 核心 API
    )
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/shared/lib)
set(INCLUDE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/shared/include)
file(GLOB_RECURSE HEADER_ALL
    ${PROJECT_SOURCE_DIR}/include/*.h
)
file(COPY ${HEADER_ALL} DESTINATION ${INCLUDE_OUTPUT_PATH})
