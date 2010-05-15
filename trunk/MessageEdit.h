#if !defined(AFX_MESSAGEEDIT_H__847A5FF7_3EAD_4B57_9492_B10A11E14633__INCLUDED_)
#define AFX_MESSAGEEDIT_H__847A5FF7_3EAD_4B57_9492_B10A11E14633__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMessageEdit ウィンドウ

class CMessageEdit : public CEdit
{
// コンストラクション
public:
	CMessageEdit();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMessageEdit)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	int m_iLineLimit;
	BOOL appendMessage(CString&);
	virtual ~CMessageEdit();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMessageEdit)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MESSAGEEDIT_H__847A5FF7_3EAD_4B57_9492_B10A11E14633__INCLUDED_)
