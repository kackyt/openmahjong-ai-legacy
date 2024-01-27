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
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "MIPIface.h"
#include "AILib.h"
#include "MahjongScoreAI.h"

using namespace std;

//#define AIDUMP
#ifdef AIDUMP
//#define AIDUMP_1
//#define AIDUMP_2
//#define AIDUMP_3
//#define AIDUMP_STAT
//#define AIDUMP_POINT
//#define AIDUMP_PARAM
#define AIDUMP_COMMAND
//#define AIDUMP_STACKTRACE
#endif

/*typedef struct {
	double sc;
	double sc1;
	double sc2;
	double scc;
	int no;
	} HAIPOINT;*/

static const int param_size[] = {
	9,
};

typedef enum {
	AI_DECISION_AGARI1,
	AI_DECISION_AGARI2,
	AI_DECISION_ORI,
} AI_DECISION;

#define MAHJONGAI( method ) MahjongAI##method
#define PLAYERNAME( method ) "KING" #method


#define MAHJONGAITYPE MAHJONGAI(Type4)
#define AINAME PLAYERNAME(v2)

MAHJONGAITYPE ai;
MahjongAIKikenhai kikenhai;


class MahjongAI {
public:
	UINT InterfaceFunc(UINT message, UINT param1, UINT param2);

#ifdef AIDUMP
	FILE *fp;
#endif
protected:
	MahjongAIState *pState;
	int machi[34];
	int menzen;
	int nakiok_flag;
	int sthai;
	int doranum;
	double tehai_score;
	int tenpai_flag;
	int jun;
	int kyokustate;
	HAIPOINT hp[14];
	int *resultBuf;
	int current_p;
	int size_p;
	AI_DECISION decision;

	int tehai2Array(int *p);
	void initParam(void);
	void destroyParam(void);
	int getParam(int index);
	void printResult(int result);
	int search(int obj, int start, int mask);
	void set_machi(void);
	void set_Tehai(int);
	int select_Score(int hai_remain);
	UINT sutehai_sub(int tsumohai);
	int calc_sutehai(void);
	int calc_sutehai_easy(double*);
	int nakability(int hai, int chii_flag);
	UINT koe_req(int no, int hai);
	UINT on_start_kyoku(int k, int c);
	UINT on_end_kyoku(UINT reason, LONG* inc_sc);
	UINT on_action(int player, int taishou, UINT action);
	UINT on_start_game(void);
	UINT on_end_game(int rank, LONG score);
	UINT on_exchange(UINT state, UINT option);
	void sendComment(int index);
};
enum {
	AI_KYOKUSTS_TSUMO,
	AI_KYOKUSTS_RON,
	AI_KYOKUSTS_TEKIAGARI,
	AI_KYOKUSTS_FURIKOMI,
	AI_KYOKUSTS_NORMAL,
};

enum {
	AI_MESSAGE_TSUMOLITTLE = 0,
	AI_MESSAGE_TSUMOMIDDLE,
	AI_MESSAGE_TSUMOBIG,
	AI_MESSAGE_RONLITTLE,
	AI_MESSAGE_RONMIDDLE,
	AI_MESSAGE_RONBIG,
	AI_MESSAGE_TEKIAGARILITTLE,
	AI_MESSAGE_TEKIAGARIMIDDLE,
	AI_MESSAGE_TEKIAGARIBIG,
	AI_MESSAGE_FURIKOMILITTLE,
	AI_MESSAGE_FURIKOMIMIDDLE,
	AI_MESSAGE_FURIKOMIBIG,
	AI_MESSAGE_TII,
	AI_MESSAGE_PON,
	AI_MESSAGE_RIICHI,
	AI_MESSAGE_TSUYOGARI,
};


static const TCHAR *message_table[] = {
	TEXT("�݂Ă��낱��ȃP�`�Ȃ��̂ł͂Ȃ��I"),
	TEXT("���l�����������悤�ȃZ���t�͂͂�����I�I�I"),
	TEXT("�͂��������`��������ɂȂ������̂�"),
	TEXT("���{�ڂɎ��ʂ̂��ȁc"),
	TEXT("���܂��Ȃǂ���̓G�ł͂Ȃ���I"),
	TEXT("��l���j���I�I�I"),
	TEXT("�t�b�c�����������͂͂₭�Ȃ����悤����"),
	TEXT("���c�������ɂ����܂̎��O�������I�Ȃ�΂��̎��O�̌��������Ă�낤"),
	TEXT("��c��͂�̂̃P���V���E�ł͂Ȃ��ȁB�Ȃ��c"),
	TEXT("���܂��Ƃ���ɂ͒v���I�ȈႢ�����邻��͗~�]�c���O���I�I"),
	TEXT("���I�I�o�A�o�J��"),
	TEXT("�c����̖��́c���Ƃǂꂭ�炢��"),
	TEXT("�Ȃ����c���܂�ď��߂Ă���͂Ȃ���"),
	TEXT("�Ȃ����c���܂�ď��߂Ă���͂Ȃ���"),
	TEXT("���{�ڂɎ��ʂ̂��ȁc"),
	TEXT("����͂������������Ĕ��������̂��D���Ȃ񂾁I"),
};

static const TCHAR * num_table[] = {
	TEXT("��"),
	TEXT("��"),
	TEXT("�O"),
	TEXT("�l"),
	TEXT("��"),
	TEXT("�Z"),
	TEXT("��"),
	TEXT("��"),
	TEXT("��"),
};

static void printStackTrace(const char *trace, ...){
	va_list list;
	FILE *file = NULL;

	va_start(list, trace);

	file = fopen(TEXT("./stacktrace.txt"), "a");
	vfprintf(file, trace, list);
	fclose(file);

	va_end(list);
}

static const TCHAR * type_table[] = {
	TEXT("��"),
	TEXT("��"),
	TEXT("��"),
	TEXT("��"),
	TEXT("��"),
	TEXT("��"),
	TEXT("�k"),
	TEXT("��"),
	TEXT("��"),
	TEXT("��"),
};


static void sethaitext(TCHAR *p, int no)
{
	if (no < 27){
		sprintf(p, "%s%s", num_table[no % 9], type_table[no / 9]);
	}
	else{
		sprintf(p, "%s", type_table[no - 27 + 3]);
	}
}

static int compare_hp(const HAIPOINT *a, const HAIPOINT *b)
{
	double res = a->sc - b->sc;
	if (res < 0){
		return 1;
	}
	else if (res > 0){
		return -1;
	}
	else{
		return 0;
	}
}


TCHAR player_name[] = TEXT(AINAME);

UINT(WINAPI *MJSendMessage)(MahjongAI*, UINT, UINT, UINT);

static int debug_count = 0;

static int compare_int(const int *a, const int *b)
{
	return *a - *b;
}

#define PARAM_MAX (sizeof(param_size)/sizeof(int))

void MahjongAI::initParam(void)
{
	int siz = 1, i;

	pState = new MahjongAIState();

	for (i = 0; i < PARAM_MAX; i++){
		siz *= param_size[i];
	}

	resultBuf = (int*)malloc(siz*sizeof(int));
	memset(resultBuf, 0x00, siz*sizeof(int));
	current_p = 0;
	size_p = siz;
	tehai_score = DBL_MAX;
}

void MahjongAI::destroyParam(void)
{
	if (resultBuf) free(resultBuf);

	delete pState;
}

int MahjongAI::getParam(int index)
{
	int i, p = current_p;
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START getParam\n");
#endif

	for (i = 0; i < index; i++){
		p /= param_size[i];
	}

#ifdef AIDUMP_STACKTRACE
	printStackTrace("END getParam\n");
#endif
	return p % param_size[index];
}

void MahjongAI::printResult(int result)
{
	int i;
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START printResult\n");
#endif
	resultBuf[current_p] += 3 - result;
#ifdef AIDUMP_PARAM
	fclose(fp);
	fp = fopen(TEXT("./AIDUMP_COMMAND.csv"),TEXT("w"));
	for(i=0;i<size_p;i++){
		fprintf(fp,"%d,",resultBuf[i]);
	}
	fprintf(fp,"\n");
#endif
	current_p++;
	if (current_p == size_p) current_p = 0;

#ifdef AIDUMP_STACKTRACE
	printStackTrace("END printResult\n");
#endif
}

void MahjongAI::sendComment(int index)
{
	time_t tim = time(NULL);

	if ((tim % 3) == 0){
		MJSendMessage(this, MJMI_FUKIDASHI, (UINT)message_table[index], 0);
	}
}


// ��v�̒�����C�ӂ̔v�������A���̈ʒu��Ԃ�
int MahjongAI::search(int obj, int start, int mask)
{
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START search\n");
#endif
	while (start < (int)pState->myself._tehai.size()){
		if (!(mask&(1 << start)) && (int)pState->myself._tehai[start].getNum() == obj) break;
		start++;
	}
#ifdef AIDUMP_STACKTRACE
	printStackTrace("END search\n");
#endif
	return start < (int)pState->myself._tehai.size() ? start : -1;
}

// ������v�𒲂ׁA�z��machi�ɓ����B
// �܂������Ă��邩�ǂ������ׂ�tenpai_flag���Z�b�g����B
void MahjongAI::set_machi(void)
{
	int i, j, cnt;
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START set_machi\n");
#endif
	(*MJSendMessage)(this, MJMI_GETMACHI, 0, (UINT)machi);
	tenpai_flag = 0;
	for (i = 0; i < 34; i++){
		if (machi[i]) {
			cnt = 0;
			for (j = 0; j < (int)pState->myself._tehai.size(); j++) if ((int)pState->myself._tehai[j].getNum() == i) cnt++;
			if (cnt + (*MJSendMessage)(this, MJMI_GETVISIBLEHAIS, i, 0) < 4){
				tenpai_flag = 1;
				return;
			}
			tenpai_flag = -1;
		}
	}
#ifdef AIDUMP_STACKTRACE
	printStackTrace("END set_machi\n");
#endif
}

#include "MJ0.h"


// ��v�̕ϐ�tehai��te_cnt���Z�b�g����
void MahjongAI::set_Tehai(int tsumohai)
{
	int i;
	MJITehai tehai;
	MJIKawahai kawahai[20];
	UINT dora[8];
	double mentsu1[27 + 34];
	double mentsu2[27 + 34];
	double mentsu3[27 + 34];
	int doralen;
	int kawalength;

	pState->tsumohai = tsumohai;

#ifdef AIDUMP_STACKTRACE
	printStackTrace("START set_Tehai\n");
#endif
	(*MJSendMessage)(this, MJMI_GETTEHAI, 0, (UINT)&tehai);

	pState->myself.fromTehai(&tehai);

	kawalength = (*MJSendMessage)(this, MJMI_GETKAWAEX, (20 << 16) + 0, (UINT)&kawahai);

	pState->myself.fromKawahai(&kawahai[0], kawalength);

	for (size_t i = 0; i < 3; i++)
	{
		(*MJSendMessage)(this, MJMI_GETTEHAI, i+1, (UINT)&tehai);
		pState->players[i].fromTehai(&tehai);
		kawalength = (*MJSendMessage)(this, MJMI_GETKAWAEX, (20 << 16) + i+1, (UINT)&kawahai);
		pState->players[i].fromKawahai(&kawahai[0], kawalength);
	}

	doralen = (*MJSendMessage)(this, MJMI_GETDORA, (UINT)dora, 0);
	pState->doras.clear();
	for (size_t i = 0; i < doralen; i++) {
		pState->doras.push_back(Pai(dora[i]));
	}

#ifdef AIDUMP_STACKTRACE
	printStackTrace("START MJ0\n");
#endif
	MJAI::MJ0::simulate(&pState->players, &pState->myself, &pState->doras);
#ifdef AIDUMP_STACKTRACE
	printStackTrace("END MJ0\n");
#endif

#ifdef AIDUMP_1
	fprintf(fp,TEXT("<NOKORI>"));

	for(i=0;i<34;i++){
		fprintf(fp,TEXT("[%.1f]"),nokori[i]);
	}

	fprintf(fp,TEXT("</NOKORI>"));
#endif

#ifdef AIDUMP_COMMAND
	fprintf(fp, TEXT("GET TEHAI\n"));
#endif
	for (i = 0; i < 34; i++){
		pState->te_cnt[i] = 0;
		pState->sute_cnt[i] = 0;
	}


	for (auto pai : pState->myself._tehai) {
		pState->te_cnt[pai.getNum()]++;
	}
	for (auto pai : pState->myself._kawahai) {
		pState->sute_cnt[pai.getNum()]++;
	}

	doranum = 0;
	for (i = 0; i < doralen; i++){
		doranum += pState->te_cnt[dora[i]];
	}
#ifdef AIDUMP_STACKTRACE
	int j;
	printStackTrace("END set_Tehai\n");
	for(i=0;i<(int)tehai.tehai_max;i++) printStackTrace("%d ",tehai.tehai[i]);
	printStackTrace("\n");
	for(i=0;i<doralen;i++) printStackTrace("%d ",dora[i]);
	printStackTrace("\n");
	for(i=0;i<4;i++){
		for(j=0;j<param[i].kawalength;j++){
			printStackTrace("%d ",param[i].pKawahai[j]);
		}
		printStackTrace("\n");
	}
#endif
}


// �̂Ĕv���̏���
// tsumohai : �������Ă����v
UINT MahjongAI::sutehai_sub(int tsumohai)
{
	int mc[34] = { 0 };
	UINT kawa[30] = { 0 };
	int mcount, mpoint;
	UINT rchk = MJPIR_SUTEHAI;
	int i, j, hai, del_hai = 0, hai_remain, tmp, furiten, kazu;
	int mhai = -1, hais;
	MJITehai tmphai;
	unsigned int seed;

	// ���� srand���s��(�f�o�b�O���₷�����邽��)
#if 0
	seed = 0xDEADBEEF;
#else
	seed = (unsigned int)time(NULL);
#endif

#ifdef AIDUMP_STACKTRACE
	printStackTrace("START sutehai_sub %d\n",seed);
#endif

	srand(seed);
	debug_count = 0;

	// ���݂̎�v�̏�Ԃ��Z�b�g����
	set_Tehai(tsumohai);

	// ���݂̑҂��v���擾����
	set_machi();

	// �c�������ꍇ�́u�c���v��
	if (tsumohai >= 0 && tsumohai < 34 && machi[tsumohai]){
		tmp = (*MJSendMessage)(this, MJMI_GETAGARITEN, 0, tsumohai);
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("GETAGARITEN %u\n"), tmp);
#endif
		if (tmp > 0) {
			return MJPIR_TSUMO;
		}
	}

	// ���[�`�������Ă���ꍇ�́u�c���؂�v
	if (pState->myself._is_riichi) return MJPIR_SUTEHAI | 13;

	// ����v�ŗ�����ꍇ�͗���
	tmp = (*MJSendMessage)(this, MJMI_KKHAIABILITY, 0, 0);

#ifdef AIDUMP_COMMAND
	fprintf(fp, TEXT("KKHAIABILITY %u\n"), tmp);
#endif

	if (tmp) return MJPIR_NAGASHI;

	// �����c�����Ă����v������Ȃ�A���̔v������
	if (tsumohai >= 0 && tsumohai < 34) pState->te_cnt[tsumohai]++;

	// �̂Ă�v�����߂�
	hai = calc_sutehai();
	if (hai < (int)pState->myself._tehai.size()) del_hai = pState->myself._tehai[hai].getNum(); else del_hai = tsumohai;

#ifdef AIDUMP_POINT
	fprintf(fp,"\n");
#endif

	if (machi[hai]){
		// bug??
	}
	// ��O�ŁA�e���p�����ꍇ�̓��[�`�������悤���Ȃ��H
	if (menzen){
		mcount = 0;
		mpoint = 0;
		furiten = 0;
		hais = 0;

		memset(&tmphai, 0, sizeof(tmphai));
		pState->te_cnt[del_hai]--;
		tmphai.tehai_max = 0;
		for (i = 0; i < 34; i++){
			for (j = 0; j < pState->te_cnt[i]; j++){
				tmphai.tehai[tmphai.tehai_max++] = i;
			}
		}
		pState->te_cnt[del_hai]++;

		hai_remain = (*MJSendMessage)(this, MJMI_GETHAIREMAIN, 0, 0);
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("GETHAIREMAIN %u\n"), hai_remain);
#endif
		tenpai_flag = (*MJSendMessage)(this, MJMI_GETMACHI, (UINT)&tmphai, (UINT)mc);
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("GETMACHI %u\n"), tenpai_flag);
#endif
		for (i = 0; i < 34; i++){
			if (mc[i]){
				mcount++;
				tmp = (*MJSendMessage)(this, MJMI_GETVISIBLEHAIS, i, 0);
#ifdef AIDUMP_COMMAND
				fprintf(fp, TEXT("GETVISIBLEHAIS %u\n"), tmp);
#endif
				if (pState->te_cnt[i] + tmp < 4 && hai_remain>60){ rchk = MJPIR_REACH; break; }
				hais += 4 - (pState->te_cnt[i] + tmp);
				mpoint += (*MJSendMessage)(this, MJMI_GETAGARITEN, (UINT)&tmphai, i);
				mhai = i;
				if (!furiten){
					kazu = (*MJSendMessage)(this, MJMI_GETKAWA, (30 << 16), (UINT)kawa);

					for (j = 0; j < kazu; j++){
						if (mc[kawa[j]]){
							furiten = 1;
							break;
						}
					}
				}

			}
		}

		/* ���̐l�̃��[�`�� */
		int rnum = 0;
		for (i = 0; i < 3; i++) {
			if (pState->players[i]._is_riichi) rnum++;
		}
		TCHAR comment[256];
		if (mcount > 0) mpoint /= mcount;

		if (mhai >= 0) {
			if (furiten) {
				if ((mpoint > 2000 || doranum >= 2) && hais > 4 && rnum <= 1) {
					rchk = MJPIR_REACH;
				}
			}
			else if (hai_remain >= 14) {
				// �ǌ`�͑����[�ł悢(1�l����)
				if (rnum <= 1) {
					// ���ʈȏ�
					if (hais >= 4) {
						rchk = MJPIR_REACH;
					}
					// ���v�҂�
					if (mhai >= 27) {
						if (rnum == 0 || mpoint >= 1600 || pState->cha == 0) {
							rchk = MJPIR_REACH;
						}
					}
					else {
						// �X�W�Ђ�����
						if ((mhai % 9) < 3 && kawa[mhai + 3] >= 1) {
							if (rnum == 0 || mpoint >= 1600 || pState->cha == 0) {
								rchk = MJPIR_REACH;
							}
						}

						if ((mhai % 9) >= 6 && kawa[mhai - 3] >= 1) {
							if (rnum == 0 || mpoint >= 1600 || pState->cha == 0) {
								rchk = MJPIR_REACH;
							}
						}

						if ((mhai % 9) >= 3 && (mhai % 9) < 6 && kawa[mhai + 3] >= 1 && kawa[mhai - 3] >= 1) {
							if (rnum == 0 || mpoint >= 1600 || pState->cha == 0) {
								rchk = MJPIR_REACH;
							}
						}

						// ���`�ł��搧 ���� �卷�łȂ���΃��[�`
						if (rnum == 0 && pState->myself._score <= 40000) {
							rchk = MJPIR_REACH;
						}
					}
				}

				// �ǂ��������[�`
				if ((rchk & MJPIR_REACH) == 0 && rnum == 1) {
					// �e or 1600�ȏ�̏オ��A��
					if (mhai >= 3 && (mpoint >= 1600 || pState->cha == 0)) {
						rchk = MJPIR_REACH;
					}
					// �����`
					if (mhai <= 2 && (mpoint >= 3200 || (pState->cha == 0 && mpoint >= 2000))) {
						rchk = MJPIR_REACH;
					}
				}

				// 2�l���[�`�̒ǂ������͗��ʈȏ�
				if (rnum == 2 && mpoint >= 1300 && hais > 4) {
					rchk = MJPIR_REACH;
				}
			}

			if ((rchk & MJPIR_REACH) == 0) {
				MJSendMessage(this, MJMI_FUKIDASHI, (UINT)"�_�}�e��", 0);
			}
		}

#ifdef _DEBUG
		sprintf(comment,"mpoint %d,mcount %d,hais %d",mpoint,mcount,hais);

		MJSendMessage(this,MJMI_FUKIDASHI,(UINT)comment,0);
#endif
	}

	// �e��t���O�̃Z�b�g
	if (rchk == MJPIR_REACH){
#ifdef AIDUMP_2
		TCHAR haitext[10];
		TCHAR haitext2[10];
		TCHAR haitext3[10];
		int j;
		fprintf(fp,"<TEHAI>");
		for(i=0;i<34;i++){
			for(j=0;j<te_cnt[i];j++){
				fprintf(fp,"%d ",i);
			}
		}
		fprintf(fp,"</TEHAI>");

		for(i=0;i<14;i++){
			sethaitext(haitext,hp[i].no);

			fprintf(fp,"<SCORE>%s:%.1f,%.1f,%.1f</SCORE>\n",
				haitext,hp[i].sc1,hp[i].sc2,hp[i].scc);
		}

#endif
		pState->myself._is_riichi = true;
	}
#ifdef AIDUMP_STACKTRACE
	printStackTrace("END sutehai_sub\n");
#endif
	return hai | rchk;
}





int MahjongAI::select_Score(int hai_remain)
{
	int i;
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START select_Score\n");
#endif

	int rnum = 0;
	int knum = 0;
	int shanten = 0;
	TENPAI_LIST list;
	MJITehai tehai;

	pState->myself.toTehai(&tehai);

	/* �V�����e�����𐔂��� */
	shanten = search_tenpai((int*)tehai.tehai, tehai.tehai_max, NULL, &list, 1, 6);

	if (shanten != 0){
		shanten = list.shanten;
	}
	else{
		shanten = 4;
	}


	/* �I���邩�U�߂邩�̔��f */
	decision = AI_DECISION_AGARI1;

	for (i = 0; i < 3; i++){
		/* ���Ƃ̃��[�`�� */
		if (pState->players[i]._is_riichi) {
			rnum++;
		}
		else {
			/* ���ߎ�̊댯�x�v�Z(���[�`����) */
			if (pState->players[i]._somete >= 0) {
				if (pState->players[i]._naki_mentsu.size() >= 3) {
					rnum++;
				}
				else {
					// ���Ոȍ~�ɗ]��v���o�Ă�������ߎ�e���p�C�C�z
					auto siz = pState->players[i]._kawahai.size();
					for (int j = 6; j < siz; j++) {
						if ((pState->players[i]._kawahai[j].getNum() / 9) == pState->players[i]._somete) {
							rnum++;
							break;
						}
					}
				}
			}
		}
		/* ���Ƃ̋򂢎d�|���l�� */
		if (pState->players[i]._naki_mentsu.size() > 0) knum++;
	}

	if (rnum == 0) {
		if (knum == 1 && shanten >= 1 && hai_remain < 24) {
			decision = AI_DECISION_ORI;
		}
		if (knum >= 2 && shanten >= 1 && pState->myself._naki_mentsu.size() == 0 && hai_remain < 16) {
			decision = AI_DECISION_ORI;
		} else if (knum >= 2 && shanten >= 1 && pState->myself._naki_mentsu.size() >= 1 && hai_remain < 8) {
			decision = AI_DECISION_ORI;
		}
	}
	else if (rnum == 1) {
		if (shanten == 1 && pState->cha == 0 && hai_remain >= 24) {
			decision = AI_DECISION_AGARI1;
		}
		else if (shanten >= 1 && pState->myself._naki_mentsu.size() == 0 && hai_remain < 24) {
			decision = AI_DECISION_ORI;
		}
		else if (shanten >= 1 && pState->myself._naki_mentsu.size() >= 1 && hai_remain < 12) {
			decision = AI_DECISION_ORI;
		}

	}
	else if (rnum == 2) {
		if (hai_remain < 34) {
			decision = AI_DECISION_ORI;
		}
	}

	if (decision == AI_DECISION_AGARI1) {
		MJSendMessage(this, MJMI_FUKIDASHI, (UINT)"�c�b�p", 0);
	}
	else {
		MJSendMessage(this, MJMI_FUKIDASHI, (UINT)"�I��", 0);
	}

#ifdef AIDUMP_STACKTRACE
	printStackTrace("END select_Score\n");
#endif

	return decision;
}

// �̂Ă�v�����߂�
int MahjongAI::calc_sutehai(void)
{
	int i, ret;

	// �̂Ă�v���������Ă݂āA�����Ƃ��]���l�̍������̂��Ƃ�
	double sc_max = -DBL_MAX;
	int sh;
	double sc, scc, scc_max = -DBL_MAX;
	int index = 0;
	TCHAR haitext[10];
	TCHAR haitext2[10];
	TCHAR haitext3[10];
	TCHAR comment[512];
	HAIPOINT hp1[14], hp2[14];
	int size1, size2;
	int decision;

#ifdef AIDUMP_STACKTRACE
	printStackTrace("START calc_sutehai\n");
#endif
	memset(hp1, 0, sizeof(hp1));
	memset(hp2, 0, sizeof(hp2));

#ifdef AIDUMP_1
	int j;
	fprintf(fp,"<CALC><TEHAI>");
	for(i=0;i<34;i++){
		for(j=0;j<te_cnt[i];j++){
			fprintf(fp,"%d ",i);
		}
	}
	fprintf(fp,"</TEHAI>");

#endif
	int hai_remain = (*MJSendMessage)(this, MJMI_GETHAIREMAIN, 0, 0);

	scc_max = ai.evalSutehai(*pState, hp1, size1);
	kikenhai.evalSutehai(*pState, hp2, size2);

	decision = select_Score(hai_remain);

	if (decision == AI_DECISION_AGARI1){
		memcpy(hp, hp1, sizeof(hp1));
//		MJSendMessage(this, MJMI_FUKIDASHI, (UINT)"����", 0);
	}
	else{
		memcpy(hp, hp2, sizeof(hp2));
//		MJSendMessage(this, MJMI_FUKIDASHI, (UINT)"����", 0);
	}

	qsort(hp, size1, sizeof(HAIPOINT), (int(*)(const void*, const void*))compare_hp);

#if 1
	{
		int max1=-1,max2=-1,max3=-1;
		double maxd1=0,maxd2=0,maxd3=0;
		for(i=0;i<34;i++){
			if(maxd1 < pState->myself._pai_kukan[i]){
				maxd3 = maxd2;
				maxd2 = maxd1;
				max3 = max2;
				max2 = max1;
				maxd1 = pState->myself._pai_kukan[i];
				max1 = i;
			}else if(maxd2 < pState->myself._pai_kukan[i]){
				maxd3 = maxd2;
				max3 = max2;
				maxd2 = pState->myself._pai_kukan[i];
				max2 = i;
			}else if(maxd3 < pState->myself._pai_kukan[i]){
				maxd3 = pState->myself._pai_kukan[i];
				max3 = i;
			}
		}
		sethaitext(haitext,max1);
		sethaitext(haitext2,max2);
		sethaitext(haitext3,max3);

		sprintf(comment,"[�v���] %s:%.1f %s:%.1f %s:%.1f",
			haitext,maxd1,
			haitext2,maxd2,
			haitext3,maxd3);

		MJSendMessage(this,MJMI_FUKIDASHI,(UINT)comment,0);

	}

	sethaitext(haitext,hp[0].no);
	sethaitext(haitext2,hp[1].no);
	sethaitext(haitext3,hp[2].no);

	sprintf(comment,"[score] %s:%.1f,%.1f\n%s:%.1f,%.1f\n%s:%.1f,%.1f",
		haitext,hp1[0].sc,hp2[0].sc,
		haitext2,hp1[1].sc,hp2[1].sc,
		haitext3,hp1[2].sc,hp2[2].sc);

	MJSendMessage(this,MJMI_FUKIDASHI,(UINT)comment,0);


#endif	

	tehai_score = scc_max;
	ret = search(hp[0].no, 0, 0);
#ifdef AIDUMP_1
	fprintf(fp,"</CALC>");
	fprintf(fp,"<MAX>%d</MAX>",hp1[0].no);
	fprintf(fp,"<RET>%d</RET>",ret);
#endif

#ifdef AIDUMP_STACKTRACE
	printStackTrace("END calc_sutehai\n");
#endif
	return ret >= 0 ? ret : 13;
}

// �̂Ă�v�����߂�
int MahjongAI::calc_sutehai_easy(double *pMax)
{
	int i, sh;
	HAIPOINT hp[14];
	int size;
	double score_max;

	memset(hp, 0, sizeof(hp));
	// �̂Ă�v���������Ă݂āA�����Ƃ��]���l�̍������̂��Ƃ�
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START calc_sutehai_easy\n");
#endif
	score_max = ai.evalSutehai(*pState, hp, size);

	if(pMax != NULL) *pMax=score_max;

	qsort(hp, size, sizeof(HAIPOINT), (int(*)(const void*, const void*))compare_hp);
#ifdef AIDUMP_STACKTRACE
	printStackTrace("END calc_sutehai_easy\n");
#endif
	sh = search(hp[0].no, 0, 0);
	return sh;
}


// �����Ƃ��ł��邩�ǂ������ׂ�
// hai : �Ώۂ̔v
// chii_flag : �`�[�ɂ��Ă��`�F�b�N���邩�ǂ���
// return : �ȉ��̒l�̘_���a
//		 1 : �|���ł���
//		 2 : �J���ł���
//		 4 : �`�[�P�i���j�ł���
//		 8 : �`�[�Q�i�E�j�ł���
//		16 : �`�[�R�i���j�ł���
//		32 : �����ł���
int MahjongAI::nakability(int hai, int chii_flag)
{
	int x, i, ret = 0, kazu, furiten;
	UINT kawa[30];
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START nakability\n");
#endif

	if (machi[hai]){
		kazu = (*MJSendMessage)(this, MJMI_GETKAWA, (30 << 16), (UINT)kawa);
		furiten = 0;

		for (i = 0; i < kazu; i++){
			if (machi[kawa[i]]){
				furiten = 1;
				break;
			}
		}

		if (furiten){
#ifdef _DEBUG
			(*MJSendMessage)(this,MJMI_FUKIDASHI,(UINT)TEXT("�t���e��"),0);
#endif
		}
		else{
			ret |= 32;
		}
	}
	if (pState->myself._is_riichi) return ret;
	if ((*MJSendMessage)(this, MJMI_GETHAIREMAIN, 0, 0) == 0) return ret;
	if ((x = search(hai, 0, 0)) >= 0){
		if (x < (int)pState->myself._tehai.size() - 1){
			if ((int)pState->myself._tehai[x + 1].getNum() == hai){
				ret |= 1;
				if (x < (int)pState->myself._tehai.size() - 2){
					if ((int)pState->myself._tehai[x + 2].getNum() == hai) ret |= 2;
				}
			}
		}
	}
	if (chii_flag){
		if (hai < 27){
			kazu = hai % 9;
			if (kazu > 1){
				if (pState->te_cnt[hai - 2] > 0 && pState->te_cnt[hai - 1] > 0) ret |= 8;
			}
			if (kazu < 7){
				if (pState->te_cnt[hai + 2] > 0 && pState->te_cnt[hai + 1] > 0) ret |= 4;
			}
			if (kazu > 0 && kazu < 8){
				if (pState->te_cnt[hai - 1] > 0 && pState->te_cnt[hai + 1] > 0) ret |= 16;
			}
		}
	}
#ifdef AIDUMP_STACKTRACE
	printStackTrace("END nakability\n");
#endif
	return ret;
}

int MahjongAI::tehai2Array(int *p)
{
	int size = 0;
	int i, j;
#ifdef AIDUMP_STACKTRACE
	printStackTrace("START tehai2Array\n");
#endif

	for (i = 0; i < 34; i++){
		for (j = 0; j < pState->te_cnt[i]; j++){
			p[size++] = i;
		}
	}

#ifdef AIDUMP_STACKTRACE
	printStackTrace("END tehai2Array\n");
#endif
	return size;
}

// ���Ƃ̎̂Ĕv�ɑ΂���A�N�V���������߂�
// no : ���ꂪ�̂Ă���
// hai : �����̂Ă���
// return : �A�N�V����
UINT MahjongAI::koe_req(int no, int hai)
{
	int chii_flag;
	int naki_ok;
	int hanpai;
	UINT tmp;
	UINT dora[8];
	int doralen;
	int doraflag;
	int i;
	int tmptehai[14];
	int tehaisize;
	int prevshanten;
	int nextshanten;
	TENPAI_LIST list;
	int sthai;
	double score_max;

#ifdef AIDUMP_STACKTRACE
	printStackTrace("START koe_req\n");
#endif
	doralen = (*MJSendMessage)(this, MJMI_GETDORA, (UINT)dora, 0);
	set_Tehai(-1);
	set_machi();
	chii_flag = (no == 3);
	naki_ok = nakability(hai, chii_flag);
	if (!naki_ok) return 0;
	if (naki_ok & 32){
		tmp = (*MJSendMessage)(this, MJMI_GETAGARITEN, 0, hai);
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("GETAGARITEN %u\n"), tmp);
#endif
		if (tmp > 0) {
			return MJPIR_RON;
		}
	}

	/* �e���p�C�܂��̓I���Ă���΂����͖��Ȃ� */
	if (tenpai_flag == 1 || decision == AI_DECISION_ORI) return 0;

#if 0

	if (naki_ok & 1){
		pState->te_cnt[hai] -= 2;
		pState->myself._naki_mentsu.push_back(Mentsu(Mentsu::TYPE_KOUTSU, hai));
		calc_sutehai_easy(&score_max);
		pState->myself._naki_mentsu.pop_back();
		pState->te_cnt[hai] += 2;

		if (score_max > 0.01 && score_max > tehai_score){
			return MJPIR_PON;
		}
	}

	if (naki_ok & 4){
		pState->te_cnt[hai + 1]--;
		pState->te_cnt[hai + 2]--;
		pState->myself._naki_mentsu.push_back(Mentsu(Mentsu::TYPE_SHUNTSU, hai));
		calc_sutehai_easy(&score_max);
		pState->myself._naki_mentsu.pop_back();
		pState->te_cnt[hai + 1]++;
		pState->te_cnt[hai + 2]++;

		if (score_max > 0.01 && score_max > tehai_score){
			return MJPIR_CHII1;
		}
	}
	if (naki_ok & 8){
		pState->te_cnt[hai - 1]--;
		pState->te_cnt[hai - 2]--;
		pState->myself._naki_mentsu.push_back(Mentsu(Mentsu::TYPE_SHUNTSU, hai - 2));
		calc_sutehai_easy(&score_max);
		pState->myself._naki_mentsu.pop_back();
		pState->te_cnt[hai - 1]++;
		pState->te_cnt[hai - 2]++;

		if (score_max > 0.01 && score_max > tehai_score){
			return MJPIR_CHII2;
		}
	}
	if (naki_ok & 16){
		pState->te_cnt[hai + 1]--;
		pState->te_cnt[hai - 1]--;
		pState->myself._naki_mentsu.push_back(Mentsu(Mentsu::TYPE_SHUNTSU, hai - 1));
		calc_sutehai_easy(&score_max);
		pState->myself._naki_mentsu.pop_back();
		pState->te_cnt[hai + 1]++;
		pState->te_cnt[hai - 1]++;

		if (score_max > 0.01 && score_max > tehai_score){
			return MJPIR_CHII3;
		}
	}
#endif
#ifdef AIDUMP_STACKTRACE
	printStackTrace("END koe_req\n");
#endif
	return 0;
}

// �ǊJ�n���̏���
// k : ��
// c : ��
UINT MahjongAI::on_start_kyoku(int k, int c)
{
	int i, j, sc_max = 0, mysc;

#ifdef AIDUMP_STACKTRACE
	printStackTrace("START on_start_kyoku\n");
#endif
	kyokustate = AI_KYOKUSTS_NORMAL;
	decision = AI_DECISION_AGARI1;
	//set_Tehai();
	pState->kyoku = k;
	pState->kaze = pState->kyoku / 4;
	pState->cha = c;
	menzen = 1;
	nakiok_flag = 0;
	jun = 0;
	sthai = -1;
	for (i = 0; i < 3; i++){
		pState->players[i].clear();
	}

	pState->myself.clear();
	//tehai_score = eval_Tehai(0);
	//set_machi();
	tehai_score = DBL_MAX;

	pState->myself._score = mysc = (*MJSendMessage)(this, MJMI_GETSCORE, 0, 0);

	for (i = 0; i < 3; i++){
		pState->players[i]._score = j = (*MJSendMessage)(this, MJMI_GETSCORE, i + 1, 0);
		if (j > sc_max) {
			sc_max = j;
		}
	}

	if (k > 4 && sc_max - mysc > 20000){
		sendComment(AI_MESSAGE_TSUYOGARI);
	}

#ifdef AIDUMP_STACKTRACE
	printStackTrace("END on_start_kyoku\n");
#endif
	return 0;
}

// �ǏI�����̏���
// reason : �I���������R
// inc_sc : �_���̕ω�
UINT MahjongAI::on_end_kyoku(UINT reason, LONG* inc_sc)
{
	int sc = *inc_sc;

	switch (kyokustate){
	case AI_KYOKUSTS_TSUMO:
		if (sc < 4000){
			sendComment(AI_MESSAGE_TSUMOLITTLE);
		}
		else if (sc < 16000){
			sendComment(AI_MESSAGE_TSUMOMIDDLE);
		}
		else{
			sendComment(AI_MESSAGE_TSUMOBIG);
		}
		break;
	case AI_KYOKUSTS_RON:
		if (sc < 4000){
			sendComment(AI_MESSAGE_RONLITTLE);
		}
		else if (sc < 16000){
			sendComment(AI_MESSAGE_RONMIDDLE);
		}
		else{
			sendComment(AI_MESSAGE_RONBIG);
		}
		break;
	case AI_KYOKUSTS_TEKIAGARI:
		if (sc > -2000){
			sendComment(AI_MESSAGE_TEKIAGARILITTLE);
		}
		else if (sc > -6000){
			sendComment(AI_MESSAGE_TEKIAGARIMIDDLE);
		}
		else{
			sendComment(AI_MESSAGE_TEKIAGARIBIG);
		}
		break;
	case AI_KYOKUSTS_FURIKOMI:
		if (sc > -4000){
			sendComment(AI_MESSAGE_FURIKOMILITTLE);
		}
		else if (sc > -16000){
			sendComment(AI_MESSAGE_FURIKOMIMIDDLE);
		}
		else{
			sendComment(AI_MESSAGE_FURIKOMIBIG);
		}
		break;
	default:
		break;
	}

	return 0;
}

// �A�N�V�����ɑ΂��鉞��������
UINT MahjongAI::on_action(int player, int taishou, UINT action)
{
	int hai = action & 63;

	if (action & MJPIR_REACH){
		if (player == 0){
			sendComment(AI_MESSAGE_RIICHI);
		} else {
			pState->players[player-1]._is_riichi = true;
			pState->players[player-1]._is_ippatsu = true;
		}
	}
	else{
		if (player == 0) {
			pState->myself._is_ippatsu = false;
		} else {
			pState->players[player-1]._is_ippatsu = false;
		}
	}

	if (action & MJPIR_TSUMO){
		if (player == 0){
			kyokustate = AI_KYOKUSTS_TSUMO;
		}
		else{
			kyokustate = AI_KYOKUSTS_TEKIAGARI;
		}
	}
	if (action & (MJPIR_SUTEHAI | MJPIR_REACH)){
		if (player == 0)
		{
			return 0;
		}
		else {
			pState->players[player-1]._anpai[hai] = 1.0f;
			for (int i = 0; i < 3; i++) if (pState->players[i]._is_riichi) pState->players[i]._anpai[hai] = 1.0f;
			return koe_req(player, hai);
		}
	}
	if (action & MJPIR_RON){
		if (player == 0){
			kyokustate = AI_KYOKUSTS_RON;
		}
		else if (taishou == 0){
#ifdef AIDUMP_1
			fprintf(fp,"<FURIKOMI/>");
#endif
			kyokustate = AI_KYOKUSTS_FURIKOMI;
		}
		else{
			kyokustate = AI_KYOKUSTS_TEKIAGARI;
		}
	}
	if ((action & MJPIR_PON) && player == 0){
		nakiok_flag = 1;
		menzen = 0;
		sendComment(AI_MESSAGE_PON);
	}
	if ((action & MJPIR_CHII1) && player == 0){
		menzen = 0;
		sendComment(AI_MESSAGE_TII);
	}
	if ((action & MJPIR_CHII2) && player == 0){
		menzen = 0;
		sendComment(AI_MESSAGE_TII);
	}
	if ((action & MJPIR_CHII3) && player == 0){
		menzen = 0;
		sendComment(AI_MESSAGE_TII);
	}
	if ((action & MJPIR_MINKAN) && player == 0){
		menzen = 0;
		sendComment(AI_MESSAGE_PON);
	}
	return 0;
}

// �����J�n���̏���
UINT MahjongAI::on_start_game(void)
{
	return 0;
}

// �����I�����̏���
// rank : ����
// score : �_��
UINT MahjongAI::on_end_game(int rank, LONG score)
{
	/*char str[40];
	sprintf(str,"%d�_�A%d�ʂ��c",score,rank+1);
	(*MJSendMessage)(this,MJMI_FUKIDASHI,(UINT)str,0);*/
	return 0;
}

// �r���Q�����̏���
// state : ���̂Ƃ��̏��
// option : ��ԂɊ֘A���đ�������
UINT MahjongAI::on_exchange(UINT ex_state, UINT option)
{
	if (ex_state == MJST_INKYOKU){
		int i, j, k, size;
		HAIPOINT hp[14];
		set_Tehai(-1);
		for (i = 0; i < 3; i++){
			pState->players[i].clear();
		}

		pState->myself.clear();

		MJIKawahai kawa[30];
		for (i = 0; i < 4; i++){
			k = (*MJSendMessage)(this, MJMI_GETKAWAEX, MAKELPARAM(i, 30), (UINT)kawa);
			for (j = 0; j < k; j++){
				pState->players[i]._anpai[kawa[j].hai & 63] = 1;
				if (kawa[j].state&MJKS_REACH) pState->players[i]._is_riichi = true;
			}
		}

		pState->kyoku = LOWORD(option);
		pState->kaze = pState->kyoku / 4;
		pState->cha = HIWORD(option);
		menzen = pState->myself.isMenzen();
		nakiok_flag = !menzen;
		sthai = -1;
		tehai_score = ai.evalSutehai(*pState, hp, size);
		set_machi();
	}
	return 0;
}

// �C���X�^���X�p�̃C���^�[�t�F�[�X�֐�
UINT MahjongAI::InterfaceFunc(UINT message, UINT param1, UINT param2)
{
	UINT ret = MJR_NOTCARED;
#ifdef AIDUMP
	fp = fopen(TEXT("./AIDUMP_COMMAND.xml"), TEXT("a"));
#endif
	MahjongScoreAI::pObj = this;
	MahjongScoreAI::pMJSendMessage = (UINT(WINAPI *)(void*, UINT, UINT, UINT))MJSendMessage;

	switch (message){
		//	case MJPI_DEBUG:
		//			return type4.getDebugInt();
	case MJPI_SUTEHAI:
		jun++;
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("SUTEHAI %u\n"), param1);
#endif
		ret = sutehai_sub(LOWORD(param1));
		break;
	case MJPI_ONACTION:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("ONACTION %u %u\n"), param1, param2);
#endif
		ret = on_action(LOWORD(param1), HIWORD(param1), param2);
		break;
	case MJPI_STARTKYOKU:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("STARTKYOKU %u %u\n"), param1, param2);
#endif
		ret = on_start_kyoku(LOWORD(param1), LOWORD(param2));
		break;
	case MJPI_ENDKYOKU:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("ENDKYOKU %u %u\n"), param1, param2);
#endif
#ifdef AIDUMP_STACKTRACE
		remove(TEXT("./stacktrace.txt"));
#endif
		ret = on_end_kyoku(param1, (LONG*)param2);
		break;
	case MJPI_STARTGAME:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("STARTGAME\n"));
#endif
		ret = on_start_game();
		break;
	case MJPI_ENDGAME:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("ENDGAME %u %u\n"), param1, param2);
#endif
		ret = on_end_game(LOWORD(param1), (LONG)param2);
		printResult(LOWORD(param1));
		break;
	case MJPI_ONEXCHANGE:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("ONEXCHANGE %u %u\n"), param1, param2);
#endif
		ret = on_exchange(LOWORD(param1), param2);
		break;
	case MJPI_CREATEINSTANCE:
		ret = sizeof(MahjongAI);
		break;
	case MJPI_INITIALIZE:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("INITIALIZE\n"));
#endif
		MJSendMessage = (UINT(WINAPI *)(MahjongAI*, UINT, UINT, UINT))param2;
		initParam();
		ret = 0;
		break;
	case MJPI_YOURNAME:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("YOUENAME\n"));
#endif
		ret = (UINT)player_name;
		break;
	case MJPI_DESTROY:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("DESTROY\n"));
#endif
		destroyParam();
		ret = 0;
		break;
	case MJPI_ISEXCHANGEABLE:
#ifdef AIDUMP_COMMAND
		fprintf(fp, TEXT("ISEXCHANGEABLE\n"));
#endif
		ret = 0; // �r���Q���ɑΉ�����B�Ή��������Ȃ��ꍇ��0�ȊO�ɂ���B
		break;
	default:
		break;
	}

#ifdef AIDUMP
	fclose(fp);
#endif
	return ret;
}

// �C���^�[�t�F�[�X�֐�
extern "C" UINT WINAPI MJPInterfaceFunc(MahjongAI* inst, UINT message, UINT param1, UINT param2)
{
	if (inst) return inst->InterfaceFunc(message, param1, param2);
	switch (message){
	case MJPI_CREATEINSTANCE:
		return sizeof(MahjongAI);
	case MJPI_INITIALIZE:
		MJSendMessage = (UINT(WINAPI *)(MahjongAI*, UINT, UINT, UINT))param2;
		MahjongScoreAI::pMJSendMessage = (UINT(WINAPI *)(void*, UINT, UINT, UINT))param2;
		return 0;
	case MJPI_YOURNAME:
		return (UINT)player_name;
	case MJPI_DESTROY:
		return 0;
	case MJPI_ISEXCHANGEABLE:
		return 0; // �r���Q���ɑΉ�����B�Ή��������Ȃ��ꍇ��0�ȊO�ɂ���B
	}
	return MJR_NOTCARED;
}
