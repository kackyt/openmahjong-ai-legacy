#if !defined(AFX_AGARIDIALOG_H__A3811369_9311_40D3_B697_BD9D3AA454F9__INCLUDED_)
#define AFX_AGARIDIALOG_H__A3811369_9311_40D3_B697_BD9D3AA454F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AgariDialog.h : ヘッダー ファイル
//

#include "MahjongStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CAgariDialog ダイアログ

class CAgariDialog : public CDialog
{
// コンストラクション
public:
	CAgariDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CAgariDialog)
	enum { IDD = IDD_AGARI };
	CMahjongStatic	m_staAgariyaku;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAgariDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAgariDialog)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_AGARIDIALOG_H__A3811369_9311_40D3_B697_BD9D3AA454F9__INCLUDED_)
