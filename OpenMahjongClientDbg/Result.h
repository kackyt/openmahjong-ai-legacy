// Result.h: CResult クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULT_H__DBFE9C3C_7615_459D_9AEB_91C809075983__INCLUDED_)
#define AFX_RESULT_H__DBFE9C3C_7615_459D_9AEB_91C809075983__INCLUDED_

#include "Pai.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResult  
{
public:
	int m_iFu;
	BOOL m_bActive;
	CResult& operator=(CResult&);
	void parseXML(IXMLDOMNodePtr);
	CPai m_machihai;
	int m_iDorakazu;
	BOOL m_bTsumo;
	int m_iMachi;
	int m_iKoScore;
	int m_iOyaScore;
	int m_iScore;
	int m_iMangan;
	int m_iHan;
	CArray<CString,CString&> m_aYaku;
	CResult();
	virtual ~CResult();

};

#endif // !defined(AFX_RESULT_H__DBFE9C3C_7615_459D_9AEB_91C809075983__INCLUDED_)
