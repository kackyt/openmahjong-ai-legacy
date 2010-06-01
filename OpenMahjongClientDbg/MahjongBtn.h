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
#if !defined(AFX_MAHJONGBTN_H__969D422C_1E03_42F0_A91A_DC0023FC8094__INCLUDED_)
#define AFX_MAHJONGBTN_H__969D422C_1E03_42F0_A91A_DC0023FC8094__INCLUDED_

#include "Taku.h"	// ClassView によって追加されました。
#include "Komae.h"	// ClassView によって追加されました。
#include "Rule.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MahjongBtn.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CMahjongBtn ウィンドウ

class CMahjongBtn : public CButton
{
// コンストラクション
public:
	CMahjongBtn();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CMahjongBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	CRule m_rule;
	CKomae m_haigaListYoko180;
	CKomae m_haigaList180;
	CKomae m_haigaListYoko;
	int getSelMode();
	void setSelMode(int);
	int m_selIndex[2];
	BOOL m_bActive;
	CKomae m_haigaList;
	void refresh(int,CTaku&);
	virtual ~CMahjongBtn();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMahjongBtn)
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	UINT getPaiState(CPai&);
	void paintPai(CPaintDC &dc,CRect &rect,CString &str,CMember *member,int count);
	int m_iPlayerIndex;
	CTaku m_taku;
	int m_iSelMode;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MAHJONGBTN_H__969D422C_1E03_42F0_A91A_DC0023FC8094__INCLUDED_)
