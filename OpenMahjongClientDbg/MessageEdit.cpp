// MessageEdit.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "MessageEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageEdit

CMessageEdit::CMessageEdit()
{
	m_iLineLimit = 100;
}

CMessageEdit::~CMessageEdit()
{
}


BEGIN_MESSAGE_MAP(CMessageEdit, CEdit)
	//{{AFX_MSG_MAP(CMessageEdit)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageEdit メッセージ ハンドラ

BOOL CMessageEdit::appendMessage(CString& message)
{
	CString str;
	int lines;
	BOOL erased = FALSE;

	GetWindowText(str);

	str += message;

	SetWindowText(str);

	lines = GetLineCount();

	if(lines > m_iLineLimit){
		// 削除する文字列をセレクト
		str = str.Mid(LineIndex(lines - m_iLineLimit));
		SetWindowText(str);

		lines = m_iLineLimit;
		erased = TRUE;
	}


	// 追加した行までスクロールする
	LineScroll(m_iLineLimit + 10);

	return erased;
}
