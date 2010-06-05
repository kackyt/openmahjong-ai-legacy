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
#ifndef __OPEN_MAHJONG_DEF__
#define __OPEN_MAHJONG_DEF__

#define MAX_SESSION  256
#define MAX_PLAYERID  65536

#define TEHAI_LIMIT  14
#define DORAHAI_LIMIT  8
#define MENTSU_LIMIT  7
#define YAKU_LIMIT  256

#define PAI_MANZU  1
#define PAI_PINZU  2
#define PAI_SOUZU  3
#define PAI_ZIHAI  4
#define PAI_CATEGORYLIMIT  5

#define MACHI_KANCHAN  1
#define MACHI_PENCHAN  2
#define MACHI_RYANMEN  3
#define MACHI_SHANPON  4
#define MACHI_TANKI  5

#define MENTSU_SYUNTSU  1
#define MENTSU_KOUTSU  2
#define MENTSU_TOITSU  3
#define MENTSU_MINKAN  4
#define MENTSU_ANKAN  5

#define MANGAN_MANGAN  1
#define MANGAN_HANEMAN  2
#define MANGAN_BAIMAN  3
#define MANGAN_SANBAIMAN  4
#define MANGAN_YAKUMAN  5

#define TYPE_DAHAI  100
#define TYPE_TII  101
#define TYPE_PON  102
#define TYPE_DAIMINKAN  103
#define TYPE_ANKAN  104
#define TYPE_KUWAEKAN  105
#define TYPE_RON  106
#define TYPE_TSUMO  107
#define TYPE_RIICHI  108
#define TYPE_PASS  109
#define TYPE_TOUHAI  110
#define TYPE_RENDA  111
#define TYPE_YONRIICHI  112
#define TYPE_KOUHAI  113
#define TYPE_YONKANTSU  114
#define TYPE_SANCHAHO  115
#define TYPE_END  116
#define TYPE_START  117

#define ID_DAHAI  100
#define ID_PASS  120
#define ID_TII  121
#define ID_PON  170
#define ID_DAIMINKAN  173
#define ID_RON  174
#define ID_TSUMO  175
#define ID_TOUHAI  176
#define ID_KAN  180
#define ID_RIICHI  200
#define ID_STATUS  300
#define ID_UPDATE 301
#define ID_START  400
#define ID_AGARIYAME  401
#define ID_CREATE  500
#define ID_CONNECT  501
#define ID_DEBUG 600
#define ID_REPEAT 601

#define CODE_NONCREATED  100
#define CODE_WAITMEMBER  101
#define CODE_WAITCOMMAND  102
#define CODE_BUSY  103
#define CODE_WAITSYNC  104
#define CODE_PROGRESSED  105

#define MSTATE_SYNC  100
#define MSTATE_NOSYNC  101
#define MSTATE_DORMANT  102

#define LIBID_AGARIHAI 100
#define LIBID_AGARITEN 101

#define RESPONCE_OK (200)
#define RESPONCE_TIMEOUT (300)
#define RESPONCE_COMMANDID (301)
#define RESPONCE_PLAYERID (302)
#define RESPONCE_STATUS (303)
#define RESPONCE_SESSION (304)
#define RESPONCE_OTHER (305)

enum{
	WM_REFRESH=(WM_USER+100),
	WM_SNDCOMMAND,
	WM_SETVOLUME,
};

#define TAG_OPENMAHJONGSERVER "os"
#define TAG_OPENMAHJONGCLIENT "oc"
#define TAG_RESPONCE "rsp"
#define TAG_CODE "cd"
#define TAG_STATE "st"
#define TAG_NAME "nm"
#define TAG_ARG "arg"
#define TAG_TAKU "tk"
#define TAG_LIBRARY "lib"
#define TAG_AGARIHAI "ah"
#define TAG_COMMAND "c"
#define TAG_TYPE "tp"
#define TAG_PLAYER "pl"
#define TAG_PRIVATEID "pid"
#define TAG_PAI "p"
#define TAG_NO "no"
#define TAG_TSUMOGIRI "tg"
#define TAG_EVENT "ev"
#define TAG_TURN "tn"
#define TAG_SEED "seed"
#define TAG_DORA "d"
#define TAG_URADORA "ud"
#define TAG_BAKAZE "bk"
#define TAG_KYOKUCOUNT "kc"
#define TAG_RIICHIBOU "rb"
#define TAG_TSUMIBOU "tb"
#define TAG_WANPAI "wp"
#define TAG_YAMA "y"
#define TAG_COUNT "cnt"
#define TAG_MESSAGE "mes"
#define TAG_TO "to"
#define TAG_FROM "from"
#define TAG_TEXT "text"
#define TAG_ID "id"
#define TAG_MENTSU "mu"
#define TAG_NAKIMENTSU "nm"
#define TAG_CATEGORY "cat"
#define TAG_NAKIHAI "nh"
#define TAG_AITE "aite"
#define TAG_RESULT "res"
#define TAG_YAKU "yk"
#define TAG_HAN "h"
#define TAG_FU "fu"
#define TAG_MANGAN "mg"
#define TAG_SCORE "sc"
#define TAG_OYASCORE "os"
#define TAG_KOSCORE "ks"
#define TAG_MACHI "mc"
#define TAG_TSUMO "tm"
#define TAG_DORAKAZU "dk"
#define TAG_MACHIHAI "mh"
#define TAG_MEMBER "m"
#define TAG_POINT "pt"
#define TAG_MSTATE "ms"
#define TAG_TEHAI "th"
#define TAG_DAHAI "dh"
#define TAG_TSUMOHAI "tmh"
#define TAG_COMMANDLIST "cl"
#define TAG_GAMESTATE "gs"
#define TAG_ZIKAZE "z"
#define TAG_OYA "oya"
#define TAG_IPPATSU "ip"
#define TAG_ENDPAI "ep"
#define TAG_KAN "kan"
#define TAG_NAKI "nk"
#define TAG_MEMBERNAKI "mn"
#define TAG_NAKILIST "nl"
#define TAG_RIICHI "rc"
#define TAG_RESULTLIST "rl"
#define TAG_ERROR "err"
#define TAG_SEQ "seq"

#define TAG_RULE "ru"
#define TAG_GLASSHAI "glh"
#define TAG_KUITAN "kt"
#define TAG_AKA "aka"
#define TAG_AOTENJO "atj"
#define TAG_HAIPAI "hp"

#endif