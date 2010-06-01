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
#pragma once
#include "afxmt.h"
#include "waveplaymethod.h"

class CWavePlayer :
	public CWavePlayMethod
{
	friend void CALLBACK waveOutCallback(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);
private:
	CWavePlayMethod *m_pMethod;
public:
	CWavePlayer(void);
	CWavePlayer(CWavePlayMethod *pPlayMethod,int uBufNum,int uBufSize);
public:
	~CWavePlayer(void);
public:
	// WaveOut デバイスを開く
	BOOL openDevice(UINT uDevID);

private:
	HANDLE m_hHeap;
private:
	int m_uBufSize;
private:
	int m_uBufNum;
private:
	LPWAVEHDR m_pWaveHdr;
private:
	void **m_ppWaveBuf;
private:
	HWAVEOUT m_hWaveout;
private:
	WAVEFORMATEX m_wfxWaveForm;
public:
	// 再生を開始する
	BOOL play(void);
public:
	sint16 * CWavePlayer::getNextWaveFix16(int length);
	double * CWavePlayer::getNextWaveFloat64(int length);
	// 再生時のPCMフォーマットを設定する
	BOOL setPlayFormat(WAVEFORMATEX * pFormat);
	void setWaveData(LPWAVEHDR pHeader);
public:
	BOOL closeDevice(void);
public:
	BOOL stop(void);

private:
	long m_lPrepPage;
public:
	int getBufferSize(void);
public:
	long getReadyBlocks(void);
public:
	int getPlayedSamples(void);
public:
	void create(int uBufNum, int uBufSize);
public:
	void setMethod(CWavePlayMethod * pPlayMethod);
public:
	void destroy(void);
public:
	int getBufferedSamples(void);
public:
	void getPlayFormat(WAVEFORMATEX * pFormat);
	CEvent m_evCallback;
};
