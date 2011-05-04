/*
 * MJ0.c
 * とつげき東北氏のMJ-0 アルゴリズムを実装します(on まうじゃんIF)
 * アルゴリズムの著作権はとつげき東北氏が所有します
 * まうじゃんの著作権は石畑恭平氏が所有します
 * プログラムの著作権はkackyが所有します
 */
/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
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

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MJ0.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	int num;
	int sum;
} MJ0_COMBI;

int MJ0(/* inputs */
		MJ0PARAM *pParam,int *pDora,int dlength,
		/* outputs */
		double *pNokorihai,double *pKikenhai,
		double *pMentsuSimo,double *pMentsuToimen,double *pMentsuKami)
{
	int i,j,k;
	int sum;
	int index;
	int mindex;
	int pai;
	int furiten;
	unsigned int r;
	int aNokorihai[34];
	int iniMentsu[3];
	int aPaikukan[4][34];
	int curMentsu[3];
	MJ0PARAM *pCurPrm;
	MJ0_COMBI aCombi[21+34];
	int aAnpai[3][34];
	int aRiichi[3];
	int aMentsu[3][5];
	int aMachi[34];
	
	iniMentsu[0] = 0;
	iniMentsu[1] = 0;
	iniMentsu[2] = 0;
	aRiichi[0] = 0;
	aRiichi[1] = 0;
	aRiichi[2] = 0;

	memset(aAnpai,0,sizeof(aAnpai));

	for(i=0;i<34;i++){
		pNokorihai[i] = 0.0;
		pKikenhai[i] = 0.0;
		aNokorihai[i] = 4;
	}

	for(i=0;i<21+34;i++){
		pMentsuSimo[i] = 0.0;
		pMentsuToimen[i] = 0.0;
		pMentsuKami[i] = 0.0;
	}

	/* 残り牌を初期化 */
	pCurPrm = pParam;
	for(j=0;j<pCurPrm->pTehai->tehai_max;j++){
		aNokorihai[pCurPrm->pTehai->tehai[j] & 63]--;
	}
	for(i=0;i<4;i++){

		for(j=0;j<pCurPrm->pTehai->minshun_max;j++){
			aNokorihai[pCurPrm->pTehai->minshun[j] & 63]--;
			aNokorihai[(pCurPrm->pTehai->minshun[j] & 63) + 1]--;
			aNokorihai[(pCurPrm->pTehai->minshun[j] & 63) + 2]--;
		}
		if(i!=0) iniMentsu[i-1]+= pCurPrm->pTehai->minshun_max;

		for(j=0;j<pCurPrm->pTehai->minkou_max;j++){
			aNokorihai[pCurPrm->pTehai->minkou[j] & 63]-=3;
		}
		if(i!=0) iniMentsu[i-1]+= pCurPrm->pTehai->minkou_max;
		
		for(j=0;j<pCurPrm->pTehai->minkan_max;j++){
			aNokorihai[pCurPrm->pTehai->minkan[j] & 63]-=4;
		}
		if(i!=0) iniMentsu[i-1]+= pCurPrm->pTehai->minkan_max;
		
		for(j=0;j<pCurPrm->pTehai->ankan_max;j++){
			aNokorihai[pCurPrm->pTehai->ankan[j] & 63]-=4;
		}
		if(i!=0) iniMentsu[i-1]+= pCurPrm->pTehai->ankan_max;

		/* 鳴かれた牌は既に勘定しているので除く */
		for(j=0;j<pCurPrm->kawalength;j++){
			if(!(pCurPrm->pKawahai[j].state & MJKS_NAKI)) aNokorihai[pCurPrm->pKawahai[j].hai & 63]--;
			/* 現物を安牌にする */
			if(i!=0){
				aAnpai[i-1][pCurPrm->pKawahai[j].hai & 63] = 1;
			}

		}

		/* もし既に固定しておく面子があるのであればそれをここで行う(TODO) */

		pCurPrm++;
	}

	/* リーチ後の捨て牌を安パイにする(捨て順を考慮してないけど) */
	i = 0;
	j = 0;
	while(1){
		if(j >= pParam[i].kawalength) break;
		if(i!=0){
			if(pParam[i].pKawahai[j].state & MJKS_REACH){
				aRiichi[i - 1] = 1;
			}
			for(k=1;k<4;k++){
				if(aRiichi[k - 1]){
					aAnpai[k - 1][pParam[i].pKawahai[j].hai & 63] = 1;
				}
			}
		}

		i = (i + 1) & 3;
		if(i==0) j++;
	}

	for(i=0;i<dlength;i++){
		aNokorihai[pDora[i] & 63]--;
	}


	/* 指定回数分シミュレーションを繰り返す */
	for(i=0;i<SIMU_SIZE;i++){
		for(j=0;j<4;j++){
			memcpy(&aPaikukan[j][0],aNokorihai,sizeof(aNokorihai));
		}

		memset(aMentsu,0xFF,sizeof(aMentsu));
		memcpy(curMentsu,iniMentsu,sizeof(iniMentsu));

		/* もし、一色系などを読むのであればここで牌空間を変更しておくと良い(TODO) */


		/* 4面子全ての相手が揃うまで繰り返す */
		while(curMentsu[0] != 4 || curMentsu[1] != 4 || curMentsu[2] != 4){
			for(j=0;j<3;j++){
				if(curMentsu[j] == 4) continue;
				/* 組み合わせ数の初期化 */

				/* 順子の組み合わせ */
				for(k=0;k<21;k++){
					pai = (k / 7) * 9 + k % 7; // 計算する牌番号の先頭の値
					aCombi[k].num = aPaikukan[j][pai] * aPaikukan[j][pai+1] * aPaikukan[j][pai+2];
				}

				/* 刻子の組み合わせ */
				for(k=0;k<34;k++){
					if(aPaikukan[j][k-21] == 4){
						aCombi[k+21].num = 4;
					}else if(aPaikukan[j][k-21] == 3){
						aCombi[k+21].num = 1;
					}else{
						aCombi[k+21].num = 0;
					}
				}

				/* 待ち読みによる組み合わせ数の更新(TODO) */

				/* 合計を計算 */
				sum = 0;
				for(k=0;k<21+34;k++){
					aCombi[k].sum = sum;
					sum += aCombi[k].num;
				}

				/* 乱数により面子を生成 */
				if(sum == 0) goto failure;
				r = ((unsigned int)rand()) % sum;

				/* そんなにたくさんないからシーケンシャルサーチで */
				for(k=0;k<21+34;k++){
					if(r < aCombi[k].sum + aCombi[k].num && aCombi[k].num != 0){
						/* この面子に決定！ */
						aMentsu[j][curMentsu[j]] = k;

						/* 面子を構成する牌を牌空間から除去 */
						if(k<21){
							if(aPaikukan[0][k] > 0)   aPaikukan[0][k]--;
							if(aPaikukan[0][k+1] > 0) aPaikukan[0][k+1]--;
							if(aPaikukan[0][k+2] > 0) aPaikukan[0][k+2]--;
							if(aPaikukan[1][k] > 0)   aPaikukan[1][k]--;
							if(aPaikukan[1][k+1] > 0) aPaikukan[1][k+1]--;
							if(aPaikukan[1][k+2] > 0) aPaikukan[1][k+2]--;
							if(aPaikukan[2][k] > 0)   aPaikukan[2][k]--;
							if(aPaikukan[2][k+1] > 0) aPaikukan[2][k+1]--;
							if(aPaikukan[2][k+2] > 0) aPaikukan[2][k+2]--;
							if(aPaikukan[3][k] > 0)   aPaikukan[3][k]--;
							if(aPaikukan[3][k+1] > 0) aPaikukan[3][k+1]--;
							if(aPaikukan[3][k+2] > 0) aPaikukan[3][k+2]--;
						}else{
							if(aPaikukan[0][k-21] >= 3) aPaikukan[0][k-21]-=3;
							if(aPaikukan[1][k-21] >= 3) aPaikukan[1][k-21]-=3;
							if(aPaikukan[2][k-21] >= 3) aPaikukan[2][k-21]-=3;
							if(aPaikukan[3][k-21] >= 3) aPaikukan[3][k-21]-=3;
						}

						if(j==0){
							pMentsuSimo[k] += 1.0;
						}else if(j==1){
							pMentsuToimen[k] += 1.0;
						}else{
							pMentsuKami[k] += 1.0;
						}

						curMentsu[j]++;
						break;
					}
				}

			}
			
		}

		/* アタマの生成 */
		for(j=0;j<3;j++){
			for(k=0;k<34;k++){
				if(aPaikukan[j][k] == 4){
					aCombi[k].num = 6;  // 4 C 2
				}else if(aPaikukan[j][k] == 3){
					aCombi[k].num = 3;  // 3 C 2
				}else if(aPaikukan[j][k] == 2){
					aCombi[k].num = 1;  // 2 C 2
				}else{
					aCombi[k].num = 0;
				}
			}
			/* 待ち読みによる組み合わせ数の更新(TODO) */

			/* 合計を計算 */
			sum = 0;
			for(k=0;k<34;k++){
				aCombi[k].sum = sum;
				sum += aCombi[k].num;
			}

			/* 乱数により対子を生成 */
			if(sum == 0) goto failure;
			r = ((unsigned int)rand()) % sum;
			/* そんなにたくさんないからシーケンシャルサーチで */
			for(k=0;k<34;k++){
				if(r < aCombi[k].sum + aCombi[k].num && aCombi[k].num != 0){
					/* この牌に決定！ */
					aMentsu[j][4] = k;
					/* アタマを構成する牌を牌空間から除去 */
					if(aPaikukan[0][k] >= 2) aPaikukan[0][k]-=2;
					if(aPaikukan[1][k] >= 2) aPaikukan[1][k]-=2;
					if(aPaikukan[2][k] >= 2) aPaikukan[2][k]-=2;
					if(aPaikukan[3][k] >= 2) aPaikukan[3][k]-=2;
					break;
				}
			}
		}

		

		/* 当たり牌を決定する */
		for(;;){
			memset(aMachi,0,sizeof(aMachi));

			j = rand() % 3; // 誰の面子を当たりにするか
			mindex = rand() % ((4 - iniMentsu[j]) * 3 + 2); // どの牌を当たり牌にするか
			if(mindex >= (4 - iniMentsu[j]) * 3){
				/* 単騎待ち */
				aMachi[aMentsu[j][4]] = 1;

				if(aMentsu[j][4] < 27){
					/* 延べ単騎 */
					for(k=0;k<4;k++){
						if((aMentsu[j][k] % 7) != 6 && 
							(aMentsu[j][k] / 7)*9 + (aMentsu[j][k] % 7) + 3 == aMentsu[j][4]){
							aMachi[aMentsu[j][4] - 3] = 1;
						}
						if((aMentsu[j][k] % 7) != 0 && 
							(aMentsu[j][k] / 7)*9 + (aMentsu[j][k] % 7) - 1 == aMentsu[j][4]){
							aMachi[aMentsu[j][4] + 3] = 1;
						}
					}
				}
			}else{
				/* 面子待ち */
				index = iniMentsu[j] + mindex/3;
				
				if(aMentsu[j][index] < 21){
					/* 順子 */
					pai = (aMentsu[j][index] / 7) * 9 + (aMentsu[j][index] % 7);
					if((mindex % 3) == 1){
						/* カンチャン */
						aMachi[pai] = 1;

						/* 暗刻カブリ */
						if(aMentsu[j][4] == pai + 1){
							aMachi[pai-1] = 1;
						}else if(aMentsu[j][4] == pai - 1){
							aMachi[pai+1] = 1;
						}

					}else if((mindex % 3) == 0 && (pai % 9) == 6){
						/* ペンチャン */
						aMachi[pai] = 1;
						/* 暗刻カブリ */
						if(aMentsu[j][4] == pai + 1){
							aMachi[pai+2] = 1;
						}else if(aMentsu[j][4] == pai + 2){
							aMachi[pai+1] = 1;
						}

					}else if((mindex % 3) == 2 && (pai % 9) == 0){
						/* ペンチャン */
						aMachi[pai+2] = 1;
						/* 暗刻カブリ */
						if(aMentsu[j][4] == pai){
							aMachi[pai+1] = 1;
						}else if(aMentsu[j][4] == pai + 1){
							aMachi[pai] = 1;
						}
					}else{
						/* 両面 */
						if((mindex % 3) == 0){
							aMachi[pai] = 1;
							aMachi[pai+3] = 1;
							/* 暗刻カブリ */
							if(aMentsu[j][4] == pai + 1){
								aMachi[pai+2] = 1;
							}else if(aMentsu[j][4] == pai + 2){
								aMachi[pai+1] = 1;
							}

						}else{
							aMachi[pai+2] = 1;
							aMachi[pai-1] = 1;
							/* 暗刻カブリ */
							if(aMentsu[j][4] == pai){
								aMachi[pai+1] = 1;
							}else if(aMentsu[j][4] == pai + 1){
								aMachi[pai] = 1;
							}
						}

					}
				}else{
					/* 刻子はシャンポン待ち */
					aMachi[aMentsu[j][index] - 21] = 1;
					aMachi[aMentsu[j][4]] = 1;
				}

			}

			furiten = 0;
			for(k=0;k<34;k++){
				if(aMachi[k] && aAnpai[j][k]){
					furiten = 1;
				}
			}

			if(furiten == 0){
				for(k=0;k<34;k++){
					if(aMachi[k]){
						pKikenhai[k] += 1.0;
					}
				}
			}
			break;

		}


		/* 残った牌を山の数としてカウント */
		for(j=0;j<34;j++){
			pNokorihai[j] += aPaikukan[3][j];
		}
failure:
		j=0;

	}

	/* 試行回数で割り算をして平均値とする */
	for(i=0;i<34;i++){
		pNokorihai[i] /= SIMU_SIZE;
		pKikenhai[i] /= SIMU_SIZE;
	}

	for(i=0;i<27+34;i++){
		pMentsuSimo[i] /= SIMU_SIZE;
		pMentsuToimen[i] /= SIMU_SIZE;
		pMentsuKami[i] /= SIMU_SIZE;
	}

	return 1;

}



#ifdef __cplusplus
}
#endif
