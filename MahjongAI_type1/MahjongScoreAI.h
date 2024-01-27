/****************************************************************************************
 * Copyright (c) 2010-2012, Takaya Kakizaki(kacky)
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

#pragma once
#include <windows.h>
#include <float.h>
#include <vector>
#include "MIPIface.h"
#include "MJ0.h"

#define SIMULATECOUNT      (5000)

#define SCORE_KOUKEI_BIAS        (1.0)
#define SCORE_MENTSU_BIAS        (1.0)
#define SCORE_ANPAI              2 * (16)
#define SCORE_KIKENHAI           2 * (100)
#define SCORE_SUZI19             2 * (29)
#define SCORE_SUZI28             2 * (48)
#define SCORE_SUZI37             2 * (55)
#define SCORE_ZIHAI              2 * (34)
#define SCORE_19                 2 * (63)
#define SCORE_28                 2 * (70)
#define SCORE_37                 2 * (71)
#define SCORE_456                2 * (123)

#define SCORE_RIICHI_BIAS        (100.0) 
#define SCORE_IPPATSU_BIAS       (1000.0) 
#define SCORE_SUPAI_19           (0.079)
#define SCORE_SUPAI_28            (0.084)
#define SCORE_SUPAI_34567         (0.1)
#define SCORE_HANPAI_TRIPLE_BIAS (0.02)
#define SCORE_HANPAI_DOUBLE_BIAS (0.01)
#define SCORE_HANPAI_BIAS       (0.001)
#define SCORE_DORA_BIAS          (0.05)

using namespace MJAI;

typedef struct {
	double sc;
	int no;
} HAIPOINT;

struct MahjongAIState {
	Players players;
	Player myself;
	PaiArrayInt te_cnt;
	PaiArrayInt sute_cnt;
	std::vector<Pai> doras;
	int kyoku;
	int cha;
	int kaze;
	int tsumohai;

	MahjongAIState() {
		for (size_t i = 0; i < 3; i++)
		{
			players.push_back(Player());
		}
	}
};

class MahjongScoreAI {
public:
	MahjongScoreAI(){}
	virtual ~MahjongScoreAI(){}
	static UINT (WINAPI *pMJSendMessage)(void*,UINT,UINT,UINT);
	static void *pObj;
	virtual double evalSutehai(MahjongAIState &param,HAIPOINT *result,int &size) {
		int i;
		double sc,sc_max = -DBL_MAX;

		size = 0;
		for(i=0;i<34;i++){
			if (param.te_cnt[i]){
				param.te_cnt[i]--;
				sc = evalSutehaiSub(param,i);
				result[size].sc = sc;
				result[size].no = i;
				
				if(sc > sc_max){
					sc_max = sc;
				}
				
				param.te_cnt[i]++;
				size++;
			}
			
		}

		return sc_max;
	}
	virtual double evalSutehaiSub(MahjongAIState &param,int hai){
		return 0.0;
	}

	virtual UINT getDebugInt(){
		return 0;
	}

	static UINT MJSendMessage(UINT p2,UINT p3,UINT p4){
		return (*pMJSendMessage)(pObj,p2,p3,p4);
	}
};

class MahjongAIKikenhai : public MahjongScoreAI {
public:
	double evalSutehaiSub(MahjongAIState &param,int hai);
};

class MahjongAIType4 : public MahjongScoreAI {
public:
	double evalSutehaiSub(MahjongAIState &param,int hai);
	virtual UINT getDebugInt();
};
