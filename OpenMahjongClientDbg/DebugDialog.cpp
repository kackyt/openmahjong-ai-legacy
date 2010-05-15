// DebugDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "DebugDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDebugDialog ダイアログ


CDebugDialog::CDebugDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDebugDialog)
	m_strRescodc1 = _T("0");
	m_strRescodc2 = _T("0");
	m_strRescodc3 = _T("0");
	m_strRescodc4 = _T("0");
	m_strRescodc5 = _T("0");
	m_strRescodc6 = _T("0");
	m_strRescodr1 = _T("0");
	m_strRescodr2 = _T("0");
	m_strRescodr3 = _T("0");
	m_strRescodr4 = _T("0");
	m_strRescodr5 = _T("0");
	m_strRescodr6 = _T("0");
	m_strRestimc1 = _T("0");
	m_strRestimc2 = _T("0");
	m_strRestimc3 = _T("0");
	m_strRestimc4 = _T("0");
	m_strRestimr1 = _T("0");
	m_strRestimr2 = _T("0");
	m_strRestimr3 = _T("0");
	m_strRestimr4 = _T("0");
	m_strCUI = _T("");
	m_strRestimc5 = _T("0");
	m_strRestimc6 = _T("0");
	m_strRestimr5 = _T("0");
	m_strRestimr6 = _T("0");
	//}}AFX_DATA_INIT
}


void CDebugDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugDialog)
	DDX_Text(pDX, IDC_RESCODC1, m_strRescodc1);
	DDX_Text(pDX, IDC_RESCODC2, m_strRescodc2);
	DDX_Text(pDX, IDC_RESCODC3, m_strRescodc3);
	DDX_Text(pDX, IDC_RESCODC4, m_strRescodc4);
	DDX_Text(pDX, IDC_RESCODC5, m_strRescodc5);
	DDX_Text(pDX, IDC_RESCODC6, m_strRescodc6);
	DDX_Text(pDX, IDC_RESCODR1, m_strRescodr1);
	DDX_Text(pDX, IDC_RESCODR2, m_strRescodr2);
	DDX_Text(pDX, IDC_RESCODR3, m_strRescodr3);
	DDX_Text(pDX, IDC_RESCODR4, m_strRescodr4);
	DDX_Text(pDX, IDC_RESCODR5, m_strRescodr5);
	DDX_Text(pDX, IDC_RESCODR6, m_strRescodr6);
	DDX_Text(pDX, IDC_RESTIMC1, m_strRestimc1);
	DDX_Text(pDX, IDC_RESTIMC2, m_strRestimc2);
	DDX_Text(pDX, IDC_RESTIMC3, m_strRestimc3);
	DDX_Text(pDX, IDC_RESTIMC4, m_strRestimc4);
	DDX_Text(pDX, IDC_RESTIMR1, m_strRestimr1);
	DDX_Text(pDX, IDC_RESTIMR2, m_strRestimr2);
	DDX_Text(pDX, IDC_RESTIMR3, m_strRestimr3);
	DDX_Text(pDX, IDC_RESTIMR4, m_strRestimr4);
	DDX_Text(pDX, IDC_CUI, m_strCUI);
	DDX_Text(pDX, IDC_RESTIMC5, m_strRestimc5);
	DDX_Text(pDX, IDC_RESTIMC6, m_strRestimc6);
	DDX_Text(pDX, IDC_RESTIMR5, m_strRestimr5);
	DDX_Text(pDX, IDC_RESTIMR6, m_strRestimr6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugDialog, CDialog)
	//{{AFX_MSG_MAP(CDebugDialog)
	ON_MESSAGE(WM_SETTIME,OnSetTime)
	ON_MESSAGE(WM_SETSTATUSCODE,OnSetStatusCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDebugDialog メッセージ ハンドラ

void CDebugDialog::setTime(int tim)
{
	m_iTimTotal++;

	if(tim < 100){
		m_aTimCount[0]++;
		m_strRestimc1.Format(_T("%d"),m_aTimCount[0]);
	}else if(tim < 200){
		m_aTimCount[1]++;
		m_strRestimc2.Format(_T("%d"),m_aTimCount[1]);
	}else if(tim < 300){
		m_aTimCount[2]++;
		m_strRestimc3.Format(_T("%d"),m_aTimCount[2]);
	}else if(tim < 400){
		m_aTimCount[3]++;
		m_strRestimc4.Format(_T("%d"),m_aTimCount[3]);
	}else if(tim < 500){
		m_aTimCount[4]++;
		m_strRestimc5.Format(_T("%d"),m_aTimCount[4]);
	}else{
		m_aTimCount[5]++;
		m_strRestimc6.Format(_T("%d"),m_aTimCount[5]);
	}

	m_strRestimr1.Format(_T("%03.1f%%"),m_aTimCount[0] * 100.0 / m_iTimTotal);
	m_strRestimr2.Format(_T("%03.1f%%"),m_aTimCount[1] * 100.0 / m_iTimTotal);
	m_strRestimr3.Format(_T("%03.1f%%"),m_aTimCount[2] * 100.0 / m_iTimTotal);
	m_strRestimr4.Format(_T("%03.1f%%"),m_aTimCount[3] * 100.0 / m_iTimTotal);
	m_strRestimr5.Format(_T("%03.1f%%"),m_aTimCount[4] * 100.0 / m_iTimTotal);
	m_strRestimr6.Format(_T("%03.1f%%"),m_aTimCount[5] * 100.0 / m_iTimTotal);

	UpdateData(FALSE);

}

BOOL CDebugDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	ZeroMemory(m_aCodCount,sizeof(m_aCodCount));
	ZeroMemory(m_aTimCount,sizeof(m_aTimCount));
	m_iTimTotal = 0;
	m_iCodTotal = 0;
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDebugDialog::setStatusCode(int code)
{
	m_iCodTotal++;

	if(code == 100){
		m_aCodCount[0]++;
		m_strRescodc1.Format(_T("%d"),m_aCodCount[0]);
	}else if(code == 101){
		m_aCodCount[1]++;
		m_strRescodc2.Format(_T("%d"),m_aCodCount[1]);
	}else if(code == 102){
		m_aCodCount[2]++;
		m_strRescodc3.Format(_T("%d"),m_aCodCount[2]);
	}else if(code == 103){
		m_aCodCount[3]++;
		m_strRescodc4.Format(_T("%d"),m_aCodCount[3]);
	}else if(code == 104){
		m_aCodCount[4]++;
		m_strRescodc5.Format(_T("%d"),m_aCodCount[4]);
	}else if(code == 105){
		m_aCodCount[5]++;
		m_strRescodc6.Format(_T("%d"),m_aCodCount[5]);
	}

	m_strRescodr1.Format(_T("%03.1f%%"),m_aCodCount[0] * 100.0 / m_iCodTotal);
	m_strRescodr2.Format(_T("%03.1f%%"),m_aCodCount[1] * 100.0 / m_iCodTotal);
	m_strRescodr3.Format(_T("%03.1f%%"),m_aCodCount[2] * 100.0 / m_iCodTotal);
	m_strRescodr4.Format(_T("%03.1f%%"),m_aCodCount[3] * 100.0 / m_iCodTotal);
	m_strRescodr5.Format(_T("%03.1f%%"),m_aCodCount[4] * 100.0 / m_iCodTotal);
	m_strRescodr6.Format(_T("%03.1f%%"),m_aCodCount[5] * 100.0 / m_iCodTotal);

	UpdateData(FALSE);
}

LRESULT CDebugDialog::OnSetTime(WPARAM wParam,LPARAM lParam)
{
	setTime(wParam);
	return 0;
}

LRESULT CDebugDialog::OnSetStatusCode(WPARAM wParam,LPARAM lParam)
{
	setStatusCode(wParam);
	return 0;
}
