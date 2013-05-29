// MahjongAITestGUI.h : MAHJONAITESTGUI アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_MAHJONAITESTGUI_H__7C4A5ADA_4FC3_47E0_A25C_7366AC9C59D4__INCLUDED_)
#define AFX_MAHJONAITESTGUI_H__7C4A5ADA_4FC3_47E0_A25C_7366AC9C59D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CMahjongAITestGUIApp:
// このクラスの動作の定義に関しては MahjongAITestGUI.cpp ファイルを参照してください。
//

class CMahjongAITestGUIApp : public CWinApp
{
public:
	CMahjongAITestGUIApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMahjongAITestGUIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CMahjongAITestGUIApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAHJONAITESTGUI_H__7C4A5ADA_4FC3_47E0_A25C_7366AC9C59D4__INCLUDED_)
