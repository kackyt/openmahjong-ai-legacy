// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__6D73E61E_CB67_4843_95B0_C6457FECE2EF__INCLUDED_)
#define AFX_STDAFX_H__6D73E61E_CB67_4843_95B0_C6457FECE2EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC のオートメーション クラス
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC のソケット拡張機能
#include <afxinet.h>
#include <afxtempl.h>

#include <mmsystem.h>

#include "OpenMahjongDef.h"

#import "msxml.dll" named_guids
using namespace MSXML;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__6D73E61E_CB67_4843_95B0_C6457FECE2EF__INCLUDED_)
