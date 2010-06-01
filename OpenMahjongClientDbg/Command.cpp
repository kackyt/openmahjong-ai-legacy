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
// Command.cpp: CCommand クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Command.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CCommand::CCommand()
{
	m_iId = 0;
	m_iType = 0;

}

CCommand::~CCommand()
{

}

void CCommand::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	CString text;
	BSTR pStr;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_ID));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iId = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_TYPE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iType = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_PAI));

	if(pNode != NULL){
		m_pai.parseXML(pNode);
	}

	pNode = pElem->selectSingleNode(_T(TAG_PLAYER));

	if(pNode != NULL){
		m_player.parseXML(pNode);
	}

	pNode = pElem->selectSingleNode(_T(TAG_NAKIMENTSU));

	if(pNode != NULL){
		m_mentsu.parseXML(pNode);
	}else{
		pNode= pElem->selectSingleNode(_T(TAG_MENTSU));

		if(pNode != NULL){
			m_mentsu.parseXML(pNode);
		}
	}

	pNode = pElem->selectSingleNode(_T(TAG_RULE));

	if(pNode != NULL){
		m_rule.parseXML(pNode);
	}

}

CCommand& CCommand::operator=(CCommand& value)
{
	m_iId = value.m_iId;
	m_iType = value.m_iType;
	m_mentsu = value.m_mentsu;
	m_pai = value.m_pai;
	m_player = value.m_player;

	return *this;
}

void CCommand::toXML(IXMLDOMDocumentPtr pDoc,IXMLDOMElementPtr pParent)
{
	IXMLDOMElementPtr pElemCom,pElemID;
	CString str;

	pElemCom = pDoc->createElement(_T(TAG_COMMAND));
	pParent->appendChild(pElemCom);
	pElemID = pDoc->createElement(_T(TAG_ID));
	pElemCom->appendChild(pElemID);

	str.Format(_T("%d"),m_iId);

	pElemID->appendChild(pDoc->createTextNode((LPCTSTR)str));

	m_player.toXML(pDoc,pElemCom);

	if(m_rule.m_bActive){
		m_rule.toXML(pDoc,pElemCom);
	}
}
