#if !defined(AFX_VOLUMEDLG_H__7C3DED2D_25D1_4DFD_AF6D_FA2558AF7409__INCLUDED_)
#define AFX_VOLUMEDLG_H__7C3DED2D_25D1_4DFD_AF6D_FA2558AF7409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeDlg.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CVolumeDlg ダイアログ

class CVolumeDlg : public CDialog
{
// コンストラクション
public:
	CVolumeDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CVolumeDlg)
	enum { IDD = IDD_VOLUME };
	CSliderCtrl	m_volSE2;
	CSliderCtrl	m_volSE1;
	CSliderCtrl	m_volBGM;
	int		m_iVolBGM;
	int		m_iVolSE1;
	CString	m_strVolBGM;
	CString	m_strVolSE1;
	CString	m_strVolSE2;
	int		m_iVolSE2;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CVolumeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CVolumeDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VOLUMEDLG_H__7C3DED2D_25D1_4DFD_AF6D_FA2558AF7409__INCLUDED_)
