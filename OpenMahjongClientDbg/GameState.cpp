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
// GameState.cpp: CGameState クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "GameState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CGameState::CGameState()
{
	m_iZikaze = 1;
	m_iCount = 1;
	m_bTsumo = FALSE;
	m_bRiichi = FALSE;
	m_bOya = FALSE;
	m_bIppatsu = FALSE;
	m_bNaki = FALSE;
}

CGameState::~CGameState()
{

}

void CGameState::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_ZIKAZE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iZikaze = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_COUNT));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iCount = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_OYA));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bOya = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_NAKI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bNaki = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_RIICHI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bRiichi = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_IPPATSU));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bIppatsu = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_TSUMO));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bTsumo = text == _T("true") ? TRUE : FALSE;
	}

	/* 鳴き面子の格納 */
	m_aNakiList.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_NAKILIST "/" TAG_NAKIMENTSU));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CNakiMentsu mentsu;
			pNode = pNodeList->Getitem(i);
			mentsu.parseXML(pNode);
			m_aNakiList.Add(mentsu);
		}
	}

}

CGameState& CGameState::operator =(CGameState& value)
{
	int i;
	m_aNakiList.RemoveAll();
	for(i=0;i<value.m_aNakiList.GetSize();i++){
		m_aNakiList.Add(value.m_aNakiList[i]);
	}
	m_bIppatsu = value.m_bIppatsu;
	m_bOya = value.m_bOya;
	m_bRiichi = value.m_bRiichi;
	m_bTsumo = value.m_bTsumo;
	m_iZikaze = value.m_iZikaze;

	return *this;
}
