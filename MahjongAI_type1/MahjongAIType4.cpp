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
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <process.h>
#include "MahjongScoreAI.h"
#include "AILib.h"

/* 牌距離毎の期待値の補正係数 */
static double dist_coef[] = {
	0.1,
	0.5,
	0.9,
	0.8,
	0.2,
	0.6,
	0.5,
	0.5,
	0.5,
	0.5
};


double probability(MahjongAIState &param,unsigned long count,double nokorisum)
{
	int cnt[34];
	int i,j;
	unsigned long id;
	double p = 1.0;

	memset(cnt,0,sizeof(cnt));

	/*
	for(i=0;i<34;i++){
		nokorisum += param.nokori[i];
	}*/

	for(i=0;i<4;i++){
		id = count % 55;

		if(id < 7){
			cnt[id]++;
			cnt[id+1]++;
			cnt[id+2]++;
		}else if(id < 14){
			cnt[id+2]++;
			cnt[id+3]++;
			cnt[id+4]++;
		}else if(id < 21){
			cnt[id+4]++;
			cnt[id+5]++;
			cnt[id+6]++;
		}else if(id < 55){
			cnt[id-21]+= 3;
		}else{
			return 0.0;
		}

		count /= 55;
	}

	id = count % 34;
	
	if(id < 34){
		cnt[id] += 2;
	}else{
		return 0.0;
	}


	for(i=0;i<34;i++){
		if(cnt[i]){
			cnt[i] -= param.te_cnt[i];
			
			if(cnt[i] > param.nokori[i]){
				return 0;
			}
			
			for(j=0;j<cnt[i];j++){
				p *= (double)(param.nokori[i] - j) / (double) nokorisum;
				nokorisum-=1.0;
			}
		}
	}

	return p;

}

double score(MahjongAIState &param,unsigned long count)
{
	int i;
	RESULT_ITEM item;
	GAMESTATE state;
	unsigned long id;

	memset(&item,0x0,sizeof(item));
	memset(&state,0x0,sizeof(state));

	state.tsumo = 1;
	state.bakaze = param.kyoku / 4;
	state.zikaze = param.cha;

	item.mentsusize = 5;
	item.menzen = 1;

	for(i=0;i<4;i++){
		id = count % 55;

		if(id < 7){
			item.mentsulist[i].category = AI_SYUNTSU;
			item.mentsulist[i].pailist[0] = id;
			item.mentsulist[i].pailist[1] = id + 1;
			item.mentsulist[i].pailist[2] = id + 2;
		}else if(id < 14){
			item.mentsulist[i].category = AI_SYUNTSU;
			item.mentsulist[i].pailist[0] = id + 2;
			item.mentsulist[i].pailist[1] = id + 3;
			item.mentsulist[i].pailist[2] = id + 4;
		}else if(id < 21){
			item.mentsulist[i].category = AI_SYUNTSU;
			item.mentsulist[i].pailist[0] = id + 4;
			item.mentsulist[i].pailist[1] = id + 5;
			item.mentsulist[i].pailist[2] = id + 6;
		}else if(id < 55){
			item.mentsulist[i].category = AI_KOUTSU;
			item.mentsulist[i].pailist[0] = id-21;
			item.mentsulist[i].pailist[1] = id-21;
			item.mentsulist[i].pailist[2] = id-21;
		}

		count /= 55;
	}

	id = count % 34;
	item.mentsulist[4].category = AI_TOITSU;
	item.mentsulist[4].pailist[0] = id;
	item.mentsulist[4].pailist[1] = id;

	item.machi = item.mentsulist[0].category == AI_SYUNTSU ? AI_MACHI_RYANMEN : AI_MACHI_SHANPON;
	item.machihai = item.mentsulist[0].pailist[0];
	item.machipos = 0;

	make_resultitem_bh(&item,&state);
	

	return (double)item.score;
}

typedef struct {
	MahjongAIState *pState;
	int hai;
	double ret;
} THREAD_PARAM;

static UINT thread_count[5];
static UINT threadorder;


/* 4つの刻子+頭 */
static DWORD WINAPI threadfunc(LPVOID pParam)
{
	int i,j,k,l,m;
	int count;
	double sum = 0.0;
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	double nokorisum = 0.0;
	double rest = 0.0;
	double probability = 1.0;

	for(count=0;count<34;count++){
		nokorisum += prm->pState->nokori[count];
	}

	for(i=0;i<34;i++){

		if(prm->pState->nokori[i] > 3 - prm->pState->te_cnt[i]){

			for(j=i+1;j<34;j++){

				if(prm->pState->nokori[j] > 3 - prm->pState->te_cnt[j]){

					for(k=j+1;k<34;k++){

						if(prm->pState->nokori[k] > 3 - prm->pState->te_cnt[k]){

							for(l=k+1;l<34;l++){

								if(prm->pState->nokori[l] > 3 - prm->pState->te_cnt[l]){


									for(m=0;m<34;m++){

										if(i != m && j != m && k != m && l != m && prm->pState->nokori[m] > 1 - prm->pState->te_cnt[m]){
                                            int dist;
											probability = 1.0;
											rest = nokorisum;
                                            dist = paidistance(prm->pState->tehai.tehai, i);
											for(count = 0;count < 3 - prm->pState->te_cnt[i];count++){
												probability *= (double)(prm->pState->nokori[i] - count) / (double) rest;
                                                probability *= dist_coef[dist + 1];
												rest -= 1.0;
											}
                                            dist = paidistance(prm->pState->tehai.tehai, j);
                                            for (count = 0; count < 3 - prm->pState->te_cnt[j]; count++){
												probability *= (double)(prm->pState->nokori[j] - count) / (double) rest;
                                                probability *= dist_coef[dist + 1];
                                                rest -= 1.0;
											}
                                            dist = paidistance(prm->pState->tehai.tehai, k);
                                            for (count = 0; count < 3 - prm->pState->te_cnt[k]; count++){
												probability *= (double)(prm->pState->nokori[k] - count) / (double) rest;
                                                probability *= dist_coef[dist + 1];
                                                rest -= 1.0;
											}
                                            dist = paidistance(prm->pState->tehai.tehai, l);
                                            for (count = 0; count < 3 - prm->pState->te_cnt[l]; count++){
												probability *= (double)(prm->pState->nokori[l] - count) / (double) rest;
                                                probability *= dist_coef[dist + 1];
                                                rest -= 1.0;
											}
                                            dist = paidistance(prm->pState->tehai.tehai, m);
                                            for (count = 0; count < 2 - prm->pState->te_cnt[m]; count++){
												probability *= (double)(prm->pState->nokori[m] - count) / (double) rest;
                                                probability *= dist_coef[dist + 1];
                                                rest -= 1.0;
											}

											/* 四暗刻は役満 */
											sum += probability * 32000;
											//thread_count[0]++;

											if(m < 9){
												m = 9;
											}else if(m < 18){
												m = 18;
											}else if(m < 27){
												m = 27;
											}else{
												break;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	thread_count[0] = threadorder++;

	prm->ret = sum;

	return 0;
}

/* 3つの刻子+1つの順子+頭 */
static DWORD WINAPI threadfunc2(LPVOID pParam)
{
	int i,j,k,l,m,n,o;
	int st;
	int count;
	double sum = 0.0;
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	double nokorisum = 0.0;
	double rest = 0.0;
	double probability = 1.0;
	RESULT_ITEM item;
	GAMESTATE state;
	int cnt[34];
	int toitsu[4] = { 0 , 0 , 0 , 0 };
	int maxc;
	int c;
	//return 0;

	for(count=0;count<34;count++){
		nokorisum += prm->pState->nokori[count];
	}

	memset(&state,0,sizeof(state));
	memset(&item,0,sizeof(item));

	state.tsumo = 1;
	state.bakaze = prm->pState->kyoku / 4;
	state.zikaze =prm->pState->cha;
	state.count = 1;



	for(i=0;i<34;i++){

		if(prm->pState->nokori[i] > 3 - prm->pState->te_cnt[i]){

			for(j=i+1;j<34;j++){

				if(prm->pState->nokori[j] > 3 - prm->pState->te_cnt[j]){

					for(k=j+1;k<34;k++){

						if(prm->pState->nokori[k] > 3 - prm->pState->te_cnt[k]){

							for(l=0;l<21;l++){
								memset(cnt,0,sizeof(cnt));
								cnt[i] = 3;
								cnt[j] = 3;
								cnt[k] = 3;

								if(l<7){
									cnt[l]++;
									cnt[l+1]++;
									cnt[l+2]++;
									st = l;

								}else if(l<14){
									cnt[l+2]++;
									cnt[l+3]++;
									cnt[l+4]++;
									st = l+2;

								}else{
									cnt[l+4]++;
									cnt[l+5]++;
									cnt[l+6]++;
									st = l+4;

								}

								if(prm->pState->nokori[st] > cnt[st] - prm->pState->te_cnt[st] - 1
								&& prm->pState->nokori[st+1] > cnt[st+1] - prm->pState->te_cnt[st+1] - 1
								&& prm->pState->nokori[st+2] > cnt[st+2] - prm->pState->te_cnt[st+2] - 1){
									toitsu[0] = -1;
									toitsu[1] = -1;
									toitsu[2] = -1;
									toitsu[3] = -1;

									maxc = -4;
									for(m=0;m<9;m++){
										c = prm->pState->te_cnt[m] - cnt[m];
										if(maxc < c && prm->pState->nokori[m] > 1 - c) {
											maxc = c;
											toitsu[0] = m;
										}
									}

									maxc = -4;

									for(m=9;m<18;m++){
										c = prm->pState->te_cnt[m] - cnt[m];
										if(maxc < c && prm->pState->nokori[m] > 1 - c) {
											maxc = c;
											toitsu[1] = m;
										}
									}

									maxc = -4;
									for(m=18;m<27;m++){
										c = prm->pState->te_cnt[m] - cnt[m];
										if(maxc < c && prm->pState->nokori[m] > 1 - c) {
											maxc = c;
											toitsu[2] = m;
										}
									}
									maxc = -4;

									for(m=27;m<34;m++){
										c = prm->pState->te_cnt[m] - cnt[m];
										if(maxc < c && prm->pState->nokori[m] > 1 - c) {
											maxc = c;
											toitsu[3] = m;
										}
									}


									for(m=0;m<4;m++){
										if(toitsu[m] < 0 ) continue;
										probability = 1.0;
										rest = nokorisum;
										cnt[toitsu[m]]+= 2;
										for(n=0;n<34;n++){
											if(cnt[n]){
												c = cnt[n] - prm->pState->te_cnt[n];
												assert(prm->pState->nokori[n] - c + 1 >= 0);
                                                int dist = paidistance(prm->pState->tehai.tehai, n);

												for(o=0;o<c;o++){
													probability *= (double)(prm->pState->nokori[n] - o) / (double) rest;
                                                    probability *= dist_coef[dist + 1];

													if(prm->pState->nokori[n] - o < 0){
                                                        probability = 0;
                                                        break;
													}
													rest-=1.0;
												}
											}
										}
										cnt[toitsu[m]]-= 2;

										item.mentsusize = 5;
										item.menzen = 1;
										item.machipos = 0;
										item.han = 0;
										item.fu = 0;
										item.mentsulist[0].category = AI_KOUTSU;
										item.mentsulist[0].pailist[0] = i;
										item.mentsulist[0].pailist[1] = i;
										item.mentsulist[0].pailist[2] = i;
										item.mentsulist[1].category = AI_KOUTSU;
										item.mentsulist[1].pailist[0] = j;
										item.mentsulist[1].pailist[1] = j;
										item.mentsulist[1].pailist[2] = j;
										item.mentsulist[2].category = AI_KOUTSU;
										item.mentsulist[2].pailist[0] = k;
										item.mentsulist[2].pailist[1] = k;
										item.mentsulist[2].pailist[2] = k;
										item.mentsulist[3].category = AI_SYUNTSU;
										item.mentsulist[3].pailist[0] = st;
										item.mentsulist[3].pailist[1] = st+1;
										item.mentsulist[3].pailist[2] = st+2;
										item.mentsulist[4].category = AI_TOITSU;
										item.mentsulist[4].pailist[0] = toitsu[m];
										item.mentsulist[4].pailist[1] = toitsu[m];


										item.machi = AI_MACHI_SHANPON;
										item.machihai = item.mentsulist[0].pailist[0];
										item.machipos = 0;
										make_resultitem_bh(&item,&state);
										assert(probability >= 0 && item.score >= 0);

										sum += probability * item.score;
										//thread_count[1]++;
									}

								}
							}
						}
					}
				}
			}
		}
	}

	thread_count[1] = threadorder++;
	prm->ret = sum;

	return 0;
}


/* 2つの刻子+2つの順子+頭 */
static DWORD WINAPI threadfunc3(LPVOID pParam)
{
	int i,j,k,l,m,n,o;
	int st,st2;
	int count;
	double sum = 0.0;
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	double nokorisum = 0.0;
	double rest = 0.0;
	double probability = 1.0;
	RESULT_ITEM item;
	GAMESTATE state;
	int cnt[34];
	int toitsu[4] = { 0 , 0 , 0 , 0 };
	int maxc;
	int c;
	//return 0;


	for(count=0;count<34;count++){
		nokorisum += prm->pState->nokori[count];
	}

	memset(&state,0,sizeof(state));
	memset(&item,0,sizeof(item));

	state.tsumo = 1;
	state.bakaze = prm->pState->kyoku / 4;
	state.zikaze =prm->pState->cha;
	state.count = 1;


	for(i=0;i<34;i++){

		if(prm->pState->nokori[i] > 3 - prm->pState->te_cnt[i]){

			for(j=i+1;j<34;j++){

				if(prm->pState->nokori[j] > 3 - prm->pState->te_cnt[j]){

					for(k=0;k<21;k++){

						for(l=k;l<21;l++){

							memset(cnt,0,sizeof(cnt));
							cnt[i] = 3;
							cnt[j] = 3;

							if(k<7){
								cnt[k]++;
								cnt[k+1]++;
								cnt[k+2]++;
								st = k;

							}else if(k<14){
								cnt[k+2]++;
								cnt[k+3]++;
								cnt[k+4]++;
								st = k+2;

							}else{
								cnt[k+4]++;
								cnt[k+5]++;
								cnt[k+6]++;
								st = k+4;
							}

							if(l<7){
								cnt[l]++;
								cnt[l+1]++;
								cnt[l+2]++;
								st2 = l;

							}else if(l<14){
								cnt[l+2]++;
								cnt[l+3]++;
								cnt[l+4]++;
								st2 = l+2;

							}else{
								cnt[l+4]++;
								cnt[l+5]++;
								cnt[l+6]++;
								st2 = l+4;

							}


							if(prm->pState->nokori[st] > cnt[st] - prm->pState->te_cnt[st] - 1
							&& prm->pState->nokori[st+1] > cnt[st+1] - prm->pState->te_cnt[st+1] - 1
							&& prm->pState->nokori[st+2] > cnt[st+2] - prm->pState->te_cnt[st+2] - 1
							&& prm->pState->nokori[st2] > cnt[st2] - prm->pState->te_cnt[st2] - 1
							&& prm->pState->nokori[st2+1] > cnt[st2+1] - prm->pState->te_cnt[st2+1] - 1
							&& prm->pState->nokori[st2+2] > cnt[st2+2] - prm->pState->te_cnt[st2+2] - 1)
							{
								toitsu[0] = -1;
								toitsu[1] = -1;
								toitsu[2] = -1;
								toitsu[3] = -1;

								maxc = -4;
								for(m=0;m<9;m++){
									c = prm->pState->te_cnt[m] - cnt[m];
									if(maxc < c && prm->pState->nokori[m] > 1 - c) {
										maxc = c;
										toitsu[0] = m;
									}
								}

								maxc = -4;

								for(m=9;m<18;m++){
									c = prm->pState->te_cnt[m] - cnt[m];
									if(maxc < c && prm->pState->nokori[m] > 1 - c) {
										maxc = c;
										toitsu[1] = m;
									}
								}

								maxc = -4;
								for(m=18;m<27;m++){
									c = prm->pState->te_cnt[m] - cnt[m];
									if(maxc < c && prm->pState->nokori[m] > 1 - c) {
										maxc = c;
										toitsu[2] = m;
									}
								}
								maxc = -4;

								for(m=27;m<34;m++){
									c = prm->pState->te_cnt[m] - cnt[m];
									if(maxc < c && prm->pState->nokori[m] > 1 - c) {
										maxc = c;
										toitsu[3] = m;
									}
								}


								for(m=0;m<4;m++){
									if(toitsu[m] < 0) continue;
									probability = 1.0;
									rest = nokorisum;
									cnt[toitsu[m]]+= 2;
									for(n=0;n<34;n++){
										if(cnt[n]){
											c = cnt[n] - prm->pState->te_cnt[n];
											assert(prm->pState->nokori[n] - c + 1 >= 0);
                                            int dist = paidistance(prm->pState->tehai.tehai, n);

											for(o=0;o<c;o++){
												probability *= (double)(prm->pState->nokori[n] - o) / (double) rest;
                                                probability *= dist_coef[dist + 1];
												if(prm->pState->nokori[n] - o < 0){
                                                    probability = 0;
                                                    break;
												}
												rest-=1.0;
											}
										}
									}
									cnt[toitsu[m]]-= 2;

									item.mentsusize = 5;
									item.menzen = 1;
									item.machipos = 0;
									item.han = 0;
									item.fu = 0;
									item.mentsulist[0].category = AI_KOUTSU;
									item.mentsulist[0].pailist[0] = i;
									item.mentsulist[0].pailist[1] = i;
									item.mentsulist[0].pailist[2] = i;
									item.mentsulist[1].category = AI_KOUTSU;
									item.mentsulist[1].pailist[0] = j;
									item.mentsulist[1].pailist[1] = j;
									item.mentsulist[1].pailist[2] = j;
									item.mentsulist[2].category = AI_SYUNTSU;
									item.mentsulist[2].pailist[0] = st;
									item.mentsulist[2].pailist[1] = st+1;
									item.mentsulist[2].pailist[2] = st+2;
									item.mentsulist[3].category = AI_SYUNTSU;
									item.mentsulist[3].pailist[0] = st2;
									item.mentsulist[3].pailist[1] = st2+1;
									item.mentsulist[3].pailist[2] = st2+2;
									item.mentsulist[4].category = AI_TOITSU;
									item.mentsulist[4].pailist[0] = toitsu[m];
									item.mentsulist[4].pailist[1] = toitsu[m];


									item.machi = AI_MACHI_SHANPON;
									item.machihai = item.mentsulist[0].pailist[0];
									item.machipos = 0;
									make_resultitem_bh(&item,&state);
									assert(probability >= 0 && item.score >= 0);

									sum += probability * item.score;
									//thread_count[2]++;
								}
							}

						}
					}
				}
			}
		}
	}

	thread_count[2] = threadorder++;
	prm->ret = sum;

	return 0;
}

/* 1つの刻子+3つの順子+頭 */
static DWORD WINAPI threadfunc4(LPVOID pParam)
{
	int i,j,k,l,m,n,o;
	int st,st2,st3;
	int count;
	double sum = 0.0;
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	double nokorisum = 0.0;
	double rest = 0.0;
	double probability = 1.0;
	RESULT_ITEM item;
	GAMESTATE state;
	int cnt[34];
	int toitsu[4] = { 0 , 0 , 0 , 0 };
	int maxc;
	int c;
	//return 0;

	for(count=0;count<34;count++){
		nokorisum += prm->pState->nokori[count];
	}

	memset(&state,0,sizeof(state));
	memset(&item,0,sizeof(item));

	state.count = 1;
	state.tsumo = 1;
	state.bakaze = prm->pState->kyoku / 4;
	state.zikaze =prm->pState->cha;

	for(i=0;i<34;i++){

		if(prm->pState->nokori[i] > 3 - prm->pState->te_cnt[i]){

			for(j=0;j<21;j++){

				for(k=j;k<21;k++){

					for(l=k;l<21;l++){

						if(j==k && k==l) continue;

						memset(cnt,0,sizeof(cnt));
						cnt[i] = 3;

						if(j<7){
							cnt[j]++;
							cnt[j+1]++;
							cnt[j+2]++;
							st = j;

						}else if(j<14){
							cnt[j+2]++;
							cnt[j+3]++;
							cnt[j+4]++;
							st = j+2;

						}else{
							cnt[j+4]++;
							cnt[j+5]++;
							cnt[j+6]++;
							st = j+4;
						}

						if(k<7){
							cnt[k]++;
							cnt[k+1]++;
							cnt[k+2]++;
							st2 = k;

						}else if(k<14){
							cnt[k+2]++;
							cnt[k+3]++;
							cnt[k+4]++;
							st2 = k+2;

						}else{
							cnt[k+4]++;
							cnt[k+5]++;
							cnt[k+6]++;
							st2 = k+4;
						}

						if(l<7){
							cnt[l]++;
							cnt[l+1]++;
							cnt[l+2]++;
							st3 = l;

						}else if(l<14){
							cnt[l+2]++;
							cnt[l+3]++;
							cnt[l+4]++;
							st3 = l+2;

						}else{
							cnt[l+4]++;
							cnt[l+5]++;
							cnt[l+6]++;
							st3 = l+4;
						}


						if(prm->pState->nokori[st] > cnt[st] - prm->pState->te_cnt[st] - 1
						&& prm->pState->nokori[st+1] > cnt[st+1] - prm->pState->te_cnt[st+1] - 1
						&& prm->pState->nokori[st+2] > cnt[st+2] - prm->pState->te_cnt[st+2] - 1
						&& prm->pState->nokori[st2] > cnt[st2] - prm->pState->te_cnt[st2] - 1
						&& prm->pState->nokori[st2+1] > cnt[st2+1] - prm->pState->te_cnt[st2+1] - 1
						&& prm->pState->nokori[st2+2] > cnt[st2+2] - prm->pState->te_cnt[st2+2] - 1
						&& prm->pState->nokori[st3] > cnt[st3] - prm->pState->te_cnt[st3] - 1
						&& prm->pState->nokori[st3+1] > cnt[st3+1] - prm->pState->te_cnt[st3+1] - 1
						&& prm->pState->nokori[st3+2] > cnt[st3+2] - prm->pState->te_cnt[st3+2] - 1)
						{
							toitsu[0] = -1;
							toitsu[1] = -1;
							toitsu[2] = -1;
							toitsu[3] = -1;

							maxc = -4;
							for(m=0;m<9;m++){
								c = prm->pState->te_cnt[m] - cnt[m];
								if(maxc < c && prm->pState->nokori[m] > 1 - c) {
									maxc = c;
									toitsu[0] = m;
								}
							}

							maxc = -4;

							for(m=9;m<18;m++){
								c = prm->pState->te_cnt[m] - cnt[m];
								if(maxc < c && prm->pState->nokori[m] > 1 - c) {
									maxc = c;
									toitsu[1] = m;
								}
							}

							maxc = -4;
							for(m=18;m<27;m++){
								c = prm->pState->te_cnt[m] - cnt[m];
								if(maxc < c && prm->pState->nokori[m] > 1 - c) {
									maxc = c;
									toitsu[2] = m;
								}
							}
							maxc = -4;

							for(m=27;m<34;m++){
								c = prm->pState->te_cnt[m] - cnt[m];
								if(maxc < c && prm->pState->nokori[m] > 1 - c) {
									maxc = c;
									toitsu[3] = m;
								}
							}


							for(m=0;m<4;m++){
								if(toitsu[m] < 0) continue;
								probability = 1.0;
								rest = nokorisum;
								cnt[toitsu[m]]+= 2;
								for(n=0;n<34;n++){
									if(cnt[n]){
										c = cnt[n] - prm->pState->te_cnt[n];
										assert(prm->pState->nokori[n] - c + 1 >= 0);
                                        int dist = paidistance(prm->pState->tehai.tehai, n);

										for(o=0;o<c;o++){
											probability *= (double)(prm->pState->nokori[n] - o) / (double) rest;
                                            probability *= dist_coef[dist + 1];
											if(prm->pState->nokori[n] - o < 0){
                                                probability = 0;
                                                break;
											}
											rest-=1.0;
										}
									}
								}
								cnt[toitsu[m]]-= 2;

								item.mentsusize = 5;
								item.menzen = 1;
								item.machipos = 0;
								item.han = 0;
								item.fu = 0;
								item.mentsulist[0].category = AI_KOUTSU;
								item.mentsulist[0].pailist[0] = i;
								item.mentsulist[0].pailist[1] = i;
								item.mentsulist[0].pailist[2] = i;
								item.mentsulist[1].category = AI_SYUNTSU;
								item.mentsulist[1].pailist[0] = st;
								item.mentsulist[1].pailist[1] = st+1;
								item.mentsulist[1].pailist[2] = st+2;
								item.mentsulist[2].category = AI_SYUNTSU;
								item.mentsulist[2].pailist[0] = st2;
								item.mentsulist[2].pailist[1] = st2+1;
								item.mentsulist[2].pailist[2] = st2+2;
								item.mentsulist[3].category = AI_SYUNTSU;
								item.mentsulist[3].pailist[0] = st3;
								item.mentsulist[3].pailist[1] = st3+1;
								item.mentsulist[3].pailist[2] = st3+2;
								item.mentsulist[4].category = AI_TOITSU;
								item.mentsulist[4].pailist[0] = toitsu[m];
								item.mentsulist[4].pailist[1] = toitsu[m];


								item.machi = AI_MACHI_SHANPON;
								item.machihai = item.mentsulist[0].pailist[0];
								item.machipos = 0;
								make_resultitem_bh(&item,&state);

								assert(probability >= 0 && item.score >= 0);

								sum += probability * item.score;

								//thread_count[3]++;
							}
						}

					}
				}
			}
		}
	}
	thread_count[3] = threadorder++;

	prm->ret = sum;

	return 0;
}

/* 4つの順子+頭 */
static DWORD WINAPI threadfunc5(LPVOID pParam)
{
	int i,j,k,l,m,n,o;
	int st,st2,st3,st4;
	int count;
	double sum = 0.0;
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	double nokorisum = 0.0;
	double rest = 0.0;
	double probability = 1.0;
	RESULT_ITEM item;
	GAMESTATE state;
	int cnt[34];
	int toitsu[4] = { 0 , 0 , 0 , 0 };
	int maxc;
	int c;

	for(count=0;count<34;count++){
		nokorisum += prm->pState->nokori[count];
	}

	memset(&state,0,sizeof(state));
	memset(&item,0,sizeof(item));

	state.tsumo = 1;
	state.bakaze = prm->pState->kyoku / 4;
	state.zikaze =prm->pState->cha;
	state.count = 1;

	for(i=0;i<21;i++){

		for(j=i;j<21;j++){

			for(k=j;k<21;k++){
				if(i==j && j==k) continue;

				for(l=k;l<21;l++){

					memset(cnt,0,sizeof(cnt));

					if(i<7){
						cnt[i]++;
						cnt[i+1]++;
						cnt[i+2]++;
						st = i;

					}else if(i<14){
						cnt[i+2]++;
						cnt[i+3]++;
						cnt[i+4]++;
						st = i+2;

					}else{
						cnt[i+4]++;
						cnt[i+5]++;
						cnt[i+6]++;
						st = i+4;
					}

					if(j<7){
						cnt[j]++;
						cnt[j+1]++;
						cnt[j+2]++;
						st2 = j;

					}else if(j<14){
						cnt[j+2]++;
						cnt[j+3]++;
						cnt[j+4]++;
						st2 = j+2;

					}else{
						cnt[j+4]++;
						cnt[j+5]++;
						cnt[j+6]++;
						st2 = j+4;
					}

					if(k<7){
						cnt[k]++;
						cnt[k+1]++;
						cnt[k+2]++;
						st3 = k;

					}else if(k<14){
						cnt[k+2]++;
						cnt[k+3]++;
						cnt[k+4]++;
						st3 = k+2;

					}else{
						cnt[k+4]++;
						cnt[k+5]++;
						cnt[k+6]++;
						st3 = k+4;
					}

					if(l<7){
						cnt[l]++;
						cnt[l+1]++;
						cnt[l+2]++;
						st4 = l;

					}else if(l<14){
						cnt[l+2]++;
						cnt[l+3]++;
						cnt[l+4]++;
						st4 = l+2;

					}else{
						cnt[l+4]++;
						cnt[l+5]++;
						cnt[l+6]++;
						st4 = l+4;
					}


					if(prm->pState->nokori[st] > cnt[st] - prm->pState->te_cnt[st] - 1
					&& prm->pState->nokori[st+1] > cnt[st+1] - prm->pState->te_cnt[st+1] - 1
					&& prm->pState->nokori[st+2] > cnt[st+2] - prm->pState->te_cnt[st+2] - 1
					&& prm->pState->nokori[st2] > cnt[st2] - prm->pState->te_cnt[st2] - 1
					&& prm->pState->nokori[st2+1] > cnt[st2+1] - prm->pState->te_cnt[st2+1] - 1
					&& prm->pState->nokori[st2+2] > cnt[st2+2] - prm->pState->te_cnt[st2+2] - 1
					&& prm->pState->nokori[st3] > cnt[st3] - prm->pState->te_cnt[st3] - 1
					&& prm->pState->nokori[st3+1] > cnt[st3+1] - prm->pState->te_cnt[st3+1] - 1
					&& prm->pState->nokori[st3+2] > cnt[st3+2] - prm->pState->te_cnt[st3+2] - 1
					&& prm->pState->nokori[st4] > cnt[st4] - prm->pState->te_cnt[st4] - 1
					&& prm->pState->nokori[st4+1] > cnt[st4+1] - prm->pState->te_cnt[st4+1] - 1
					&& prm->pState->nokori[st4+2] > cnt[st4+2] - prm->pState->te_cnt[st4+2] - 1)
					{
						toitsu[0] = -1;
						toitsu[1] = -1;
						toitsu[2] = -1;
						toitsu[3] = -1;

						maxc = -4;
						for(m=0;m<9;m++){
							c = prm->pState->te_cnt[m] - cnt[m];
							if(maxc < c && prm->pState->nokori[m] > 1 - c) {
								maxc = c;
								toitsu[0] = m;
							}
						}

						maxc = -4;

						for(m=9;m<18;m++){
							c = prm->pState->te_cnt[m] - cnt[m];
							if(maxc < c && prm->pState->nokori[m] > 1 - c) {
								maxc = c;
								toitsu[1] = m;
							}
						}

						maxc = -4;
						for(m=18;m<27;m++){
							c = prm->pState->te_cnt[m] - cnt[m];
							if(maxc < c && prm->pState->nokori[m] > 1 - c) {
								maxc = c;
								toitsu[2] = m;
							}
						}
						maxc = -4;

						for(m=27;m<34;m++){
							c = prm->pState->te_cnt[m] - cnt[m];
							if(maxc < c && prm->pState->nokori[m] > 1 - c) {
								maxc = c;
								toitsu[3] = m;
							}
						}


						for(m=0;m<4;m++){
							if(toitsu[m] < 0) continue;
							probability = 1.0;
							rest = nokorisum;
							cnt[toitsu[m]]+= 2;
							for(n=0;n<34;n++){
								if(cnt[n]){
									c = cnt[n] - prm->pState->te_cnt[n];
									assert(prm->pState->nokori[n] - c + 1 >= 0);
                                    int dist = paidistance(prm->pState->tehai.tehai, n);

									for(o=0;o<c;o++){
										probability *= (double)(prm->pState->nokori[n] - o) / (double) rest;
                                        probability *= dist_coef[dist + 1];
										if(prm->pState->nokori[n] - o < 0){
                                            probability = 0;
                                            break;
										}
										rest-=1.0;
									}
								}
							}
							cnt[toitsu[m]]-= 2;

							item.han = 0;
							item.fu = 0;
							item.mentsusize = 5;
							item.menzen = 1;
							item.machipos = 0;
							item.mentsulist[0].category = AI_SYUNTSU;
							item.mentsulist[0].pailist[0] = st;
							item.mentsulist[0].pailist[1] = st+1;
							item.mentsulist[0].pailist[2] = st+2;
							item.mentsulist[1].category = AI_SYUNTSU;
							item.mentsulist[1].pailist[0] = st2;
							item.mentsulist[1].pailist[1] = st2+1;
							item.mentsulist[1].pailist[2] = st2+2;
							item.mentsulist[2].category = AI_SYUNTSU;
							item.mentsulist[2].pailist[0] = st3;
							item.mentsulist[2].pailist[1] = st3+1;
							item.mentsulist[2].pailist[2] = st3+2;
							item.mentsulist[3].category = AI_SYUNTSU;
							item.mentsulist[3].pailist[0] = st4;
							item.mentsulist[3].pailist[1] = st4+1;
							item.mentsulist[3].pailist[2] = st4+2;
							item.mentsulist[4].category = AI_TOITSU;
							item.mentsulist[4].pailist[0] = toitsu[m];
							item.mentsulist[4].pailist[1] = toitsu[m];


							item.machi = AI_MACHI_RYANMEN;
							item.machihai = item.mentsulist[0].pailist[0];
							item.machipos = 0;
							make_resultitem_bh(&item,&state);
							assert(probability >= 0 && item.score >= 0);

							sum += probability * item.score;
							//thread_count[4]++;
						}
					}
				}

			}
		}
	}
	thread_count[4] = threadorder++;

	prm->ret = sum;

	return 0;
}


#define THREADNUM (5)

double MahjongAIType4::evalSutehaiSub(MahjongAIState &param,int hai)
{
	double sum = 0.0;
	DWORD dwID;
	THREAD_PARAM tparam[THREADNUM];
	HANDLE hThread[THREADNUM];
	int i;
	char message[256];

	for(i=0;i<THREADNUM;i++){
		tparam[i].pState = &param;
		tparam[i].hai = hai;
		tparam[i].ret = 0.0;
	}

	memset(thread_count,0,sizeof(thread_count));
	memset(hThread,0,sizeof(hThread));
	threadorder = 0;

	hThread[0] = (HANDLE)CreateThread(NULL,0,threadfunc,&tparam[0],0,&dwID);
	hThread[1] = (HANDLE)CreateThread(NULL,0,threadfunc2,&tparam[1],0,&dwID);
	hThread[2] = (HANDLE)CreateThread(NULL,0,threadfunc3,&tparam[2],0,&dwID);
	hThread[3] = (HANDLE)CreateThread(NULL,0,threadfunc4,&tparam[3],0,&dwID);
	hThread[4] = (HANDLE)CreateThread(NULL,0,threadfunc5,&tparam[4],0,&dwID);

	WaitForMultipleObjects(THREADNUM,hThread,TRUE,INFINITE);

	for(i=0;i<THREADNUM;i++){
		sum += tparam[i].ret;
	}

#ifdef _DEBUG
	sprintf(message,"[%d] %.2lf (%.2lf[%d],%.2lf[%d],%.2lf[%d],%.2lf[%d],%.2lf[%d])\r\n",hai,sum,tparam[0].ret,thread_count[0],tparam[1].ret,thread_count[1],tparam[2].ret,thread_count[2],tparam[3].ret,thread_count[3],tparam[4].ret,thread_count[4]);

	MJSendMessage(MJMI_FUKIDASHI,(UINT)message,0);
#endif

	return sum;
	
}

UINT MahjongAIType4::getDebugInt()
{
	return 0;//thread_count[0] + thread_count[1] + thread_count[2] + thread_count[3] + thread_count[4];
}
