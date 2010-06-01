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
// RuleDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "RuleDialog.h"
#include "Rule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuleDialog ダイアログ


CRuleDialog::CRuleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRuleDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuleDialog)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CRuleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuleDialog)
	DDX_Control(pDX, IDC_CMBHAIPAI, m_cmbHaipai);
	DDX_Control(pDX, IDC_CMBAOTENJO, m_cmbAotenjo);
	DDX_Control(pDX, IDC_CMBAKADORA, m_cmbAka);
	DDX_Control(pDX, IDC_CMBKUITAN, m_cmbKuitan);
	DDX_Control(pDX, IDC_CMBGLASSHAI, m_cmbGlasshai);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRuleDialog, CDialog)
	//{{AFX_MSG_MAP(CRuleDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuleDialog メッセージ ハンドラ

void CRuleDialog::setAccessMode(BOOL isWritable)
{
	m_bWritable = isWritable;

}

void CRuleDialog::setRule(CRule& rule)
{
	m_rule = rule;
	m_rule.m_bActive = TRUE;
}

void CRuleDialog::getRule(CRule& rule)
{
	rule = m_rule;
	rule.m_bActive = TRUE;
}

BOOL CRuleDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_bWritable){
		m_cmbGlasshai.EnableWindow(TRUE);
		m_cmbKuitan.EnableWindow(TRUE);
		m_cmbAka.EnableWindow(TRUE);
		m_cmbAotenjo.EnableWindow(TRUE);
		m_cmbHaipai.EnableWindow(TRUE);
	}else{
		m_cmbGlasshai.EnableWindow(FALSE);
		m_cmbKuitan.EnableWindow(FALSE);
		m_cmbAka.EnableWindow(FALSE);
		m_cmbAotenjo.EnableWindow(FALSE);
		m_cmbHaipai.EnableWindow(FALSE);
	}

	m_cmbGlasshai.SetCurSel(m_rule.m_iGlasshai);
	m_cmbKuitan.SetCurSel(m_rule.m_iKuitan);
	m_cmbAka.SetCurSel(m_rule.m_iAka);
	m_cmbAotenjo.SetCurSel(m_rule.m_iAotenjo);
	m_cmbHaipai.SetCurSel(m_rule.m_iHaipai);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CRuleDialog::OnOK() 
{
	UpdateData(TRUE);

	m_rule.m_bActive = TRUE;
	m_rule.m_iGlasshai = m_cmbGlasshai.GetCurSel();
	m_rule.m_iKuitan = m_cmbKuitan.GetCurSel();
	m_rule.m_iAka = m_cmbAka.GetCurSel();
	m_rule.m_iAotenjo = m_cmbAotenjo.GetCurSel();
	m_rule.m_iHaipai = m_cmbHaipai.GetCurSel();
	
	CDialog::OnOK();
}
