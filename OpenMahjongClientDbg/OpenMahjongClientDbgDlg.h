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
// OpenMahjongClientDbgDlg.h : ヘッダー ファイル
//

#if !defined(AFX_OPENMAHJONGCLIENTDBGDLG_H__F00E9E9C_E252_4199_B645_8A686A31936A__INCLUDED_)
#define AFX_OPENMAHJONGCLIENTDBGDLG_H__F00E9E9C_E252_4199_B645_8A686A31936A__INCLUDED_

#include "AgariDialog.h"	// ClassView によって追加されました。
#include <afxmt.h>
#include "Taku.h"	// ClassView によって追加されました。
#include "ConnectDlg.h"	// ClassView によって追加されました。
#include "Player.h"
#include "MIPIface.h"
#include "MahjongStatic.h"
#include "MahjongBtn.h"
#include "MyButton.h"
#include "Library.h"
#include "Message.h"
#include "DebugDialog.h"	// ClassView によって追加されました。
#include "MessageEdit.h"
#include "WavePlayer.h"	// ClassView によって追加されました。
#include "WavePlayMatrix.h"	// ClassView によって追加されました。
#include "WaveFilePlayMethod.h"	// ClassView によって追加されました。
#include "Rule.h"	// ClassView によって追加されました。
#include "VolumeDlg.h"	// ClassView によって追加されました。

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define COMP_MAX (256)

/////////////////////////////////////////////////////////////////////////////
// COpenMahjongClientDbgDlg ダイアログ

class COpenMahjongClientDbgDlg : public CDialog
{
// 構築
public:
	CWaveFilePlayMethod m_fileSE;
	int m_hVolSE2;
	LRESULT OnSetVolume(WPARAM,LPARAM);
	CVolumeDlg m_volDlg;
	int m_hVolSE1;
	int m_hVolBGM;
	CRule m_rule;
	CWaveFilePlayMethod m_fileNormal;
	CWaveFilePlayMethod m_fileCommand;
	CWavePlayMatrix m_matrix;
	CWavePlayer m_player;
	BOOL m_bFirst;
	void sendCommand(CArray<CCommand,CCommand&>&,CString&);
	void queueMessage(CMessage&);
	CWinThread * m_pWorkThread;
	int m_iCurState;
	void resetPlayer();
	int m_iAgari;
	CAgariDialog m_agariDlg;
	CDebugDialog m_debugDlg;
	CMutex m_queMutex;
	CArray<CMessage,CMessage&> m_aMessageQueue;
	void appendMessageText(CString&);
	int m_iPlayerIndex;
	int m_iSelType;
	void sendLibrary(CLibrary&);
	int m_iSyncTick;
	void sendString(CString&,CString&);
	BOOL m_bBusy;
	LRESULT OnSndCommand(WPARAM,LPARAM);
	LRESULT OnRefresh(WPARAM,LPARAM);
	int m_iLogicNum;
	int m_iSession;
	int sendCommand(CCommand&,CString&);
	CPlayer * m_pCurPlayer;
	BOOL searchComp(CString&);
	void gameSync();
	CTaku *m_pCurTaku;
	CTaku m_aTakuAll[4];
	CConnectDlg m_connDlg;
	CString m_strHostURL;
	int m_iCompNum;
	int m_iPlayerNum;
	HMODULE hComp[COMP_MAX];
	MJPIFunc pFunc[COMP_MAX];
	CString compName[COMP_MAX];
	LPVOID pInst[4];
	int structMode[4];
	CPlayer m_players[4];
	COpenMahjongClientDbgDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(COpenMahjongClientDbgDlg)
	enum { IDD = IDD_OPENMAHJONGCLIENTDBG_DIALOG };
	CButton	m_btnSndMes;
	CEdit	m_ctlSendText;
	CButton	m_btnConnect;
	CMessageEdit	m_messageEdit;
	CButton	m_btnSndTo3;
	CButton	m_btnSndTo2;
	CButton	m_btnSndTo1;
	CButton	m_btnTsumo;
	CButton	m_btnTouhai;
	CButton	m_btnRon;
	CButton	m_btnRiichi;
	CButton	m_btnPon;
	CButton	m_btnPass;
	CButton	m_btnKan;
	CButton	m_btnDecide;
	CButton	m_btnTii;
	CMahjongBtn	m_btnMahjong;
	CButton	m_btnSync;
	CString	m_strCUIMessage;
	int		m_iComID;
	CString	m_strTakunum;
	CString	m_strSyncTick;
	CString	m_strSendText;
	BOOL	m_bDebugPrint;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COpenMahjongClientDbgDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COpenMahjongClientDbgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnconnect();
	afx_msg void OnCmdsnd();
	afx_msg void OnBtnsync();
	afx_msg void OnBtnsyncapl();
	afx_msg void OnBtndecide();
	afx_msg void OnBtnkan();
	afx_msg void OnBtnpass();
	afx_msg void OnBtnpon();
	afx_msg void OnBtnriichi();
	afx_msg void OnBtnron();
	afx_msg void OnBtntii();
	afx_msg void OnBtntouhai();
	afx_msg void OnBtntsumo();
	afx_msg void OnSndmes();
	afx_msg void OnSndtoall();
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnAbort();
	afx_msg void OnBtnrule();
	afx_msg void OnBtnvol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void resetBtnState();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OPENMAHJONGCLIENTDBGDLG_H__F00E9E9C_E252_4199_B645_8A686A31936A__INCLUDED_)
