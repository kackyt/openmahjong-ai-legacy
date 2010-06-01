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
// Member.cpp: CMember クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Member.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CMember::CMember()
{

}

CMember::~CMember()
{

}

void CMember::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_MSTATE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iState = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_POINT));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iPoint = _tcstol((const TCHAR*)text,NULL,0);
	}

	/* 打牌の格納 */
	m_aDahai.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_DAHAI "/ "  TAG_PAI));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CPai pai;
			pNode = pNodeList->Getitem(i);
			pai.parseXML(pNode);
			m_aDahai.Add(pai);
		}
	}

	/* 手牌の格納 */
	m_aTehai.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_TEHAI "/" TAG_PAI));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CPai pai;
			pNode = pNodeList->Getitem(i);
			pai.parseXML(pNode);
			m_aTehai.Add(pai);
		}
	}

	/* コマンドリストの格納 */
	m_aCommandList.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_COMMANDLIST "/" TAG_COMMAND));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CCommand com;
			pNode = pNodeList->Getitem(i);
			com.parseXML(pNode);
			m_aCommandList.Add(com);
		}
	}

	/* ツモ牌の格納 */
	pNode = pElem->selectSingleNode(_T(TAG_TSUMOHAI));

	if(pNode != NULL){
		m_gamestate.m_bTsumo = TRUE;
		pNode = pElem->selectSingleNode(_T(TAG_TSUMOHAI "/" TAG_PAI));
		if(pNode != NULL){
			m_tsumohai.parseXML(pNode);
		}
	}else{
		m_gamestate.m_bTsumo = FALSE;
	}


	/* プレーヤーの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_PLAYER));

	if(pNode != NULL){
		m_player.parseXML(pNode);
	}

	/* ゲーム状態の格納 */
	pNode = pElem->selectSingleNode(_T(TAG_GAMESTATE));

	if(pNode != NULL){
		m_gamestate.parseXML(pNode);
	}

	/* あがりの格納 */
	m_aResultList.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_RESULTLIST "/" TAG_RESULT));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CResult res;
			pNode = pNodeList->Getitem(i);
			res.parseXML(pNode);
			m_aResultList.Add(res);
		}
	}

}

BOOL CMember::isExecutableCommand(CCommand& command)
{
	int i;

	for(i=0;i<m_aCommandList.GetSize();i++){
		if(m_aCommandList[i].m_iId == command.m_iId){
			return TRUE;
		}
	}

	return FALSE;
}

CMember& CMember::operator =(CMember& value)
{
	int i;
	m_aCommandList.RemoveAll();
	for(i=0;i<value.m_aCommandList.GetSize();i++){
		m_aCommandList.Add(value.m_aCommandList[i]);
	}
	m_aDahai.Copy(value.m_aDahai);
	m_aResultList.RemoveAll();
	for(i=0;i<value.m_aResultList.GetSize();i++){
		m_aResultList.Add(value.m_aResultList[i]);
	}
	m_aTehai.Copy(value.m_aTehai);
	m_gamestate = value.m_gamestate;
	m_iPoint = value.m_iPoint;
	m_iState = value.m_iState;
	m_player = value.m_player;

	return *this;
}
