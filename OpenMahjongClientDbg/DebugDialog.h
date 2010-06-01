/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
 * All rights reserved.

  ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満たす場合に限り、再頒布および使用が許可されます。 

  ・ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売促進に、オープン麻雀の名前またはコントリビューターの名前を使用してはならない。


  本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、明示黙示を問わず、
  商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証もありません。
  著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか
  （過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した
  （代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）
  直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。 

****************************************************************************************/
#if !defined(AFX_DEBUGDIALOG_H__0BC4D97F_FB57_4DCA_B2D9_BADECBA4CE04__INCLUDED_)
#define AFX_DEBUGDIALOG_H__0BC4D97F_FB57_4DCA_B2D9_BADECBA4CE04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugDialog.h : ヘッダー ファイル
//
enum {
	WM_SETTIME = (WM_USER + 100),
	WM_SETSTATUSCODE,
};

/////////////////////////////////////////////////////////////////////////////
// CDebugDialog ダイアログ

class CDebugDialog : public CDialog
{
// コンストラクション
public:
	LRESULT OnSetStatusCode(WPARAM,LPARAM);
	LRESULT OnSetTime(WPARAM,LPARAM);
	void setStatusCode(int);
	void setTime(int);
	CDebugDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDebugDialog)
	enum { IDD = IDD_DEBUG };
	CString	m_strRescodc1;
	CString	m_strRescodc2;
	CString	m_strRescodc3;
	CString	m_strRescodc4;
	CString	m_strRescodc5;
	CString	m_strRescodc6;
	CString	m_strRescodr1;
	CString	m_strRescodr2;
	CString	m_strRescodr3;
	CString	m_strRescodr4;
	CString	m_strRescodr5;
	CString	m_strRescodr6;
	CString	m_strRestimc1;
	CString	m_strRestimc2;
	CString	m_strRestimc3;
	CString	m_strRestimc4;
	CString	m_strRestimr1;
	CString	m_strRestimr2;
	CString	m_strRestimr3;
	CString	m_strRestimr4;
	CString	m_strCUI;
	CString	m_strRestimc5;
	CString	m_strRestimc6;
	CString	m_strRestimr5;
	CString	m_strRestimr6;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDebugDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDebugDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_aTimCount[6];
	int m_iTimTotal;
	int m_aCodCount[6];
	int m_iCodTotal;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DEBUGDIALOG_H__0BC4D97F_FB57_4DCA_B2D9_BADECBA4CE04__INCLUDED_)
