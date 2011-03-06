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
// ConnectDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "ConnectDlg.h"
#include "OpenMahjongClientDbgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg ダイアログ


CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDlg)
	m_playerName = _T("");
	m_iSession = 0;
	m_strDst = _T("");
	m_iNewSess = 0;
	//}}AFX_DATA_INIT
}


void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
	DDX_Control(pDX, IDC_BTNRULE, m_btnRule);
	DDX_Control(pDX, IDC_SESSION, m_editSess);
	DDX_Control(pDX, IDC_CMBCOMP3, m_cmbComp3);
	DDX_Control(pDX, IDC_CMBCOMP2, m_cmbComp2);
	DDX_Control(pDX, IDC_CMBCOMP1, m_cmbComp1);
	DDX_Text(pDX, IDC_PNAME, m_playerName);
	DDX_Text(pDX, IDC_SESSION, m_iSession);
	DDX_CBString(pDX, IDC_CONNECTDST, m_strDst);
	DDX_Radio(pDX, IDC_RADIONEW, m_iNewSess);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	ON_BN_CLICKED(IDC_BTNRULE, OnBtnrule)
	ON_BN_CLICKED(IDC_DEBUG, OnDebug)
	ON_BN_CLICKED(IDC_RADIONEW, OnRadionew)
	ON_BN_CLICKED(IDC_RADIOAPPEND, OnRadioappend)
	ON_BN_CLICKED(IDC_RESTART, OnRestart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg メッセージ ハンドラ

BOOL CConnectDlg::PreTranslateMessage(MSG* pMsg) 
{
	// キーが押されたとき
	if( pMsg->message == WM_KEYDOWN ){
		// 文字と数字
		switch(pMsg->wParam){
			case VK_RETURN:
			case VK_ESCAPE:
				return TRUE;
			default:
				break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CConnectDlg::OnInitDialog() 
{
	COpenMahjongClientDbgDlg *pObj = (COpenMahjongClientDbgDlg *)m_pTopObj;
	int i;
	CDialog::OnInitDialog();
	m_iNewSess = 0;

	m_cmbComp1.AddString(_T("なし"));
	m_cmbComp1.SetCurSel(0);
	m_cmbComp2.AddString(_T("なし"));
	m_cmbComp2.SetCurSel(0);
	m_cmbComp3.AddString(_T("なし"));
	m_cmbComp3.SetCurSel(0);
	for(i=0;i<pObj->m_iLogicNum;i++){
		m_cmbComp1.AddString(pObj->compName[i]);
		m_cmbComp2.AddString(pObj->compName[i]);
		m_cmbComp3.AddString(pObj->compName[i]);
	}

	m_btnRule.EnableWindow(TRUE);
	m_editSess.EnableWindow(FALSE);

	UpdateData(FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CConnectDlg::OnOK() 
{
	UpdateData(TRUE);

	if(m_strDst == _T("")){
		MessageBox(_T("接続先を入れてください"));
		return;
	}

	if(m_iNewSess < 2 && m_playerName == _T("")){
		MessageBox(_T("プレーヤー名を入れてください"));
		return;
	}

	m_cmbComp1.GetLBText(m_cmbComp1.GetCurSel(),m_strComp1);
		
	m_cmbComp2.GetLBText(m_cmbComp2.GetCurSel(),m_strComp2);

	m_cmbComp3.GetLBText(m_cmbComp3.GetCurSel(),m_strComp3);
	
	CDialog::OnOK();
}

void CConnectDlg::OnBtnrule() 
{
	m_ruleDlg.setAccessMode(TRUE);
	m_ruleDlg.DoModal();
}

void CConnectDlg::OnDebug() 
{
	m_btnRule.EnableWindow(FALSE);
	m_editSess.EnableWindow(TRUE);	
}

void CConnectDlg::OnRadionew() 
{
	m_btnRule.EnableWindow(TRUE);
	m_editSess.EnableWindow(FALSE);
}

void CConnectDlg::OnRadioappend() 
{
	m_btnRule.EnableWindow(FALSE);
	m_editSess.EnableWindow(TRUE);	
}

void CConnectDlg::OnRestart() 
{
	m_btnRule.EnableWindow(FALSE);
	m_editSess.EnableWindow(FALSE);	
}
