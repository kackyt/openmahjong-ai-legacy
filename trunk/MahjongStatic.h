#if !defined(AFX_MAHJONGSTATIC_H__A8FD3147_9799_4CC5_AD2C_F0DFA9D62659__INCLUDED_)
#define AFX_MAHJONGSTATIC_H__A8FD3147_9799_4CC5_AD2C_F0DFA9D62659__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MahjongStatic.h : ヘッダー ファイル
//
#include "Result.h"	// ClassView によって追加されました。

/////////////////////////////////////////////////////////////////////////////
// CMahjongStatic ウィンドウ

class CMahjongStatic : public CStatic
{
// コンストラクション
public:
	CMahjongStatic();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMahjongStatic)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	int m_iType;
	CResult m_result;
	virtual ~CMahjongStatic();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMahjongStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAHJONGSTATIC_H__A8FD3147_9799_4CC5_AD2C_F0DFA9D62659__INCLUDED_)
