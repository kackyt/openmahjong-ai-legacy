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
	}else{
		m_cmbGlasshai.EnableWindow(FALSE);
		m_cmbKuitan.EnableWindow(FALSE);
		m_cmbAka.EnableWindow(FALSE);
		m_cmbAotenjo.EnableWindow(FALSE);
	}

	m_cmbGlasshai.SetCurSel(m_rule.m_iGlasshai);
	m_cmbKuitan.SetCurSel(m_rule.m_iKuitan);
	m_cmbAka.SetCurSel(m_rule.m_iAka);
	m_cmbAotenjo.SetCurSel(m_rule.m_iAotenjo);
	
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
	
	CDialog::OnOK();
}
