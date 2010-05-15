#if !defined(AFX_MAHJONGBTN_H__969D422C_1E03_42F0_A91A_DC0023FC8094__INCLUDED_)
#define AFX_MAHJONGBTN_H__969D422C_1E03_42F0_A91A_DC0023FC8094__INCLUDED_

#include "Taku.h"	// ClassView によって追加されました。
#include "Komae.h"	// ClassView によって追加されました。
#include "Rule.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MahjongBtn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMahjongBtn ウィンドウ

class CMahjongBtn : public CButton
{
// コンストラクション
public:
	CMahjongBtn();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMahjongBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CRule m_rule;
	CKomae m_haigaListYoko180;
	CKomae m_haigaList180;
	CKomae m_haigaListYoko;
	int getSelMode();
	void setSelMode(int);
	int m_selIndex[2];
	BOOL m_bActive;
	CKomae m_haigaList;
	void refresh(int,CTaku&);
	virtual ~CMahjongBtn();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMahjongBtn)
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void paintPai(CPaintDC &dc,CRect &rect,CString &str,CMember *member,int count);
	int m_iPlayerIndex;
	CTaku m_taku;
	int m_iSelMode;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAHJONGBTN_H__969D422C_1E03_42F0_A91A_DC0023FC8094__INCLUDED_)
