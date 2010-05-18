// OpenMahjongClientDbgDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <time.h>
#include "OpenMahjongClientDbg.h"
#include "OpenMahjongClientDbgDlg.h"
#include "AILib.h"
#include "RuleDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define DEBUGDUMP

static const UINT ruleTable[] =
{
	1,0,0,0,
	250,0,0,0,
	0,0,0,1,
	0,1,2,1,
	0,1,3,1,
	1,0xf,0,0,
	0,0
};

static const UINT ieTable[][4] = {
	{ 0, 1, 2, 3},
	{ 3, 0, 1, 2},
	{ 2, 3, 0, 1},
	{ 1, 2, 3, 0},
};

static const TCHAR *ieStrTable[] = {_T("東"),_T("南"),_T("西"),_T("北")};
static const TCHAR *posStrTable[] = {_T("自分"),_T("下家"),_T("対面"),_T("上家")};

static UINT __cdecl syncFunc(LPVOID param)
{
	COpenMahjongClientDbgDlg *pDlg = (COpenMahjongClientDbgDlg *)param;
	AfxOleInit();
	CoInitialize(NULL);

	pDlg->gameSync();

	pDlg->SendMessage(WM_REFRESH);
	if(pDlg->m_iAgari == 1){
		pDlg->m_btnMahjong.refresh(pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer),*pDlg->m_pCurTaku);
		
		pDlg->m_bFirst = TRUE;
		pDlg->m_fileNormal.closeWaveFile();
		pDlg->m_fileCommand.closeWaveFile();
		pDlg->m_fileCommand.openWaveFile(_T("./wav/agari.wav"));
		if(pDlg->m_pCurTaku->m_event.m_command.m_iType == TYPE_KOUHAI){
			AfxMessageBox(_T("荒牌平局"));
		}else{
			pDlg->m_agariDlg.DoModal();
		}
		pDlg->m_fileCommand.closeWaveFile();
		pDlg->m_fileNormal.openWaveFile(_T("./wav/bgmnormal.wav"));
		pDlg->SendMessage(WM_SNDCOMMAND,400,0);
	}else if(pDlg->m_iAgari == 0){
		pDlg->m_btnMahjong.refresh(pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer),*pDlg->m_pCurTaku);
		
		pDlg->m_fileCommand.closeWaveFile();
		pDlg->m_fileCommand.openWaveFile(_T("./wav/command.wav"));
	}else{
		pDlg->m_fileNormal.closeWaveFile();
	}

	pDlg->m_pWorkThread = NULL;

	return 1;
}

//#define MJ_TRACE

UINT WINAPI MJSendMessage(LPVOID inst,UINT message,UINT param1,UINT param2)
{
	COpenMahjongClientDbgDlg *pDlg = (COpenMahjongClientDbgDlg *)AfxGetApp()->m_pMainWnd;
	int idx,i,score;
	CMember *member;
	MJITehai *pTehai;
	UINT *p;
	UINT ret = 0;
	CMessage mes;
	int stmode = 0;
	static int libcount = 0;

	switch(message){
	case MJMI_GETTEHAI:
		for(i=0;i<pDlg->m_iCompNum;i++){
			if(pDlg->pInst[i] == inst){
				stmode = pDlg->structMode[i];
				break;
			}
		}
		TRACE("GETTEHAI\n");
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ASSERT(idx >= 0 && idx <= 3);
		idx = (idx + param1) % 4;
		pTehai = (MJITehai*)param2;
		if(stmode == 1){
			pDlg->m_pCurTaku->getMJITehai(idx,(MJITehai1*)pTehai);
		}else{
			pDlg->m_pCurTaku->getMJITehai(idx,pTehai);
		}
		if(pTehai->tehai_max > 14){
			AfxDebugBreak();
		}
		for(i=0;i<pTehai->tehai_max;i++){
			TRACE(_T("%d "),pTehai->tehai[i]);
		}
		TRACE(_T("\n"));
		ret =  1;
		break;
	case MJMI_GETMACHI:
		TRACE("GETMACHI %d\n",libcount++);
		p = (UINT*)param2;
		for(i=0;i<34;i++){
			p[i] = 0;
		}
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ASSERT(idx >= 0 && idx <= 3);
		member = &pDlg->m_pCurTaku->m_members[idx];

		if(param1 != 0){
			TENPAI_LIST tlist[9];
			int num,j;

			pTehai = (MJITehai*)param1;
			num = search_tenpai((int *)pTehai->tehai,pTehai->tehai_max,tlist,9,0);

			for(i=0;i<num;i++){
				for(j=0;j<34;j++){
					if(tlist[i].machi[j] == AI_FLAG_MACHI){
						p[j] = 1;
					}
				}
			}
			
			ret = num > 0 ? 1 : 0;
		}else{
			for(i=0;i<member->m_aResultList.GetSize();i++){
				CResult& result = member->m_aResultList[i];
				idx = (UINT)result.m_machihai;
				p[idx] = 1;
			}
			ret = member->m_aResultList.GetSize() > 0 ? 1 : 0;
		}
		break;
	case MJMI_GETAGARITEN:
		TRACE("GETAGARITEN %d\n",libcount++);
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ASSERT(idx >= 0 && idx <= 3);
		member = &pDlg->m_pCurTaku->m_members[idx];
		score = 0;
		if(param1 != 0){
			CLibrary lib;
			pTehai = (MJITehai*)param1;
			lib.setMember(LIBID_AGARITEN,*member,pTehai,(int)param2);
			pDlg->sendLibrary(lib);
			for(i=0;i<lib.m_aResultList.GetSize();i++){
				CResult& result = lib.m_aResultList[i];
				if(result.m_iHan == 0) continue;
				idx = (UINT)result.m_machihai;
				if(idx == (int)param2){
					score = result.m_iScore;
					break;
				}
			}

			ret = score;
		}else{

			for(i=0;i<member->m_aResultList.GetSize();i++){
				CResult& result = member->m_aResultList[i];
				if(result.m_iHan == 0) continue;
				idx = (UINT)result.m_machihai;
				if(idx == (int)param2){
					score = result.m_iScore;
					break;
				}
			}
			ret = score;
		}
		break;
	case MJMI_GETKAWA:
#ifdef MJ_TRACE
		TRACE("GETKAWA\n");
#endif
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ASSERT(idx >= 0 && idx <= 3);
		idx = (idx + LOWORD(param1)) % 4;
		ret = pDlg->m_pCurTaku->getKawahai(idx,(UINT*)param2);		
		break;
	case MJMI_GETKAWAEX:
#ifdef MJ_TRACE
		TRACE("GETKAWAEX\n");
#endif
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ASSERT(idx >= 0 && idx <= 3);
		idx = (idx + LOWORD(param1)) % 4;
		ret = pDlg->m_pCurTaku->getKawahaiEx(idx,(MJIKawahai*)param2);
		break;
	case MJMI_GETDORA:
#ifdef MJ_TRACE
		TRACE("GETDORA\n");
#endif
		p = (UINT*)param1;

		for(i=0;i<pDlg->m_pCurTaku->m_aDora.GetSize();i++){
			*p = pDlg->m_pCurTaku->m_aDora[i];
			p++;
		}

		ret = pDlg->m_pCurTaku->m_aDora.GetSize();
		break;
	case MJMI_GETSCORE:
#ifdef MJ_TRACE
		TRACE("GETSCORE\n");
#endif
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ASSERT(idx >= 0 && idx <= 3);
		idx = (idx + param1) % 4;
		ret = pDlg->m_pCurTaku->m_members[idx].m_iPoint;
		break;
	case MJMI_GETKYOKU:
#ifdef MJ_TRACE
		TRACE("GETKYOKU\n");
#endif
		ret = pDlg->m_pCurTaku->m_iKyokuCount;
		break;
	case MJMI_GETHONBA:
#ifdef MJ_TRACE
		TRACE("GETHONBA\n");
#endif
		ret = pDlg->m_pCurTaku->m_iTsumibou;
		break;
	case MJMI_GETREACHBOU:
#ifdef MJ_TRACE
		TRACE("GETREACHBOU\n");
#endif
		ret = pDlg->m_pCurTaku->m_iRiichibou;
		break;
	case MJMI_GETHAIREMAIN:
#ifdef MJ_TRACE
		TRACE("GETHAIREMAIN\n");
#endif
		ret = pDlg->m_pCurTaku->m_iYama;
		break;
	case MJMI_GETVISIBLEHAIS:
#ifdef MJ_TRACE
		TRACE("GETVISIBLEHAIS\n");
#endif
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ret = pDlg->m_pCurTaku->getVisibleHais(param1,idx);
		break;
	case MJMI_ANKANABILITY:
#ifdef MJ_TRACE
		TRACE("GETANKANABILITY\n");
#endif
		idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
		ASSERT(idx >= 0 && idx <= 3);
		member = &pDlg->m_pCurTaku->m_members[idx];
		idx = 0;
		p = (UINT*)param1;
		for(i=0;i<member->m_aCommandList.GetSize();i++){
			if(member->m_aCommandList[i].m_iType == TYPE_ANKAN || member->m_aCommandList[i].m_iType == TYPE_KUWAEKAN){
				p[idx] = member->m_aCommandList[i].m_pai;
				idx++;
			}
		}
		ret = idx;
		break;
	case MJMI_KKHAIABILITY:
		ret = 0;
		break;
	case MJMI_SSPUTOABILITY:
		ret = 0;
		break;
	case MJMI_LASTTSUMOGIRI:
#ifdef MJ_TRACE
		TRACE("LASTTSUMOGIRI\n");
#endif
		ret = 0;
		break;
	case MJMI_GETRULE:
#ifdef MJ_TRACE
		TRACE("GETRULE");
#endif
		switch(param1){
		case MJRL_KUITAN:
			ret = pDlg->m_rule.m_iKuitan;
			break;
		default:
			ret = ruleTable[param1-1];
			break;
		}
		break;
	case MJMI_SETSTRUCTTYPE:
		ret = MJR_NOTCARED;
		for(i=0;i<pDlg->m_iCompNum;i++){
			if(pDlg->pInst[i] == inst){
				ret = pDlg->structMode[i];
				pDlg->structMode[i] = param1;
				break;
			}
		}
		break;
	case MJMI_FUKIDASHI:
		if(pDlg->m_pCurTaku != NULL){
			idx = pDlg->m_pCurTaku->getMemberIndex(pDlg->m_pCurPlayer);
			ASSERT(idx >= 0 && idx <= 3);
			for(i=0;i<4;i++){
				if(i != idx){
					mes.m_aPlayerTo.Add(pDlg->m_pCurTaku->m_members[i].m_player);
				}
			}
			mes.m_playerFrom = *pDlg->m_pCurPlayer;
			mes.m_strText = (LPCTSTR)param1;

			pDlg->queueMessage(mes);
		}

		ret = 1;
		break;
	case MJMI_SETAUTOFUKIDASHI:
	case MJMI_GETWAREME:
		ret = 0;
		break;
	case MJMI_GETVERSION:
		ret = 12;
		break;
	default:
		ret = 0;
		break;
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////////////
// COpenMahjongClientDbgDlg ダイアログ

COpenMahjongClientDbgDlg::COpenMahjongClientDbgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenMahjongClientDbgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenMahjongClientDbgDlg)
	m_strCUIMessage = _T("");
	m_iComID = 0;
	m_strTakunum = _T("");
	m_strSyncTick = _T("");
	m_strSendText = _T("");
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pCurPlayer = NULL;
}

void COpenMahjongClientDbgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenMahjongClientDbgDlg)
	DDX_Control(pDX, IDC_SNDMES, m_btnSndMes);
	DDX_Control(pDX, IDC_SENDTEXT, m_ctlSendText);
	DDX_Control(pDX, IDC_BTNCONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_MESSAGE, m_messageEdit);
	DDX_Control(pDX, IDC_SNDTO3, m_btnSndTo3);
	DDX_Control(pDX, IDC_SNDTO2, m_btnSndTo2);
	DDX_Control(pDX, IDC_SNDTO1, m_btnSndTo1);
	DDX_Control(pDX, IDC_BTNTSUMO, m_btnTsumo);
	DDX_Control(pDX, IDC_BTNTOUHAI, m_btnTouhai);
	DDX_Control(pDX, IDC_BTNRON, m_btnRon);
	DDX_Control(pDX, IDC_BTNRIICHI, m_btnRiichi);
	DDX_Control(pDX, IDC_BTNPON, m_btnPon);
	DDX_Control(pDX, IDC_BTNPASS, m_btnPass);
	DDX_Control(pDX, IDC_BTNKAN, m_btnKan);
	DDX_Control(pDX, IDC_BTNDECIDE, m_btnDecide);
	DDX_Control(pDX, IDC_BTNTII, m_btnTii);
	DDX_Control(pDX, IDC_BTNMAHJONG, m_btnMahjong);
	DDX_Control(pDX, IDC_BTNSYNC, m_btnSync);
	DDX_Text(pDX, IDC_CUI, m_strCUIMessage);
	DDX_Text(pDX, IDC_COMID, m_iComID);
	DDX_Text(pDX, IDC_TAKUNUM, m_strTakunum);
	DDX_Text(pDX, IDC_SYNCTICK, m_strSyncTick);
	DDX_Text(pDX, IDC_SENDTEXT, m_strSendText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpenMahjongClientDbgDlg, CDialog)
	//{{AFX_MSG_MAP(COpenMahjongClientDbgDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNCONNECT, OnBtnconnect)
	ON_BN_CLICKED(IDC_CMDSND, OnCmdsnd)
	ON_BN_CLICKED(IDC_BTNSYNC, OnBtnsync)
	ON_BN_CLICKED(IDC_BTNSYNCAPL, OnBtnsyncapl)
	ON_BN_CLICKED(IDC_BTNDECIDE, OnBtndecide)
	ON_BN_CLICKED(IDC_BTNKAN, OnBtnkan)
	ON_BN_CLICKED(IDC_BTNPASS, OnBtnpass)
	ON_BN_CLICKED(IDC_BTNPON, OnBtnpon)
	ON_BN_CLICKED(IDC_BTNRIICHI, OnBtnriichi)
	ON_BN_CLICKED(IDC_BTNRON, OnBtnron)
	ON_BN_CLICKED(IDC_BTNTII, OnBtntii)
	ON_BN_CLICKED(IDC_BTNTOUHAI, OnBtntouhai)
	ON_BN_CLICKED(IDC_BTNTSUMO, OnBtntsumo)
	ON_BN_CLICKED(IDC_SNDMES, OnSndmes)
	ON_BN_CLICKED(IDC_SNDTOALL, OnSndtoall)
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_ABORT, OnAbort)
	ON_MESSAGE(WM_REFRESH,OnRefresh)
	ON_MESSAGE(WM_SNDCOMMAND,OnSndCommand)
	ON_BN_CLICKED(IDC_BTNRULE, OnBtnrule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenMahjongClientDbgDlg メッセージ ハンドラ

BOOL COpenMahjongClientDbgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString dirPath = _T(".\\comp\\*.dll");
	CFileFind find;

	AfxOleInit();
	CoInitialize(NULL);

	m_pWorkThread = NULL;
	m_pCurPlayer = NULL;
	m_pCurTaku = NULL;
	m_iLogicNum = 0;
	m_iCompNum = 0;
	m_iPlayerNum = 0;
	m_iSession = -1;
	m_bBusy = FALSE;
	m_iSyncTick = 1000;
	m_iSelType = -1;
	m_iPlayerIndex = -1;
	m_iCurState = 0;

	m_player.create(10,1024);
	m_player.openDevice(WAVE_MAPPER);
	m_player.setMethod(&m_matrix);

	m_matrix.setMaxChannels(1);

	m_fileNormal.setPlayer(&m_player);
	m_matrix.appendChannel(&m_fileNormal,1);
	m_matrix.appendChannel(&m_fileCommand,1);
	m_player.play();


	m_btnSndTo1.SetCheck(1);
	m_btnSndTo2.SetCheck(1);
	m_btnSndTo3.SetCheck(1);

	m_strSyncTick = _T("1000");
	m_strTakunum.Format(_T("卓番号 : %d"),m_iSession);
	m_connDlg.m_pTopObj = this;
	
	// デフォルトの接続URL
	m_strHostURL = _T("");

	if(find.FindFile(dirPath)){
		BOOL flag;

		do{
			flag = find.FindNextFile();
			hComp[m_iLogicNum] = LoadLibrary(find.GetFilePath());
			pFunc[m_iLogicNum] = (MJPIFunc)GetProcAddress(hComp[m_iLogicNum],_T("MJPInterfaceFunc"));
			compName[m_iLogicNum] = (LPCTSTR)pFunc[m_iLogicNum](NULL,MJPI_YOURNAME,0,0);
			m_iLogicNum++;
		}while(flag);
	}

	m_debugDlg.Create(IDD_DEBUG,GetDesktopWindow());
	m_debugDlg.ShowWindow(SW_SHOW);


	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	UpdateData(FALSE);
	
	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void COpenMahjongClientDbgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR COpenMahjongClientDbgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void COpenMahjongClientDbgDlg::OnBtnconnect() 
{
	int nResponse = m_connDlg.DoModal(),i;
	CString text;
	CCommand com;
	IXMLDOMDocumentPtr pDoc;
	IXMLDOMNodePtr pNode;
	BSTR pStr;
	UINT res;

	UpdateData(TRUE);

	m_pCurPlayer = NULL;
	m_pCurTaku = NULL;

	if (nResponse == IDOK){
		/* プレーヤ設定をリセット */
		resetPlayer();

		m_queMutex.Lock();

		m_aMessageQueue.RemoveAll();

		m_queMutex.Unlock();

		m_strHostURL = m_connDlg.m_strDst;
		if(m_connDlg.m_iNewSess == 2){
			/* デバッグ接続 */
			com.m_iId = ID_DEBUG;
			m_iSession = m_connDlg.m_iSession;
			sendCommand(com,text);
			pDoc.CreateInstance(CLSID_DOMDocument);
			pDoc->loadXML((LPCTSTR)text);
			pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_TAKU));

			if(pNode != NULL){
				m_aTakuAll[0].parseXML(pNode);
			}

			m_pCurTaku = &m_aTakuAll[0];
			for(i=0;i<4;i++){
				m_players[i].m_iId = m_aTakuAll[0].m_members[i].m_player.m_iId;
				if(searchComp(m_aTakuAll[0].m_members[i].m_player.m_strName)){
					res = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ISEXCHANGEABLE,0,0);
					if(res == 0){
						m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONEXCHANGE,MJST_INKYOKU,(m_aTakuAll[0].m_iKyokuCount << 16) | (m_aTakuAll[0].m_members[i].m_gamestate.m_iZikaze - 1));
					}else{
						m_players[i].m_bIsComp = FALSE;
					}
				}
			}
		}else{
			/* プレーヤーの設定 */
			m_players[0].m_bIsComp = FALSE;
			m_players[0].m_strName = m_connDlg.m_playerName;
			m_iPlayerNum = 1;
			m_iCompNum = 0;

			if(m_connDlg.m_strComp1 != _T("なし")){
				searchComp(m_connDlg.m_strComp1);
			}
			
			if(m_connDlg.m_strComp2 != _T("なし")){
				searchComp(m_connDlg.m_strComp2);
			}

			if(m_connDlg.m_strComp3 != _T("なし")){
				searchComp(m_connDlg.m_strComp3);
			}

			if(m_connDlg.m_iNewSess != 0){
				m_iSession = m_connDlg.m_iSession;
			}
			for(i=0;i<m_iPlayerNum+m_iCompNum;i++){
				if(i== 0 && m_connDlg.m_iNewSess == 0){
					/* 新しいセッションの作成 */
					com.m_iId = ID_CREATE;
					m_iSession = -1;
					m_connDlg.m_ruleDlg.getRule(com.m_rule);
					m_rule = com.m_rule;
					m_btnMahjong.m_rule = com.m_rule;
				}else{
					/* 既存のセッションに接続 */
					com.m_iId = ID_CONNECT;
				}
				com.m_player = m_players[i];
				sendCommand(com,text);
				pDoc.CreateInstance(CLSID_DOMDocument);
				pDoc->loadXML((LPCTSTR)text);
				if(i== 0 && m_connDlg.m_iNewSess == 0){
					pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER))->Getattributes()->getNamedItem(_T("session"));
					if(pNode != NULL){
						text = pNode->GetnodeValue().bstrVal;
						m_iSession = _tcstol((const TCHAR*)text,NULL,0);
					}
				}
				pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_COMMAND "/" TAG_PLAYER "/" TAG_ID));

				if(pNode != NULL){
					pNode->get_text(&pStr);
					text = pStr;
					m_players[i].m_iId = _tcstol((const TCHAR*)text,NULL,0);
				}

				if(m_connDlg.m_iNewSess != 0){
					pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_COMMAND "/" TAG_RULE));
					m_rule.parseXML(pNode);
					m_btnMahjong.m_rule = m_rule;
				}

				if(m_players[i].m_bIsComp){
					m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_STARTGAME,0,0);
				}
			}
		}

		m_strTakunum.Format(_T("卓番号 : %d"),m_iSession);
		m_bFirst = TRUE;
	}

	UpdateData(FALSE);
}

void COpenMahjongClientDbgDlg::OnCmdsnd() 
{
	IXMLDOMDocumentPtr pDoc;
	IXMLDOMNodePtr pNode;
	CString recvMessage;
	CCommand command;

	UpdateData(TRUE);
	m_strCUIMessage = _T("");
	m_debugDlg.m_strCUI = _T("");

	command.m_iId = m_iComID;
	if(m_pCurPlayer != NULL){
		command.m_player = *m_pCurPlayer;
	}

	while(sendCommand(command,recvMessage) < 0);

	m_strCUIMessage += _T("\r\n------ responce --------\r\n");
	m_strCUIMessage += recvMessage;
	m_debugDlg.m_strCUI += recvMessage;
			
	pDoc.CreateInstance(CLSID_DOMDocument);
	pDoc->loadXML((LPCTSTR)recvMessage);

	pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_TAKU));

	if(pNode != NULL){
		m_pCurTaku->parseXML(pNode);
		m_btnMahjong.refresh(m_pCurTaku->getMemberIndex(m_pCurPlayer),*m_pCurTaku);
	}

	m_strCUIMessage += _T("\r\n------ responce end --------\r\n");

	m_bFirst = TRUE;

	UpdateData(FALSE);

}

void COpenMahjongClientDbgDlg::OnBtnsync() 
{
	if(m_iSession >= 0){
		UpdateData(TRUE);
		m_btnConnect.EnableWindow(FALSE);
		m_btnSync.EnableWindow(FALSE);
		UpdateData(FALSE);
		if(m_bFirst){
			m_fileNormal.setLoop(TRUE);
			m_fileNormal.closeWaveFile();
			m_fileCommand.closeWaveFile();
			m_fileNormal.openWaveFile(_T("./wav/bgmnormal.wav"));
		}else{
			m_bFirst = TRUE;
		}
		m_pWorkThread = AfxBeginThread(syncFunc,this);
		while(m_pWorkThread->PostThreadMessage(WM_APP,0,0) == 0);
	}
}

void COpenMahjongClientDbgDlg::gameSync()
{
	IXMLDOMDocumentPtr pDoc;
	IXMLDOMNodePtr pNode,pNodeTaku;
	IXMLDOMNodeListPtr pNodeList;
	IXMLDOMNodeListPtr pNodeListTaku;
	IXMLDOMNodeListPtr pNodeListCode;
	CString recvMessage;
	CCommand command;
	CArray<CCommand,CCommand&> aCommand;
	CString text;
	BSTR pStr;
	UINT ret;
	int i,j,code,type,ind,ind2,kaze1,kaze2,kaze3;
	int aCode[4],iPlayerDlgIndex;
	MSG msg;
	LONG pointdiff[4];
	BOOL bPlayerCommand = FALSE;

	ZeroMemory(pointdiff,sizeof(pointdiff));

	m_bBusy = TRUE;

	while(!bPlayerCommand){
		aCommand.RemoveAll();

		for(i=0;i<m_iPlayerNum+m_iCompNum;i++){
			command.m_iId = m_bFirst ? ID_STATUS : ID_UPDATE;
			command.m_player = m_players[i];
			aCommand.Add(command);
		}

		sendCommand(aCommand,recvMessage);

		pDoc.CreateInstance(CLSID_DOMDocument);
		pDoc->loadXML((LPCTSTR)recvMessage);
		
		pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_ERROR));
		/* エラーを確認したら出力する */
		if(pNode != NULL){
			pNode->get_text(&pStr);
			text.Format(_T("system:ServerError %s"),pStr);
			appendMessageText(text);
		}
		
		pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_CODE));
		if(pNode != NULL){
			pNode->get_text(&pStr);
			text = pStr;
			code = _tcstol((const TCHAR*)text,NULL,0);
			
			if(code == RESPONCE_SESSION){
				AfxMessageBox(_T("セッションが切断しました。再度セッションの新規作成をお願いします。"));
				m_fileNormal.closeWaveFile();
				m_iAgari = -1;
				return;
			}else if(code == RESPONCE_PLAYERID){
				AfxMessageBox(_T("プレーヤの確認で異常が発生しました。再接続をお願いします。"));
				m_fileNormal.closeWaveFile();
				m_iAgari = -1;
				return;
			}
		}

		pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_STATE "/" TAG_CODE));
		
		if(pNode != NULL){
			pNode->get_text(&pStr);
			text = pStr;
			code = _tcstol((const TCHAR*)text,NULL,0);
			
			
			
			if(code == CODE_BUSY){
				Sleep(m_iSyncTick);
				continue;
			}
		}

		pNodeListTaku = pDoc->selectNodes(_T(TAG_OPENMAHJONGSERVER "/" TAG_TAKU));

		if(pNodeListTaku == NULL || pNodeListTaku->Getlength() < m_iPlayerNum+m_iCompNum){
			/* もし見つからなかったらタイムアウトもしくはサーバーエラーの可能性があるので、
			ステータスの取得をやり直す */
			m_bFirst = TRUE;
			continue;
		}

		pNodeListCode = pDoc->selectNodes(_T(TAG_OPENMAHJONGSERVER "/" TAG_STATE "/" TAG_CODE));

		for(i=0;i<pNodeListCode->Getlength();i++){
			pNode = pNodeListCode->Getitem(i);
			if(pNode != NULL){
				pNode->get_text(&pStr);
				text = pStr;
				aCode[i] = _tcstol((const TCHAR*)text,NULL,0);
			}
		}

		for(i=0;i<pNodeListTaku->Getlength();i++){
			pNodeTaku = pNodeListTaku->Getitem(i);
			
			if(pNodeTaku != NULL){
				if(m_bFirst){
					m_aTakuAll[i].parseXML(pNodeTaku);
				}else{
					CTaku value;
					value.parseXML(pNodeTaku);
					m_aTakuAll[i].update(value);
				}
			}
		}

		if(aCode[0] != CODE_WAITSYNC && aCode[0] != CODE_BUSY){
			m_bFirst = FALSE;
		}

		i = 0;
		while(i<m_iPlayerNum+m_iCompNum){


			m_pCurPlayer = &m_players[i];
			m_pCurTaku = &m_aTakuAll[i];

			pNode = pNodeListCode->Getitem(i);
			if(pNode != NULL){
				code = aCode[i];
				m_debugDlg.PostMessage(WM_SETSTATUSCODE,code,0);

				if(m_players[i].m_bIsComp){
					ind = m_pCurTaku->getMemberIndex(&m_players[i]);
					kaze1 = m_pCurTaku->m_members[ind].m_gamestate.m_iZikaze - 1;
					ind2 = m_pCurTaku->getMemberIndex(&m_pCurTaku->m_event.m_command.m_player);
					kaze2 = m_pCurTaku->m_members[ind2].m_gamestate.m_iZikaze - 1;
					type = m_pCurTaku->m_event.m_command.m_iType;

					if(code != CODE_WAITSYNC){

						switch(type){
						case TYPE_KOUHAI:
							m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ENDKYOKU,MJEK_RYUKYOKU,(UINT)pointdiff);
							break;
						case TYPE_START:
							m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_STARTKYOKU,m_pCurTaku->m_iKyokuCount,m_pCurTaku->m_members[ind].m_gamestate.m_iZikaze - 1);
							break;
						case TYPE_DAHAI:
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,ieTable[kaze1][kaze2],MJPIR_SUTEHAI | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							break;
						case TYPE_TII:
							kaze3 = m_pCurTaku->m_members[m_pCurTaku->m_event.m_command.m_mentsu.m_iAite].m_gamestate.m_iZikaze - 1;
							switch(m_pCurTaku->m_event.m_command.m_mentsu.getNakiPos()){
							case 0:
								ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,(ieTable[kaze1][kaze3] << 16) | ieTable[kaze1][kaze2],MJPIR_CHII1 | (UINT)m_pCurTaku->m_event.m_command.m_pai);
								break;
							case 1:
								ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,(ieTable[kaze1][kaze3] << 16) | ieTable[kaze1][kaze2],MJPIR_CHII2 | (UINT)m_pCurTaku->m_event.m_command.m_pai);
								break;
							case 2:
								ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,(ieTable[kaze1][kaze3] << 16) | ieTable[kaze1][kaze2],MJPIR_CHII3 | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							default:
								break;
							}

							break;
						case TYPE_PON:
							kaze3 = m_pCurTaku->m_members[m_pCurTaku->m_event.m_command.m_mentsu.m_iAite].m_gamestate.m_iZikaze - 1;
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,(ieTable[kaze1][kaze3] << 16) | ieTable[kaze1][kaze2],MJPIR_PON | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							break;
						case TYPE_RON:
							kaze3 = m_pCurTaku->m_members[m_pCurTaku->m_event.m_command.m_mentsu.m_iAite].m_gamestate.m_iZikaze - 1;
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,(ieTable[kaze1][kaze3] << 16) | ieTable[kaze1][kaze2],MJPIR_RON | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							pointdiff[ieTable[kaze1][kaze3]] = -(m_pCurTaku->m_event.m_result.m_iScore + m_pCurTaku->m_iTsumibou * 300);
							pointdiff[ieTable[kaze1][kaze2]] = (m_pCurTaku->m_event.m_result.m_iScore + m_pCurTaku->m_iTsumibou * 300 + m_pCurTaku->m_iRiichibou * 1000);
							m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ENDKYOKU,MJEK_AGARI,(UINT)pointdiff);
							break;
						case TYPE_TSUMO:
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,ieTable[kaze1][kaze2],MJPIR_TSUMO | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							if(kaze2 == 0){
								for(j=0;j<4;j++){
									if(j==(int)ieTable[kaze1][kaze2]){
										pointdiff[j] = (m_pCurTaku->m_event.m_result.m_iKoScore + m_pCurTaku->m_iTsumibou * 100) * 3 + m_pCurTaku->m_iRiichibou * 1000;
									}else{
										pointdiff[j] = -(m_pCurTaku->m_event.m_result.m_iKoScore + m_pCurTaku->m_iTsumibou * 100);
									}
								}
							}else{
								for(j=0;j<4;j++){
									if(j==(int)ieTable[kaze1][kaze2]){
										pointdiff[j] = (m_pCurTaku->m_event.m_result.m_iKoScore + m_pCurTaku->m_iTsumibou * 100) * 2 + (m_pCurTaku->m_event.m_result.m_iOyaScore + m_pCurTaku->m_iTsumibou * 100) + m_pCurTaku->m_iRiichibou * 1000;
									}else if((kaze1 + j) % 4 == 0){
										pointdiff[j] = -(m_pCurTaku->m_event.m_result.m_iOyaScore + m_pCurTaku->m_iTsumibou * 100);
									}else{
										pointdiff[j] = -(m_pCurTaku->m_event.m_result.m_iKoScore + m_pCurTaku->m_iTsumibou * 100);
									}
								}
							}
							m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ENDKYOKU,MJEK_AGARI,(UINT)pointdiff);
							break;
						case TYPE_RIICHI:
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,ieTable[kaze1][kaze2],MJPIR_REACH | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							break;
						case TYPE_ANKAN:
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,ieTable[kaze1][kaze2],MJPIR_ANKAN | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							break;
						case TYPE_KUWAEKAN:
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,ieTable[kaze1][kaze2],MJPIR_MINKAN | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							break;
						case TYPE_DAIMINKAN:
							kaze3 = m_pCurTaku->m_members[m_pCurTaku->m_event.m_command.m_mentsu.m_iAite].m_gamestate.m_iZikaze - 1;
							ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_ONACTION,(ieTable[kaze1][kaze3] << 16) | ieTable[kaze1][kaze2],MJPIR_MINKAN | (UINT)m_pCurTaku->m_event.m_command.m_pai);
							break;
						default:
							break;
						}
					}
					if(code == CODE_WAITCOMMAND){
						/* コンピュータの行動を取得 */
						int ind = m_pCurTaku->getMemberIndex(&m_players[i]);
						command.m_player = m_players[i];
						if(m_pCurTaku->m_members[ind].m_aCommandList[0].m_iId == ID_START){
							sendCommand(m_pCurTaku->m_members[ind].m_aCommandList[0],recvMessage);
						}else{
							if(ind == m_pCurTaku->m_iTurn){
								UINT paiID = m_pCurTaku->m_members[ind].m_gamestate.m_bTsumo ? m_pCurTaku->m_members[ind].m_aTehai[m_pCurTaku->m_members[ind].m_aTehai.GetUpperBound()] : 63;
								
								ret = m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_SUTEHAI,(UINT)paiID,0);
								switch(ret & 0xFFFFFF00){
								case MJPIR_SUTEHAI:
									if((ret & 0xFF) == 13){
										command.m_iId = ID_DAHAI + m_pCurTaku->m_members[m_pCurTaku->m_iTurn].m_aTehai.GetUpperBound();
									}else{
										command.m_iId = ID_DAHAI + (ret & 0xFF);
									}
									break;
								case MJPIR_REACH:
									command.m_iId = ID_RIICHI + (ret & 0xFF);
									if(!m_pCurTaku->m_members[ind].isExecutableCommand(command)){
										command.m_iId = ID_DAHAI + (ret & 0xFF);
									}
									break;
								case MJPIR_TSUMO:
									command.m_iId = ID_TSUMO;
									if(!m_pCurTaku->m_members[ind].isExecutableCommand(command)){
										command.m_iId = ID_DAHAI + m_pCurTaku->m_members[m_pCurTaku->m_iTurn].m_aTehai.GetUpperBound();
									}
									break;
								case MJPIR_NAGASHI:
									command.m_iId = ID_TOUHAI;
									if(!m_pCurTaku->m_members[ind].isExecutableCommand(command)){
										command.m_iId = ID_DAHAI + m_pCurTaku->m_members[m_pCurTaku->m_iTurn].m_aTehai.GetUpperBound();
									}
									break;
								case MJPIR_KAN:
									command.m_iId = ID_KAN + (ret & 0xFF);
									if(!m_pCurTaku->m_members[ind].isExecutableCommand(command)){
										command.m_iId = ID_DAHAI + m_pCurTaku->m_members[m_pCurTaku->m_iTurn].m_aTehai.GetUpperBound();
									}
									break;
								default:
									command.m_iId = ID_DAHAI + m_pCurTaku->m_members[m_pCurTaku->m_iTurn].m_aTehai.GetUpperBound();
									break;
								}

								while(sendCommand(command,recvMessage) < 0);
							}else{
								/* とりあえず全部パス */
								switch(ret & 0xFFFFFF00){
								case MJPIR_CHII1:
									for(j=0;j<m_pCurTaku->m_members[ind].m_aCommandList.GetSize();j++){
										if(m_pCurTaku->m_members[ind].m_aCommandList[j].m_mentsu.getNakiPos() == 0){
											while(sendCommand(m_pCurTaku->m_members[ind].m_aCommandList[j],recvMessage) < 0);
											break;
										}
									}
									break;
								case MJPIR_CHII2:
									for(j=0;j<m_pCurTaku->m_members[ind].m_aCommandList.GetSize();j++){
										if(m_pCurTaku->m_members[ind].m_aCommandList[j].m_mentsu.getNakiPos() == 2){
											while(sendCommand(m_pCurTaku->m_members[ind].m_aCommandList[j],recvMessage) < 0);
											break;
										}
									}
									break;
								case MJPIR_CHII3:
									for(j=0;j<m_pCurTaku->m_members[ind].m_aCommandList.GetSize();j++){
										if(m_pCurTaku->m_members[ind].m_aCommandList[j].m_mentsu.getNakiPos() == 1){
											while(sendCommand(m_pCurTaku->m_members[ind].m_aCommandList[j],recvMessage) < 0);
											break;
										}
									}
									break;
								case MJPIR_PON:
									command.m_iId = ID_PON;
									while(sendCommand(command,recvMessage) < 0);
									break;
								case MJPIR_KAN:
									command.m_iId = ID_DAIMINKAN;
									while(sendCommand(command,recvMessage) < 0);
									break;
								case MJPIR_RON:
									command.m_iId = ID_RON;
									while(sendCommand(command,recvMessage) < 0);
									break;
								default:
									command.m_iId = ID_PASS;
									while(sendCommand(command,recvMessage) < 0);
									break;
								}
							}
						}
					}
				}else{
					// 自分宛のメッセージを引っ張り出してくる
					pNodeTaku = pNodeListTaku->Getitem(i);
					pNodeList = pNodeTaku->selectNodes(_T(TAG_MESSAGE));
					for(j=0;j<pNodeList->Getlength();j++){
						CMessage mes;
						int index;
						CString mestext;
						pNode = pNodeList->Getitem(j);
						mes.parseXML(pNode);

						index = m_pCurTaku->getMemberIndex(&mes.m_playerFrom);

						mestext.Format(_T("%s[%s]：%s\r\n"),mes.m_playerFrom.m_strName,
							ieStrTable[m_pCurTaku->m_members[index].m_gamestate.m_iZikaze - 1],mes.m_strText);

						appendMessageText(mestext);
					}

					m_iPlayerIndex = m_pCurTaku->getMemberIndex(&m_players[i]);
					for(j=0;j<4;j++){
						kaze3 = ieTable[m_pCurTaku->m_members[m_iPlayerIndex].m_gamestate.m_iZikaze - 1][m_pCurTaku->m_members[j].m_gamestate.m_iZikaze - 1];

						text.Format(_T("%s(%s)"),m_pCurTaku->m_members[j].m_player.m_strName,posStrTable[kaze3]);
						if(kaze3 == 1){
							m_btnSndTo1.SetWindowText((LPCTSTR)text);
						}else if(kaze3 == 2){
							m_btnSndTo2.SetWindowText((LPCTSTR)text);
						}else if(kaze3 == 3){
							m_btnSndTo3.SetWindowText((LPCTSTR)text);
						}
					}

					if(code != CODE_WAITSYNC && code != CODE_BUSY && m_pCurTaku->m_event.m_command.m_iType == TYPE_RIICHI
						&& m_pCurTaku->m_event.m_command.m_player.m_iId != m_players[i].m_iId
						&& !m_pCurTaku->m_members[m_iPlayerIndex].m_gamestate.m_bRiichi){
						// ライバルがリーチ
						m_fileNormal.closeWaveFile();
						m_fileNormal.openWaveFile(_T("./wav/bgmenemy.wav"));
					}

					if(code != CODE_WAITSYNC){
						switch(m_pCurTaku->m_event.m_command.m_iType){
						case TYPE_DAHAI:
							m_fileCommand.closeWaveFile();
							m_fileCommand.openWaveFile(_T("./wav/dahai.wav"));
							break;
						case TYPE_TII:
						case TYPE_PON:
						case TYPE_DAIMINKAN:
						case TYPE_ANKAN:
						case TYPE_KUWAEKAN:
							m_fileCommand.closeWaveFile();
							m_fileCommand.openWaveFile(_T("./wav/naki.wav"));
							break;
						case TYPE_RIICHI:
							m_fileCommand.closeWaveFile();
							m_fileCommand.openWaveFile(_T("./wav/riichi.wav"));
							break;
						default:
							break;
						}
					}
					if(code == CODE_PROGRESSED){
						m_btnMahjong.refresh(m_pCurTaku->getMemberIndex(&m_players[i]),*m_pCurTaku);
					}else if(code == CODE_WAITCOMMAND){
						int index = m_pCurTaku->getMemberIndex(&m_players[i]);
						iPlayerDlgIndex = i;
						m_pCurTaku->printState(index,text);
						m_strCUIMessage += text;
						m_bBusy = FALSE;
						m_iAgari = 0;
						/* 実行可能なコマンドをサーチする */
						for(j=0;j<m_pCurTaku->m_members[index].m_aCommandList.GetSize();j++){
							switch(m_pCurTaku->m_members[index].m_aCommandList[j].m_iType){
							case TYPE_START:
								m_agariDlg.m_staAgariyaku.m_iType = TYPE_START;
								m_agariDlg.m_staAgariyaku.m_result = m_pCurTaku->m_event.m_result;
								m_iAgari = 1;
								break;
							case TYPE_KOUHAI:
								m_agariDlg.m_staAgariyaku.m_iType = TYPE_KOUHAI;
								m_iAgari = 1;
								break;
							case TYPE_TII:
								m_btnTii.EnableWindow(TRUE);
								m_btnDecide.EnableWindow(TRUE);
								break;
							case TYPE_PON:
								m_btnPon.EnableWindow(TRUE);
								m_btnDecide.EnableWindow(TRUE);
								break;
							case TYPE_DAIMINKAN:
							case TYPE_ANKAN:
							case TYPE_KUWAEKAN:
								m_btnKan.EnableWindow(TRUE);
								m_btnDecide.EnableWindow(TRUE);
								break;
							case TYPE_RON:
								m_btnRon.EnableWindow(TRUE);
								break;
							case TYPE_TSUMO:
								m_btnTsumo.EnableWindow(TRUE);
								break;
							case TYPE_RIICHI:
								m_btnRiichi.EnableWindow(TRUE);
								m_btnDecide.EnableWindow(TRUE);
								break;
							case TYPE_PASS:
								m_btnPass.EnableWindow(TRUE);
								break;
							case TYPE_TOUHAI:
								m_btnTouhai.EnableWindow(TRUE);
								break;
							default:
								break;
							}
						}

						MJITehai tehai;
						TENPAI_LIST tenpai_list;
						CString mestext,tmptext,tmptext2;

						m_pCurTaku->getMJITehai(index,&tehai);

						if(m_pCurTaku->m_members[index].m_gamestate.m_bTsumo == TRUE){
							search_tenpai((int *)tehai.tehai,tehai.tehai_max,&tenpai_list,1,99);
							tmptext = _T("");
							if(tenpai_list.shanten == 0){
								for(j=0;j<34;j++){
									if(tenpai_list.machi[j] == AI_FLAG_MACHI){
										tmptext2.Format(_T(" %d"),j);
										tmptext += tmptext2;
									}
								}
							}
							mestext.Format(_T("Debug：向聴数 %d %s\r\n"),tenpai_list.shanten,tmptext);
							
							appendMessageText(mestext);
						}

						bPlayerCommand = TRUE;
					}

				}

			}else{
				break;
			}

			i++;
		}

		// WM_QUIT メッセージを受け取ったら終了
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if(msg.message == WM_QUIT){
				return;
			}
		}
		Sleep(m_iSyncTick);

	}

	m_pCurPlayer = &m_players[iPlayerDlgIndex];
	m_pCurTaku = &m_aTakuAll[iPlayerDlgIndex];

}

BOOL COpenMahjongClientDbgDlg::searchComp(CString& name)
{
	int i,size;
	UINT nokori[] = { 4 , 5 , 6,  7};

	m_players[m_iCompNum + m_iPlayerNum].m_strName = name;

	for(i=0;i<m_iLogicNum;i++){
		if(compName[i] == name){
			m_players[m_iCompNum + m_iPlayerNum].m_pFunc = pFunc[i];

			/* インスタンスの生成 */
			size = pFunc[i](NULL,MJPI_CREATEINSTANCE,0,0);
			if(size > 0){
				pInst[m_iCompNum] = malloc(size);
				structMode[m_iCompNum] = 0;
				memset(pInst[m_iCompNum],0,size);
			}else{
				pInst[m_iCompNum] = NULL;
			}

			m_players[m_iCompNum + m_iPlayerNum].m_bIsComp = TRUE;
			m_players[m_iCompNum + m_iPlayerNum].m_pInst = pInst[m_iCompNum];
			m_iCompNum++;
			pFunc[i](pInst[m_iCompNum-1],MJPI_INITIALIZE,0,(UINT)MJSendMessage);
			return TRUE;
		}
	}

	m_players[m_iCompNum + m_iPlayerNum].m_bIsComp = FALSE;
	m_iPlayerNum++;

	return FALSE;
}

int COpenMahjongClientDbgDlg::sendCommand(CCommand& command,CString& recvMessage)
{
	IXMLDOMDocumentPtr pDoc;
	IXMLDOMElementPtr pElemRoot,pElemCom,pElemID,pElemPlayer;
	IXMLDOMNodePtr pNode;
	HRESULT hr = pDoc.CreateInstance(CLSID_DOMDocument);
	CString str;
	BSTR bstrXML;
	CString sendMessage;
	int i;
	int code = -1;

	recvMessage = _T("");

	pElemRoot = pDoc->createElement(_T(TAG_OPENMAHJONGCLIENT));
	pDoc->appendChild(pElemRoot);

	str.Format(_T("%d"),m_iSession);

	pElemRoot->setAttribute(_T("session"),(LPCTSTR)str);
	pElemRoot->setAttribute(_T("version"),_T("0.1"));

	if(command.m_iId != 0){
		command.toXML(pDoc,pElemRoot);
	}

	/* キューに入ったメッセージを送信 */
	m_queMutex.Lock();

	for(i=0;i<m_aMessageQueue.GetSize();i++){
		m_aMessageQueue[i].toXML(pDoc,pElemRoot);
	}

	m_aMessageQueue.RemoveAll();

	m_queMutex.Unlock();


	pDoc->get_xml(&bstrXML);

	sendMessage = _T("<?xml version=\"1.0\" encoding=\"Shift_JIS\" ?>");
	sendMessage += bstrXML;

	sendString(sendMessage,recvMessage);

	pDoc->loadXML((LPCTSTR)recvMessage);
	pNode = pDoc->selectSingleNode(_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_CODE));
	if(pNode != NULL){
		pNode->get_text(&bstrXML);
		str = bstrXML;
		code = _tcstol((const TCHAR*)str,NULL,0);
	}

	return code;

}

LRESULT COpenMahjongClientDbgDlg::OnRefresh(WPARAM wParam,LPARAM lParam)
{
	UpdateData(TRUE);
	m_btnConnect.EnableWindow(TRUE);
	m_btnSync.EnableWindow(TRUE);
	UpdateData(FALSE);

	return 0;
}

LRESULT COpenMahjongClientDbgDlg::OnSndCommand(WPARAM wParam,LPARAM lParam)
{
	IXMLDOMDocumentPtr pDoc;
	IXMLDOMNodePtr pNode;
	CCommand com;
	CString str;
	CString text;

	HRESULT hr = pDoc.CreateInstance(CLSID_DOMDocument);

	if(!m_bBusy && m_iSession >= -1){
		com.m_iId = wParam;
		if(m_pCurPlayer != NULL){
			com.m_player = *m_pCurPlayer;
		}

		UpdateData(TRUE);
		resetBtnState();
		m_btnMahjong.setSelMode(0);
		m_btnConnect.EnableWindow(FALSE);
		m_btnSync.EnableWindow(FALSE);
		m_btnDecide.EnableWindow(FALSE);
		m_btnKan.EnableWindow(FALSE);
		m_btnPass.EnableWindow(FALSE);
		m_btnPon.EnableWindow(FALSE);
		m_btnRiichi.EnableWindow(FALSE);
		m_btnRon.EnableWindow(FALSE);
		m_btnTii.EnableWindow(FALSE);
		m_btnTouhai.EnableWindow(FALSE);
		m_btnTsumo.EnableWindow(FALSE);
		UpdateData(FALSE);
		m_iCurState = 1;
		while(sendCommand(com,str) < 0);
		m_iCurState = 0;
		
		m_pWorkThread = AfxBeginThread(syncFunc,this);
		while(m_pWorkThread->PostThreadMessage(WM_APP,0,0) == 0);

	}

	return 0;
}

void COpenMahjongClientDbgDlg::sendString(CString& sendMessage,CString& recvMessage)
{
	CInternetSession inet;
	DWORD protocol = 0;
	CString serverName;
	CString fileName;
	INTERNET_PORT port;
	CHttpConnection *conn;
	CHttpFile *file;
	CString errorMessage;
	clock_t start,finish;

#ifdef DEBUGDUMP
	m_strCUIMessage += sendMessage;
#endif

	inet.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT,2000);
	inet.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,2000);
	inet.SetOption(INTERNET_OPTION_CONTROL_SEND_TIMEOUT,2000);
	inet.SetOption(INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT,2000);

	// URLをパースする
	AfxParseURL(m_strHostURL,protocol,serverName,fileName,port);

	// https対応(出来るかな？)
	if(protocol == AFX_INET_SERVICE_HTTPS){
		conn = inet.GetHttpConnection(serverName,INTERNET_FLAG_SECURE,port);
	}else{
		conn = inet.GetHttpConnection(serverName,port);
	}
	if(conn != NULL){
		if(protocol == AFX_INET_SERVICE_HTTPS){
			file = conn->OpenRequest(CHttpConnection::HTTP_VERB_POST,fileName,NULL,1,NULL,NULL,INTERNET_FLAG_SECURE);
		}else{
			file = conn->OpenRequest(CHttpConnection::HTTP_VERB_POST,fileName);
		}
		if(file != NULL){
			CString strBuf;
			DWORD dwStatus;
			BOOL res;
			CString header = _T("Content-type:application/xml");
			try{
				start = clock();
				file->SendRequest(header,(LPVOID)(LPCTSTR)sendMessage,(DWORD)sendMessage.GetLength());
				finish = clock();

				// 負荷調節のためにウェイトを少し入れる
				Sleep(50);

#ifdef DEBUGDUMP
				m_strCUIMessage += _T("\r\n------ responce --------\r\n");
#endif
			
				file->QueryInfoStatusCode(dwStatus);

				switch(dwStatus){
				case HTTP_STATUS_OK:
					
					do{
						res = file->ReadString(strBuf);
#ifdef DEBUGDUMP
					m_strCUIMessage += strBuf;
#endif
						recvMessage += strBuf;
					}while(res);

					m_debugDlg.PostMessage(WM_SETTIME,(finish - start) * 1000 / CLOCKS_PER_SEC,0);

					break;
				default:
					errorMessage.Format(_T("system: HTTP ERROR CODE %d\r\n"),dwStatus);
					appendMessageText(errorMessage);
					break;
				}
#ifdef DEBUGDUMP
				m_strCUIMessage += _T("\r\n------ responce end --------\r\n");
#endif
			}catch(CInternetException *e){
				TCHAR pEM[255];

				e->GetErrorMessage(pEM,255);

				errorMessage.Format(_T("system:%s"),pEM);

				appendMessageText(errorMessage);

				e->Delete();
			}
			file->Close();
			delete file;
		}

		conn->Close();
		delete conn;
	}

}

void COpenMahjongClientDbgDlg::OnBtnsyncapl() 
{
	UpdateData(TRUE);

	m_iSyncTick = _tcstol((LPCTSTR)m_strSyncTick,NULL,0);

	UpdateData(FALSE);
	
}

void COpenMahjongClientDbgDlg::sendLibrary(CLibrary& library)
{
	IXMLDOMDocumentPtr pDoc,pDoc2;
	IXMLDOMElementPtr pElemRoot,pElemLib,pElemID,pElem2,pElem3,pElem4,pElem5,pElem6,pElemPlayer;
	IXMLDOMElementPtr pElem;
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	HRESULT hr = pDoc.CreateInstance(CLSID_DOMDocument);
	CString str;
	BSTR bstrXML;
	CString sendMessage,recvMessage;
	int i,j;

	recvMessage = _T("");

	pElemRoot = pDoc->createElement(_T(TAG_OPENMAHJONGCLIENT));
	pDoc->appendChild(pElemRoot);

	str.Format(_T("%d"),m_iSession);

	pElemRoot->setAttribute(_T("session"),(LPCTSTR)str);
	pElemRoot->setAttribute(_T("version"),_T("0.1"));
	pElemLib = pDoc->createElement(_T("library"));

	str.Format(_T("%d"),library.m_iId);
	pElem = pDoc->createElement(_T("id"));
	pElem->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemLib->appendChild(pElem);

	/* プレーヤーデータ */
	library.m_player.toXML(pDoc,pElemLib);

	/* 手牌データ */
	pElem = pDoc->createElement(_T(TAG_TEHAI));
	pElemLib->appendChild(pElem);

	for(i=0;i<library.m_aTehai.GetSize();i++){
		pElem2 = pDoc->createElement(_T(TAG_PAI));
		pElem3 = pDoc->createElement(_T(TAG_CATEGORY));
		str.Format(_T("%d"),library.m_aTehai[i].m_iCategory);
		pElem3->appendChild(pDoc->createTextNode((LPCTSTR)str));
		pElem2->appendChild(pElem3);
		pElem3 = pDoc->createElement(_T(TAG_NO));
		str.Format(_T("%d"),library.m_aTehai[i].m_iNo);
		pElem3->appendChild(pDoc->createTextNode((LPCTSTR)str));
		pElem2->appendChild(pElem3);
		pElem3 = pDoc->createElement(_T(TAG_ID));
		str.Format(_T("%d"),library.m_aTehai[i].m_iId);
		pElem3->appendChild(pDoc->createTextNode((LPCTSTR)str));
		pElem2->appendChild(pElem3);
		pElem->appendChild(pElem2);
	}


	/* 鳴き牌等データ */
	pElem = pDoc->createElement(_T(TAG_GAMESTATE));
	pElemLib->appendChild(pElem);
	pElem2 = pDoc->createElement(_T(TAG_ZIKAZE));
	str.Format(_T("%d"),library.m_gamestate.m_iZikaze);
	pElem2->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElem->appendChild(pElem2);

	pElem2 = pDoc->createElement(_T(TAG_COUNT));
	str.Format(_T("%d"),library.m_gamestate.m_iCount);
	pElem2->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElem->appendChild(pElem2);

	pElem2 = pDoc->createElement(_T(TAG_NAKI));
	pElem2->appendChild(pDoc->createTextNode(library.m_gamestate.m_bNaki ? _T("true") : _T("false")));
	pElem->appendChild(pElem2);

	pElem2 = pDoc->createElement(_T(TAG_OYA));
	pElem2->appendChild(pDoc->createTextNode(library.m_gamestate.m_bOya ? _T("true") : _T("false")));
	pElem->appendChild(pElem2);

	pElem2 = pDoc->createElement(_T(TAG_IPPATSU));
	pElem2->appendChild(pDoc->createTextNode(library.m_gamestate.m_bIppatsu ? _T("true") : _T("false")));
	pElem->appendChild(pElem2);

	pElem2 = pDoc->createElement(_T(TAG_RIICHI));
	pElem2->appendChild(pDoc->createTextNode(library.m_gamestate.m_bRiichi ? _T("true") : _T("false")));
	pElem->appendChild(pElem2);

	pElem2 = pDoc->createElement(_T(TAG_TSUMO));
	pElem2->appendChild(pDoc->createTextNode(library.m_gamestate.m_bTsumo ? _T("true") : _T("false")));
	pElem->appendChild(pElem2);

	pElem2 = pDoc->createElement(_T(TAG_NAKILIST));
	pElem->appendChild(pElem2);

	for(i=0;i<library.m_gamestate.m_aNakiList.GetSize();i++){
		pElem3 = pDoc->createElement(_T(TAG_NAKIMENTSU));
		pElem4 = pDoc->createElement(_T(TAG_AITE));
		str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_iAite);
		pElem4->appendChild(pDoc->createTextNode((LPCTSTR)str));
		pElem3->appendChild(pElem4);

		pElem4 = pDoc->createElement(_T(TAG_NAKIHAI));
		pElem3->appendChild(pElem4);
		pElem5 = pDoc->createElement(_T(TAG_PAI));
		pElem4->appendChild(pElem5);
		str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_nakihai.m_iCategory);
		pElem6 = pDoc->createElement(_T(TAG_CATEGORY));
		pElem5->appendChild(pElem6);
		pElem6->appendChild(pDoc->createTextNode((LPCTSTR)str));
		str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_nakihai.m_iNo);
		pElem6 = pDoc->createElement(_T(TAG_NO));
		pElem5->appendChild(pElem6);
		pElem6->appendChild(pDoc->createTextNode((LPCTSTR)str));
		str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_nakihai.m_iId);
		pElem6 = pDoc->createElement(_T(TAG_ID));
		pElem5->appendChild(pElem6);
		pElem6->appendChild(pDoc->createTextNode((LPCTSTR)str));

		pElem4 = pDoc->createElement(_T(TAG_MENTSU));
		pElem3->appendChild(pElem4);
		pElem5 = pDoc->createElement(_T(TAG_CATEGORY));
		pElem4->appendChild(pElem5);
		str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_iCategory);
		pElem5->appendChild(pDoc->createTextNode((LPCTSTR)str));
		for(j=0;j<library.m_gamestate.m_aNakiList[i].m_aPaiList.GetSize();j++){
			pElem5 = pDoc->createElement(_T(TAG_PAI));
			pElem4->appendChild(pElem5);
			str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_aPaiList[j].m_iCategory);
			pElem6 = pDoc->createElement(_T(TAG_CATEGORY));
			pElem5->appendChild(pElem6);
			pElem6->appendChild(pDoc->createTextNode((LPCTSTR)str));
			str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_aPaiList[j].m_iNo);
			pElem6 = pDoc->createElement(_T(TAG_NO));
			pElem5->appendChild(pElem6);
			pElem6->appendChild(pDoc->createTextNode((LPCTSTR)str));
			str.Format(_T("%d"),library.m_gamestate.m_aNakiList[i].m_aPaiList[j].m_iId);
			pElem6 = pDoc->createElement(_T(TAG_ID));
			pElem5->appendChild(pElem6);
			pElem6->appendChild(pDoc->createTextNode((LPCTSTR)str));
		}

	}

	pElemRoot->appendChild(pElemLib);

	pDoc->get_xml(&bstrXML);

	sendMessage = _T("<?xml version=\"1.0\" encoding=\"Shift_JIS\" ?>");
	sendMessage += bstrXML;

	sendString(sendMessage,recvMessage);

	pDoc2.CreateInstance(CLSID_DOMDocument);
	pDoc2->loadXML((LPCTSTR)recvMessage);

	pNodeList = pDoc2->selectNodes(_T(TAG_OPENMAHJONGSERVER "/" TAG_LIBRARY "/" TAG_RESULTLIST "/" TAG_RESULT));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CResult result;
			pNode = pNodeList->Getitem(i);
			result.parseXML(pNode);
			library.m_aResultList.Add(result);
		}
	}

}

void COpenMahjongClientDbgDlg::OnBtndecide() 
{
	CPai aPai[2];
	int i,j,index;
	BOOL matched[2] = { FALSE, FALSE};
	int id = -1;


	index = m_pCurTaku->getMemberIndex(m_pCurPlayer);

	for(i=0;i<m_btnMahjong.getSelMode();i++){
		if(m_btnMahjong.m_selIndex[i] >= 0){
			aPai[i] = m_pCurTaku->m_members[index].m_aTehai[m_btnMahjong.m_selIndex[i]];
		}
	}

	/* 選択した牌の一致 */
	for(i=0;i<m_pCurTaku->m_members[index].m_aCommandList.GetSize();i++){

		if(m_iSelType == TYPE_RIICHI){
			if(m_pCurTaku->m_members[index].m_aCommandList[i].m_pai.match(aPai[0]) && m_pCurTaku->m_members[index].m_aCommandList[i].m_iType == TYPE_RIICHI){
				id = m_pCurTaku->m_members[index].m_aCommandList[i].m_iId;
			}
		}else{
			matched[0] = FALSE;
			matched[1] = FALSE;

			for(j=0;j<m_pCurTaku->m_members[index].m_aCommandList[i].m_mentsu.m_aPaiList.GetSize();j++){
				if(m_pCurTaku->m_members[index].m_aCommandList[i].m_mentsu.m_aPaiList[j].match(aPai[0])){
					matched[0] = TRUE;
				}
				if(m_pCurTaku->m_members[index].m_aCommandList[i].m_mentsu.m_aPaiList[j].match(aPai[1])){
					matched[1] = TRUE;
				}
			}

			if(m_btnMahjong.getSelMode() == 1){
				if(matched[0]){
					/* タイプのチェック */
					switch(m_iSelType){
					case TYPE_ANKAN:
						if(m_pCurTaku->m_members[index].m_aCommandList[i].m_iType == TYPE_ANKAN || m_pCurTaku->m_members[index].m_aCommandList[i].m_iType == TYPE_DAIMINKAN ||
							m_pCurTaku->m_members[index].m_aCommandList[i].m_iType == TYPE_KUWAEKAN){
							id = m_pCurTaku->m_members[index].m_aCommandList[i].m_iId;
						}
						break;
					default:
						break;
					}
				}
			}else if(m_btnMahjong.getSelMode() == 2){
				if(matched[0] && matched[1]){
					/* タイプのチェック */
					switch(m_iSelType){
					case TYPE_TII:
						if(m_pCurTaku->m_members[index].m_aCommandList[i].m_iType == TYPE_TII){
							id = m_pCurTaku->m_members[index].m_aCommandList[i].m_iId;
						}
						break;
					case TYPE_PON:
						if(m_pCurTaku->m_members[index].m_aCommandList[i].m_iType == TYPE_PON){
							id = m_pCurTaku->m_members[index].m_aCommandList[i].m_iId;
						}
						break;
					default:
						break;
					}
				}
			}
		}

		if(id != -1) break;

	}

	if(id == -1){
		AfxMessageBox(_T("選択した牌が正しくありません"));
	}else{
		if(m_iSelType == TYPE_RIICHI){
			// リーチ音に変更！
			m_fileNormal.closeWaveFile();
			m_fileNormal.openWaveFile(_T("./wav/bgmriichi.wav"));
		}
		SendMessage(WM_SNDCOMMAND,id,0);
	}
	
}

void COpenMahjongClientDbgDlg::OnBtnkan() 
{
	int state = m_btnKan.GetState() & 0x0003;
	if(state == 0x0001){
		resetBtnState();
		m_iSelType = TYPE_ANKAN;
		m_btnMahjong.setSelMode(1);
	}else{
		m_iSelType = -1;
		m_btnMahjong.setSelMode(0);
	}
}

void COpenMahjongClientDbgDlg::OnBtnpass() 
{
	SendMessage(WM_SNDCOMMAND,ID_PASS,0);
}

void COpenMahjongClientDbgDlg::OnBtnpon() 
{
	int state = m_btnPon.GetState() & 0x0003;
	if(state == 0x0001){
		resetBtnState();
		m_iSelType = TYPE_PON;
		m_btnMahjong.setSelMode(2);
	}else{
		m_iSelType = -1;
		m_btnMahjong.setSelMode(0);
	}
	
}

void COpenMahjongClientDbgDlg::OnBtnriichi() 
{
	int state = m_btnRiichi.GetState() & 0x0003;
	if(state == 0x0001){
		resetBtnState();
		m_iSelType = TYPE_RIICHI;
		m_btnMahjong.setSelMode(1);
	}else{
		m_iSelType = -1;
		m_btnMahjong.setSelMode(0);
	}	
}

void COpenMahjongClientDbgDlg::OnBtnron() 
{
	SendMessage(WM_SNDCOMMAND,ID_RON,0);
}

void COpenMahjongClientDbgDlg::OnBtntii() 
{
	int state = m_btnTii.GetState() & 0x0003;
	if(state == 0x0001){
		resetBtnState();
		m_iSelType = TYPE_TII;
		m_btnMahjong.setSelMode(2);
	}else{
		m_iSelType = -1;
		m_btnMahjong.setSelMode(0);
	}
	
}

void COpenMahjongClientDbgDlg::OnBtntouhai() 
{
	SendMessage(WM_SNDCOMMAND,ID_TOUHAI,0);
}

void COpenMahjongClientDbgDlg::OnBtntsumo() 
{
	SendMessage(WM_SNDCOMMAND,ID_TSUMO,0);
}

void COpenMahjongClientDbgDlg::resetBtnState()
{
	switch(m_iSelType){
	case TYPE_TII:
		m_btnTii.SetCheck(0);
		break;
	case TYPE_PON:
		m_btnPon.SetCheck(0);
		break;
	case TYPE_DAIMINKAN:
	case TYPE_ANKAN:
	case TYPE_KUWAEKAN:
		m_btnKan.SetCheck(0);
		break;
	case TYPE_RON:
		m_btnRon.SetCheck(0);
		break;
	case TYPE_RIICHI:
		m_btnRiichi.SetCheck(0);
		break;
	default:
		break;
	}

	m_iSelType = -1;

}

void COpenMahjongClientDbgDlg::appendMessageText(CString& text)
{
	m_messageEdit.appendMessage(text);
}

void COpenMahjongClientDbgDlg::OnSndmes() 
{
	CMessage mes;
	int kaze,i;
	CString echoText;
	CCommand com;

	if(m_iPlayerIndex >= 0){
		UpdateData(TRUE);
		if(m_strSendText != _T("")){
			mes.m_strText = m_strSendText;
			mes.m_playerFrom = m_pCurTaku->m_members[m_iPlayerIndex].m_player;
			
			for(i=0;i<4;i++){
				kaze = ieTable[m_pCurTaku->m_members[m_iPlayerIndex].m_gamestate.m_iZikaze - 1][m_pCurTaku->m_members[i].m_gamestate.m_iZikaze - 1];
				if((kaze == 1 && (m_btnSndTo1.GetCheck() & 1))
					|| (kaze == 2 && (m_btnSndTo2.GetCheck() & 1))
					|| (kaze == 3 && (m_btnSndTo3.GetCheck() & 1))){
					mes.m_aPlayerTo.Add(m_pCurTaku->m_members[i].m_player);
				}
			}
			
			queueMessage(mes);
			
			// ダミーメッセージを送信
			sendCommand(com,echoText);
			
			// ローカルエコー送信
			echoText.Format(_T("%s[%s]：%s\r\n"),m_pCurTaku->m_members[m_iPlayerIndex].m_player.m_strName,
				ieStrTable[m_pCurTaku->m_members[m_iPlayerIndex].m_gamestate.m_iZikaze - 1],
				m_strSendText);
			
			appendMessageText(echoText);
			
			m_strSendText = _T("");
		}

		UpdateData(FALSE);
	}
}

void COpenMahjongClientDbgDlg::OnSndtoall() 
{
	UpdateData(TRUE);
	m_btnSndTo1.SetCheck(1);
	m_btnSndTo2.SetCheck(1);
	m_btnSndTo3.SetCheck(1);
	UpdateData(FALSE);
}

BOOL COpenMahjongClientDbgDlg::PreTranslateMessage(MSG* pMsg) 
{
	TCHAR cName[256];
	CString strClassName;
	if( pMsg->message == WM_KEYDOWN ){
		::GetClassName(::GetFocus(),cName,sizeof(cName));
		strClassName = cName;

		// 文字と数字
		switch(pMsg->wParam){
			case VK_RETURN:
				OnSndmes();
			case VK_ESCAPE:
				return TRUE;
			case VK_TAB:
				break;
			default:
				if(strClassName != _T("Edit")){
					m_ctlSendText.SetFocus();
					//m_ctlSendText.SendMessage(WM_CHAR,pMsg->wParam,pMsg->lParam);
				}
				break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void COpenMahjongClientDbgDlg::resetPlayer()
{
	int i;

	for(i=0;i<m_iPlayerNum+m_iCompNum;i++){
		if(m_players[i].m_bIsComp){
			m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_DESTROY,0,0);
			free(m_players[i].m_pInst);
		}
	}

	m_iPlayerNum = 0;
	m_iCompNum = 0;

}

void COpenMahjongClientDbgDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	resetPlayer();
	m_fileCommand.closeWaveFile();
	m_fileNormal.closeWaveFile();
	m_player.stop();
	m_player.closeDevice();

}

BOOL COpenMahjongClientDbgDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if(m_iCurState == 1){
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void COpenMahjongClientDbgDlg::OnAbort() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(m_pWorkThread){
		m_pWorkThread->PostThreadMessage(WM_QUIT,0,0);
	}
	
}

void COpenMahjongClientDbgDlg::queueMessage(CMessage& mes)
{
	m_queMutex.Lock();

	m_aMessageQueue.Add(mes);

	m_queMutex.Unlock();

}

void COpenMahjongClientDbgDlg::sendCommand(CArray<CCommand,CCommand&> &aCommand,CString& recvMessage)
{
	IXMLDOMDocumentPtr pDoc;
	IXMLDOMElementPtr pElemRoot,pElemCom,pElemID,pElemPlayer;
	IXMLDOMNodePtr pNode;
	HRESULT hr = pDoc.CreateInstance(CLSID_DOMDocument);
	CString str;
	BSTR bstrXML;
	CString sendMessage;
	int i;

	recvMessage = _T("");

	pElemRoot = pDoc->createElement(_T(TAG_OPENMAHJONGCLIENT));
	pDoc->appendChild(pElemRoot);

	str.Format(_T("%d"),m_iSession);

	pElemRoot->setAttribute(_T("session"),(LPCTSTR)str);
	pElemRoot->setAttribute(_T("version"),_T("0.1"));

	for(i=0;i<aCommand.GetSize();i++){
		aCommand[i].toXML(pDoc,pElemRoot);
	}

	/* キューに入ったメッセージを送信 */
	m_queMutex.Lock();

	for(i=0;i<m_aMessageQueue.GetSize();i++){
		m_aMessageQueue[i].toXML(pDoc,pElemRoot);
	}

	m_aMessageQueue.RemoveAll();

	m_queMutex.Unlock();


	pDoc->get_xml(&bstrXML);

	sendMessage = _T("<?xml version=\"1.0\" encoding=\"Shift_JIS\" ?>");
	sendMessage += bstrXML;

	sendString(sendMessage,recvMessage);


}

void COpenMahjongClientDbgDlg::OnBtnrule() 
{
	CRuleDialog ruleDialog;

	ruleDialog.setRule(m_rule);
	ruleDialog.setAccessMode(FALSE);
	
	ruleDialog.DoModal();
}
