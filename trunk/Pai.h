// Pai.h: CPai クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAI_H__16AB8A35_5601_46CB_89F4_1F11DA494FA4__INCLUDED_)
#define AFX_PAI_H__16AB8A35_5601_46CB_89F4_1F11DA494FA4__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PAI_NIL     (34)
#define PAI_NOTINIT (63)

class CPai  
{
public:
	BOOL match(CPai&);
	void set(UINT id);
	BOOL m_bRiichi;
	void getDora(CPai&);
	void getName(CString&);
	operator UINT();
	void parseXML(IXMLDOMNodePtr);
	BOOL m_bTsumogiri;
	int m_iId;
	int m_iNo;
	int m_iCategory;
	CPai();
	virtual ~CPai();

};

#endif // !defined(AFX_PAI_H__16AB8A35_5601_46CB_89F4_1F11DA494FA4__INCLUDED_)
