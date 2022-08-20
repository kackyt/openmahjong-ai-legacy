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
#include <assert.h>
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
        if (pai == nextpai - 1 && pai == nnextpai - 2){
			return i;
		}
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
		if (pai == nextpai + 1 && pai == nnextpai + 2){
			return i-2;
		}
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
static int setstartmentsu(int *paiarray, int paiSize,int *mentsu,int atamapos)
{
    int i, paicountlistcount, pos, koutsuflag, syuntsuflag;
    PAICOUNT paicountlist[AI_TEHAI_LIMIT + 1];

    /* 最初の面子に頭を設定 */
	for (i = 0; i < paiSize; i++)
        mentsu[i] = AI_UKIHAI;
    mentsu[atamapos] = AI_ATAMA;
    mentsu[atamapos+1] = AI_ATAMA;

    /* 牌の集計を取る */
    paicountlistcount = 0;
    pos = 0;

    paicountlist[pos].pai = AI_NIL_PAI;
	for (i = 0; i < paiSize; i++) {
        /* 頭は集計に入れない */
        if ((i == atamapos) || (i == atamapos + 1))
            continue;
        if ((paicountlist[pos].pai != AI_NIL_PAI) &&
            (paicountlist[pos].pai == (paiarray[i] & 0xFF))) {
            paicountlist[pos].count++;
        } else {
            paicountlistcount++;
            if (paicountlist[pos].pai != AI_NIL_PAI)
                pos++;
            paicountlist[pos].pai = (paiarray[i] & 0xFF);
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

static int getKoritsuhai(PAICOUNT *paicountlist,int countSize)
{
	int count = 0,atama = 0;
    int pai, prevpai,nextpai;
    int i;

	for(i = 0;i < countSize;i++){
		pai = paicountlist[i].pai;
		nextpai = - 1;
		prevpai = - 1;
		if(i < countSize - 1){
			if(paicountlist[i+1].count){
				nextpai = paicountlist[i+1].pai;
			}
		}
		if(i>0){
			if(paicountlist[i-1].count){
				prevpai = paicountlist[i-1].pai;
			}
		}

		switch(paicountlist[i].count){
		case 4:
		case 1:
			if(pai > 26){
				count++;
			}else if(((pai/9) != (nextpai/9) || pai + 1 != nextpai) && ((pai/9) != (prevpai/9) || pai - 1 == prevpai)){
					count++;
			}
			break;
		case 2:
			if(pai > 26){
				atama++;
			}else if(((pai/9) != (nextpai/9) || pai + 1 != nextpai) && ((pai/9) != (prevpai/9) || pai - 1 == prevpai)){
					count++;
			}
			break;
		case 3:
		default:
			break;
		}
	}

	if(atama < 2){
		return count;
	}else{
		return count - (atama - 1) * 2;
	}
}

/* 牌の数を数える */
int setpaicount(PAICOUNT *paicountlist,int *paiarray,int paiSize, int *mentsu)
{
    int pos, i;

    pos = 0;
    paicountlist[pos].pai = AI_NIL_PAI;
    for (i = 0; i < paiSize; i++) {
        /* 浮き牌以外は集計に入れない */
        if (mentsu[i] != AI_UKIHAI){
        }else if ((paicountlist[pos].pai == (paiarray[i] & 0xFF))) {
            paicountlist[pos].count++;
        } else {
            if (paicountlist[pos].pai != AI_NIL_PAI)
                pos++;
            paicountlist[pos].pai = (paiarray[i] & 0xFF);
            paicountlist[pos].count = 1;
            paicountlist[pos].startpos = i;
        }
    }
    if (paicountlist[pos].pai != AI_NIL_PAI) {
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
		paicountlist[listpos].startpos += 3;
		paicountlist[listpos].count -= 3;

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
		paicountlist[listpos].startpos += 3;
		paicountlist[listpos].count -= 3;
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
		paicountlist[listpos].startpos++;
		paicountlist[listpos + 1].startpos++;
		paicountlist[listpos + 2].startpos++;
		paicountlist[listpos].count--;
		paicountlist[listpos + 1].count--;
		paicountlist[listpos + 2].count--;

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
		paicountlist[listpos].startpos++;
		paicountlist[listpos + 1].startpos++;
		paicountlist[listpos + 2].startpos++;
		paicountlist[listpos].count--;
		paicountlist[listpos + 1].count--;
		paicountlist[listpos + 2].count--;
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

/* 上がりの組み合わせを探索する関数 */
int search_agari(int *paiarray,int paiSize,AGARI_LIST *pList,int actualPaiSize,void *inf,int (*getPoint)(AGARI_LIST*,void*))
{
    PAICOUNT paicountlist[16][AI_TEHAI_LIMIT + 20];
	PAICOUNT paicountlisttmp[AI_TEHAI_LIMIT + 20];
	PAICOUNT paicountlisttmp2[AI_TEHAI_LIMIT + 20];
    int atamaque[AI_TEHAI_LIMIT + 20];
    int atamaque_count, atamapos;
    int mentsu_stack[16][AI_TEHAI_LIMIT + 20];
    int count_stack[16];
    int mentsu[AI_TEHAI_LIMIT + 20];
    int stackpos;
    int i, j, flag;
	int paicount_size;
	int paicount;
	AGARI_LIST agari_list;
	int prevagari[AI_TEHAI_LIMIT];
	int pts,maxpts = 0;
	int total = 0;
	int resultcount = 0;
	int koritsuhai;


	if(pList != NULL) memset(pList,0,sizeof(AGARI_LIST));
	memset(mentsu_stack,0,sizeof(mentsu_stack));
	memset(mentsu,0,sizeof(mentsu));
	memset(&agari_list,0,sizeof(agari_list));
	memset(prevagari,0,sizeof(prevagari));

    /* 頭の候補を得る */
    atamaque_count = 0;

	if(paiSize >= 2){
		i = 0;
		while(i<paiSize-1){
			if((paiarray[i] & 0xFF) == (paiarray[i + 1] & 0xFF)){
				atamaque[atamaque_count] = i;
				mentsu[i] = AI_ATAMA;
				mentsu[i+1] = AI_ATAMA;
				atamaque_count++;
				i++;
			}
			i++;
		}

		/* 七対子の判定 */

		if(atamaque_count >= 7 && actualPaiSize == 14){
			j=0;
			for(i=0;i<paiSize ;i++){
				if(mentsu[i] == AI_ATAMA){
					agari_list.tehai[j++] = paiarray[i];
					if(j==14) break;
				}
			}

			agari_list.tehai_max = actualPaiSize;

			pts = getPoint(&agari_list,inf);

			if(maxpts < pts){
				maxpts = pts;
				if(pList != NULL) *pList = agari_list;
			}

			total += pts;
			resultcount++;
		}

	}

	memset(mentsu_stack[0],0,sizeof(int)*(AI_TEHAI_LIMIT + 20));

	paicount_size = setpaicount(paicountlisttmp, paiarray,paiSize, mentsu_stack[0]);
	koritsuhai = getKoritsuhai(paicountlisttmp,paicount_size);

	if(koritsuhai > paiSize - actualPaiSize){
		return 0;
	}

#if 1
    /* 頭として取ったあとで刻子/順子などの面子の状況を見る(両面、カンチャン、ペンチャン、シャンポン待ち) */
    for (i = 0; i < atamaque_count; i++) {
        atamapos = atamaque[i];
		memset(mentsu_stack[0],0,sizeof(int)*(AI_TEHAI_LIMIT + 20));
		
        /* 探索を開始する面子の初期状態を設定 */
		mentsu_stack[0][atamapos] = AI_ATAMA;
		mentsu_stack[0][atamapos+1] = AI_ATAMA;

		memcpy(paicountlist[0],paicountlisttmp,sizeof(paicountlisttmp));
		for(j=0;j<paicount_size;j++){
			if(paicountlist[0][j].pai == paiarray[atamapos]){
				paicountlist[0][j].count -=2;
				break;
			}
		}

		count_stack[0] = 2;

        	
        /* 探索 */
        stackpos = 0;
        do {
            //paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos],&paicount);
			koritsuhai = getKoritsuhai(paicountlist[stackpos],paicount_size);

			if(koritsuhai <= paiSize - actualPaiSize){
				
				if (count_stack[stackpos] < actualPaiSize){
					paicount = count_stack[stackpos];
					memcpy(paicountlisttmp2,paicountlist[stackpos],sizeof(paicountlisttmp2));
					memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
					flag = setkoutsu(paicountlist[stackpos], mentsu_stack[stackpos],paicount_size);
					if (flag) {
						count_stack[stackpos] += 3;
						stackpos++;

						
						/* 次に備えて待避しておいた内容をコピー */
						count_stack[stackpos] = paicount;
						memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
						memcpy(paicountlist[stackpos], paicountlisttmp2,sizeof(paicountlisttmp2));
					}
					
					flag = setsyuntsu(paicountlist[stackpos], mentsu_stack[stackpos],paicount_size);
					if (flag) {
						count_stack[stackpos] += 3;
						stackpos++;
					}
					
				} else {
					paicount = 0;
					for(j=0;j<paiSize;j++){
						if(mentsu_stack[stackpos][j] != AI_UKIHAI){
							assert(paicount < 14);
							agari_list.tehai[paicount++] = paiarray[j];
						}
					}
					
					agari_list.tehai_max = actualPaiSize;
					if(memcmp(prevagari,&agari_list.tehai[0],actualPaiSize*sizeof(int))){
						memcpy(prevagari,&agari_list.tehai[0],actualPaiSize*sizeof(int));
						
						pts = getPoint(&agari_list,inf);
						
						if(maxpts < pts){
							maxpts = pts;
							if(pList != NULL) *pList = agari_list;
						}
						total += pts;
						resultcount++;
					}
					
				}
			}
            stackpos--;
        } while(stackpos >= 0);
    }
#endif

	return total;
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
        initresult = setstartmentsu(paiarray, paiSize,mentsu_stack[0],atamapos);
        if (initresult != 0)
            return initresult;
        	
        /* 探索 */
        stackpos = 0;
        do {
            paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
            if (istenpai(paicountlist,paicount_size,&tenpai_list,paiarray[atamapos]) < 0) {  /* 浮き牌がまだある */
				orflag = AI_FALSE;
                memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
                flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

				flag = setsyuntsul(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
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
			memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
			flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
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

int search_score(int *paiarray,int paiSize,void *inf,int (*callback)(int*paiarray,int*mentsu,int length,int machi,void *inf))
{
    PAICOUNT paicountlist[AI_TEHAI_LIMIT + 1];
	int notanki = 0;
    int atamaque[AI_TEHAI_LIMIT];
    int atamaque_count, atamapos;
    int mentsu_stack[256][AI_TEHAI_LIMIT];
    int mentsu[AI_TEHAI_LIMIT];
    int stackpos;
    int i,initresult, flag,orflag;
	int paicount_size;
	int maxval = 0,val;
	int machi;
	TENPAI_LIST tenpai_list; // dummy

	memset(mentsu_stack,0,sizeof(mentsu_stack));
	memset(mentsu,0,sizeof(mentsu));
	
    /* 頭の候補を得る */
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
			val = callback(paiarray,mentsu,paiSize,AI_MACHI_TANKI,inf);
			if(val > maxval) maxval = val;
		}

	}

   
	
    /* 頭として取ったあとで刻子/順子などの面子の状況を見る(両面、カンチャン、ペンチャン、シャンポン待ち) */
    for (i = 0; i < atamaque_count; i++) {
        atamapos = atamaque[i];
		
        /* 探索を開始する面子の初期状態を設定 */
        initresult = setstartmentsu(paiarray, paiSize,mentsu_stack[0],atamapos);
        if (initresult != 0)
            return 0;
        	
        /* 探索 */
        stackpos = 0;
        do {
            paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
            if ((machi = istenpai(paicountlist,paicount_size,&tenpai_list,paiarray[atamapos])) < 0) {  /* 浮き牌がまだある */
				orflag = AI_FALSE;
                memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
                flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
                    /* 次に備えて待避しておいた内容をコピー */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

				flag = setsyuntsul(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
				}
            } else {
				val = callback(paiarray,mentsu_stack[stackpos],paiSize,machi,inf);
				if(val > maxval) maxval = val;
            }
            stackpos--;
        } while(stackpos >= 0);
    }

	memset(mentsu_stack,0,sizeof(mentsu_stack));

	/* 探索 */
	stackpos = 0;
	do {
		paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
		if ((machi = isatamamachi(paicountlist,paicount_size,&tenpai_list)) < 0) {  /* 浮き牌がまだある */
			orflag = AI_FALSE;
			memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
			flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				/* 次に備えて待避しておいた内容をコピー */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			if(flag != 1){
				flag = setsyuntsul(paicountlist,mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
				}
			}

		} else {
			val = callback(paiarray,mentsu_stack[stackpos],paiSize,machi,inf);
			if(val > maxval) maxval = val;
		}
		stackpos--;
	} while(stackpos >= 0);


    return maxval;
	
}

/* 手牌の差分を返す(入力値、出力値は牌の種類別のカウント値) */
void tehai_diff_fromcount(const unsigned *tecount_now, unsigned *tecount_future, int n) {
    int i;
    for (i = 0; i < n; i++){
        if (tecount_now[i] > tecount_future[i]) {
            tecount_future[i] = tecount_now[i] - tecount_future[i];
        }
        else{
            tecount_future[i] = 0;
        }
    }
}

/* 手牌の差分を返す(入力値は手牌を列挙した配列、出力値は種類別のカウント値) */
void tehai_diff(const unsigned *tehai_now, const unsigned *tehai_future, unsigned *result) {
    unsigned tecount_now[34] = { 0 };
    int i;
    memset(result, 0, sizeof(unsigned)* 34);

    /* 種類別のカウント値を計算する */
    for (i = 0; i < 14; i++){
        tecount_now[tehai_now[i]]++;
        result[tehai_future[i]]++;
    }

    tehai_diff_fromcount(tecount_now, result, 34);
}

/* 手牌と特定の牌との距離を返す
配列は既にソートされていると仮定する
配列から特定の牌を抜き出す操作をする場合、その場所には0x3F(34より大きければよい)を入れる。

手牌との距離とは
1)数牌の場合
手牌の中でその手牌に近い数牌の数との差の絶対値
例えば一萬を持っていたとして

手牌に一萬があれば0
二萬があれば1
三萬なら2
となる
手牌に萬子がなければ-1(無限大)となる

2)字牌の場合
手牌に同じものを持っていれば0、なければ-1(無限大)

*/
int paidistance(const unsigned *tehai, unsigned pai) {
    int i;
    unsigned prev = 0,n=0;
    unsigned distp, distn;
    pai &= 0x3F;


    for (i = 0; i < 14; i++){
        n = tehai[i] & 0x3F;
        /* 存在しない牌は飛ばす(手牌を抜き出す場合に便利) */
        if (n>33) continue;

        if (n >= pai) {
            if (pai  >= 27) {
                /* 字牌 */
                return (n == pai) ? 0 : -1;
            }
            else if ((pai % 9) == 0) {
                /* 一萬、一索、一筒 */
                return (n - pai < 9) ? (n - pai) : -1;
            }
            else if ((pai % 9) == 8) {
                /* 九萬、九索、九筒 */
                return (pai - prev < 9) ? (pai - prev) : -1;
            }
            else{
                /* その他の牌 */

                /* 後ろの牌との距離(種類が違う場合は99) */
                if (n / 9 == pai / 9){
                    distn = n - pai;
                }
                else{
                    distn = 99;
                }

                /* 前の牌との距離(種類が違う場合は99) */
                if (prev / 9 == pai / 9){
                    distp = pai - prev;
                }
                else {
                    distp = 99;
                }

                /* 小さい方を返す */
                if (distn > distp){
                    return (distp < 9) ? distp : -1;
                }
                else{
                    return (distn < 9) ? distn : -1;
                }
            }

        }
        prev = n;

    }

    /* 見つからない(つまり自分が一番後ろ) */
    if (pai >= 27) {
        /* 字牌 */
        return -1;
    }
    else if ((pai % 9) == 0) {
        /* 一萬、一索、一筒 */
        return -1;
    }
    else if ((pai % 9) == 8) {
        /* 九萬、九索、九筒 */
        return (pai - prev < 9) ? (pai - prev) : -1;
    }
    else{
        /* その他の牌 */

        /* 前の牌との距離(種類が違う場合は99) */
        if (prev / 9 == pai / 9){
            distp = pai - prev;
        }
        else {
            distp = 99;
        }

        return (distp < 9) ? distp : -1;
    }

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