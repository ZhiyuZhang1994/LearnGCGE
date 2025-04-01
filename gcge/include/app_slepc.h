/**
 *    @file  app_slepc.h
 *   @brief  app of slepc 
 *
 *  slepc的操作接口, 针对 BV
 *
 *  @author  Yu Li, liyu@tjufe.edu.cn
 *
 *       Created:  2020/9/13
 *      Revision:  none
 */
#ifndef _APP_SLEPC_H_
#define _APP_SLEPC_H_

#include "app_lapack.h"
#include "ops.h"

#if OPS_USE_SLEPC

#include <petscksp.h>
#include <petscmat.h>
#include <petscoptions.h>
#include <petscpc.h>
#include <petscsys.h>
#include <petscvec.h>
#include <petscviewer.h>
#include <slepcbv.h>

void OPS_SLEPC_Set(struct OPS_ *ops);
void Testable_MultiVecCreateByMat(BV *des_bv, int num_vec, Mat src_mat, struct OPS_ *ops);

#endif

#endif /* -- #ifndef _APP_SLEPC_H_ -- */
