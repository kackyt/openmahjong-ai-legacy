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
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define AI_TEHAI_LIMIT    (14)
#define AI_FLAG_NONE          (0)
#define AI_FLAG_MACHI         (8)   // 待ち牌
#define AI_FLAG_EFFECT_SYUNTSU  (7)   // 有効牌
#define AI_FLAG_EFFECT_KOUTSU   (6)   // 有効牌
#define AI_FLAG_EFFECT_KANTSU   (5)   // 有効牌
#define AI_FLAG_EFFECT_RYANTAH  (4)   // 有効牌
#define AI_FLAG_EFFECT_ATAMA    (3)   // 有効牌
#define AI_FLAG_EFFECT_KANTAH   (2)   // 有効牌
#define AI_FLAG_EFFECT_PENTAH   (1)   // 有効牌

	typedef struct{
		int tehai[14];
		int tehai_max;
	} AGARI_LIST;

	typedef struct{
		int mentsuflag[14];
		int machi[34];
		int shanten;
	} TENPAI_LIST;

	typedef struct {
		int pai;
		int count;
		int startpos;
	} PAICOUNT;

#define AI_UKIHAI          (0)
#define AI_KOUTSU          (1)
#define AI_SYUNTSU_START   (2)
#define AI_SYUNTSU_MIDDLE  (3)
#define AI_SYUNTSU_END     (4)
#define AI_KANTSU          (5)
#define AI_ATAMA           (6)
#define AI_TOITSU          (7)
#define AI_MENTSU_LIMIT    (8)

#define AI_NIL_PAI         (63)

#define AI_MACHI_RYANMEN    (0)
#define AI_MACHI_PENCHAN    (1)
#define AI_MACHI_KANCHAN    (2)
#define AI_MACHI_SHANPON      (3)
#define AI_MACHI_TANKI      (4)

#define AI_TRUE         (1)
#define AI_FALSE        (0)

extern int search_agari(int *paiarray,int paiSize,AGARI_LIST *pList,int listSize,int actualPaiSize);
extern int search_tenpai(int *paiarray,int paiSize,int *pMachi,TENPAI_LIST *pList,int listSize,int maxshanten);
extern double probabilityFunction(double,int);
extern int permutation(int m,int n);
extern int factorial(int n);
extern int combination(int m,int n);

#ifdef __cplusplus
}
#endif