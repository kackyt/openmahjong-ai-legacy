// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//
#ifndef WITHQT

#if !defined(AFX_STDAFX_H__25F2E1E9_CAE5_437B_BD1A_C9701C0B03FB__INCLUDED_)
#define AFX_STDAFX_H__25F2E1E9_CAE5_437B_BD1A_C9701C0B03FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _MSC_VER
#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します

#include <afx.h>
#include <afxwin.h>

// TODO: プログラムで必要なヘッダー参照を追加してください。
#include <afxtempl.h>

#include <mmsystem.h>

#import "msxml3.dll" named_guids
using namespace MSXML2;

#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__25F2E1E9_CAE5_437B_BD1A_C9701C0B03FB__INCLUDED_)

#endif
