
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "AILib.h"
#include "MIPIface.h"


typedef struct {
	UINT hai[18];
	int num;
} MJ_SUTEHAI;

typedef struct {
	UINT tehai[13];
	MJ_SUTEHAI sutehai[4];
	UINT dora[4];
	int doranum;
	int tehai_max;
	UINT tsumohai;
	int kyoku;
	int zikaze;
} MJ_TAKU;

static MJ_TAKU taku = {
	{6,6,8,8,8,13,14,14,15,20,20,22,24},
	{
		{ {32,8,29,27,31,10},6},
		{ {17,28,7,10,2,2},6},
		{ {32,29,18,30,11,27},6},
		{ {18,32,29,30,31,28,17},7},
	},
	{0},
	1,
	13,
	25,
	7,
	1
};
extern "C" {
UINT WINAPI MJPInterfaceFunc(void* inst,UINT message,UINT param1,UINT param2);
};

typedef struct {
	GAMESTATE gamestate;
	int agarihai;
} MJ_GAMESTATE;

static int scoreCallback(int*paiarray,int*mentsu,int length,int machi,void *inf)
{
	RESULT_ITEM item;
	MJ_GAMESTATE *state = (MJ_GAMESTATE *)inf;
	make_resultitem(paiarray,mentsu,length,&item,&state->gamestate,state->agarihai,machi);

	return item.score;
	//return item.mentsusize == 7 ? item.score/300 : item.score;
}

UINT WINAPI MJSendMessage(LPVOID inst,UINT message,UINT param1,UINT param2)
{
	UINT ret = 0;
	int idx;
	int i,j;
	MJITehai *pTehai;
	UINT *p;
	MJ_SUTEHAI *pSutehai;
	MJIKawahai *pKawahai;

	switch(message){
	case MJMI_GETTEHAI:
		pTehai = (MJITehai*)param2;
		memset(pTehai,0,sizeof(MJITehai));
		if(param1 == 0){
			memcpy(pTehai->tehai,taku.tehai,taku.tehai_max*sizeof(UINT));
			pTehai->tehai_max = taku.tehai_max;
		}
		ret = 1;
		break;
	case MJMI_GETMACHI:
		p = (UINT*)param2;
		for(i=0;i<34;i++){
			p[i] = 0;
		}
		ret = 0;
		if(param1 != 0){
			TENPAI_LIST tlist;
			int num;
			pTehai = (MJITehai*)param1;
			num = search_tenpai((int *)pTehai->tehai,pTehai->tehai_max,(int *)p,&tlist,1,0);
			ret = num > 0 ? 1 : 0;
		}
		break;
	case MJMI_GETAGARITEN:
		if(param1 != 0){
			MJ_GAMESTATE gs;
			pTehai = (MJITehai*)param1;

			memset(&gs,0,sizeof(gs));
			gs.gamestate.zikaze = taku.zikaze;
			gs.gamestate.bakaze = taku.kyoku / 4;

			for(i=0;i<pTehai->ankan_max;i++){
				gs.gamestate.nakilist[gs.gamestate.naki].category = AI_ANKAN;
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[0] = pTehai->ankan[0];
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[1] = pTehai->ankan[1];
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[2] = pTehai->ankan[2];
				gs.gamestate.naki++;
			}

			for(i=0;i<pTehai->minkan_max;i++){
				gs.gamestate.nakilist[gs.gamestate.naki].category = AI_MINKAN;
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[0] = pTehai->minkan[i];
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[1] = pTehai->minkan[i];
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[2] = pTehai->minkan[i];
				gs.gamestate.naki++;
			}

			for(i=0;i<pTehai->minkou_max;i++){
				gs.gamestate.nakilist[gs.gamestate.naki].category = AI_KOUTSU;
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[0] = pTehai->minkou[i];
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[1] = pTehai->minkou[i];
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[2] = pTehai->minkou[i];
				gs.gamestate.naki++;
			}

			for(i=0;i<pTehai->minshun_max;i++){
				gs.gamestate.nakilist[gs.gamestate.naki].category = AI_SYUNTSU;
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[0] = pTehai->minshun[i];
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[1] = pTehai->minshun[i]+1;
				gs.gamestate.nakilist[gs.gamestate.naki].pailist[2] = pTehai->minshun[i]+2;
				gs.gamestate.naki++;
			}

			p = (unsigned int *)&gs.gamestate.dorapai[0];
			for(i=0;i<taku.doranum;i++){
				switch(taku.dora[i]){
				case 33:
					p[i] = 31;
					break;
				case 30:
					p[i] = 27;
					break;
				case 8:
					p[i] = 0;
					break;
				case 17:
					p[i] = 9;
					break;
				case 26:
					p[i] = 18;
					break;
				default:
					p[i] = taku.dora[i] + 1;
					break;
				}
			}

			gs.gamestate.dorasize = taku.doranum;
			gs.gamestate.count = 2;

			gs.agarihai = (int)param2;

			ret = search_score((int *)pTehai->tehai,pTehai->tehai_max,&gs,scoreCallback);
		}
		break;
	case MJMI_GETKAWA:
		idx = LOWORD(param1);
		memcpy((UINT*)param2,&taku.sutehai[idx].hai,sizeof(UINT)*taku.sutehai[idx].num);
		ret = taku.sutehai[idx].num;
		break;
	case MJMI_GETKAWAEX:
		idx = LOWORD(param1);
		pKawahai = (MJIKawahai*)param2;
		pSutehai = &taku.sutehai[idx];
		for(i=0;i<pSutehai->num;i++){
			pKawahai[i].hai = pSutehai->hai[i];
			pKawahai[i].state = 0;
		}
		ret = pSutehai->num;
		break;
	case MJMI_GETDORA:
		p = (UINT*)param1;
#if 0
		for(i=0;i<taku.doranum;i++){
				p[i] = taku.dora[i];
		}
#else
		for(i=0;i<taku.doranum;i++){
			switch(taku.dora[i]){
			case 33:
				p[i] = 31;
				break;
			case 30:
				p[i] = 27;
				break;
			case 8:
				p[i] = 0;
				break;
			case 17:
				p[i] = 9;
				break;
			case 26:
				p[i] = 18;
				break;
			default:
				p[i] = taku.dora[i] + 1;
				break;
			}
		}
#endif
		ret = taku.doranum;
		break;
	case MJMI_GETHAIREMAIN:
		ret = 70;
		for(i=0;i<4;i++){
			ret-= taku.sutehai[i].num;
		}
		ret--;
		break;
	case MJMI_GETVISIBLEHAIS:
		ret = 0;
		idx = LOWORD(param1);

		for(i=0,pSutehai = &taku.sutehai[0];i<4;i++,pSutehai++){
			for(j=0;j<pSutehai->num;j++){
				if(pSutehai->hai[j] == idx) ret++;
			}
		}

		
		for(i=0;i<taku.doranum;i++){
			if(taku.dora[i] == idx){
				ret++;
			}
		}
		
		break;
	case MJMI_FUKIDASHI:
		printf((const char*)param1);
		printf("\n");
		break;
	case MJMI_GETSCORE:
		ret = 25000;
		break;
	case MJMI_GETVERSION:
		ret = 12;
		break;
	case MJMI_GETKYOKU:
	case MJMI_GETHONBA:
	case MJMI_GETREACHBOU:
	case MJMI_ANKANABILITY:
	case MJMI_KKHAIABILITY:
	case MJMI_LASTTSUMOGIRI:
	case MJMI_GETRULE:
	case MJMI_SETSTRUCTTYPE:
	case MJMI_SETAUTOFUKIDASHI:
	case MJMI_GETWAREME:
	default:
		ret = 0;
		break;
	}

	return ret;

}

//#define LOAD_LIBRARY

int main()
{
	UINT size;
	void *inst;
	UINT ret;
	MJPIFunc func;
#ifdef LOAD_LIBRARY
	HMODULE comp;
	
	comp = ::LoadLibrary("MahjongAI.dll");
	if(comp == NULL){
		printf("Cannot load Library.\n");
		return -1;
	}

	func = (MJPIFunc)::GetProcAddress(comp,"MJPInterfaceFunc");

	if(func == NULL){
		printf("Cannot load Function.\n");
		return -1;
	}
#else
	func = MJPInterfaceFunc;
#endif

	func(NULL,MJPI_INITIALIZE,0,(UINT)MJSendMessage);
	size = func(NULL,MJPI_CREATEINSTANCE,0,0);
	if(size > 0){
		inst = malloc(size);
	}


	/* 途中参加でエミュレート */
	func(inst,MJPI_ONEXCHANGE,MJST_INKYOKU,MAKELPARAM(taku.kyoku,taku.zikaze));

	ret = func(inst,MJPI_SUTEHAI,taku.tsumohai,0);

	printf("ret = %d flag = %04x\n",ret & 0x3F ,ret & 0xFF80);

	free(inst);
#ifdef LOAD_LIBRARY
	::FreeLibrary(comp);
#endif
	return 0;
}

