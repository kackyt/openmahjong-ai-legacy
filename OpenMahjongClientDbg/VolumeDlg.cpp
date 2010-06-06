// VolumeDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "VolumeDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg ダイアログ


CVolumeDlg::CVolumeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVolumeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolumeDlg)
	m_iVolBGM = 0;
	m_iVolSE1 = 0;
	m_strVolBGM = _T("");
	m_strVolSE1 = _T("");
	m_strVolSE2 = _T("");
	m_iVolSE2 = 0;
	//}}AFX_DATA_INIT
}


void CVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeDlg)
	DDX_Control(pDX, IDC_VOLSE2, m_volSE2);
	DDX_Control(pDX, IDC_VOLSE1, m_volSE1);
	DDX_Control(pDX, IDC_VOLBGM, m_volBGM);
	DDX_Slider(pDX, IDC_VOLBGM, m_iVolBGM);
	DDX_Slider(pDX, IDC_VOLSE1, m_iVolSE1);
	DDX_Text(pDX, IDC_STRVOLBGM, m_strVolBGM);
	DDX_Text(pDX, IDC_STRVOLSE1, m_strVolSE1);
	DDX_Text(pDX, IDC_STRVOLSE2, m_strVolSE2);
	DDX_Slider(pDX, IDC_VOLSE2, m_iVolSE2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CVolumeDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg メッセージ ハンドラ

void CVolumeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	UpdateData(TRUE);

	m_strVolBGM.Format(_T("%d"),m_iVolBGM);
	m_strVolSE1.Format(_T("%d"),m_iVolSE1);
	m_strVolSE2.Format(_T("%d"),m_iVolSE2);

	GetParent()->SendMessage(WM_SETVOLUME,0,m_iVolBGM);
	GetParent()->SendMessage(WM_SETVOLUME,1,m_iVolSE1);
	GetParent()->SendMessage(WM_SETVOLUME,2,m_iVolSE2);

	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CVolumeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int prevBGM = m_iVolBGM;
	int prevSE1 = m_iVolSE1;
	int prevSE2 = m_iVolSE2;
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_volBGM.SetRange(-30,10,TRUE);
	m_volSE1.SetRange(-30,10,TRUE);
	m_volSE2.SetRange(-30,10,TRUE);
	m_iVolBGM = prevBGM;
	m_iVolSE1 = prevSE1;
	m_iVolSE2 = prevSE2;
	m_strVolBGM.Format(_T("%d"),m_iVolBGM);
	m_strVolSE1.Format(_T("%d"),m_iVolSE1);
	m_strVolSE2.Format(_T("%d"),m_iVolSE2);

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
