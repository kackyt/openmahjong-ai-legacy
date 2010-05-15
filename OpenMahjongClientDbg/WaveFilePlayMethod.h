#pragma once
#include "WavePlayer.h"
#include "waveplaymethod.h"

class CWaveFilePlayMethod :
	public CWavePlayMethod
{
public:
	CWaveFilePlayMethod(void);
public:
	~CWaveFilePlayMethod(void);
public:
	sint16 * getNextWaveFix16(int length);
public:
	double * getNextWaveFloat64(int length);
	void setPlayer(CWavePlayer * pPlayer);
	void closeWaveFile(void);
	// wavファイルを開く
	BOOL openWaveFile(LPCTSTR strFilePath);
private:
	CWavePlayer *m_pPlayer;
	WAVEFORMATEX m_wfxWaveForm;
	HMMIO m_hMio;
private:
	sint16 *m_pBufFix;
	f64 *m_pBufFloat;
private:
	int m_nDataSize;
private:
	CMutex m_mutex;
	BOOL m_loop;
public:
	// ループの設定をする
	void setLoop(bool loop);
public:
	void getPlayFormat(WAVEFORMATEX * pFormat);
public:
	int getWaveSamples(void);
};
