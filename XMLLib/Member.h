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
// Member.h: CMember �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBER_H__AEABC3C8_3C44_48CF_92D2_AC795B8A7D0F__INCLUDED_)
#define AFX_MEMBER_H__AEABC3C8_3C44_48CF_92D2_AC795B8A7D0F__INCLUDED_

#include "OMXMLCommon.h"
#include "Player.h"	// ClassView �ɂ���Ēǉ�����܂����B
#include "Pai.h"
#include "Command.h"
#include "GameState.h"	// ClassView �ɂ���Ēǉ�����܂����B
#include "Result.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OMMember
{
public:
        OMPai m_tsumohai;
        OMMember& operator = (OMMember&);
        BOOL isExecutableCommand(OMCommand&);
        OMGameState m_gamestate;
        void parseXML(QDomNode);
        OM_DEFARRAY(OMResult,m_aResultList);
        OM_DEFARRAY(OMCommand,m_aCommandList);
        OM_DEFARRAY(OMPai,m_aTehai);
        OM_DEFARRAY(OMPai,m_aDahai);
	int m_iState;
	int m_iPoint;
        OMPlayer m_player;
        OMMember();
        virtual ~OMMember();

};

#endif // !defined(AFX_MEMBER_H__AEABC3C8_3C44_48CF_92D2_AC795B8A7D0F__INCLUDED_)