#if !defined(AFX_DEBUGDIALOG_H__0BC4D97F_FB57_4DCA_B2D9_BADECBA4CE04__INCLUDED_)
#define AFX_DEBUGDIALOG_H__0BC4D97F_FB57_4DCA_B2D9_BADECBA4CE04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugDialog.h : ヘッダー ファイル
//
enum {
	WM_SETTIME = (WM_USER + 100),
	WM_SETSTATUSCODE,
};

/////////////////////////////////////////////////////////////////////////////
// CDebugDialog ダイアログ

class CDebugDialog : public CDialog
{
// コンストラクション
public:
	LRESULT OnSetStatusCode(WPARAM,LPARAM);
	LRESULT OnSetTime(WPARAM,LPARAM);
	void setStatusCode(int);
	void setTime(int);
	CDebugDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDebugDialog)
	enum { IDD = IDD_DEBUG };
	CString	m_strRescodc1;
	CString	m_strRescodc2;
	CString	m_strRescodc3;
	CString	m_strRescodc4;
	CString	m_strRescodc5;
	CString	m_strRescodc6;
	CString	m_strRescodr1;
	CString	m_strRescodr2;
	CString	m_strRescodr3;
	CString	m_strRescodr4;
	CString	m_strRescodr5;
	CString	m_strRescodr6;
	CString	m_strRestimc1;
	CString	m_strRestimc2;
	CString	m_strRestimc3;
	CString	m_strRestimc4;
	CString	m_strRestimr1;
	CString	m_strRestimr2;
	CString	m_strRestimr3;
	CString	m_strRestimr4;
	CString	m_strCUI;
	CString	m_strRestimc5;
	CString	m_strRestimc6;
	CString	m_strRestimr5;
	CString	m_strRestimr6;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDebugDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDebugDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_aTimCount[6];
	int m_iTimTotal;
	int m_aCodCount[6];
	int m_iCodTotal;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DEBUGDIALOG_H__0BC4D97F_FB57_4DCA_B2D9_BADECBA4CE04__INCLUDED_)
