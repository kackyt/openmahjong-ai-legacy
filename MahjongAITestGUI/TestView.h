#if !defined(AFX_TESTVIEW_H__2456EE92_D85A_4389_ADF3_AEF43090EC2C__INCLUDED_)
#define AFX_TESTVIEW_H__2456EE92_D85A_4389_ADF3_AEF43090EC2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TestView.h : ヘッダー ファイル
//
#include "Komae.h"

/////////////////////////////////////////////////////////////////////////////
// CTestView ウィンドウ

class CTestView : public CButton
{
// コンストラクション
public:
	CTestView();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTestView)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTestView();
	CKomae m_haigaList;
	int m_aTehai[13];
	int m_iTehaiSize;
	int m_iTsumohai;
	int m_aSutehai[18];
	int m_iSutehaiSize;
	int m_iDora;
	void reset();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TESTVIEW_H__2456EE92_D85A_4389_ADF3_AEF43090EC2C__INCLUDED_)
