#if !defined(AFX_RULEDIALOG_H__6DB009DA_AF31_42B9_8AE7_73FE09EABFFC__INCLUDED_)
#define AFX_RULEDIALOG_H__6DB009DA_AF31_42B9_8AE7_73FE09EABFFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuleDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRuleDialog ダイアログ

class CRuleDialog : public CDialog
{
// コンストラクション
public:
	CRuleDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CRuleDialog)
	enum { IDD = IDD_RULE };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRuleDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRuleDialog)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RULEDIALOG_H__6DB009DA_AF31_42B9_8AE7_73FE09EABFFC__INCLUDED_)
