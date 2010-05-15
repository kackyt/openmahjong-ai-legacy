// OpenMahjongClientDbg.h : OPENMAHJONGCLIENTDBG アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_OPENMAHJONGCLIENTDBG_H__4881DFF9_D3CA_4AE8_902E_6E3A58653CBA__INCLUDED_)
#define AFX_OPENMAHJONGCLIENTDBG_H__4881DFF9_D3CA_4AE8_902E_6E3A58653CBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// COpenMahjongClientDbgApp:
// このクラスの動作の定義に関しては OpenMahjongClientDbg.cpp ファイルを参照してください。
//

class COpenMahjongClientDbgApp : public CWinApp
{
public:
	COpenMahjongClientDbgApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COpenMahjongClientDbgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(COpenMahjongClientDbgApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPENMAHJONGCLIENTDBG_H__4881DFF9_D3CA_4AE8_902E_6E3A58653CBA__INCLUDED_)
