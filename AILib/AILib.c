#include <string.h>
#include "AILib.h"
#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------- */
/* あがりの形を考える部分 */

/* 刻子かどうかを調べる */
static int iskoutsu(PAICOUNT *paicountlist,int length)
{
	int i;
	for(i=0;i<length;i++){
		if (paicountlist[i].count >= 3){
	        return i;
		}
	}
    return -1;
}

static int iskoutsul(PAICOUNT *paicountlist,int length)
{
	int i;
	for(i=length-1;i>=0;i--){
		if (paicountlist[i].count >= 3){
	        return i;
		}
	}
    return -1;
}

/* 順子かどうかを調べる(右方向のみ) */
static int issyuntsu(PAICOUNT *paicountlist,int length)
{
    int pai, nextpai,nnextpai;
    int i;
	if (length < 3) return -1;

    for (i = 0; i < length-2; i++) {
        pai = paicountlist[i].pai;
        nextpai = paicountlist[i+1].pai;
        nnextpai = paicountlist[i+2].pai;
		if( pai > 26)
            return -1;
        if ((paicountlist[i].count == 0) || (paicountlist[i+1].count == 0) || (paicountlist[i+2].count == 0))
            continue;
		if((pai % 9) >= 7)
			continue;

		if(((pai/9) != (nextpai/9)) || ((pai/9) != (nnextpai/9))) continue;
        if (pai == nextpai - 1 && pai == nnextpai - 2) return i;
    }
    return -1;
}
/* 順子かどうかを調べる（左方向） */
static int issyuntsul(PAICOUNT *paicountlist,int length){
	int i,pai,nextpai,nnextpai;
	if (length < 3) return -1;
	for(i=length-1;i>=2;i--){
		pai = paicountlist[i].pai;
		nextpai = paicountlist[i-1].pai;
		nnextpai = paicountlist[i-2].pai;
		if (pai>26) continue;
		if ((paicountlist[i].count == 0) || (paicountlist[i-1].count == 0) || (paicountlist[i-2].count == 0)) continue;
		if(((pai/9) != (nextpai/9)) || ((pai/9) != (nnextpai/9))) continue;
		if (pai == nextpai + 1 && pai == nnextpai + 2) return i-2;
	}
	
	return -1;
}

/* 十三不塔/国士無双かどうか調べる */
static int search_siisan(PAICOUNT *paicountlist)
{
    int pai, nextpai;
    
    while (paicountlist->pai != AI_NIL_PAI) {
        pai     = paicountlist->pai;
        nextpai = (paicountlist + 1)->pai;
        if (nextpai == AI_NIL_PAI)
            return AI_TRUE;
        if (paicountlist->count >= 2)
            return AI_FALSE;
        if ((pai > 26) &&
            ((pai == nextpai - 1) ||
             (pai == nextpai - 2))) {
            return AI_FALSE;
        }
        paicountlist++;
    }
    return AI_TRUE;
}

/* 探索を開始する面子の初期状態を設定 */
static int setstartmentsu(int *paiarray, int *mentsu,int atamapos)
{
    int i, paicountlistcount, pos, koutsuflag, syuntsuflag;
    PAICOUNT paicountlist[AI_TEHAI_LIMIT + 1];

    /* 最初の面子に頭を設定 */
    for (i = 0; i < AI_TEHAI_LIMIT; i++)
        mentsu[i] = AI_UKIHAI;
    mentsu[atamapos] = AI_ATAMA;
    mentsu[atamapos+1] = AI_ATAMA;

    /* 牌の集計を取る */
    paicountlistcount = 0;
    pos = 0;

    paicountlist[pos].pai = AI_NIL_PAI;
    for (i = 0; i < AI_TEHAI_LIMIT; i++) {
        /* 頭は集計に入れない */
        if ((i == atamapos) || (i == atamapos + 1))
            continue;
        if ((paicountlist[pos].pai != AI_NIL_PAI) &&
            (paicountlist[pos].pai == paiarray[i])) {
            paicountlist[pos].count++;
        } else {
            paicountlistcount++;
            if (paicountlist[pos].pai != AI_NIL_PAI)
                pos++;
            paicountlist[pos].pai = paiarray[i];
            paicountlist[pos].count = 1;
            paicountlist[pos].startpos = i;
        }
    }
    paicountlist[pos + 1].pai = AI_NIL_PAI;

#if 0
    /* 十三不塔/国士無双か調べる */
    if (search_siisan(paicountlist)) {
        /* そうなら設定して終了 */
        return AI_TRUE;
    }
#endif

    /* 探索を始めるために確定しているところを決める */
    for (i = 0; i < paicountlistcount; i++) {
        
        koutsuflag  = iskoutsu(paicountlist + i,1) >= 0;
        syuntsuflag = issyuntsu(paicountlist + i,3) >= 0;
        
        /* 今の位置が刻子で順子でなければ確定 */
        if ((koutsuflag) && (!syuntsuflag)) {
            if ((i >= 1) && (issyuntsu(paicountlist + (i - 1),3) >= 0))
                continue;
            if ((i >= 2) && (issyuntsu(paicountlist + (i - 2),3) >= 0))
                continue;

            pos = paicountlist[i].startpos;
            mentsu[pos] = AI_KOUTSU;
            mentsu[pos + 1] = AI_KOUTSU;
            mentsu[pos + 2] = AI_KOUTSU;
            continue;
        }

#if 0
        /* 今の位置が順子でその3つの牌が刻子でなければ確定 */
        if ((!koutsuflag) && (syuntsuflag)) {
            if (i <= paicountlistcount - 3) {
                if (iskoutsu(paicountlist + i + 1))
                    continue;
                if (iskoutsu(paicountlist + i + 2))
                    continue;
    		    if ((paicountlist + i)->count >= 2)
                    continue;
                pos = paicountlist[i].startpos;
                mentsu[pos] = AI_SYUNTSU_START;
                paicountlist[i].count--;
                paicountlist[i].startpos++;
                pos = paicountlist[i + 1].startpos;
                mentsu[pos] = AI_SYUNTSU_MIDDLE;
                paicountlist[i + 1].count--;
                paicountlist[i + 1].startpos++;
                pos = paicountlist[i + 2].startpos;
                mentsu[pos] = AI_SYUNTSU_END;
                paicountlist[i + 2].count--;
                paicountlist[i + 2].startpos++;
                continue;
            }
        }
#endif
    }
    return 0;
}

/* 牌の数を数える */
int setpaicount(PAICOUNT *paicountlist,int *paiarray,int paiSize, int *mentsu)
{
    int pos, i;

    pos = 0;
    paicountlist[pos].pai = AI_NIL_PAI;
    for (i = 0; i < paiSize; i++) {
        /* 浮き牌以外は集計に入れない */
        if (mentsu[i] != AI_UKIHAI)
            continue;
        if ((paicountlist[pos].pai != AI_NIL_PAI) &&
            (paicountlist[pos].pai == paiarray[i])) {
            paicountlist[pos].count++;
        } else {
            if (paicountlist[pos].pai != AI_NIL_PAI)
                pos++;
            paicountlist[pos].pai = paiarray[i];
            paicountlist[pos].count = 1;
            paicountlist[pos].startpos = i;
        }
    }
    if (paicountlist[0].pai != AI_NIL_PAI) {
        pos++;
        paicountlist[pos].pai = AI_NIL_PAI;
    }
    return pos;
}

static void set_machi(int *machi,int pai){
	machi[pai] = AI_FLAG_EFFECT_ATAMA;
	if(pai > 26){
		/* 字牌 */
	}else if((pai % 9) == 0){
		machi[pai + 1] = AI_FLAG_EFFECT_PENTAH;
		machi[pai + 2] = AI_FLAG_EFFECT_KANTAH;
	}else if((pai % 9) == 1){
		machi[pai - 1] = AI_FLAG_EFFECT_PENTAH;
		machi[pai + 1] = AI_FLAG_EFFECT_RYANTAH;
		machi[pai + 2] = AI_FLAG_EFFECT_KANTAH;
	}else if((pai % 9) == 7){
		machi[pai - 2] = AI_FLAG_EFFECT_KANTAH;
		machi[pai - 1] = AI_FLAG_EFFECT_RYANTAH;
		machi[pai + 1] = AI_FLAG_EFFECT_PENTAH;
	}else if((pai % 9) == 8){
		machi[pai - 2] = AI_FLAG_EFFECT_KANTAH;
		machi[pai - 1] = AI_FLAG_EFFECT_PENTAH;
	}else{
		machi[pai - 2] = AI_FLAG_EFFECT_KANTAH;
		machi[pai - 1] = AI_FLAG_EFFECT_RYANTAH;
		machi[pai + 1] = AI_FLAG_EFFECT_RYANTAH;
		machi[pai + 2] = AI_FLAG_EFFECT_KANTAH;
	}
}

static int search_tahtsu(PAICOUNT *paicountlist,int size,TENPAI_LIST *machilist,int atamahai){
	int i;
	int separate = 0;
	int numtah = 0;

	for(i=0;i<size;i++){
		if(paicountlist[i].count == 2){
			machilist->machi[paicountlist[i].pai] = AI_FLAG_EFFECT_KOUTSU;
			separate = 1;
			numtah++;
		}else if(atamahai >= 34){
			machilist->machi[paicountlist[i].pai] = AI_FLAG_EFFECT_ATAMA;
		}

		/* もう面子になっているものとの順子形成が残っているがそれは後で＾＾ */

		/* アタマ候補牌との順子形成 */
		if(atamahai <= 26 && (paicountlist[i].pai/9 == atamahai/9)){
			if((paicountlist[i].pai % 9) + 1 == (atamahai % 9)){
				if((paicountlist[i].pai % 9) == 0){
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
				}else if((paicountlist[i+1].pai % 9) == 8){
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}else{
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}
			}else if((paicountlist[i].pai % 9) + 2 == (atamahai % 9)){
				machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
			}else if((atamahai % 9) + 1 ==(paicountlist[i].pai % 9)){
				if((atamahai % 9) == 0){
					machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
				}else if((paicountlist[i].pai % 9) == 8){
					machilist->machi[paicountlist[i].pai - 2] = AI_FLAG_EFFECT_SYUNTSU;
				}else{
					machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
					machilist->machi[paicountlist[i].pai - 2] = AI_FLAG_EFFECT_SYUNTSU;
				}
			}else if((atamahai % 9) + 2 == (paicountlist[i].pai % 9)){
				machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
			}
		}

		if(i<size-1){
			if(paicountlist[i].pai > 26 || (paicountlist[i].pai/9 != paicountlist[i+1].pai/9)){
				separate = 0;
			}else if((paicountlist[i].pai % 9) + 1 == (paicountlist[i+1].pai % 9)){
				if((paicountlist[i].pai % 9) == 0){
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
				}else if((paicountlist[i].pai % 9) == 8){
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}else{
					machilist->machi[paicountlist[i].pai + 2] = AI_FLAG_EFFECT_SYUNTSU;
					machilist->machi[paicountlist[i].pai - 1] = AI_FLAG_EFFECT_SYUNTSU;
				}
				if(separate == 0){
					numtah++;
					separate = 1;
				}else{
					separate = 0;
				}
			}else if((paicountlist[i].pai % 9) + 2 == (paicountlist[i+1].pai % 9)){
				machilist->machi[paicountlist[i].pai + 1] = AI_FLAG_EFFECT_SYUNTSU;
				if(separate == 0){
					numtah++;
					separate = 1;
				}else{
					separate = 0;
				}
			}

		}else{
			separate = 0;
		}

	}

	return numtah;
}

static int search_shanten_atamaless(PAICOUNT *paicountlist,int size,TENPAI_LIST *machilist){
	int i;
	int count = 0;
	int numtah;

	for(i=0;i<size;i++){
		count += paicountlist[i].count;
		set_machi(machilist->machi,paicountlist[i].pai);
	}

	if(count == 4){
		numtah = search_tahtsu(paicountlist,size,machilist,63);
		if(numtah == 2 || numtah == 1){
			machilist->shanten = 1;
		}else{
			machilist->shanten = 2;
		}
	}else if(count == 7){
		numtah = search_tahtsu(paicountlist,size,machilist,63);
		if(numtah == 3 || numtah == 2){
			machilist->shanten = 2;
		}else if(numtah == 1){
			machilist->shanten = 3;
		}else{
			machilist->shanten = 4;
		}
	}else if(count == 10){
		numtah = search_tahtsu(paicountlist,size,machilist,63);
		if(numtah == 5 || numtah == 4 || numtah == 3){
			machilist->shanten = 3;
		}else if(numtah == 2){
			machilist->shanten = 4;
		}else if(numtah == 1){
			machilist->shanten = 5;
		}else{
			machilist->shanten = 6;
		}
	}else{
		numtah = search_tahtsu(paicountlist,size,machilist,63);
		if(numtah == 6 || numtah == 5 || numtah == 4){
			machilist->shanten = 4;
		}else if(numtah == 3){
			machilist->shanten = 5;
		}else if(numtah == 2){
			machilist->shanten = 6;
		}else if(numtah == 1){
			machilist->shanten = 7;
		}else{
			machilist->shanten = 8;
		}
	}

	return machilist->shanten;
}

static int search_shanten(PAICOUNT *paicountlist,int size,TENPAI_LIST *machilist,int atamahai){
	int i;
	int count = 0;
	int numtah;

	for(i=0;i<size;i++){
		count += paicountlist[i].count;
		set_machi(machilist->machi,paicountlist[i].pai);
	}

	machilist->machi[atamahai] = AI_FLAG_EFFECT_KOUTSU;

	if(count == 2){
		/* イーシャンテン */
		machilist->shanten = 1;
	}else if(count == 5){
		numtah = search_tahtsu(paicountlist,size,machilist,atamahai);
		if(numtah == 2){
			machilist->shanten = 1;
		}else if(numtah == 1){
			machilist->shanten = 2;
		}else{
			machilist->shanten = 3;
		}
	}else if(count == 8){
		numtah = search_tahtsu(paicountlist,size,machilist,atamahai);
		if(numtah == 4 || numtah == 3){
			machilist->shanten = 2;
		}else if(numtah == 2){
			machilist->shanten = 3;
		}else if(numtah == 1){
			machilist->shanten = 4;
		}else{
			machilist->shanten = 5;
		}
	}else{
		numtah = search_tahtsu(paicountlist,size,machilist,atamahai);
		if(numtah == 5 || numtah == 4){
			machilist->shanten = 3;
		}else if(numtah == 3){
			machilist->shanten = 4;
		}else if(numtah == 2){
			machilist->shanten = 5;
		}else if(numtah == 1){
			machilist->shanten = 6;
		}else{
			machilist->shanten = 7;
		}
	}

	return machilist->shanten;

}

static int istenpai(PAICOUNT *paicountlist,int size,TENPAI_LIST *machilist,int atamahai){
	if (size == 1){
		if (paicountlist[0].count == 2){
			/*シャンポン待ち*/
			machilist->shanten = 0;
			machilist->machi[atamahai] = AI_FLAG_MACHI;
			machilist->machi[paicountlist[0].pai] = AI_FLAG_MACHI;
			return AI_MACHI_SHANPON;
		}else{
			return -1;
		}
	}else if(size == 2){
		if(paicountlist[0].count == 1 && paicountlist[1].count == 1 && paicountlist[0].pai/9 == paicountlist[1].pai/9 &&
			paicountlist[0].pai<=26){
			if(paicountlist[0].pai + 1 == paicountlist[1].pai){
				if(paicountlist[0].pai % 9 == 0){
					// ペンチャン待ち
					machilist->shanten = 0;
					machilist->machi[paicountlist[0].pai + 2] = AI_FLAG_MACHI;
					return AI_MACHI_PENCHAN;
				}else if (paicountlist[1].pai % 9 == 8){
					// ペンチャン待ち
					machilist->shanten = 0;
					machilist->machi[paicountlist[0].pai - 1] = AI_FLAG_MACHI;
					return AI_MACHI_PENCHAN;
				}else{
					// 両面待ち
					machilist->shanten = 0;
					machilist->machi[paicountlist[0].pai - 1] = AI_FLAG_MACHI;
					machilist->machi[paicountlist[0].pai + 2] = AI_FLAG_MACHI;
					return AI_MACHI_RYANMEN;
				}
			}else if(paicountlist[0].pai + 2 == paicountlist[1].pai){
				// カンチャン待ち
				machilist->shanten = 0;
				machilist->machi[paicountlist[0].pai + 1] = AI_FLAG_MACHI;
				return AI_MACHI_KANCHAN;
			}else{
				return -1;
			}

		}
	}

	return -1;
}


/* 浮き牌があるか返す */
static int isukihai(PAICOUNT *paicountlist)
{
    if (paicountlist[0].pai == AI_NIL_PAI) {
        return AI_FALSE;
    }
    return AI_TRUE;
}

/* 刻子をセットする */
static int setkoutsu(PAICOUNT *paicountlist, int *mentsu,int length)
{
    int pos;
	int listpos;
    
    if ((listpos = iskoutsu(paicountlist,length)) >= 0) {
        pos = paicountlist[listpos].startpos;
        mentsu[pos] = AI_KOUTSU;
        mentsu[pos + 1] = AI_KOUTSU;
        mentsu[pos + 2] = AI_KOUTSU;
        return AI_TRUE;
    }
    return AI_FALSE;
}

/* 刻子をセットする(左方向)*/
static int setkoutsul(PAICOUNT *paicountlist,int *mentsu,int length){
	int pos;
	int listpos;

    if ((listpos = iskoutsul(paicountlist,length)) >= 0) {
		pos = paicountlist[listpos].startpos;
		mentsu[pos] = AI_KOUTSU;
		mentsu[pos + 1] = AI_KOUTSU;
		mentsu[pos + 2] = AI_KOUTSU;
		return AI_TRUE;
	}
	return AI_FALSE;
}

/* 順子をセットする */
int setsyuntsu(PAICOUNT *paicountlist, int *mentsu,int length)
{
    int pos;
	int listpos;
    
    if ((listpos = issyuntsu(paicountlist,length)) >= 0) {
        pos = paicountlist[listpos].startpos;
        mentsu[pos] = AI_SYUNTSU_START;
        pos = paicountlist[listpos + 1].startpos;
        mentsu[pos] = AI_SYUNTSU_MIDDLE;
        pos = paicountlist[listpos + 2].startpos;
        mentsu[pos] = AI_SYUNTSU_END;
		if(length < 4 || (paicountlist[listpos + 2].pai + 1 != paicountlist[listpos + 3].pai)){
			return 1;
		}else{
			return 2;
		}
    }
    return AI_FALSE;
}

/* 順子をセットする(左方向)*/
static int setsyuntsul(PAICOUNT *paicountlist,int *mentsu,int length){
	int pos;
	int listpos;

	if ((listpos = issyuntsul(paicountlist,length)) >= 0){
		pos = paicountlist[listpos].startpos;
		mentsu[pos] = AI_SYUNTSU_START;
		pos = paicountlist[listpos + 1].startpos;
		mentsu[pos] = AI_SYUNTSU_MIDDLE;
		pos = paicountlist[listpos + 2].startpos;
		mentsu[pos] = AI_SYUNTSU_END;
		return AI_TRUE;
	}
	return AI_FALSE;
}

/* 対子がセットできるかどうか調べる */
static int istoitsu(PAICOUNT *paicountlist, int atamapai)
{
    int count;
    
    count = 0;
    while (paicountlist->pai != AI_NIL_PAI) {
        if (paicountlist->count != 2)
            return AI_FALSE;
        if ((paicountlist->pai == atamapai))
            return AI_FALSE;
        
        paicountlist++;
        count++;
    }
    if (count >= 6)
	    return AI_TRUE;
    return AI_FALSE;
}

/* 対子をセットする */
static int settoitsu(PAICOUNT *paicountlist, int *mentsu)
{
    int pos;
    
    while (paicountlist->pai != AI_NIL_PAI) {
        pos = paicountlist->startpos;
        mentsu[pos] = AI_TOITSU;
        mentsu[pos + 1] = AI_TOITSU;
        paicountlist++;
    }
    return AI_TRUE;
}

static int isatamamachi(PAICOUNT *paicountlist,int size,TENPAI_LIST *machilist){
	if(size == 1 && paicountlist[0].count == 1){
		machilist->shanten = 0;
		machilist->machi[paicountlist[0].pai] = AI_FLAG_MACHI;
		return AI_MACHI_TANKI;
	}else{
		return -1;
	}
}

int search_tenpai(int *paiarray,int paiSize,int *pMachi,TENPAI_LIST *pList,int listSize,int maxshanten)
{
    PAICOUNT paicountlist[AI_TEHAI_LIMIT + 1];
	int notanki = 0;
    int atamaque[AI_TEHAI_LIMIT];
    int atamaque_count, atamapos;
    int mentsu_stack[256][AI_TEHAI_LIMIT];
    int mentsu[AI_TEHAI_LIMIT];
    int stackpos;
    int i, j, initresult, flag,orflag;
	int paicount_size;
	int resultcount;
	int maxval,maxind;
	TENPAI_LIST tenpai_list;

	if(pList != NULL && listSize > 0) memset(pList,0,sizeof(TENPAI_LIST) * listSize);
	memset(&tenpai_list,0,sizeof(tenpai_list));
	memset(mentsu_stack,0,sizeof(mentsu_stack));
	memset(mentsu,0,sizeof(mentsu));
	if(pMachi != NULL) memset(pMachi,0,sizeof(int)*34);
	
    /* 頭の候補を得る */
    resultcount = 0;
    atamaque_count = 0;

	if(paiSize >= 4){
		i = 0;
		while(i<paiSize-1){
			if(paiarray[i] == paiarray[i + 1]){
				atamaque[atamaque_count] = i;
				mentsu[i] = AI_ATAMA;
				mentsu[i+1] = AI_ATAMA;
				atamaque_count++;
				i++;
			}
			i++;
		}

		/* 七対子の判定 */

		if(atamaque_count == 6){
			for(i=0;i<paiSize;i++){
				if(mentsu[i] == AI_UKIHAI){
					tenpai_list.machi[paiarray[i]] = AI_FLAG_MACHI;
				}
			}
			tenpai_list.shanten = 0;
		}else{
			for(i=0;i<paiSize;i++){
				if(mentsu[i] == AI_UKIHAI){
					tenpai_list.machi[paiarray[i]] = AI_FLAG_EFFECT_ATAMA;
				}
			}

			tenpai_list.shanten = 6 - atamaque_count;
		}
		memcpy(&tenpai_list.mentsuflag,mentsu,AI_TEHAI_LIMIT*sizeof(int));
		if(pList != NULL && tenpai_list.shanten < maxshanten){
			if(resultcount == listSize){
				maxval = 0;
				maxind = 0;
				for(j=0;j<listSize;j++){
					if(pList[j].shanten > maxval){
						maxval = pList[j].shanten;
						maxind = j;
					}
				}
				
				if(maxval > tenpai_list.shanten){
					pList[maxind] = tenpai_list;
				}
			}else{
				pList[resultcount] = tenpai_list;
				resultcount++;
			}
		}

		if(pMachi != NULL && tenpai_list.shanten == 0){
			for(j=0;j<34;j++){
				pMachi[j] = pMachi[j] || tenpai_list.machi[j];
			}
		}

		memset(&tenpai_list,0,sizeof(tenpai_list));
	}

   
	
    /* 頭として取ったあとで刻子/順子などの面子の状況を見る(両面、カンチャン、ペンチャン、シャンポン待ち) */
    for (i = 0; i < atamaque_count; i++) {
        atamapos = atamaque[i];
		
        /* 探索を開始する面子の初期状態を設定 */
        initresult = setstartmentsu(paiarray, mentsu_stack[0],atamapos);
        if (initresult != 0)
            return initresult;
        	
        /* 探索 */
        stackpos = 0;
        do {
            paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
            if (istenpai(paicountlist,paicount_size,&tenpai_list,paiarray[atamapos]) < 0) {  /* 浮き牌がまだある */
				orflag = AI_FALSE;
                memcpy(mentsu, mentsu_stack[stackpos], AI_TEHAI_LIMIT * sizeof(int));
                flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, AI_TEHAI_LIMIT * sizeof(int));
                }

                flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, AI_TEHAI_LIMIT * sizeof(int));
                }

                flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, AI_TEHAI_LIMIT * sizeof(int));
                }

				if(flag != 1){
					flag = setsyuntsul(paicountlist,mentsu_stack[stackpos],paicount_size);
					orflag = orflag || flag;
					if (flag) {
						stackpos++;
					}
				}

				if(!orflag && maxshanten > 0){
					/* 探索終了 */
					memcpy(&tenpai_list.mentsuflag,mentsu_stack[stackpos],AI_TEHAI_LIMIT*sizeof(int));
					search_shanten(paicountlist,paicount_size,&tenpai_list,paiarray[atamapos]);
					if(pList != NULL && tenpai_list.shanten <= maxshanten){
						if(resultcount == listSize){
							maxval = 0;
							maxind = 0;
							for(j=0;j<listSize;j++){
								if(pList[j].shanten > maxval){
									maxval = pList[j].shanten;
									maxind = j;
								}
							}

							if(maxval > tenpai_list.shanten){
								pList[maxind] = tenpai_list;
							}
						}else{
							pList[resultcount] = tenpai_list;
							resultcount++;
						}
					}

					memset(&tenpai_list,0,sizeof(tenpai_list));
				}

            } else {
				memcpy(&tenpai_list.mentsuflag,mentsu_stack[stackpos],AI_TEHAI_LIMIT*sizeof(int));
				if(pMachi != NULL){
					for(j=0;j<34;j++){
						pMachi[j] = pMachi[j] || tenpai_list.machi[j];
					}
				}
				if(pList != NULL){
					if(resultcount == listSize){
						maxval = 0;
						maxind = 0;
						for(j=0;j<listSize;j++){
							if(pList[j].shanten > maxval){
								maxval = pList[j].shanten;
								maxind = j;
							}
						}
						pList[maxind] = tenpai_list;
					}else{
						pList[resultcount] = tenpai_list;
						resultcount++;
					}
				}
				memset(&tenpai_list,0,sizeof(tenpai_list));

            }
            stackpos--;
        } while(stackpos >= 0);
    }

	memset(mentsu_stack,0,sizeof(mentsu_stack));

	/* 探索 */
	stackpos = 0;
	do {
		paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
		if (isatamamachi(paicountlist,paicount_size,&tenpai_list) < 0) {  /* 浮き牌がまだある */
			orflag = AI_FALSE;
			memcpy(mentsu, mentsu_stack[stackpos], AI_TEHAI_LIMIT * sizeof(int));
			flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, AI_TEHAI_LIMIT * sizeof(int));
			}
			
			flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, AI_TEHAI_LIMIT * sizeof(int));
			}
			
			flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, AI_TEHAI_LIMIT * sizeof(int));
			}
			
			if(flag != 1){
				flag = setsyuntsul(paicountlist,mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
				}
			}
			
			if(!orflag && maxshanten > 0){
				/* 探索終了 */
				memcpy(&tenpai_list.mentsuflag,mentsu_stack[stackpos],AI_TEHAI_LIMIT*sizeof(int));
				search_shanten_atamaless(paicountlist,paicount_size,&tenpai_list);
				if(pList != NULL && tenpai_list.shanten <= maxshanten){
					if(resultcount == listSize){
						maxval = 0;
						maxind = 0;
						for(j=0;j<listSize;j++){
							if(pList[j].shanten > maxval){
								maxval = pList[j].shanten;
								maxind = j;
							}
						}
						
						if(maxval > tenpai_list.shanten){
							pList[maxind] = tenpai_list;
						}
					}else{
						pList[resultcount] = tenpai_list;
						resultcount++;
					}
				}
				
				memset(&tenpai_list,0,sizeof(tenpai_list));
			}
			
		} else {
			memcpy(&tenpai_list.mentsuflag,mentsu_stack[stackpos],AI_TEHAI_LIMIT*sizeof(int));

			if(pMachi != NULL){
				for(j=0;j<34;j++){
					pMachi[j] = pMachi[j] || tenpai_list.machi[j];
				}
			}

			if(pList != NULL){ 
				if(resultcount == listSize){
					maxval = 0;
					maxind = 0;
					for(j=0;j<listSize;j++){
						if(pList[j].shanten > maxval){
							maxval = pList[j].shanten;
							maxind = j;
						}
					}
					pList[maxind] = tenpai_list;
				}else{
					pList[resultcount] = tenpai_list;
					resultcount++;
				}
			}
			memset(&tenpai_list,0,sizeof(tenpai_list));
			
		}
		stackpos--;
	} while(stackpos >= 0);


    return resultcount;
	
}

double probabilityFunction(double base,int n){
	double d = 1.0 - base;
	int i;
	double p = 1.0;

	for(i=0;i<n;i++){
		p *= d;
	}

	return 1.0 - p;
}

int permutation(int m,int n){
	int i;
	int num = 1;
	for(i=1;i<n;i++){
		num *= m;
		m--;
	}

	return num;

}

int factorial(int n){
	int i;
	int num = 1;
	for(i=2;i<=n;i++){
		num *= i;
	}

	return num;
}

int combination(int m,int n){
	return permutation(m,n)/factorial(n);
}

#ifdef __cplusplus
}
#endif