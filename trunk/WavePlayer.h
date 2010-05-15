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
