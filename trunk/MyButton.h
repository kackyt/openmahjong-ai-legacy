#if !defined(AFX_MYBUTTON_H__DEDDE70F_638F_4F43_AF30_069D7CBFED01__INCLUDED_)
#define AFX_MYBUTTON_H__DEDDE70F_638F_4F43_AF30_069D7CBFED01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton ウィンドウ

class CMyButton : public CButton
{
// コンストラクション
public:
	CMyButton();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMyButton)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CFont m_pushedFont;
	CBrush m_pushedBrush;
	CFont m_defFont;
	virtual ~CMyButton();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MYBUTTON_H__DEDDE70F_638F_4F43_AF30_069D7CBFED01__INCLUDED_)
