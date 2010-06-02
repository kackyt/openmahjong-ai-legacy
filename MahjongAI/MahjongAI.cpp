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
#include <windows.h>
#include <stdio.h>
#include "MIPIface.h"
#include "AILib.h"

#define AIDUMP
#define AIDUMP_1

#define SCORE_KOUKEI_BIAS        (1.0)
#define SCORE_MENTSU_BIAS        (1.0)
#define SCORE_ANPAI              (0.08)
#define SCORE_SUZI               (0.04)
#define SCORE_RIICHI_BIAS        (2.0) 
#define SCORE_SUPAI_BIAS         (0.1)
#define SCORE_HANPAI_TRIPLE_BIAS (0.02)
#define SCORE_HANPAI_BIAS        (0.01)
#define SCORE_DORA_BIAS          (0.05)

class MahjongAI {
public :
	UINT InterfaceFunc(UINT message,UINT param1,UINT param2);

protected :
#ifdef AIDUMP
	FILE *fp;
#endif
	int machi[34];
	char reach_flag[4];
	int te_cnt[34];
	int menzen;
	MJITehai tehai;
	int nakiok_flag;
	int sthai;
	double tehai_score;
	char anpai[34][4];
	int cha;
	int tenpai_flag;
	static int kyoku,kaze;
	int jun;

	int search(int obj,int start,int mask);
	void set_machi(void);
	void set_Tehai(void);
	UINT sutehai_sub(int tsumohai);
	double eval_Tehai_sub(int atama_flag);
	double eval_Tehai_sub2(int atama_flag);
	double eval_Tehai_sub3(int atama_flag,int remain,int shanten,int depth);
	double eval_Tehai_recursion(int atama_flag,int remain,int shanten,int depth);
	double eval_Tehai(void);
	double eval_hai(int hai);
	double eval_sutehai(int hai);
	int calc_sutehai(void);
	int nakability(int hai,int chii_flag);
	UINT koe_req(int no,int hai);
	UINT on_start_kyoku(int k,int c);
	UINT on_end_kyoku(UINT reason,LONG* inc_sc);
	UINT on_action(int player,int taishou,UINT action);
	UINT on_start_game(void);
	UINT on_end_game(int rank,LONG score);
	UINT on_exchange(UINT state,UINT option);
};

int MahjongAI::kyoku,MahjongAI::kaze;

TCHAR player_name[] = TEXT("テストAI");

UINT (WINAPI *MJSendMessage)(MahjongAI*,UINT,UINT,UINT);


// 手牌の中から任意の牌を見つけ、その位置を返す
int MahjongAI::search(int obj,int start,int mask)
{
	while(start<(int)tehai.tehai_max){
		if (!(mask&(1<<start)) && (int)tehai.tehai[start]==obj) break;
		start++;
	}
	return start<(int)tehai.tehai_max ? start : -1;
}

// 当たり牌を調べ、配列machiに入れる。
// また張っているかどうか調べてtenpai_flagをセットする。
void MahjongAI::set_machi(void)
{
	int i,j,cnt;
	(*MJSendMessage)(this,MJMI_GETMACHI,0,(UINT)machi);
	tenpai_flag = 0;
	for(i=0;i<34;i++){
		if (machi[i]) {
			cnt = 0;
			for(j=0;j<(int)tehai.tehai_max;j++) if ((int)tehai.tehai[j]==i) cnt++;
			if (cnt+(*MJSendMessage)(this,MJMI_GETVISIBLEHAIS,i,0)<4){
				tenpai_flag = 1;
				return;
			}
			tenpai_flag = -1;
		}
	}
}

#include "MJ0.h"

// 手牌の変数tehaiとte_cntをセットする
void MahjongAI::set_Tehai(void)
{
	int i;
#ifdef AIDUMP_1
	MJITehai mjtehai[4];
	MJ0PARAM param[4];
	MJIKawahai kawahai[4][20];
	UINT dora[8];
	double nokori[34];
	double kikenhai[34];
	double mentsu1[27+34];
	double mentsu2[27+34];
	double mentsu3[27+34];
	int doralen;

	param[0].pTehai = (MJITehai1 *)&mjtehai[0];
	param[1].pTehai = (MJITehai1 *)&mjtehai[1];
	param[2].pTehai = (MJITehai1 *)&mjtehai[2];
	param[3].pTehai = (MJITehai1 *)&mjtehai[3];
	(*MJSendMessage)(this,MJMI_GETTEHAI,0,(UINT)&mjtehai[0]);
	(*MJSendMessage)(this,MJMI_GETTEHAI,1,(UINT)&mjtehai[1]);
	(*MJSendMessage)(this,MJMI_GETTEHAI,2,(UINT)&mjtehai[2]);
	(*MJSendMessage)(this,MJMI_GETTEHAI,3,(UINT)&mjtehai[3]);
	param[0].pKawahai = &kawahai[0][0];
	param[1].pKawahai = &kawahai[1][0];
	param[2].pKawahai = &kawahai[2][0];
	param[3].pKawahai = &kawahai[3][0];
	param[0].kawalength = (*MJSendMessage)(this,MJMI_GETKAWAEX,(20 << 16) + 0,(UINT)&kawahai[0][0]);
	param[1].kawalength = (*MJSendMessage)(this,MJMI_GETKAWAEX,(20 << 16) + 1,(UINT)&kawahai[1][0]);
	param[2].kawalength = (*MJSendMessage)(this,MJMI_GETKAWAEX,(20 << 16) + 2,(UINT)&kawahai[2][0]);
	param[3].kawalength = (*MJSendMessage)(this,MJMI_GETKAWAEX,(20 << 16) + 3,(UINT)&kawahai[3][0]);

	doralen = (*MJSendMessage)(this,MJMI_GETDORA,(UINT)dora,0);

	MJ0(&param[0],(int*)dora,doralen,nokori,kikenhai,mentsu1,mentsu2,mentsu3);

	fprintf(fp,TEXT("<NOKORI>"));

	for(i=0;i<34;i++){
		fprintf(fp,TEXT("[%.1f]"),nokori[i]);
	}

	fprintf(fp,TEXT("</NOKORI>"));
#endif

	(*MJSendMessage)(this,MJMI_GETTEHAI,0,(UINT)&tehai);
#ifdef AIDUMP_COMMAND
	fprintf(fp,TEXT("GET TEHAI\n"));
#endif
	for(i=0;i<34;i++) te_cnt[i] = 0;
	for(i=0;i<(int)tehai.tehai_max;i++) te_cnt[tehai.tehai[i]]++;
}

// 捨て牌時の処理
// tsumohai : 今つもってきた牌
UINT MahjongAI::sutehai_sub(int tsumohai)
{
	int mc[34];
	UINT rchk=MJPIR_SUTEHAI;
	int i,hai,del_hai,hai_remain,tmp;

	// 現在の手牌の状態をセットする
	set_Tehai();
	
	// 現在の待ち牌を取得する
	set_machi();

	// ツモった場合は「ツモ」る
	if (tsumohai>=0 && tsumohai<34) if (machi[tsumohai]) return MJPIR_TSUMO;

	// リーチをかけている場合は「ツモ切り」
	if (reach_flag[0]) return MJPIR_SUTEHAI | 13;

	// 九種九牌で流せる場合は流す
	tmp = (*MJSendMessage)(this,MJMI_KKHAIABILITY,0,0);

#ifdef AIDUMP_COMMAND
	fprintf(fp,TEXT("KKHAIABILITY %u\n"),tmp);
#endif

	if (tmp) return MJPIR_NAGASHI;
	
	// もしツモってきた牌があるなら、その牌を入れる
	if (tsumohai>=0 && tsumohai<34) te_cnt[tsumohai]++;
	
	// 捨てる牌を決める
	hai = calc_sutehai();
	if (hai<(int)tehai.tehai_max) del_hai = tehai.tehai[hai]; else del_hai = tsumohai;
	
	// 門前で、テンパった場合はリーチをかけようかなぁ？
	if (menzen){
		if (hai<(int)tehai.tehai_max){
			tehai.tehai[hai] = tsumohai;
		}
		hai_remain = (*MJSendMessage)(this,MJMI_GETHAIREMAIN,0,0);
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("GETHAIREMAIN %u\n"),hai_remain);
#endif
		tenpai_flag = (*MJSendMessage)(this,MJMI_GETMACHI,(UINT)&tehai,(UINT)mc);
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("GETMACHI %u\n"),tenpai_flag);
#endif
		for(i=0;i<34;i++){
			if (mc[i]){
				tmp = (*MJSendMessage)(this,MJMI_GETVISIBLEHAIS,i,0);
#ifdef AIDUMP_COMMAND
				fprintf(fp,TEXT("GETVISIBLEHAIS %u\n"),tmp);
#endif

				if (te_cnt[i]+tmp<4){
					if (hai_remain>60){ rchk = MJPIR_REACH; break;}
					tmp = (*MJSendMessage)(this,MJMI_GETAGARITEN,(UINT)&tehai,i);
#ifdef AIDUMP_COMMAND
					fprintf(fp,TEXT("GETAGARITEN %u\n"),tmp);
#endif
					
					if (!tmp){ rchk = MJPIR_REACH; break;}
				}
			}
		}
	}

	// 各種フラグのセット
	if (rchk==MJPIR_REACH) reach_flag[0] = 1;
	return hai|rchk;
}

/* 再帰計算用 */
double MahjongAI::eval_Tehai_recursion(int atama_flag,int remain,int shanten,int depth)
{
	int i;

	// 捨てる牌を一つ一つ試してみて、もっとも評価値の高いものをとる
	double sc_max = -1;
	double sc;

#ifdef AIDUMP_1
	int j;
	fprintf(fp,"<TEHAI> ");
	for(i=0;i<34;i++){
		for(j=0;j<te_cnt[i];j++){
			fprintf(fp,"%d ",i);
		}
	}
	fprintf(fp,"</TEHAI>");

#endif

	for(i=0;i<34;i++){
		if (!te_cnt[i]) continue;
#ifdef AIDUMP_1
		fprintf(fp,"<SUTEHAI><NUM>%d</NUM>",i);
#endif
		te_cnt[i]--;
		sc = eval_Tehai_sub3(atama_flag,remain,shanten,depth);
#ifdef AIDUMP_1
		fprintf(fp,"<TOTAL>%.3f</TOTAL>",sc);
#endif
		if (sc>sc_max){
			sc_max = sc;
		}
		te_cnt[i]++;
#ifdef AIDUMP_1
		fprintf(fp,"</SUTEHAI>");
#endif
	}

	return sc_max;
}

// 手牌を部分的に評価する
double MahjongAI::eval_Tehai_sub3(int atama_flag,int remain,int shanten,int depth)
{
#define MAI_TENPAI_SIZE (20)
	TENPAI_LIST list[MAI_TENPAI_SIZE];
	int yukohai[34];
	int yukonum;
	int num;
	int i,j;
	int c;
	int painum,pais;
	double maxval,val;
	int paiarray[14];
	int machi[34];
	
	painum = 0;

#ifdef AIDUMP_1
	fprintf(fp,"<TEHAI>");
#endif
	/* 面倒だけど、また手牌の配列に戻す (暫定) */
	for(i=0;i<34;i++){
		for(j=0;j<te_cnt[i];j++){
			paiarray[painum++] = i;
#ifdef AIDUMP_1
			fprintf(fp,"%d ",i);
#endif
		}
	}

#ifdef AIDUMP_1
	fprintf(fp,"</TEHAI>");
#endif

	memset(yukohai,0,sizeof(yukohai));
	memset(list,0,sizeof(list));

	maxval = 0.0;

	num = search_tenpai(paiarray,painum,machi,list,MAI_TENPAI_SIZE,shanten);

	for(c=0;c<=shanten;c++){
		for(i=0;i<num;i++){
			if(list[i].shanten == c){
				for(j=0;j<34;j++){
					if(yukohai[j] < list[i].machi[j]){
						yukohai[j] = list[i].machi[j];
					}
				}
			}
		}

		yukonum = 0;
		val = 0.0;
		// 有効牌のうち、シャンテン数分を持ってくる確率を求める
		// (面子にならない可能性があるがここはちょっと簡略化)
		for(i=0;i<34;i++){
			// 牌の数で有効牌をソートする
			if(yukohai[i] > 0){
				pais = 4 - te_cnt[i] - (*MJSendMessage)(this,MJMI_GETVISIBLEHAIS,i,0);
				if(c == 0 || depth == 0){
					return probabilityFunction((double)pais / (double)remain,remain/4); // テンパイ
				}else{
					pais = remain/c;
					te_cnt[i]++;
					if(pais > 0){
						val += probabilityFunction((double)pais / (double)remain,pais/4) * eval_Tehai_recursion(atama_flag,remain-pais,c-1,depth-1);
					}
					te_cnt[i]--;
				}
				yukonum++;
			}
		}

		if(yukonum > 0) return val;

	}

	return 0.0;
}

// 手牌を部分的に評価する
double MahjongAI::eval_Tehai_sub2(int atama_flag)
{
#define MAI_TENPAI_SIZE (20)
	TENPAI_LIST list[MAI_TENPAI_SIZE];
	int yukohai[34];
	int machi[34];
	int yukonum;
	int num;
	int i,j;
	int painum;
	int shanten,remain;
	int junremain;
	int maxshanten;
	double tmp,maxval;
	int paiarray[14];
	int avepais;
	double avebias; // 牌の個数および有効牌の評価点の平均値を求める
	
	painum = 0;

	/* 面倒だけど、また手牌の配列に戻す (暫定) */
	for(i=0;i<34;i++){
		for(j=0;j<te_cnt[i];j++){
			paiarray[painum++] = i;
			fprintf(fp,"%d ",i);
		}
	}

	if(jun > 18){
		junremain = 1;
	}else{
		junremain = 18 - jun;
	}


	remain = (*MJSendMessage)(this,MJMI_GETHAIREMAIN,0,0);

	memset(yukohai,0,sizeof(yukohai));
	memset(list,0,sizeof(list));

	maxval = 0.0;

	// ４シャンテンまで計算する
	num = search_tenpai(paiarray,painum,machi,list,MAI_TENPAI_SIZE,4);

	for(shanten=1;shanten<=4;shanten++){
		for(i=0;i<num;i++){
			if(list[i].shanten == shanten){
				for(j=0;j<34;j++){
					if(yukohai[j] < list[i].machi[j]){
						yukohai[j] = list[i].machi[j];
					}
				}
			}
		}

		avepais = 0;
		avebias = 0.0;
		yukonum = 0;
		// 有効牌のうち、シャンテン数分を持ってくる確率を求める
		// (面子にならない可能性があるがここはちょっと簡略化)
		for(i=0;i<34;i++){
			// 牌の数で有効牌をソートする
			if(yukohai[i] > 0){
				painum = 4 - (*MJSendMessage)(this,MJMI_GETVISIBLEHAIS,i,0);

				// 牌の個数および有効牌の評価点の合計を求める
				avepais += painum;
				if(yukohai[i] >= AI_FLAG_EFFECT_KOUTSU){
					avebias += SCORE_MENTSU_BIAS;
				}else if(yukohai[i] >= AI_FLAG_EFFECT_RYANTAH){
					avebias += SCORE_KOUKEI_BIAS;
				}else{
					avebias += 1.0;
				}
				yukonum++;
			}
		}

		tmp = 1.0;

		for(i=0;i<shanten;i++){
			tmp *= probabilityFunction((double)avepais / (double)yukonum / (double)remain,junremain/shanten);
		}

		tmp *= avebias / (double)yukonum; // * combination(yukonum,shanten);

		if(tmp > maxval){
			maxval = tmp;
			maxshanten = shanten;
		}

	}

#ifdef AIDUMP_1
	fprintf(fp,"shanten = %d value=%.3f",maxshanten,maxval);
#endif

	return maxval;
}
// 手牌を部分的に評価する
double MahjongAI::eval_Tehai_sub(int atama_flag)
{
	int p=0,sc_max=0,sc,kazu,chk;
	char c;
	UINT tmp;

	for(p=0;p<34;p++) if (te_cnt[p]) break;
	if (p==34) return 0;
	c = te_cnt[p];

	// 刻子の評価
	if (c>=3) {
		te_cnt[p]-=3;
		sc_max = eval_Tehai_sub(atama_flag)+18;
		te_cnt[p]+=3;
	}

	// 対子の評価
	if (c>=2) {
		te_cnt[p]-=2;
		tmp = (*MJSendMessage)(this,MJMI_GETVISIBLEHAIS,p,0);
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("GETVISIBLEHAIS %u\n"),tmp);
#endif
		sc = eval_Tehai_sub(1)+6*(!anpai[p][0] || !atama_flag)+!atama_flag*12-tmp;
		if (sc>sc_max) sc_max = sc;
		te_cnt[p]+=2;
	}

	// 数牌の評価
	if (p<27){
		kazu = p%9;
		if (kazu<7){
			if (te_cnt[p+2]){
				te_cnt[p]--; te_cnt[p+2]--;

				// カンチャンの評価
				if (!anpai[p+1][0] && sthai!=p+1){
					tmp = (*MJSendMessage)(this,MJMI_GETVISIBLEHAIS,p+1,0);
#ifdef AIDUMP_COMMAND
					fprintf(fp,TEXT("GETVISIBLEHAIS %u\n"),tmp);
#endif
					sc = eval_Tehai_sub(atama_flag)+8-tmp;
					if (sc>sc_max) sc_max = sc;
					if (kazu<5) if (te_cnt[p+4]) if (!anpai[p+3][0] && sthai!=p+3){
						te_cnt[p+4]--;
						sc = eval_Tehai_sub(atama_flag)+12;
						if (sc>sc_max) sc_max = sc;
						te_cnt[p+4]++;
					}
				}

				// 順子の評価
				if (te_cnt[p+1]){
					te_cnt[p+1]--;
					sc = eval_Tehai_sub(atama_flag)+18;
					if (sc>sc_max) sc_max = sc;
					te_cnt[p+1]++;
				}
				te_cnt[p]++; te_cnt[p+2]++;
			}
		}

		// 両面待ち、ペンチャンの評価
		if (kazu<8){
			if (te_cnt[p+1]){
				chk = 0;
				if (kazu>0) if (anpai[p-1][0] || sthai==p-1) chk = 1;
				if (kazu<7) if (anpai[p+2][0] || sthai==p+2) chk = 1;
				if (!chk){
					te_cnt[p]--; te_cnt[p+1]--;
					sc = eval_Tehai_sub(atama_flag)+8+(kazu>0 && kazu<7)*4;
					if (sc>sc_max) sc_max = sc;
					te_cnt[p]++; te_cnt[p+1]++;
				}
			}
		}
	}

	// 牌を一つ削った状態での評価点も出してみる
	te_cnt[p]--;
	sc = eval_Tehai_sub(atama_flag);
	if (sc>sc_max) sc_max = sc;
	te_cnt[p]++;
	return sc_max;
}

// 手牌を評価して評価値を返す
double MahjongAI::eval_Tehai(void)
{
	double ret;
	int remain = (*MJSendMessage)(this,MJMI_GETHAIREMAIN,0,0);
#ifdef AIDUMP_1
	fprintf(fp,"<EVAL>");
#endif
	TENPAI_LIST list;
	list.shanten = 8;
#if 0
	int num = search_tenpai((int*)tehai.tehai,tehai.tehai_max,&list,1,7);
	ret = eval_Tehai_sub3(0,remain,list.shanten,1);
#else
	ret = eval_Tehai_sub(0);
#endif
	int i;
	for(i=0;i<34;i++){
		if (!te_cnt[i]) continue;
		ret *= eval_hai(i); //*te_cnt[i];
	}
#ifdef AIDUMP_1
	fprintf(fp,"</EVAL>");
#endif
	return ret;
}

// 牌を評価して評価値を返す
double MahjongAI::eval_hai(int hai)
{
	double ret = 1.0;
	int j,doras;
	UINT dora[6];
	UINT tmp;

	if (hai<27){
		//ret+= SCORE_SUPAI_BIAS;
	}else {
		tmp = (*MJSendMessage)(this,MJMI_GETVISIBLEHAIS,hai,0);
	
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("GETVISIBLEHAIS %u\n"),tmp);
#endif
		if (hai>30 || hai==cha+27 || hai==kaze+27){
			if(te_cnt[hai] >= 3){
				ret += SCORE_HANPAI_TRIPLE_BIAS;
			}
			if (tmp<2 && te_cnt[hai] == 2)
				ret+= SCORE_HANPAI_BIAS;
		}
	}

	doras = (*MJSendMessage)(this,MJMI_GETDORA,(UINT)dora,0);

#ifdef AIDUMP_COMMAND
	fprintf(fp,TEXT("GETDORA %u\n"),doras);
#endif
	for(j=0;j<doras;j++)
		if (hai==(int)dora[j]) ret+=SCORE_DORA_BIAS*te_cnt[hai];
	return ret;
}

// 捨て牌を評価して評価値を返す
double MahjongAI::eval_sutehai(int hai)
{
	int i;
	double ret = 0;

	for(i=1;i<4;i++){
		if (anpai[hai][i]) ret += SCORE_ANPAI;
		else if (hai<27){
			int kazu = hai%9,fl=1;
			if (kazu>2) if (!anpai[hai-3][i]) fl = 0;
			if (fl) if (kazu<6) if (!anpai[hai+3][i]) fl = 0;
			if (fl) ret += SCORE_SUZI;
		}
		else if (!reach_flag[i]) ret *= SCORE_RIICHI_BIAS;
	}
	return ret;
}

// 捨てる牌を決める
int MahjongAI::calc_sutehai(void)
{
	int i,ret;

	// 捨てる牌を一つ一つ試してみて、もっとも評価値の高いものをとる
	double sc_max = -1;
	int sh;
	double sc,scc,scc_max=-1;

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

	for(i=0;i<34;i++){
		if (!te_cnt[i]) continue;
#ifdef AIDUMP_1
		fprintf(fp,"<SUTEHAI><NUM>%d</NUM>",i);
#endif
		te_cnt[i]--;
		sthai = i;
		sc = eval_Tehai();
#ifdef AIDUMP_1
		fprintf(fp,"<TOTAL>%.3f</TOTAL>",sc);
#endif
		scc = eval_sutehai(i);
#ifdef AIDUMP_1
		fprintf(fp,"<SUTEHAI_VAL>%.3f</SUTEHAI_VAL>",scc);
#endif
#ifdef AIDUMP_1
		fprintf(fp,"</SUTEHAI>");
#endif

		scc += sc;
		if (scc>scc_max){ scc_max = scc; sc_max = sc; sh = i;}
		te_cnt[i]++;
	}
#ifdef AIDUMP_1
		fprintf(fp,"</CALC>");
#endif
	tehai_score = sc_max;
	ret = search(sh,0,0);

	return ret>=0 ? ret : 13;
}

// 鳴くことができるかどうか調べる
// hai : 対象の牌
// chii_flag : チーについてもチェックするかどうか
// return : 以下の値の論理和
//		 1 : ポンできる
//		 2 : カンできる
//		 4 : チー１（左）できる
//		 8 : チー２（右）できる
//		16 : チー３（中）できる
//		32 : ロンできる
int MahjongAI::nakability(int hai,int chii_flag)
{
	int x,ret=0,kazu;

	if (machi[hai]) ret |= 32;
	if (reach_flag[0]) return ret;
	if ((*MJSendMessage)(this,MJMI_GETHAIREMAIN,0,0)==0) return ret;
	if ((x=search(hai,0,0))>=0){
		if (x<(int)tehai.tehai_max-1){
			if ((int)tehai.tehai[x+1]==hai){
				ret |= 1;
				if (x<(int)tehai.tehai_max-2){
					if ((int)tehai.tehai[x+2]==hai) ret |= 2;
				}
			}
		}
	}
	if (chii_flag){
		if (hai<27){
			kazu = hai%9;
			if (kazu>1){
				if (te_cnt[hai-2]>0 && te_cnt[hai-1]>0) ret |= 8;
			}
			if (kazu<7){
				if (te_cnt[hai+2]>0 && te_cnt[hai+1]>0) ret |= 4;
			}
			if (kazu>0 && kazu<8){
				if (te_cnt[hai-1]>0 && te_cnt[hai+1]>0) ret |= 16;
			}
		}
	}
	return ret;
}

// 他家の捨て牌に対するアクションを決める
// no : だれが捨てたか
// hai : 何を捨てたか
// return : アクション
UINT MahjongAI::koe_req(int no,int hai)
{
	int chii_flag;
	double sc;
	int naki_ok;
	UINT tmp;

	set_Tehai();
	set_machi();
	chii_flag = (no == 3);
	naki_ok = nakability(hai,chii_flag);
	if (!naki_ok) return 0;
	if (naki_ok&32){
		tmp = (*MJSendMessage)(this,MJMI_GETAGARITEN,0,hai);
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("GETAGARITEN %u\n"),tmp);
#endif
		if (tmp>0) {
			return MJPIR_RON;
		}
	}
	if (tenpai_flag==1) return 0;
	sthai = -1;
	if (naki_ok&1){
		if (hai>=27){
			if (te_cnt[hai]==2){
				if (hai>=31 || hai-27==cha || hai-27==kaze || nakiok_flag){
					return MJPIR_PON;
				}
			}
		} else {
			if (nakiok_flag){
				te_cnt[hai] -= 2;
				sc = eval_Tehai();
				te_cnt[hai] += 2;
				if (sc+eval_hai(hai)*3+7>tehai_score) {
					return MJPIR_PON;
				}
			}
		}
	}
	if (!nakiok_flag) return 0;
	if (naki_ok&4){
		te_cnt[hai+1]--; te_cnt[hai+2]--;
		sc = eval_Tehai()+eval_hai(hai)+eval_hai(hai+1)+eval_hai(hai+2);
		te_cnt[hai+1]++; te_cnt[hai+2]++;
		if (sc+7>tehai_score){
			return MJPIR_CHII1;
		}
	}
	if (naki_ok&8){
		te_cnt[hai-1]--; te_cnt[hai-2]--;
		sc = eval_Tehai()+eval_hai(hai)+eval_hai(hai-1)+eval_hai(hai-2);
		te_cnt[hai-1]++; te_cnt[hai-2]++;
		if (sc+7>tehai_score){
			return MJPIR_CHII2;
		}
	}
	if (naki_ok&16){
		te_cnt[hai-1]--; te_cnt[hai+1]--;
		sc = eval_Tehai()+eval_hai(hai)+eval_hai(hai+1)+eval_hai(hai-1);
		te_cnt[hai-1]++; te_cnt[hai+1]++;
		if (sc+7>tehai_score){
			return MJPIR_CHII3;
		}
	}
	return 0;
}

// 局開始時の処理
// k : 局
// c : 家
UINT MahjongAI::on_start_kyoku(int k,int c)
{
	int i,j;
	set_Tehai();
	for(i=0;i<34;i++) {
		for (j=0;j<4;j++) anpai[i][j] = 0;
	}
	kyoku = k;
	kaze = kyoku/4;
	cha = c;
	menzen = 1;
	nakiok_flag = 0;
	jun = 0;
	sthai = -1;
	for(i=0;i<4;i++) reach_flag[i] = 0;
	tehai_score = eval_Tehai();
	set_machi();
	return 0;
}

// 局終了時の処理
// reason : 終了した理由
// inc_sc : 点数の変化
UINT MahjongAI::on_end_kyoku(UINT reason,LONG* inc_sc)
{
	if (*inc_sc>5000) (*MJSendMessage)(this,MJMI_FUKIDASHI,(UINT)TEXT("よしよし…"),0);
	return 0;
}

// アクションに対する応答をする
UINT MahjongAI::on_action(int player,int taishou,UINT action)
{
	int hai = action&63;

	if (action & MJPIR_REACH) reach_flag[player] = 1;
	if (action & (MJPIR_SUTEHAI | MJPIR_REACH)){
		anpai[hai][player] = 1;
		for(int i=0;i<4;i++) if (reach_flag[i]) anpai[hai][i] = 1;
		if (player==0) return 0;
		return koe_req(player,hai);
	}
	if ((action & MJPIR_RON) && taishou==0){
		(*MJSendMessage)(this,MJMI_FUKIDASHI,(UINT)TEXT("うっ…"),0);
	}
	if ((action & MJPIR_PON) && player==0){
		nakiok_flag = 1;
		menzen = 0;
	}
	if ((action & MJPIR_CHII1) && player==0){
		menzen = 0;
	}
	if ((action & MJPIR_CHII2) && player==0){
		menzen = 0;
	}
	if ((action & MJPIR_CHII3) && player==0){
		menzen = 0;
	}
	if ((action & MJPIR_MINKAN) && player==0){
		menzen = 0;
	}
	return 0;
}

// 半荘開始時の処理
UINT MahjongAI::on_start_game(void)
{
	return 0;
}

// 半荘終了時の処理
// rank : 順位
// score : 点数
UINT MahjongAI::on_end_game(int rank,LONG score)
{
	/*char str[40];
	sprintf(str,"%d点、%d位か…",score,rank+1);
	(*MJSendMessage)(this,MJMI_FUKIDASHI,(UINT)str,0);*/
	return 0;
}

// 途中参加時の処理
// state : そのときの状態
// option : 状態に関連して送られる情報
UINT MahjongAI::on_exchange(UINT state,UINT option)
{
	if (state==MJST_INKYOKU){
		int i,j,k;
		set_Tehai();
		for(i=0;i<34;i++) {
			for (j=0;j<4;j++) anpai[i][j] = 0;
		}

		MJIKawahai kawa[30];
		for(i=0;i<4;i++){
			k = (*MJSendMessage)(this,MJMI_GETKAWAEX,MAKELPARAM(i,30),(UINT)kawa);
			reach_flag[i] = 0;
			for(j=0;j<k;j++){
				anpai[kawa[j].hai&63][j] = 1;
				if (kawa[j].state&MJKS_REACH) reach_flag[i] = 1;
			}
		}

		kyoku = LOWORD(option);
		kaze = kyoku/4;
		cha = HIWORD(option);
		menzen = tehai.minshun_max+tehai.minkan_max+tehai.minkou_max==0;
		nakiok_flag = !menzen;
		sthai = -1;
		tehai_score = eval_Tehai();
		set_machi();
	}
	return 0;
}

// インスタンス用のインターフェース関数
UINT MahjongAI::InterfaceFunc(UINT message,UINT param1,UINT param2)
{
	UINT ret = MJR_NOTCARED;
#ifdef AIDUMP
	fp = fopen(TEXT("./AIDUMP_COMMAND.xml"),TEXT("a"));
#endif

	switch(message){
	case MJPI_SUTEHAI :
		jun++;
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("SUTEHAI %u\n"),param1);
#endif
		ret = sutehai_sub(LOWORD(param1));
		break;
	case MJPI_ONACTION :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("ONACTION %u %u\n"),param1,param2);
#endif
		ret = on_action(LOWORD(param1),HIWORD(param1),param2);
		break;
	case MJPI_STARTKYOKU :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("STARTKYOKU %u %u\n"),param1,param2);
#endif
		ret = on_start_kyoku(LOWORD(param1),LOWORD(param2));
		break;
	case MJPI_ENDKYOKU :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("ENDKYOKU %u %u\n"),param1,param2);
#endif
		ret = on_end_kyoku(param1,(LONG*)param2);
		break;
	case MJPI_STARTGAME :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("STARTGAME\n"));
#endif
		ret = on_start_game();
		break;
	case MJPI_ENDGAME :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("ENDGAME %u %u\n"),param1,param2);
#endif
		ret = on_end_game(LOWORD(param1),(LONG)param2);
		break;
	case MJPI_ONEXCHANGE :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("ONEXCHANGE %u %u\n"),param1,param2);
#endif
		ret = on_exchange(LOWORD(param1),param2);
		break;
	case MJPI_CREATEINSTANCE :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("CREATEINSTANCE\n"));
#endif
		ret = sizeof(MahjongAI);
		break;
	case MJPI_INITIALIZE :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("INITIALIZE\n"));
#endif
		MJSendMessage = (UINT (WINAPI *)(MahjongAI*,UINT,UINT,UINT))param2;
		ret = 0;
		break;
	case MJPI_YOURNAME :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("YOUENAME\n"));
#endif
		ret = (UINT)player_name;
		break;
	case MJPI_DESTROY :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("DESTROY\n"));
#endif
		ret = 0;
		break;
	case MJPI_ISEXCHANGEABLE :
#ifdef AIDUMP_COMMAND
		fprintf(fp,TEXT("ISEXCHANGEABLE\n"));
#endif
		ret = 0; // 途中参加に対応する。対応したくない場合は0以外にする。
		break;
	}

#ifdef AIDUMP
	fclose(fp);
#endif
	return ret;
}

// インターフェース関数
UINT WINAPI MJPInterfaceFunc(MahjongAI* inst,UINT message,UINT param1,UINT param2)
{
	if (inst) return inst->InterfaceFunc(message,param1,param2);
	switch(message){
	case MJPI_CREATEINSTANCE :
		return sizeof(MahjongAI);
	case MJPI_INITIALIZE :
		MJSendMessage = (UINT (WINAPI *)(MahjongAI*,UINT,UINT,UINT))param2;
		return 0;
	case MJPI_YOURNAME :
		return (UINT)player_name;
	case MJPI_DESTROY :
		return 0;
	case MJPI_ISEXCHANGEABLE :
		return 0; // 途中参加に対応する。対応したくない場合は0以外にする。
	}
	return MJR_NOTCARED;
}
