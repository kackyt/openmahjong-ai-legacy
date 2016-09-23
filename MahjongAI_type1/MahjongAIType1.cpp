/****************************************************************************************
 * Copyright (c) 2010-2012, Takaya Kakizaki(kacky)
 * All rights reserved.

  ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満たす場合に限り、再頒布および使用が許可されます。 

  ・ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売促進に、オープン麻雀の名前またはコントリビューターの名前を使用してはならない。


  本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、明示黙示を問わず、
  商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証もありません。
  著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか
  （過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した
  （代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）
  直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。 

****************************************************************************************/
#include <math.h>
#include <stdlib.h>
#include <process.h>
#include "MahjongScoreAI.h"
#include "AILib.h"

/* 牌距離毎の期待値の補正係数 */
static double dist_coef[] = {
    0.88,
    0.90,
    0.99,
    0.98,
    0.94,
    0.97,
    0.93,
    0.93,
    0.93,
    0.93
};

/* 牌の種類ごとの期待値の補正係数 */
static double kind_coef[] = {
    0.97,
    0.98,
    0.99,
    0.96,
    0.95
};

static double getKindCoef(MahjongAIState *state, unsigned num){
    num &= 0x3F;

    if (num >= 31){
        return kind_coef[3];
    }
    else if (num >= 27){
        if (state->cha == num - 27 || state->kaze == num - 27){
            return kind_coef[3];
        }
        else{
            return kind_coef[4];
        }
    }
    else{
        switch (num % 9) {
        case 0:
        case 8:
            return kind_coef[0];
        case 1:
        case 7:
            return kind_coef[1];
        default:
            return kind_coef[2];
        }
    }
}



static int compare_hai(const void *a, const void *b){
	return (*(int*)a & 0xFF) - (*(int*)b & 0xFF);
}


#if 0
static int getPoint(AGARI_LIST *pList,void *ptr)
{
	return 0;
}
#else
static int getPoint(AGARI_LIST *pList,void *ptr)
{
	MJITehai resulthai;
	int val,i,j;
	int isChiitoi = 1;
	MahjongAIState *obj=(MahjongAIState*)ptr;
	double coef = 1.0;
	int remain = MahjongScoreAI::MJSendMessage(MJMI_GETHAIREMAIN,0,0)/4;

	obj->myself.toTehai(&resulthai);
	
	for(i=0;i<pList->tehai_max;i++){
		//if(remain < pList->tehai[i] >> 8) return 0;
        if (pList->tehai[i] >> 8){
            int dist = paidistance(obj->myself._tehai, pList->tehai[i]);
            coef *= dist_coef[dist + 1];
            coef *= getKindCoef(obj,pList->tehai[i]);
        }
		pList->tehai[i] = pList->tehai[i] & 0xFF;
	}
	memcpy(&resulthai.tehai,pList->tehai,sizeof(int)*pList->tehai_max);
	resulthai.tehai_max = pList->tehai_max-1;
	//qsort(&resulthai.tehai,pList->tehai_max,sizeof(int),(int (*)(const void*, const void*))compare_int);
	val = MahjongScoreAI::MJSendMessage(MJMI_GETAGARITEN,(UINT)&resulthai,(UINT)resulthai.tehai[pList->tehai_max-1]);

    return val * coef;

}

#endif

typedef struct {
    MahjongAIState *pState;
    int hai;
    int remain;
    double ret;
} THREAD_PARAM;



// 手牌を評価して評価値を返す
static unsigned __stdcall evalSutehaiSubSub(void * param)
{
    THREAD_PARAM *prm = (THREAD_PARAM *)param;
    int simtehai[34];
    int remain = prm->remain;
    int i, j, k, res, painum, maxpts, pts, debug = 0;
    double tmp, tmp2, ret, index;
    AGARI_LIST list;
    MJITehai resulthai;
    int paicount;
    double nokorihais;
    double nokoribuf[34];

	prm->pState->myself.toTehai(&resulthai);

    double value = 0;

    if (remain / 4 == 0){
        paicount = 1;
    }
    else{
        /* 8枚以上はツモらない */
        paicount = remain / 4 > 8 ? 8 : remain / 4;
    }

    //if(paicount < 5){
    //	paicount = 5;
    //}

    for (i = 0; i<SIMULATECOUNT; i++){
        tmp = 0.0;
        for (j = 0; j<34; j++){
        }
		for (j = 0; j<34; j++) {
			tmp += prm->pState->myself._pai_kukan[j];
			nokoribuf[j] = prm->pState->myself._pai_kukan[j];
		}
		nokorihais = tmp;
		for (j = 0; j < prm->pState->myself._tehai.size(); j++) {
			simtehai[j] = prm->pState->myself._tehai[j].getNum();
		}

        for (j = 0; j<paicount; j++){
            index = (nokorihais + 1.0) * (double)rand() / (double)(1.0 + RAND_MAX);
            tmp = 0.0;
            for (k = 0; k<34; k++){
                tmp += nokoribuf[k];
                if (index < tmp){
                    simtehai[prm->pState->myself._tehai.size() + j] = 0x100 + k;
                    tmp2 = nokoribuf[k] > 1.0 ? 1.0 : nokoribuf[k];
                    nokoribuf[k] -= tmp2;
                    nokorihais -= tmp2;
                    break;
                }
            }
        }

        qsort(simtehai, prm->pState->myself._tehai.size() + paicount, sizeof(int), compare_hai);

        maxpts = search_agari(simtehai, prm->pState->myself._tehai.size() + paicount, &list, prm->pState->myself._tehai.size() + 1, prm->pState, getPoint);
        value += (double)maxpts * 100 / SIMULATECOUNT;
        if (maxpts > 0){
            if (list.tehai[0] == 0 && list.tehai[1] == 1 && list.tehai[2] == 2){
                debug++;
            }
        }

        /* 高速化のための工夫 */
        //if(i>SIMULATECOUNT/10 && 10.0 * value * SIMULATECOUNT / i < max_val) break;

    }

    prm->ret = value;
    //fprintf(fp,"%d",debug);
	_endthreadex(0);

    return 0;
}



#define THREADNUM (5)

double MahjongAIType1::evalSutehaiSub(MahjongAIState &param, int hai)
{
    double sum = 0.0;
    THREAD_PARAM tparam[THREADNUM];
    HANDLE hThread[THREADNUM];
    int i;
    int remain = MJSendMessage(MJMI_GETHAIREMAIN, 0, 0);
    memset(hThread, 0, sizeof(hThread));

    for (i = 0; i<THREADNUM; i++){
        tparam[i].pState = &param;
        tparam[i].hai = hai;
        tparam[i].ret = 0.0;
        tparam[i].remain = remain;
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, evalSutehaiSubSub, &tparam[i], 0, NULL);
    }

    WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);

    for (i = 0; i<THREADNUM; i++){
        sum += tparam[i].ret;
    }


    return sum;

}



