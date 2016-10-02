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
#include "MahjongScoreAI.h"
#include "AILib.h"

// 手牌を部分的に評価する
static double eval_Tehai_sub(MahjongAIState &param,int sthai,int atama_flag)
{
	int p=0,sc_max=0,sc,kazu,chk;
	char c;
	UINT tmp;

	for(p=0;p<34;p++) if (param.te_cnt[p]) break;
	if (p==34) return 0;
	c = param.te_cnt[p];

	// 刻子の評価
	if (c>=3) {
		param.te_cnt[p]-=3;
		sc_max = eval_Tehai_sub(param,sthai,atama_flag)+18;
		param.te_cnt[p]+=3;
	}

	// 対子の評価
	if (c>=2) {
		param.te_cnt[p]-=2;
		tmp = MahjongScoreAI::MJSendMessage(MJMI_GETVISIBLEHAIS,p,0);
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("GETVISIBLEHAIS %u\n"),tmp);
#endif
		sc = eval_Tehai_sub(param,sthai,1)+6*(!param.anpai[p][0] || !atama_flag)+!atama_flag*12-tmp;
		if (sc>sc_max) sc_max = sc;
		param.te_cnt[p]+=2;
	}

	// 数牌の評価
	if (p<27){
		kazu = p%9;
		if (kazu<7){
			if (param.te_cnt[p+2]){
				param.te_cnt[p]--; param.te_cnt[p+2]--;

				// カンチャンの評価
				if (!param.anpai[p+1][0] && sthai!=p+1){
					tmp = MahjongScoreAI::MJSendMessage(MJMI_GETVISIBLEHAIS,p+1,0);
#ifdef AIDUMP_COMMAND
					fprintf(fp,TEXT("GETVISIBLEHAIS %u\n"),tmp);
#endif
					sc = eval_Tehai_sub(param,sthai,atama_flag)+8-tmp;
					if (sc>sc_max) sc_max = sc;
					if (kazu<5) if (param.te_cnt[p+4]) if (!param.anpai[p+3][0] && sthai!=p+3){
						param.te_cnt[p+4]--;
						sc = eval_Tehai_sub(param,sthai,atama_flag)+12;
						if (sc>sc_max) sc_max = sc;
						param.te_cnt[p+4]++;
					}
				}

				// 順子の評価
				if (param.te_cnt[p+1]){
					param.te_cnt[p+1]--;
					sc = eval_Tehai_sub(param,sthai,atama_flag)+18;
					if (sc>sc_max) sc_max = sc;
					param.te_cnt[p+1]++;
				}
				param.te_cnt[p]++; param.te_cnt[p+2]++;
			}
		}

		// 両面待ち、ペンチャンの評価
		if (kazu<8){
			if (param.te_cnt[p+1]){
				chk = 0;
				if (kazu>0) if (param.anpai[p-1][0] || sthai==p-1) chk = 1;
				if (kazu<7) if (param.anpai[p+2][0] || sthai==p+2) chk = 1;
				if (!chk){
					param.te_cnt[p]--; param.te_cnt[p+1]--;
					sc = eval_Tehai_sub(param,sthai,atama_flag)+8+(kazu>0 && kazu<7)*4;
					if (sc>sc_max) sc_max = sc;
					param.te_cnt[p]++; param.te_cnt[p+1]++;
				}
			}
		}
	}

	// 牌を一つ削った状態での評価点も出してみる
	param.te_cnt[p]--;
	sc = eval_Tehai_sub(param,sthai,atama_flag);
	if (sc>sc_max) sc_max = sc;
	param.te_cnt[p]++;
	return sc_max;
}

// 牌を評価して評価値を返す
static double eval_hai(MahjongAIState &param,int hai,UINT *dora,int doras)
{
	double ret = 1.0;
	int j;
	double tmp;

	if (hai<27){
		if((hai % 9) == 0 || (hai % 9) == 8){
			ret += SCORE_SUPAI_19;
		}else if((hai % 9) == 1 || (hai % 9) == 7){
			ret += SCORE_SUPAI_28;
		}else{
			ret += SCORE_SUPAI_34567;
		}
	}else {
		tmp = param.nokori[hai];
	
		if (hai>30 || hai==param.cha+27 || hai==param.kaze+27){
			if(param.te_cnt[hai] >= 3){
				ret += SCORE_HANPAI_TRIPLE_BIAS;
			}
			if (tmp>1.0 && param.te_cnt[hai] == 2)
				ret+= SCORE_HANPAI_DOUBLE_BIAS;
			if (tmp>2.0 && param.te_cnt[hai] == 1)
				ret+= SCORE_HANPAI_BIAS;
		}
	}


	for(j=0;j<doras;j++)
		if (hai==(int)dora[j]) ret+=SCORE_DORA_BIAS;
	return ret;
}

// 手牌を評価して評価値を返す
double MahjongAIType3::evalSutehaiSub(MahjongAIState &param,int hai)
{
	int i,j;
	double ret1,ret2;
	int shanten;
	TENPAI_LIST list;
	int doras;
	UINT dora[6];
	ret1 = 0.0;

	doras = MJSendMessage(MJMI_GETDORA,(UINT)dora,0);

	/* シャンテン数を数える */
	for(i=0;i<34;i++){
		if (param.te_cnt[i]){
			ret1 += eval_hai(param,i,dora,doras);
		}
	}
	shanten = search_tenpai((int*)param.tehai.tehai,param.tehai.tehai_max,NULL,&list,1,6);

	ret2 = eval_Tehai_sub(param,hai,0);

	return (ret1 + ret2) * pow(0.8,shanten);
}
