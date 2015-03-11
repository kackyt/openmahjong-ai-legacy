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

typedef struct {
	MahjongAIState *pState;
	int hai;
	double ret;
	double val;
	double nokorisum;
	int count;
	int shanten;
	char debug[255];
} THREAD_PARAM;




#if 0
/* 牌距離毎の期待値の補正係数 */
static double dist_coef[] = {
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1
};

/* 牌の種類ごとの期待値の補正係数 */
static double kind_coef[] = {
	1,
	1,
	1,
	1,
	1
};

#else
/* 牌距離毎の期待値の補正係数 */
static double dist_coef[] = {
	0.988,
	0.990,
	0.999,
	0.998,
	0.994,
	0.997,
	0.993,
	0.993,
	0.993,
	0.993
};

/* 牌の種類ごとの期待値の補正係数 */
static double kind_coef[] = {
    0.997,
    0.998,
    0.999,
    0.996,
    0.995
};
#endif

static double getKindCoef(MahjongAIState *state,unsigned num){
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


/* ベストな上がり形を探索する */
static void bestagari(THREAD_PARAM *prm, RESULT_ITEM *ret, int *cnt, int *koutsucnt, int *shuntsucnt, int koutsunum, int shuntsunum, int koutsupos, int shuntsupos, int atama){
	int i,pai;
	int nakicnt;
	RESULT_ITEM item;
	GAMESTATE state;

	memset(&state, 0, sizeof(state));
	memset(&item, 0, sizeof(item));

	state.tsumo = 1;
	state.bakaze = prm->pState->kyoku / 4;
	state.zikaze = prm->pState->cha;
	state.count = 1;

	nakicnt = prm->pState->tehai.ankan_max + prm->pState->tehai.minkan_max + prm->pState->tehai.minkou_max + prm->pState->tehai.minshun_max;

	if (koutsunum + shuntsunum + nakicnt >= 4){
		int count = 0;
		int j;

		item.mentsusize = 5;
		item.menzen = 1;
		item.machipos = 0;
		item.han = 0;
		item.fu = 0;

		if (prm->pState->tehai.minkan_max > 0
			|| prm->pState->tehai.minkou_max > 0
			|| prm->pState->tehai.minshun_max > 0
			){
			item.menzen = 0;
		}

		for (i = 0; i < 34; i++){
			for (j = 0; j < koutsucnt[i]; j++){
				assert(count < 4);
				item.mentsulist[count].category = AI_KOUTSU;
				item.mentsulist[count].pailist[0] = i;
				item.mentsulist[count].pailist[1] = i;
				item.mentsulist[count].pailist[2] = i;

				count++;
			}
		}

		for (i = 0; i < 21; i++){
			for (j = 0; j < shuntsucnt[i]; j++){
				assert(count < 4);
				pai = (i / 7) * 9 + i % 7;
				item.mentsulist[count].category = AI_SYUNTSU;
				item.mentsulist[count].pailist[0] = pai;
				item.mentsulist[count].pailist[1] = pai + 1;
				item.mentsulist[count].pailist[2] = pai + 2;

				count++;
			}
		}

		for (i = 0; i < prm->pState->tehai.ankan_max; i++){
			assert(count < 4);
			item.mentsulist[count].category = AI_ANKAN;
			item.mentsulist[count].pailist[0] = prm->pState->tehai.ankan[0];
			item.mentsulist[count].pailist[1] = prm->pState->tehai.ankan[0];
			item.mentsulist[count].pailist[2] = prm->pState->tehai.ankan[0];
			count++;
		}
		for (i = 0; i < prm->pState->tehai.minkan_max; i++){
			assert(count < 4);
			item.mentsulist[count].category = AI_MINKAN;
			item.mentsulist[count].pailist[0] = prm->pState->tehai.minkan[0];
			item.mentsulist[count].pailist[1] = prm->pState->tehai.minkan[0];
			item.mentsulist[count].pailist[2] = prm->pState->tehai.minkan[0];
			count++;
		}

		for (i = 0; i < prm->pState->tehai.minkou_max; i++){
			assert(count < 4);
			item.mentsulist[count].category = AI_KOUTSU;
			item.mentsulist[count].pailist[0] = prm->pState->tehai.minkou[0];
			item.mentsulist[count].pailist[1] = prm->pState->tehai.minkou[0];
			item.mentsulist[count].pailist[2] = prm->pState->tehai.minkou[0];
			count++;
		}

		for (i = 0; i < prm->pState->tehai.minshun_max; i++){
			assert(count < 4);
			item.mentsulist[count].category = AI_SYUNTSU;
			item.mentsulist[count].pailist[0] = prm->pState->tehai.minshun[0];
			item.mentsulist[count].pailist[1] = prm->pState->tehai.minshun[1];
			item.mentsulist[count].pailist[2] = prm->pState->tehai.minshun[2];
			count++;
		}

		item.mentsulist[count].category = AI_TOITSU;
		item.mentsulist[count].pailist[0] = atama;
		item.mentsulist[count].pailist[1] = atama;

		item.machi = item.mentsulist[0].category == AI_SYUNTSU ? AI_MACHI_RYANMEN : AI_MACHI_SHANPON;
		item.machihai = item.mentsulist[0].pailist[0];
		item.machipos = 0;

		make_resultitem_bh(&item, &state);

		if (ret->score < item.score){
			*ret = item;
		}
	}
	else{
		/* 刻子を探索 */
		for (i = koutsupos; i < 34; i++){
			if (cnt[i] >= 3) {
				cnt[i] -= 3;
				koutsucnt[i]++;
				bestagari(prm, ret, cnt, koutsucnt, shuntsucnt, koutsunum + 1, shuntsunum, i + 1, shuntsupos, atama);

				koutsucnt[i]--;
				cnt[i] += 3;

			}
		}

		/* 順子を探索 */
		for (i = shuntsupos; i < 21; i++){
			pai = (i / 7) * 9 + (i % 7);
			if (cnt[pai] >= 1 && cnt[pai + 1] >= 1 && cnt[pai + 2] >= 1){
				cnt[pai]--;
				cnt[pai + 1]--;
				cnt[pai + 2]--;
				shuntsucnt[i]++;
				bestagari(prm, ret, cnt, koutsucnt, shuntsucnt, koutsunum, shuntsunum + 1, koutsupos, i, atama);
				cnt[pai]++;
				cnt[pai + 1]++;
				cnt[pai + 2]++;
				shuntsucnt[i]--;
			}
		}
	}
}





static double calcscore(THREAD_PARAM *prm, int *cnt, int *koutsucnt, int *shuntsucnt){
	RESULT_ITEM item;
	int i, j, count = 0, pai;
	int maxc, m, c,n,o;
	int toitsu[4];
	double probability,rest,ret = 0.0,value;
	GAMESTATE state;

	memset(&state, 0, sizeof(state));
	memset(&item, 0, sizeof(item));

	state.tsumo = 1;
	state.bakaze = prm->pState->kyoku / 4;
	state.zikaze = prm->pState->cha;
	state.count = 1;

	item.mentsusize = 5;
	item.menzen = 1;
	item.machipos = 0;
	item.han = 0;
	item.fu = 0;

	if (prm->pState->tehai.minkan_max > 0
		|| prm->pState->tehai.minkou_max > 0
		|| prm->pState->tehai.minshun_max > 0
		){
		item.menzen = 0;
	}

	for (i = 0; i < 34; i++){
		for (j = 0; j < koutsucnt[i];j++){
			assert(count < 4);
			item.mentsulist[count].category = AI_KOUTSU;
			item.mentsulist[count].pailist[0] = i;
			item.mentsulist[count].pailist[1] = i;
			item.mentsulist[count].pailist[2] = i;

			count++;
		}
	}

	for (i = 0; i < 21; i++){
		for (j = 0; j < shuntsucnt[i]; j++){
			assert(count < 4);
			pai = (i / 7) * 9 + i % 7;
			item.mentsulist[count].category = AI_SYUNTSU;
			item.mentsulist[count].pailist[0] = pai;
			item.mentsulist[count].pailist[1] = pai + 1;
			item.mentsulist[count].pailist[2] = pai + 2;

			count++;
		}
	}

	for (i = 0; i < prm->pState->tehai.ankan_max; i++){
		assert(count < 4);
		state.nakilist[state.naki].category = AI_ANKAN;
		state.nakilist[state.naki].pailist[0] = prm->pState->tehai.ankan[0];
		state.nakilist[state.naki].pailist[1] = prm->pState->tehai.ankan[0];
		state.nakilist[state.naki].pailist[2] = prm->pState->tehai.ankan[0];
		state.naki++;
	}
	for (i = 0; i < prm->pState->tehai.minkan_max; i++){
		assert(count < 4);
		state.nakilist[state.naki].category = AI_MINKAN;
		state.nakilist[state.naki].pailist[0] = prm->pState->tehai.minkan[0];
		state.nakilist[state.naki].pailist[1] = prm->pState->tehai.minkan[0];
		state.nakilist[state.naki].pailist[2] = prm->pState->tehai.minkan[0];
		state.naki++;
	}

	for (i = 0; i < prm->pState->tehai.minkou_max; i++){
		assert(count < 4);
		state.nakilist[state.naki].category = AI_KOUTSU;
		state.nakilist[state.naki].pailist[0] = prm->pState->tehai.minkou[0];
		state.nakilist[state.naki].pailist[1] = prm->pState->tehai.minkou[0];
		state.nakilist[state.naki].pailist[2] = prm->pState->tehai.minkou[0];
		state.naki++;
	}

	for (i = 0; i < prm->pState->tehai.minshun_max; i++){
		assert(count < 4);
		state.nakilist[state.naki].category = AI_SYUNTSU;
		state.nakilist[state.naki].pailist[0] = prm->pState->tehai.minshun[0];
		state.nakilist[state.naki].pailist[1] = prm->pState->tehai.minshun[1];
		state.nakilist[state.naki].pailist[2] = prm->pState->tehai.minshun[2];
		state.naki++;
	}

	/* アタマを決定 */
	maxc = -4;
	for (m = 0; m < 34; m++){
		c = prm->pState->te_cnt[m] - cnt[m];
		if (maxc < c && prm->pState->nokori[m] > 1 - c) {
			maxc = c;
			toitsu[0] = m;
		}
	}

#if 0
	maxc = -4;

	for (m = 9; m < 18; m++){
		c = prm->pState->te_cnt[m] - cnt[m];
		if (maxc < c && prm->pState->nokori[m] > 1 - c) {
			maxc = c;
			toitsu[1] = m;
		}
	}

	maxc = -4;
	for (m = 18; m < 27; m++){
		c = prm->pState->te_cnt[m] - cnt[m];
		if (maxc < c && prm->pState->nokori[m] > 1 - c) {
			maxc = c;
			toitsu[2] = m;
		}
	}
	maxc = -4;

	for (m = 27; m < 34; m++){
		c = prm->pState->te_cnt[m] - cnt[m];
		if (maxc < c && prm->pState->nokori[m] > 1 - c) {
			maxc = c;
			toitsu[3] = m;
		}
	}
#endif

	for (m = 0; m < 1; m++){
		int koutsucnt2[34];
		int shuntsucnt2[21];
		int cnt2[34];
		RESULT_ITEM best;

		if (toitsu[m] < 0) continue;
		probability = 1.0;
		rest = prm->nokorisum;

#if 0
		for (n = 0; n < 34; n++){
			if (cnt[n] > prm->pState->te_cnt[n]) {
				cnt2[n] = cnt[n];
			}
			else{
				cnt2[n] = prm->pState->te_cnt[n];
			}
		}

		memset(koutsucnt2, 0, sizeof(koutsucnt2));
		memset(shuntsucnt2, 0, sizeof(shuntsucnt2));
		memset(&best, 0, sizeof(best));


		bestagari(prm, &best, cnt2, koutsucnt2, shuntsucnt2, 0, 0, 0, 0, toitsu[m]);

		if (best.score == 0){
			continue;
		}

		memset(koutsucnt2, 0, sizeof(koutsucnt2));
		memset(shuntsucnt2, 0, sizeof(shuntsucnt2));

		for (n = 0; n < best.mentsusize - (prm->pState->tehai.ankan_max + prm->pState->tehai.minkan_max + prm->pState->tehai.minkou_max + prm->pState->tehai.minshun_max + 1); n++){
			if (best.mentsulist[n].category == AI_SYUNTSU){
				shuntsucnt2[(best.mentsulist[n].pailist[0] / 9) * 7 + (best.mentsulist[n].pailist[0] % 9)]++;
			}
			else{
				koutsucnt2[best.mentsulist[n].pailist[0]]++;
			}
		}

		if (memcmp(koutsucnt2, koutsucnt, sizeof(koutsucnt2)) != 0 || memcmp(shuntsucnt2, shuntsucnt, sizeof(shuntsucnt2)) != 0){
			prm->count++;
			continue;
		}
#endif

		cnt[toitsu[m]] += 2;
		for (n = 0; n < 34; n++){
			if (cnt[n]){
				c = cnt[n] - prm->pState->te_cnt[n];
				assert(prm->pState->nokori[n] - c + 1 >= 0);
				int dist = paidistance(prm->pState->tehai.tehai, n);

				for (o = 0; o < c; o++){
					probability *= (double)(prm->pState->nokori[n] - o) / (double)rest;
					probability *= dist_coef[dist + 1];
					probability *= getKindCoef(prm->pState, n);

					if (prm->pState->nokori[n] - o < 0){
						probability = 0;
						break;
					}
					rest -= 1.0;
				}
			}
		}
		cnt[toitsu[m]] -= 2;

		item.mentsulist[count].category = AI_TOITSU;
		item.mentsulist[count].pailist[0] = toitsu[m];
		item.mentsulist[count].pailist[1] = toitsu[m];

		item.machi = item.mentsulist[0].category == AI_SYUNTSU ? AI_MACHI_RYANMEN : AI_MACHI_SHANPON;
		item.machihai = item.mentsulist[0].pailist[0];
		item.machipos = 0;

		make_resultitem_bh(&item, &state);
		assert(probability >= 0 && item.score >= 0);

		//value = 1.0 * probability * probability * item.score + 0.0 * probability * item.score;
		value = 0.8 * probability * probability * item.score + 0.2 * probability * item.score;

		if (prm->val < probability){
			prm->val = probability;
			sprintf(prm->debug, "%d*%d*%d*%d*%d",
				item.mentsulist[0].category * 100 + item.mentsulist[0].pailist[0],
				item.mentsulist[1].category * 100 + item.mentsulist[1].pailist[0],
				item.mentsulist[2].category * 100 + item.mentsulist[2].pailist[0],
				item.mentsulist[3].category * 100 + item.mentsulist[3].pailist[0],
				item.mentsulist[4].category * 100 + item.mentsulist[4].pailist[0]);
		}

		if (ret < value) {
			ret = value;
		}
	}

	return ret;
}

static double koutsupoint(THREAD_PARAM *prm, int *cnt, int *koutsucnt, int *shuntsucnt, int koutsunum, int shuntsunum, int koutsupos, int shuntsupos);
static double shuntsupoint(THREAD_PARAM *prm, int *cnt, int *koutsucnt, int *shuntsucnt, int koutsunum, int shuntsunum, int koutsupos, int shuntsupos);


static double shuntsupoint(THREAD_PARAM *prm, int *cnt, int *koutsucnt, int *shuntsucnt, int koutsunum, int shuntsunum, int koutsupos, int shuntsupos){
	int i;
	double ret = 0.0;
	int pai,diff = 0;

	for (i = 0; i < 34; i++){
		if (cnt[i] > prm->pState->te_cnt[i]) diff += cnt[i] - prm->pState->te_cnt[i];
	}

	if (diff > prm->shanten + 2 || diff >= 5 ) return ret;

	for (i = shuntsupos; i < 21; i++){
		pai = (i / 7) * 9 + i % 7;
		
#if 1
		/* 自分の手牌に絡まない手は除去 */
		if (prm->pState->te_cnt[pai] == 0 && prm->pState->te_cnt[pai + 1] == 0 && prm->pState->te_cnt[pai + 2] == 0) continue;
#endif

		if (prm->pState->nokori[pai] > cnt[pai] - prm->pState->te_cnt[pai]
			&& prm->pState->nokori[pai + 1] > cnt[pai + 1] - prm->pState->te_cnt[pai + 1]
			&& prm->pState->nokori[pai + 2] > cnt[pai + 2] - prm->pState->te_cnt[pai + 2]) {

#if 0
			if (prm->pState->te_cnt[pai + 1] == 0 && prm->pState->te_cnt[pai] > 0 && prm->pState->te_cnt[pai + 2] > 0){
				/* カンチャン待ちかつ両面とかぶってる場合は確率を計算しない */

				if ((pai % 9) > 0 && prm->pState->te_cnt[pai - 1] > 0) continue; // 両面とかぶってる
				if ((pai % 9) < 7 && prm->pState->te_cnt[pai + 3] > 0) continue;
			}
#endif
			cnt[pai]++;
			cnt[pai + 1]++;
			cnt[pai + 2]++;
			shuntsucnt[i]++;

			
			if (shuntsunum - 1 > 0) {
				ret += shuntsupoint(prm, cnt, koutsucnt, shuntsucnt, koutsunum, shuntsunum - 1, koutsupos, i);
			}else if (koutsunum > 0) {
				ret += koutsupoint(prm, cnt, koutsucnt, shuntsucnt, koutsunum, shuntsunum - 1, koutsupos, i);
			}
			else{
				ret += calcscore(prm, cnt, koutsucnt, shuntsucnt);
			}

			cnt[pai]--;
			cnt[pai + 1]--;
			cnt[pai + 2]--;
			shuntsucnt[i]--;

		}
	}

	return ret;
}


static double koutsupoint(THREAD_PARAM *prm, int *cnt, int *koutsucnt, int *shuntsucnt, int koutsunum, int shuntsunum, int koutsupos, int shuntsupos){
	int i;
	double ret = 0.0;
	int diff = 0;
	for (i = 0; i < 34; i++){
		if (cnt[i] > prm->pState->te_cnt[i]) diff += cnt[i] - prm->pState->te_cnt[i];
	}

	if (diff > prm->shanten + 2 || diff >= 5 ) return ret;

	for (i = koutsupos; i < 34; i++){
#if 1
		/* 自分の手牌に絡まない手は除去 */
		if (prm->pState->te_cnt[i] == 0) continue;
#endif
		if (prm->pState->nokori[i] > 2 + cnt[i] - prm->pState->te_cnt[i]) {
			cnt[i] += 3;
			koutsucnt[i]++;

			if (koutsunum - 1 > 0) {
				ret += koutsupoint(prm, cnt, koutsucnt, shuntsucnt, koutsunum - 1, shuntsunum, i + 1, shuntsupos);
			}
			else if (shuntsunum > 0) {
				ret += shuntsupoint(prm, cnt, koutsucnt, shuntsucnt, koutsunum - 1, shuntsunum, i + 1, shuntsupos);
			}
			else{
				ret += calcscore(prm, cnt, koutsucnt, shuntsucnt);
			}

			cnt[i] -= 3;
			koutsucnt[i]--;

		}
	}

	return ret;
}

/* 4つの刻子+頭 */
static DWORD WINAPI threadfunc(LPVOID pParam)
{
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	int koutsucnt[34];
	int shuntsucnt[21];
	int cnt[34];
	int nakisum;

	memset(koutsucnt, 0, sizeof(koutsucnt));
	memset(shuntsucnt, 0, sizeof(shuntsucnt));
	memset(cnt, 0, sizeof(cnt));

	nakisum = prm->pState->tehai.ankan_max + prm->pState->tehai.minkan_max + prm->pState->tehai.minkou_max + prm->pState->tehai.minshun_max;

	if (nakisum < 1) {
		prm->ret = koutsupoint(prm, cnt, koutsucnt, shuntsucnt, 4, 0, 0, 0);
	}

	return 0;
}



/* 3つの刻子+1つの順子+頭 */
static DWORD WINAPI threadfunc2(LPVOID pParam)
{
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	int koutsucnt[34];
	int shuntsucnt[21];
	int cnt[34];
	int nakisum;

	memset(koutsucnt, 0, sizeof(koutsucnt));
	memset(shuntsucnt, 0, sizeof(shuntsucnt));
	memset(cnt, 0, sizeof(cnt));

	nakisum = prm->pState->tehai.ankan_max + prm->pState->tehai.minkan_max + prm->pState->tehai.minkou_max + prm->pState->tehai.minshun_max;

	if (nakisum < 2) {
		prm->ret = koutsupoint(prm, cnt, koutsucnt, shuntsucnt, 3, 1 - nakisum, 0, 0);
	}

	return 0;
}


/* 2つの刻子+2つの順子+頭 */
static DWORD WINAPI threadfunc3(LPVOID pParam)
{
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	int koutsucnt[34];
	int shuntsucnt[21];
	int cnt[34];
	int nakisum;

	memset(koutsucnt, 0, sizeof(koutsucnt));
	memset(shuntsucnt, 0, sizeof(shuntsucnt));
	memset(cnt, 0, sizeof(cnt));

	nakisum = prm->pState->tehai.ankan_max + prm->pState->tehai.minkan_max + prm->pState->tehai.minkou_max + prm->pState->tehai.minshun_max;

	if (nakisum < 3) {
		prm->ret = koutsupoint(prm, cnt, koutsucnt, shuntsucnt, 2, 2 - nakisum, 0, 0);
	}

	return 0;
}

/* 1つの刻子+3つの順子+頭 */
static DWORD WINAPI threadfunc4(LPVOID pParam)
{
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	int koutsucnt[34];
	int shuntsucnt[21];
	int cnt[34];
	int nakisum;

	memset(koutsucnt, 0, sizeof(koutsucnt));
	memset(shuntsucnt, 0, sizeof(shuntsucnt));
	memset(cnt, 0, sizeof(cnt));

	nakisum = prm->pState->tehai.ankan_max + prm->pState->tehai.minkan_max + prm->pState->tehai.minkou_max + prm->pState->tehai.minshun_max;

	if (nakisum < 4) {
		prm->ret = koutsupoint(prm, cnt, koutsucnt, shuntsucnt, 1, 3 - nakisum, 0, 0);
	}

	return 0;
}

/* 4つの順子+頭 */
static DWORD WINAPI threadfunc5(LPVOID pParam)
{
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	int koutsucnt[34];
	int shuntsucnt[21];
	int cnt[34];
	int nakisum;

	memset(koutsucnt, 0, sizeof(koutsucnt));
	memset(shuntsucnt, 0, sizeof(shuntsucnt));
	memset(cnt, 0, sizeof(cnt));

	nakisum = prm->pState->tehai.ankan_max + prm->pState->tehai.minkan_max + prm->pState->tehai.minkou_max + prm->pState->tehai.minshun_max;

	if (nakisum >= 4) {
		prm->ret = calcscore(prm, cnt, koutsucnt, shuntsucnt);
	} else{
		prm->ret = shuntsupoint(prm, cnt, koutsucnt, shuntsucnt, 0, 4 - nakisum, 0, 0);
	}

	return 0;
}



double chiitoipoint(THREAD_PARAM *prm, int cnt,unsigned long long tehaibitmap,unsigned long long bitmap,int pos){
	double rest = 0.0;
	double probability = 1.0;
	RESULT_ITEM item;
	GAMESTATE state;
	int i,num;
	double sum = 0.0;
	int dist;

	if (cnt == 7){
		for (i = 0; i<34; i++){
			rest += prm->pState->nokori[i];
		}
		memset(&state, 0, sizeof(state));
		memset(&item, 0, sizeof(item));

		state.tsumo = 1;
		state.bakaze = prm->pState->kyoku / 4;
		state.zikaze = prm->pState->cha;
		state.count = 1;


		for (i = 0,num = 0; i < 34; i++){
			if (tehaibitmap & (1ULL << i)) {
				item.mentsulist[num].category = AI_TOITSU;
				item.mentsulist[num].pailist[0] = i;
				item.mentsulist[num].pailist[1] = i;
				num++;
			}
			else if (bitmap & (1ULL << i)) {
				dist = paidistance(prm->pState->tehai.tehai, i);
				for (int c = 0; c < 2 - prm->pState->te_cnt[i]; c++){
					probability *= (double)(prm->pState->nokori[i] - c) / (double)rest;
					probability *= dist_coef[dist + 1];
					probability *= getKindCoef(prm->pState, i);
					rest -= 1.0;
				}
				item.mentsulist[num].category = AI_TOITSU;
				item.mentsulist[num].pailist[0] = i;
				item.mentsulist[num].pailist[1] = i;
				num++;
			}
		}

		item.han = 0;
		item.fu = 0;
		item.mentsusize = 5;
		item.menzen = 1;
		item.machipos = 0;

		item.machi = AI_MACHI_TANKI;
		item.machihai = item.mentsulist[0].pailist[0];
		item.machipos = 0;
		make_resultitem_bh(&item, &state);

		if (probability >= 0) {
			return 1.0 * (0.8 * probability * probability * item.score + 0.2 * probability * item.score);
		}
		else{
			return 0;
		}
	}
	else{
		for (i = pos; i < 34; i++){
			if (prm->pState->nokori[i] > 1 - prm->pState->te_cnt[i] && ((tehaibitmap & (1ULL << i)) == 0) ){
				sum += chiitoipoint(prm, cnt + 1, tehaibitmap,bitmap | (1ULL << i) ,i+1);
			}
		}

		return sum;
	}
}


/* 七対子 */
static DWORD WINAPI threadfunc6(LPVOID pParam)
{
	int count;
	THREAD_PARAM *prm = (THREAD_PARAM*)pParam;
	int toitsunum = 0;
	unsigned long long bitmap = 0;

	if (prm->pState->tehai.ankan_max > 0 || prm->pState->tehai.minkan_max > 0 || prm->pState->tehai.minkou_max > 0 || prm->pState->tehai.minshun_max > 0) return 0;

	for (count = 0; count<34; count++){
		if (prm->pState->te_cnt[count] >= 2) {
			bitmap |= 1ULL << count;
			toitsunum++;
		}
	}

	if (toitsunum >= 3) {
		prm->ret =  chiitoipoint(prm, toitsunum, bitmap,0,0);
	}

	return 0;
}

//#define SINGLETHREAD
#define THREADNUM (6)

double MahjongAIType4::evalSutehaiSub(MahjongAIState &param,int hai)
{
	double sum = 0.0;
	DWORD dwID;
	THREAD_PARAM tparam[THREADNUM];
	HANDLE hThread[THREADNUM];
	int i,j;
	char message[256];
	double nokorisum = 0.0;
	int tehai[14];
	int tehainum = 0;
	TENPAI_LIST list;
	int machi[34];

	for (i = 0; i < 34; i++){
		for (j = 0; j < param.te_cnt[i]; j++){
			tehai[tehainum] = i;
			tehainum++;
		}
	}
	search_tenpai(tehai,tehainum,machi,&list,1,6);

	for (i = 0; i<34; i++){
		nokorisum += param.nokori[i];
	}

	for(i=0;i<THREADNUM;i++){
		tparam[i].pState = &param;
		tparam[i].hai = hai;
		tparam[i].ret = 0.0;
		tparam[i].val = 0.0;
		tparam[i].nokorisum = nokorisum;
		tparam[i].count = 0;
		tparam[i].shanten = list.shanten;
		tparam[i].debug[0] = '\0';
	}

#ifdef SINGLETHREAD
	threadfunc(&tparam[0]);
	threadfunc2(&tparam[1]);
	threadfunc3(&tparam[2]);
	threadfunc4(&tparam[3]);
	threadfunc5(&tparam[4]);
	threadfunc6(&tparam[5]);
#else
	memset(hThread,0,sizeof(hThread));

	hThread[0] = (HANDLE)CreateThread(NULL,0,threadfunc,&tparam[0],0,&dwID);
	hThread[1] = (HANDLE)CreateThread(NULL,0,threadfunc2,&tparam[1],0,&dwID);
	hThread[2] = (HANDLE)CreateThread(NULL,0,threadfunc3,&tparam[2],0,&dwID);
	hThread[3] = (HANDLE)CreateThread(NULL,0,threadfunc4,&tparam[3],0,&dwID);
	hThread[4] = (HANDLE)CreateThread(NULL,0,threadfunc5,&tparam[4],0,&dwID);
	hThread[5] = (HANDLE)CreateThread(NULL, 0, threadfunc6, &tparam[5], 0, &dwID);

	WaitForMultipleObjects(THREADNUM, hThread, TRUE, INFINITE);
#endif

	for(i=0;i<THREADNUM;i++){
		sum += tparam[i].ret;
	}

#ifdef _DEBUG
	sprintf(message, "[%d] %.2lf (%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf)\r\n", hai, sum, tparam[0].ret, tparam[1].ret, tparam[2].ret, tparam[3].ret, tparam[4].ret,tparam[5].ret);

	MJSendMessage(MJMI_FUKIDASHI,(UINT)message,0);
	sprintf(message, "           (%d,%d,%d,%d,%d,%d)\r\n", tparam[0].count, tparam[1].count, tparam[2].count, tparam[3].count, tparam[4].count, tparam[5].count);

	MJSendMessage(MJMI_FUKIDASHI, (UINT)message, 0);
	sprintf(message, "           (%.2le,%.2le,%.2le,%.2le,%.2le,%.2le)\r\n", tparam[0].val, tparam[1].val, tparam[2].val, tparam[3].val, tparam[4].val, tparam[5].val);

	MJSendMessage(MJMI_FUKIDASHI, (UINT)message, 0);
	sprintf(message, "           (%s,%s,%s,%s,%s,%s)\r\n", tparam[0].debug, tparam[1].debug, tparam[2].debug, tparam[3].debug, tparam[4].debug, tparam[5].debug);

	MJSendMessage(MJMI_FUKIDASHI, (UINT)message, 0);
#endif

	return sum;
	
}

UINT MahjongAIType4::getDebugInt()
{
	return 0;//thread_count[0] + thread_count[1] + thread_count[2] + thread_count[3] + thread_count[4];
}
