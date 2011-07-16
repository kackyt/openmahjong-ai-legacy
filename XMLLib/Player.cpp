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
// Player.cpp: CPlayer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

OMPlayer::OMPlayer()
{
	m_iId = 0;
	m_bIsComp = FALSE;
	m_pFunc = NULL;
	m_pInst = NULL;
	m_strName = _T("");
	m_iPrivateId = 0;

}

OMPlayer::~OMPlayer()
{

}

void OMPlayer::parseXML(QDomNode pElem)
{
        QDomNode pNode;
        BSTR pStr;

        pNode = OM_GETELEMENT(pElem,_T(TAG_ID));

        OM_TOLONG(pNode,m_iId);

        pNode = OM_GETELEMENT(pElem,_T(TAG_NAME));

        if(!OM_ISNULL(pNode)){
                OM_GETTEXT(pNode,pStr);
                m_strName = QString(pStr);
        }

        pNode = OM_GETELEMENT(pElem,_T(TAG_PRIVATEID));
        OM_TOLONG(pNode,m_iPrivateId);
}

void OMPlayer::toXML(QDomDocument pDoc,QDomElement pParent)
{
        QDomElement pElemPlayer,pElemID,pElemName;
        QString str;

        pElemPlayer = OM_EVAL(pDoc,createElement(_T(TAG_PLAYER)));

        pElemID = OM_EVAL(pDoc,createElement(_T(TAG_ID)));
	str.Format(_T("%d"),m_iId);
        OM_EVAL(pElemID,appendChild(OM_CREATETEXT(pDoc,str)));
        OM_EVAL(pElemPlayer,appendChild(pElemID));

        pElemName = OM_EVAL(pDoc,createElement(_T(TAG_NAME)));
        OM_EVAL(pElemName,appendChild(OM_CREATETEXT(pDoc,m_strName)));
        OM_EVAL(pElemPlayer,appendChild(pElemName));

        pElemID = OM_EVAL(pDoc,createElement(_T(TAG_PRIVATEID)));
	str.Format(_T("%d"),m_iPrivateId);
        OM_EVAL(pElemID,appendChild(OM_CREATETEXT(pDoc,str)));
        OM_EVAL(pElemPlayer,appendChild(pElemID));

        OM_EVAL(pParent,appendChild(pElemPlayer));

}
