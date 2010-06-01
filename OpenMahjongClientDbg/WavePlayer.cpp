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
#include "StdAfx.h"
#include "WavePlayer.h"

CWavePlayer::CWavePlayer()
: m_lPrepPage(0)
, m_hHeap(NULL)
, m_hWaveout(NULL)
{
}

CWavePlayer::CWavePlayer(CWavePlayMethod *pPlayMethod,int uBufNum,int uBufSize)
: m_lPrepPage(0)
, m_hHeap(NULL)
, m_hWaveout(NULL)
{
	create(uBufNum,uBufSize);
	setMethod(pPlayMethod);
}

CWavePlayer::~CWavePlayer(void)
{
	destroy();
}

sint16 * CWavePlayer::getNextWaveFix16(int length){
	return m_pMethod->getNextWaveFix16(length);
}

double * CWavePlayer::getNextWaveFloat64(int length){
	return m_pMethod->getNextWaveFloat64(length);
}



// WaveOut デバイスを開く
BOOL CWavePlayer::openDevice(UINT uDevID)
{
	if (m_hWaveout)
	{
		return FALSE;
	}
	MMRESULT ret = waveOutOpen(
		&m_hWaveout,
		uDevID,
		&m_wfxWaveForm,
		(DWORD)waveOutCallback,
		(DWORD)this,
		CALLBACK_FUNCTION);

	if(ret != MMSYSERR_NOERROR){
		return FALSE;
	}
	return TRUE;
}

// 再生を開始する
BOOL CWavePlayer::play(void)
{
	int i;
	MMRESULT ret;

	for(i=0;i<m_uBufNum;i++){
		LPWAVEHDR pHeader = &m_pWaveHdr[i];
		pHeader->lpData = (LPSTR)m_ppWaveBuf[i];
		pHeader->dwBufferLength = m_uBufSize;
		pHeader->dwUser = 0;
		pHeader->dwFlags = 0;
		pHeader->dwLoops = 0;

		setWaveData(pHeader);

		ret = waveOutPrepareHeader(m_hWaveout,pHeader,sizeof(WAVEHDR));

		if(ret != MMSYSERR_NOERROR){
			break;
		}

		ret = waveOutWrite(m_hWaveout,pHeader,sizeof(WAVEHDR));
		++m_lPrepPage;

		if(ret != MMSYSERR_NOERROR){
			break;
		}

	}

	if(ret == MMSYSERR_NOERROR) m_evCallback.SetEvent();
	return (ret == MMSYSERR_NOERROR) ? TRUE : FALSE;
}

// 再生時のPCMフォーマットを設定する
BOOL CWavePlayer::setPlayFormat(WAVEFORMATEX * pFormat)
{
	if(m_hWaveout){
		return FALSE;
	}

	m_wfxWaveForm = *pFormat;

	return TRUE;
}


void CWavePlayer::setWaveData(LPWAVEHDR pHeader)
{
	sint16 *pWaveFix16;
	double *pWaveFloat64;
	int i;

	pWaveFix16 = getNextWaveFix16(m_uBufSize>>1);
	if(pWaveFix16 != NULL){
		memcpy(pHeader->lpData,pWaveFix16,m_uBufSize);
	}else{
		pWaveFloat64 = getNextWaveFloat64(m_uBufSize>>1);
		pWaveFix16 = (sint16 *)pHeader->lpData;
		if(pWaveFloat64 != NULL){
			for(i=0;i<m_uBufSize>>1;i++){
				// クリップ処理
				if(pWaveFloat64[i] > 1.0){
					pWaveFix16[i] = 32767;
				}else if(pWaveFloat64[i] < -1.0){
					pWaveFix16[i] = -32768;
				}else{
					pWaveFix16[i] = (sint16)(pWaveFloat64[i] * 32767.0); // 16bit固定小数点にキャスト
				}
			}
		}else{
			ZeroMemory(pWaveFix16,m_uBufSize);
		}
	}
}


// コールバック関数
static void CALLBACK waveOutCallback(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	MMRESULT ret;
	CWavePlayer *pInst = (CWavePlayer *)dwInstance;
	LPWAVEHDR pHdr =(LPWAVEHDR)dwParam1;

	pInst->m_evCallback.ResetEvent();

	switch(uMsg){
		case MM_WOM_OPEN:
			break;
		case MM_WOM_CLOSE:
			break;
		case MM_WOM_DONE:
			if(pInst->m_lPrepPage == 0) break;
			if (!(pHdr->dwFlags & WHDR_DONE)) break;			// まだ演奏中である
			if (!(pHdr->dwFlags & WHDR_PREPARED)) break;		// 準備されたバッファではない


			ret = waveOutUnprepareHeader(hwo,pHdr,sizeof(WAVEHDR));
			if(ret != MMSYSERR_NOERROR) break;
			pInst->m_lPrepPage--;
			pInst->setWaveData(pHdr);

			pHdr->dwFlags = 0;

			ret = waveOutPrepareHeader(hwo,pHdr,sizeof(WAVEHDR));

			if(ret != MMSYSERR_NOERROR) break;

			ret = waveOutWrite(hwo,pHdr,sizeof(WAVEHDR));

			if(ret != MMSYSERR_NOERROR) break;
			pInst->m_lPrepPage++;
			break;
		default:
			break;
	}

	pInst->m_evCallback.SetEvent();
}

BOOL CWavePlayer::closeDevice(void)
{
	if (! m_hWaveout)		return TRUE;

	MMRESULT ret = waveOutClose(m_hWaveout);
	m_hWaveout = NULL;

	if (ret != MMSYSERR_NOERROR)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CWavePlayer::stop(void)
{
	if (! m_hWaveout)		return TRUE;
	// コールバック関数の実行終了を待機する
	WaitForSingleObject(m_evCallback.m_hObject,INFINITE);
	m_lPrepPage = 0;

	MMRESULT ret = waveOutReset(m_hWaveout);

	if (ret != MMSYSERR_NOERROR)
	{
		return FALSE;
	}

	for (int i=0; i<m_uBufNum; i++)
	{
		LPWAVEHDR pHeader = &m_pWaveHdr[i];

		if (! (pHeader->dwFlags & WHDR_PREPARED))	continue;
		if (! (pHeader->dwFlags & WHDR_DONE))		continue;

		ret = waveOutUnprepareHeader(m_hWaveout, pHeader, sizeof(WAVEHDR));

		if (ret != MMSYSERR_NOERROR)
		{
			return FALSE;
		}
	}

	return ret == MMSYSERR_NOERROR ? TRUE:FALSE;
}

int CWavePlayer::getBufferSize(void)
{
	return m_uBufSize;
}

long CWavePlayer::getReadyBlocks(void)
{
	int i;
	long count = 0;
	LPWAVEHDR pHdr = m_pWaveHdr;

	for(i=0;i<m_uBufNum;i++){
		if(!(pHdr->dwFlags & WHDR_DONE)) count++;
		pHdr++;
	}
	return count;
}

int CWavePlayer::getPlayedSamples(void)
{
	MMTIME mmt;
	mmt.wType = TIME_SAMPLES;

	waveOutGetPosition(m_hWaveout,&mmt,sizeof(MMTIME));
	return mmt.u.sample;
}

void CWavePlayer::create(int uBufNum, int uBufSize)
{
	int i;
	m_hWaveout = NULL;
	m_uBufNum = uBufNum;
	m_uBufSize = uBufSize;
	m_hHeap = HeapCreate(0,0,0);
	m_pWaveHdr = (LPWAVEHDR)HeapAlloc(m_hHeap,HEAP_ZERO_MEMORY,sizeof(WAVEHDR)*uBufNum);
	m_ppWaveBuf = (void **)HeapAlloc(m_hHeap,HEAP_ZERO_MEMORY,sizeof(void *)*uBufNum);
	for(i=0;i<uBufNum;i++){
		m_ppWaveBuf[i] = HeapAlloc(m_hHeap,HEAP_ZERO_MEMORY,uBufSize);
	}

	m_wfxWaveForm.wFormatTag = WAVE_FORMAT_PCM;
	m_wfxWaveForm.nChannels = 1;
	m_wfxWaveForm.nSamplesPerSec = 44100;
	m_wfxWaveForm.wBitsPerSample = 16;
	m_wfxWaveForm.cbSize = 0;
	m_wfxWaveForm.nBlockAlign = m_wfxWaveForm.wBitsPerSample * m_wfxWaveForm.nChannels / 8;
	m_wfxWaveForm.nAvgBytesPerSec = m_wfxWaveForm.nSamplesPerSec * m_wfxWaveForm.nBlockAlign;
}

void CWavePlayer::setMethod(CWavePlayMethod * pPlayMethod)
{
	m_pMethod = pPlayMethod;
}

void CWavePlayer::destroy(void)
{
	if(m_hHeap) HeapDestroy(m_hHeap);
	m_hHeap = NULL;
}

int CWavePlayer::getBufferedSamples(void)
{
	return (getReadyBlocks()*getBufferSize() - (getPlayedSamples() % getBufferSize())) / sizeof(sint16);
}


void CWavePlayer::getPlayFormat(WAVEFORMATEX * pFormat)
{
	*pFormat = m_wfxWaveForm;
}
