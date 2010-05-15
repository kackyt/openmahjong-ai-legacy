// Taku.h: CTaku クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAKU_H__D62DB02C_B097_4C6E_AFB0_BD812D314DA0__INCLUDED_)
#define AFX_TAKU_H__D62DB02C_B097_4C6E_AFB0_BD812D314DA0__INCLUDED_

#include "Pai.h"
#include "Member.h"	// ClassView によって追加されました。
#include "TakuEvent.h"	// ClassView によって追加されました。
#include "MIPIface.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaku  
{
public:
	int getKawahaiEx(int,MJIKawahai*);
	void getMJITehai(int,MJITehai1 *);
	void update(CTaku&);
	CTaku& operator=(CTaku&);
	void printState(int,CString&);
	int getVisibleHais(UINT,int);
	int getKawahai(int,UINT*);
	void getMJITehai(int,MJITehai*);
	int getMemberIndex(CPlayer*);
	CTakuEvent m_event;
	CMember m_members[4];
	void parseXML(IXMLDOMNodePtr);
	int m_iYama;
	int m_iTsumibou;
	int m_iRiichibou;
	int m_iKyokuCount;
	int m_iBakaze;
	CArray<CPai,CPai&> m_aUradora;
	CArray<CPai,CPai&> m_aDora;
	int m_iTurn;
	CTaku();
	virtual ~CTaku();

};

#endif // !defined(AFX_TAKU_H__D62DB02C_B097_4C6E_AFB0_BD812D314DA0__INCLUDED_)
