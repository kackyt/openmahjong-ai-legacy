#if !defined(AFX_CONNECTDLG_H__CF340994_31A2_45F7_9DFF_4A4F8CEE4ECA__INCLUDED_)
#define AFX_CONNECTDLG_H__CF340994_31A2_45F7_9DFF_4A4F8CEE4ECA__INCLUDED_

#include "RuleDialog.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg ダイアログ

class CConnectDlg : public CDialog
{
// コンストラクション
public:
	CRuleDialog m_ruleDlg;
	CString m_strComp3;
	CString m_strComp2;
	CString m_strComp1;
	LPVOID m_pTopObj;
	CConnectDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CConnectDlg)
	enum { IDD = IDD_CONNECT };
	CComboBox	m_cmbComp3;
	CComboBox	m_cmbComp2;
	CComboBox	m_cmbComp1;
	CString	m_playerName;
	int		m_iSession;
	CString	m_strDst;
	int		m_iNewSess;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CConnectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CConnectDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnrule();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CONNECTDLG_H__CF340994_31A2_45F7_9DFF_4A4F8CEE4ECA__INCLUDED_)
