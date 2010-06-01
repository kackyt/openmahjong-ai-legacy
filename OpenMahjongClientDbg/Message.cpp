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
// Message.cpp: CMessage クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CMessage::CMessage()
{

}

CMessage::~CMessage()
{

}

void CMessage::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	pNode = pElem->selectSingleNode(_T(TAG_TEXT));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		m_strText = pStr;
	}

	pNode = pElem->selectSingleNode(_T(TAG_FROM "/" TAG_PLAYER));

	if(pNode != NULL){
		m_playerFrom.parseXML(pNode);
	}

	pNodeList = pElem->selectNodes(_T(TAG_TO "/" TAG_PLAYER));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CPlayer player;
			pNode = pNodeList->Getitem(i);
			player.parseXML(pNode);
			m_aPlayerTo.Add(player);
		}
	}
}

void CMessage::toXML(IXMLDOMDocumentPtr pDoc,IXMLDOMElementPtr pParent)
{
	IXMLDOMElementPtr pElemMes,pElemTo,pElemFrom,pElemText;
	int i;

	pElemMes = pDoc->createElement(_T(TAG_MESSAGE));

	pElemTo = pDoc->createElement(_T(TAG_TO));

	for(i=0;i<m_aPlayerTo.GetSize();i++){
		m_aPlayerTo[i].toXML(pDoc,pElemTo);
	}

	pElemMes->appendChild(pElemTo);


	pElemFrom = pDoc->createElement(_T(TAG_FROM));
	m_playerFrom.toXML(pDoc,pElemFrom);
	pElemMes->appendChild(pElemFrom);

	pElemText = pDoc->createElement(_T(TAG_TEXT));
	pElemText->appendChild(pDoc->createTextNode((LPCTSTR)m_strText));
	pElemMes->appendChild(pElemText);

	pParent->appendChild(pElemMes);

}

CMessage& CMessage::operator =(CMessage& value)
{
	m_aPlayerTo.Copy(value.m_aPlayerTo);

	m_playerFrom = value.m_playerFrom;

	m_strText = value.m_strText;

	return *this;
}


