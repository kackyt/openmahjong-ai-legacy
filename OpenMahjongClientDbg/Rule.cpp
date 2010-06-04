/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
 * All rights reserved.

  �\�[�X�R�[�h�`�����o�C�i���`�����A�ύX���邩���Ȃ������킸�A�ȉ��̏����𖞂����ꍇ�Ɍ���A�ĔЕz����юg�p��������܂��B 

  �E�\�[�X�R�[�h���ĔЕz����ꍇ�A��L�̒��쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 

  �E�o�C�i���`���ōĔЕz����ꍇ�A�Еz���ɕt���̃h�L�������g���̎����ɁA��L�̒��쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 

  �E���ʂɂ����ʂ̋��Ȃ��ɁA�{�\�t�g�E�F�A����h���������i�̐�`�܂��͔̔����i�ɁA�I�[�v�������̖��O�܂��̓R���g���r���[�^�[�̖��O���g�p���Ă͂Ȃ�Ȃ��B


  �{�\�t�g�E�F�A�́A���쌠�҂���уR���g���r���[�^�[�ɂ���āu����̂܂܁v�񋟂���Ă���A�����َ����킸�A
  ���ƓI�Ȏg�p�\���A����ѓ���̖ړI�ɑ΂���K�����Ɋւ���Öق̕ۏ؂��܂߁A�܂�����Ɍ��肳��Ȃ��A�����Ȃ�ۏ؂�����܂���B
  ���쌠�҂��R���g���r���[�^�[���A���R�̂�������킸�A ���Q�����̌�����������킸�A���ӔC�̍������_��ł��邩���i�ӔC�ł��邩
  �i�ߎ����̑��́j�s�@�s�ׂł��邩���킸�A���ɂ��̂悤�ȑ��Q����������\����m�炳��Ă����Ƃ��Ă��A�{�\�t�g�E�F�A�̎g�p�ɂ���Ĕ�������
  �i��֕i�܂��͑�p�T�[�r�X�̒��B�A�g�p�̑r���A�f�[�^�̑r���A���v�̑r���A�Ɩ��̒��f���܂߁A�܂�����Ɍ��肳��Ȃ��j
  ���ڑ��Q�A�Ԑڑ��Q�A�����I�ȑ��Q�A���ʑ��Q�A�����I���Q�A�܂��͌��ʑ��Q�ɂ��āA��ؐӔC�𕉂�Ȃ����̂Ƃ��܂��B 

****************************************************************************************/
// Rule.cpp: CRule �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Rule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CRule::CRule()
{
	m_bActive = FALSE;
	m_iGlasshai = 0;
	m_iKuitan = 1;
	m_iAka = 0;
	m_iAotenjo = 0;
	m_iHaipai = 0;
}

CRule::~CRule()
{

}

void CRule::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	CString text;
	BSTR pStr;

	m_bActive = TRUE;

	pNode = pElem->selectSingleNode(_T(TAG_GLASSHAI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iGlasshai = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_KUITAN));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iKuitan = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_AKA));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iAka = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_AOTENJO));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iAotenjo = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_HAIPAI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iHaipai = _tcstol((const TCHAR*)text,NULL,0);
	}
}

void CRule::toXML(IXMLDOMDocumentPtr pDoc,IXMLDOMElementPtr pParent)
{
	IXMLDOMElementPtr pElemKey,pElemRule;
	CString str;

	pElemRule = pDoc->createElement(_T(TAG_RULE));

	pElemKey = pDoc->createElement(_T(TAG_GLASSHAI));
	str.Format(_T("%d"),m_iGlasshai);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pElemKey = pDoc->createElement(_T(TAG_KUITAN));
	str.Format(_T("%d"),m_iKuitan);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pElemKey = pDoc->createElement(_T(TAG_AKA));
	str.Format(_T("%d"),m_iAka);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pElemKey = pDoc->createElement(_T(TAG_AOTENJO));
	str.Format(_T("%d"),m_iAotenjo);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pElemKey = pDoc->createElement(_T(TAG_HAIPAI));
	str.Format(_T("%d"),m_iHaipai);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pParent->appendChild(pElemRule);

}