#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define AI_TEHAI_LIMIT    (14)
#define AI_FLAG_NONE          (0)
#define AI_FLAG_MACHI         (8)   // ‘Ò‚¿”v
#define AI_FLAG_EFFECT_SYUNTSU  (7)   // —LŒø”v
#define AI_FLAG_EFFECT_KOUTSU   (6)   // —LŒø”v
#define AI_FLAG_EFFECT_KANTSU   (5)   // —LŒø”v
#define AI_FLAG_EFFECT_RYANTAH  (4)   // —LŒø”v
#define AI_FLAG_EFFECT_ATAMA    (3)   // —LŒø”v
#define AI_FLAG_EFFECT_KANTAH   (2)   // —LŒø”v
#define AI_FLAG_EFFECT_PENTAH   (1)   // —LŒø”v

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

extern int search_tenpai(int *paiarray,int paiSize,TENPAI_LIST *pList,int listSize,int maxshanten);
extern double probabilityFunction(double,int);
extern int permutation(int m,int n);
extern int factorial(int n);
extern int combination(int m,int n);

#ifdef __cplusplus
}
#endif