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
