/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
 * All rights reserved.

  �\�[�X�R�[�h�`�����o�C�i���`�����A�ύX���邩���Ȃ������킸�A�ȉ��̏����𖞂����ꍇ�Ɍ���A�ĔЕz����юg�p��������܂��B 

  �E�\�[�X�R�[�h���ĔЕz����ꍇ�A��L�̒��쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 

  �E�o�C�i���`���ōĔЕz����ꍇ�A�Еz���ɕt���̃h�L�������g���̎����ɁA��L�̒��쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 

  �E���ʂɂ����ʂ̋��Ȃ��ɁA�{�\�t�g�E�F�A����h���������i�̐�`�܂��͔̔����i�ɁA�I�[�v�������̖��O�܂��̓R���g���r���[�^�[�̖��O���g�p���Ă͂Ȃ�Ȃ��B


  �{�\�t�g�E�F�A�́A���쌠�҂���уR���g���r���[�^�[�ɂ���āu����̂܂܁v�񋟂���Ă���A�����َ����킸�A
  ���ƓI�Ȏg�p�\���A����ѓ���̖ړI�ɑ΂���K�����Ɋւ���Öق̕ۏ؂��܂߁A�܂�����Ɍ��肳��Ȃ��A�����Ȃ�ۏ؂�����܂���B
  ���쌠�҂��R���g���r���[�^�[���A���R�̂�������킸�A ���Q�����̌�����������킸�A���ӔC�̍������_��ł��邩���i�ӔC�ł��邩
  �i�ߎ����̑��́j�s�@�s�ׂł��邩���킸�A���ɂ��̂悤�ȑ��Q����������\����m�炳��Ă����Ƃ��Ă��A�{�\�t�g�E�F�A�̎g�p�ɂ���Ĕ�������
  �i��֕i�܂��͑�p�T�[�r�X�̒��B�A�g�p�̑r���A�f�[�^�̑r���A���v�̑r���A�Ɩ��̒��f���܂߁A�܂�����Ɍ��肳��Ȃ��j
  ���ڑ��Q�A�Ԑڑ��Q�A�����I�ȑ��Q�A���ʑ��Q�A�����I���Q�A�܂��͌��ʑ��Q�ɂ��āA��ؐӔC�𕉂�Ȃ����̂Ƃ��܂��B 

****************************************************************************************/
#include <assert.h>
#include <string.h>
#include "AILib.h"
#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------- */
/* ������̌`���l���镔�� */

/* ���q���ǂ����𒲂ׂ� */
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

/* ���q���ǂ����𒲂ׂ�(�E�����̂�) */
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
/* ���q���ǂ����𒲂ׂ�i�������j */
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

/* �\�O�s��/���m���o���ǂ������ׂ� */
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

/* �T�����J�n����ʎq�̏�����Ԃ�ݒ� */
static int setstartmentsu(int *paiarray, int paiSize,int *mentsu,int atamapos)
{
    int i, paicountlistcount, pos, koutsuflag, syuntsuflag;
    PAICOUNT paicountlist[AI_TEHAI_LIMIT + 1];

    /* �ŏ��̖ʎq�ɓ���ݒ� */
	for (i = 0; i < paiSize; i++)
        mentsu[i] = AI_UKIHAI;
    mentsu[atamapos] = AI_ATAMA;
    mentsu[atamapos+1] = AI_ATAMA;

    /* �v�̏W�v����� */
    paicountlistcount = 0;
    pos = 0;

    paicountlist[pos].pai = AI_NIL_PAI;
	for (i = 0; i < paiSize; i++) {
        /* ���͏W�v�ɓ���Ȃ� */
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
    /* �\�O�s��/���m���o�����ׂ� */
    if (search_siisan(paicountlist)) {
        /* �����Ȃ�ݒ肵�ďI�� */
        return AI_TRUE;
    }
#endif

    /* �T�����n�߂邽�߂Ɋm�肵�Ă���Ƃ�������߂� */
    for (i = 0; i < paicountlistcount; i++) {
        
        koutsuflag  = iskoutsu(paicountlist + i,1) >= 0;
        syuntsuflag = issyuntsu(paicountlist + i,3) >= 0;
        
        /* ���̈ʒu�����q�ŏ��q�łȂ���Ίm�� */
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
        /* ���̈ʒu�����q�ł���3�̔v�����q�łȂ���Ίm�� */
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

/* �v�̐��𐔂��� */
int setpaicount(PAICOUNT *paicountlist,int *paiarray,int paiSize, int *mentsu)
{
    int pos, i;

    pos = 0;
    paicountlist[pos].pai = AI_NIL_PAI;
    for (i = 0; i < paiSize; i++) {
        /* �����v�ȊO�͏W�v�ɓ���Ȃ� */
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
		/* ���v */
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

		/* �����ʎq�ɂȂ��Ă�����̂Ƃ̏��q�`�����c���Ă��邪����͌�ŁO�O */

		/* �A�^�}���v�Ƃ̏��q�`�� */
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
		/* �C�[�V�����e�� */
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
			/*�V�����|���҂�*/
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
					// �y���`�����҂�
					machilist->shanten = 0;
					machilist->machi[paicountlist[0].pai + 2] = AI_FLAG_MACHI;
					return AI_MACHI_PENCHAN;
				}else if (paicountlist[1].pai % 9 == 8){
					// �y���`�����҂�
					machilist->shanten = 0;
					machilist->machi[paicountlist[0].pai - 1] = AI_FLAG_MACHI;
					return AI_MACHI_PENCHAN;
				}else{
					// ���ʑ҂�
					machilist->shanten = 0;
					machilist->machi[paicountlist[0].pai - 1] = AI_FLAG_MACHI;
					machilist->machi[paicountlist[0].pai + 2] = AI_FLAG_MACHI;
					return AI_MACHI_RYANMEN;
				}
			}else if(paicountlist[0].pai + 2 == paicountlist[1].pai){
				// �J���`�����҂�
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


/* �����v�����邩�Ԃ� */
static int isukihai(PAICOUNT *paicountlist)
{
    if (paicountlist[0].pai == AI_NIL_PAI) {
        return AI_FALSE;
    }
    return AI_TRUE;
}

/* ���q���Z�b�g���� */
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

/* ���q���Z�b�g����(������)*/
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

/* ���q���Z�b�g���� */
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

/* ���q���Z�b�g����(������)*/
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

/* �Ύq���Z�b�g�ł��邩�ǂ������ׂ� */
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

/* �Ύq���Z�b�g���� */
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

/* �オ��̑g�ݍ��킹��T������֐� */
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

    /* ���̌��𓾂� */
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

		/* ���Ύq�̔��� */

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
    /* ���Ƃ��Ď�������Ƃō��q/���q�Ȃǂ̖ʎq�̏󋵂�����(���ʁA�J���`�����A�y���`�����A�V�����|���҂�) */
    for (i = 0; i < atamaque_count; i++) {
        atamapos = atamaque[i];
		memset(mentsu_stack[0],0,sizeof(int)*(AI_TEHAI_LIMIT + 20));
		
        /* �T�����J�n����ʎq�̏�����Ԃ�ݒ� */
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

        	
        /* �T�� */
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

						
						/* ���ɔ����đҔ����Ă��������e���R�s�[ */
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
	
    /* ���̌��𓾂� */
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

		/* ���Ύq�̔��� */

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

   
	
    /* ���Ƃ��Ď�������Ƃō��q/���q�Ȃǂ̖ʎq�̏󋵂�����(���ʁA�J���`�����A�y���`�����A�V�����|���҂�) */
    for (i = 0; i < atamaque_count; i++) {
        atamapos = atamaque[i];
		
        /* �T�����J�n����ʎq�̏�����Ԃ�ݒ� */
        initresult = setstartmentsu(paiarray, paiSize,mentsu_stack[0],atamapos);
        if (initresult != 0)
            return initresult;
        	
        /* �T�� */
        stackpos = 0;
        do {
            paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
            if (istenpai(paicountlist,paicount_size,&tenpai_list,paiarray[atamapos]) < 0) {  /* �����v���܂����� */
				orflag = AI_FALSE;
                memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
                flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* ���ɔ����đҔ����Ă��������e���R�s�[ */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* ���ɔ����đҔ����Ă��������e���R�s�[ */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
                    /* ���ɔ����đҔ����Ă��������e���R�s�[ */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

				flag = setsyuntsul(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
				}

				if(!orflag && maxshanten > 0){
					/* �T���I�� */
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

	/* �T�� */
	stackpos = 0;
	do {
		paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
		if (isatamamachi(paicountlist,paicount_size,&tenpai_list) < 0) {  /* �����v���܂����� */
			orflag = AI_FALSE;
			memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
			flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
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
				/* �T���I�� */
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

int search_tenpai2(int* paiarray, int paiSize, int* pMachi, TENPAI_LIST* pList, int listSize, int maxshanten, GAMESTATE* gamestate)
{
	PAICOUNT paicountlist[AI_TEHAI_LIMIT + 1];
	int notanki = 0;
	int atamaque[AI_TEHAI_LIMIT];
	int atamaque_count, atamapos;
	int mentsu_stack[256][AI_TEHAI_LIMIT];
	int mentsu[AI_TEHAI_LIMIT];
	int stackpos;
	int i, j, initresult, flag, orflag;
	int paicount_size;
	int resultcount;
	int maxval, maxind;
	TENPAI_LIST tenpai_list;

	if (pList != NULL && listSize > 0) memset(pList, 0, sizeof(TENPAI_LIST) * listSize);
	memset(&tenpai_list, 0, sizeof(tenpai_list));
	memset(mentsu_stack, 0, sizeof(mentsu_stack));
	memset(mentsu, 0, sizeof(mentsu));
	if (pMachi != NULL) memset(pMachi, 0, sizeof(int) * 34);

	/* ���̌��𓾂� */
	resultcount = 0;
	atamaque_count = 0;

	if (paiSize >= 4) {
		i = 0;
		while (i < paiSize - 1) {
			if (paiarray[i] == paiarray[i + 1]) {
				atamaque[atamaque_count] = i;
				mentsu[i] = AI_ATAMA;
				mentsu[i + 1] = AI_ATAMA;
				atamaque_count++;
				i++;
			}
			i++;
		}

		/* ���Ύq�̔��� */

		if (atamaque_count == 6) {
			for (i = 0; i < paiSize; i++) {
				if (mentsu[i] == AI_UKIHAI) {
					tenpai_list.machi[paiarray[i]] = AI_FLAG_MACHI;
				}
			}
			tenpai_list.shanten = 0;
		}
		else {
			for (i = 0; i < paiSize; i++) {
				if (mentsu[i] == AI_UKIHAI) {
					tenpai_list.machi[paiarray[i]] = AI_FLAG_EFFECT_ATAMA;
				}
			}

			tenpai_list.shanten = 6 - atamaque_count;
		}
		memcpy(&tenpai_list.mentsuflag, mentsu, AI_TEHAI_LIMIT * sizeof(int));
		if (pList != NULL && tenpai_list.shanten <= maxshanten) {
			if (resultcount == listSize) {
				maxval = 0;
				maxind = 0;
				for (j = 0; j < listSize; j++) {
					if (pList[j].shanten > maxval) {
						maxval = pList[j].shanten;
						maxind = j;
					}
				}

				if (maxval > tenpai_list.shanten) {
					pList[maxind] = tenpai_list;
				}
			}
			else {
				pList[resultcount] = tenpai_list;
				resultcount++;
			}
		}

		if (pMachi != NULL && tenpai_list.shanten == 0) {
			for (j = 0; j < 34; j++) {
				pMachi[j] = pMachi[j] || tenpai_list.machi[j];
			}
		}

		memset(&tenpai_list, 0, sizeof(tenpai_list));
	}



	/* ���Ƃ��Ď�������Ƃō��q/���q�Ȃǂ̖ʎq�̏󋵂�����(���ʁA�J���`�����A�y���`�����A�V�����|���҂�) */
	for (i = 0; i < atamaque_count; i++) {
		atamapos = atamaque[i];

		/* �T�����J�n����ʎq�̏�����Ԃ�ݒ� */
		initresult = setstartmentsu(paiarray, paiSize, mentsu_stack[0], atamapos);
		if (initresult != 0)
			return initresult;

		/* �T�� */
		stackpos = 0;
		do {
			paicount_size = setpaicount(paicountlist, paiarray, paiSize, mentsu_stack[stackpos]);
			if (istenpai(paicountlist, paicount_size, &tenpai_list, paiarray[atamapos]) < 0) {  /* �����v���܂����� */
				orflag = AI_FALSE;
				memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
				flag = setkoutsu(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;

					/* ���ɔ����đҔ����Ă��������e���R�s�[ */
					memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
				}

				flag = setkoutsul(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;

					/* ���ɔ����đҔ����Ă��������e���R�s�[ */
					memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
				}

				flag = setsyuntsu(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
					/* ���ɔ����đҔ����Ă��������e���R�s�[ */
					memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
				}

				flag = setsyuntsul(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
				}

				if (!orflag && maxshanten > 0) {
					/* �T���I�� */
					memcpy(&tenpai_list.mentsuflag, mentsu_stack[stackpos], AI_TEHAI_LIMIT * sizeof(int));
					search_shanten(paicountlist, paicount_size, &tenpai_list, paiarray[atamapos]);
					if (pList != NULL && tenpai_list.shanten <= maxshanten) {
						if (resultcount == listSize) {
							maxval = 0;
							maxind = 0;
							for (j = 0; j < listSize; j++) {
								if (pList[j].shanten > maxval) {
									maxval = pList[j].shanten;
									maxind = j;
								}
							}

							if (maxval > tenpai_list.shanten) {
								pList[maxind] = tenpai_list;
							}
						}
						else {
							pList[resultcount] = tenpai_list;
							resultcount++;
						}
					}

					memset(&tenpai_list, 0, sizeof(tenpai_list));
				}

			}
			else {
				memcpy(&tenpai_list.mentsuflag, mentsu_stack[stackpos], AI_TEHAI_LIMIT * sizeof(int));
				if (pMachi != NULL) {
					for (j = 0; j < 34; j++) {
						pMachi[j] = pMachi[j] || tenpai_list.machi[j];
					}
				}
				if (pList != NULL) {
					if (resultcount == listSize) {
						maxval = 0;
						maxind = 0;
						for (j = 0; j < listSize; j++) {
							if (pList[j].shanten > maxval) {
								maxval = pList[j].shanten;
								maxind = j;
							}
						}
						pList[maxind] = tenpai_list;
					}
					else {
						pList[resultcount] = tenpai_list;
						resultcount++;
					}
				}
				memset(&tenpai_list, 0, sizeof(tenpai_list));

			}
			stackpos--;
		} while (stackpos >= 0);
	}

	memset(mentsu_stack, 0, sizeof(mentsu_stack));

	/* �T�� */
	stackpos = 0;
	do {
		paicount_size = setpaicount(paicountlist, paiarray, paiSize, mentsu_stack[stackpos]);
		if (isatamamachi(paicountlist, paicount_size, &tenpai_list) < 0) {  /* �����v���܂����� */
			orflag = AI_FALSE;
			memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
			flag = setkoutsu(paicountlist, mentsu_stack[stackpos], paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;

				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}

			flag = setkoutsul(paicountlist, mentsu_stack[stackpos], paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;

				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}

			flag = setsyuntsu(paicountlist, mentsu_stack[stackpos], paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}

			if (flag != 1) {
				flag = setsyuntsul(paicountlist, mentsu_stack[stackpos], paicount_size);
				orflag = orflag || flag;
				if (flag) {
					stackpos++;
				}
			}

			if (!orflag && maxshanten > 0) {
				/* �T���I�� */
				memcpy(&tenpai_list.mentsuflag, mentsu_stack[stackpos], AI_TEHAI_LIMIT * sizeof(int));
				search_shanten_atamaless(paicountlist, paicount_size, &tenpai_list);
				if (pList != NULL && tenpai_list.shanten <= maxshanten) {
					if (resultcount == listSize) {
						maxval = 0;
						maxind = 0;
						for (j = 0; j < listSize; j++) {
							if (pList[j].shanten > maxval) {
								maxval = pList[j].shanten;
								maxind = j;
							}
						}

						if (maxval > tenpai_list.shanten) {
							pList[maxind] = tenpai_list;
						}
					}
					else {
						pList[resultcount] = tenpai_list;
						resultcount++;
					}
				}

				memset(&tenpai_list, 0, sizeof(tenpai_list));
			}

		}
		else {
			memcpy(&tenpai_list.mentsuflag, mentsu_stack[stackpos], AI_TEHAI_LIMIT * sizeof(int));

			if (pMachi != NULL) {
				for (j = 0; j < 34; j++) {
					pMachi[j] = pMachi[j] || tenpai_list.machi[j];
				}
			}

			if (pList != NULL) {
				if (resultcount == listSize) {
					maxval = 0;
					maxind = 0;
					for (j = 0; j < listSize; j++) {
						if (pList[j].shanten > maxval) {
							maxval = pList[j].shanten;
							maxind = j;
						}
					}
					pList[maxind] = tenpai_list;
				}
				else {
					pList[resultcount] = tenpai_list;
					resultcount++;
				}
			}
			memset(&tenpai_list, 0, sizeof(tenpai_list));

		}
		stackpos--;
	} while (stackpos >= 0);


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
	
    /* ���̌��𓾂� */
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

		/* ���Ύq�̔��� */

		if(atamaque_count == 6){
			val = callback(paiarray,mentsu,paiSize,AI_MACHI_TANKI,inf);
			if(val > maxval) maxval = val;
		}

	}

   
	
    /* ���Ƃ��Ď�������Ƃō��q/���q�Ȃǂ̖ʎq�̏󋵂�����(���ʁA�J���`�����A�y���`�����A�V�����|���҂�) */
    for (i = 0; i < atamaque_count; i++) {
        atamapos = atamaque[i];
		
        /* �T�����J�n����ʎq�̏�����Ԃ�ݒ� */
        initresult = setstartmentsu(paiarray, paiSize,mentsu_stack[0],atamapos);
        if (initresult != 0)
            return 0;
        	
        /* �T�� */
        stackpos = 0;
        do {
            paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
            if ((machi = istenpai(paicountlist,paicount_size,&tenpai_list,paiarray[atamapos])) < 0) {  /* �����v���܂����� */
				orflag = AI_FALSE;
                memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
                flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* ���ɔ����đҔ����Ă��������e���R�s�[ */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
					
                    /* ���ɔ����đҔ����Ă��������e���R�s�[ */
                    memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
                }

                flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
				orflag = orflag || flag;
                if (flag) {
                    stackpos++;
                    /* ���ɔ����đҔ����Ă��������e���R�s�[ */
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

	/* �T�� */
	stackpos = 0;
	do {
		paicount_size = setpaicount(paicountlist, paiarray,paiSize, mentsu_stack[stackpos]);
		if ((machi = isatamamachi(paicountlist,paicount_size,&tenpai_list)) < 0) {  /* �����v���܂����� */
			orflag = AI_FALSE;
			memcpy(mentsu, mentsu_stack[stackpos], sizeof(mentsu));
			flag = setkoutsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setkoutsul(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				
				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
				memcpy(mentsu_stack[stackpos], mentsu, sizeof(mentsu));
			}
			
			flag = setsyuntsu(paicountlist, mentsu_stack[stackpos],paicount_size);
			orflag = orflag || flag;
			if (flag) {
				stackpos++;
				/* ���ɔ����đҔ����Ă��������e���R�s�[ */
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

/* ��v�̍�����Ԃ�(���͒l�A�o�͒l�͔v�̎�ޕʂ̃J�E���g�l) */
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

/* ��v�̍�����Ԃ�(���͒l�͎�v��񋓂����z��A�o�͒l�͎�ޕʂ̃J�E���g�l) */
void tehai_diff(const unsigned *tehai_now, const unsigned *tehai_future, unsigned *result) {
    unsigned tecount_now[34] = { 0 };
    int i;
    memset(result, 0, sizeof(unsigned)* 34);

    /* ��ޕʂ̃J�E���g�l���v�Z���� */
    for (i = 0; i < 14; i++){
        tecount_now[tehai_now[i]]++;
        result[tehai_future[i]]++;
    }

    tehai_diff_fromcount(tecount_now, result, 34);
}

/* ��v�Ɠ���̔v�Ƃ̋�����Ԃ�
�z��͊��Ƀ\�[�g����Ă���Ɖ��肷��
�z�񂩂����̔v�𔲂��o�����������ꍇ�A���̏ꏊ�ɂ�0x3F(34���傫����΂悢)������B

��v�Ƃ̋����Ƃ�
1)���v�̏ꍇ
��v�̒��ł��̎�v�ɋ߂����v�̐��Ƃ̍��̐�Βl
�Ⴆ�Έ��݂������Ă����Ƃ���

��v�Ɉ��݂������0
���݂������1
�O�݂Ȃ�2
�ƂȂ�
��v���ݎq���Ȃ����-1(������)�ƂȂ�

2)���v�̏ꍇ
��v�ɓ������̂������Ă����0�A�Ȃ����-1(������)

*/
int paidistance(const unsigned *tehai, unsigned pai) {
    int i;
    unsigned prev = 0,n=0;
    unsigned distp, distn;
    pai &= 0x3F;


    for (i = 0; i < 14; i++){
        n = tehai[i] & 0x3F;
        /* ���݂��Ȃ��v�͔�΂�(��v�𔲂��o���ꍇ�ɕ֗�) */
        if (n>33) continue;

        if (n >= pai) {
            if (pai  >= 27) {
                /* ���v */
                return (n == pai) ? 0 : -1;
            }
            else if ((pai % 9) == 0) {
                /* ���݁A����A�ꓛ */
                return (n - pai < 9) ? (n - pai) : -1;
            }
            else if ((pai % 9) == 8) {
                /* ���݁A����A�㓛 */
                return (pai - prev < 9) ? (pai - prev) : -1;
            }
            else{
                /* ���̑��̔v */

                /* ���̔v�Ƃ̋���(��ނ��Ⴄ�ꍇ��99) */
                if (n / 9 == pai / 9){
                    distn = n - pai;
                }
                else{
                    distn = 99;
                }

                /* �O�̔v�Ƃ̋���(��ނ��Ⴄ�ꍇ��99) */
                if (prev / 9 == pai / 9){
                    distp = pai - prev;
                }
                else {
                    distp = 99;
                }

                /* ����������Ԃ� */
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

    /* ������Ȃ�(�܂莩������Ԍ��) */
    if (pai >= 27) {
        /* ���v */
        return -1;
    }
    else if ((pai % 9) == 0) {
        /* ���݁A����A�ꓛ */
        return -1;
    }
    else if ((pai % 9) == 8) {
        /* ���݁A����A�㓛 */
        return (pai - prev < 9) ? (pai - prev) : -1;
    }
    else{
        /* ���̑��̔v */

        /* �O�̔v�Ƃ̋���(��ނ��Ⴄ�ꍇ��99) */
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