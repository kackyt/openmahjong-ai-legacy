/*
 * MJ0.h
 * とつげき東北氏のMJ-0 アルゴリズムを実装します(on まうじゃんIF)
 * アルゴリズムの著作権はとつげき東北氏が所有します
 * まうじゃんの著作権は石畑恭平氏が所有します
 * プログラムの著作権はkackyが所有します
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int UINT;
typedef unsigned short USHORT;
#include "MIPIface.h"

#define SIMU_SIZE (5000)

typedef struct{
	MJITehai1 *pTehai;
	MJIKawahai *pKawahai;
	int kawalength;
} MJ0PARAM;

extern int MJ0(/* inputs */
		MJ0PARAM *pParam,int *pDora,int dlength,
		/* outputs */
		double *pNokoriHai,double *pKikenhai,
		double *pMentsuSimo,double *pMentsuToimen,double *pMentsuKami);

#ifdef __cplusplus
}
#endif
