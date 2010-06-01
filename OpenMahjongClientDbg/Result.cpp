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
// Result.cpp: CResult クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Result.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CResult::CResult()
{
	m_bActive = FALSE;
}

CResult::~CResult()
{

}

void CResult::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	m_bActive = TRUE;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_DORAKAZU));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iDorakazu = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_HAN));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iHan = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_FU));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iFu = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_MACHI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iMachi = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_MANGAN));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iMangan = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_SCORE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iScore = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_OYASCORE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iOyaScore = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_KOSCORE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iKoScore = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_TSUMO));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bTsumo = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_MACHIHAI "/" TAG_PAI));

	if(pNode != NULL){
		m_machihai.parseXML(pNode);
	}

	/* 役名リストを格納 */
	m_aYaku.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_YAKU));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			pNode = pNodeList->Getitem(i);
			pNode->get_text(&pStr);
			text = pStr;
			m_aYaku.Add(text);
		}
	}


}

CResult& CResult::operator=(CResult& value)
{
	m_iDorakazu = value.m_iDorakazu;
	m_iFu = value.m_iFu;
	m_bActive = value.m_bActive;
	m_iHan = value.m_iHan;
	m_iMachi = value.m_iMachi;
	m_iMangan = value.m_iMangan;
	m_iScore = value.m_iScore;
	m_iOyaScore = value.m_iOyaScore;
	m_iKoScore = value.m_iKoScore;
	m_bTsumo = value.m_bTsumo;
	m_machihai = value.m_machihai;
	m_aYaku.Copy(value.m_aYaku);

	return *this;
}
