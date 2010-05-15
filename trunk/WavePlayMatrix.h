#pragma once
#include "waveplaymethod.h"

class CWavePlayMatrixHook;

class CWavePlayMatrix :
	public CWavePlayMethod
{
public:
	typedef int WPMHANDLE;
	typedef unsigned int WPMCHMAP;
	typedef struct {
		CWavePlayMethod *pMethod;
		WPMCHMAP        uMap;
		double          volume;
		int             channels;
	} CWavePlayMatrixParam;
	CWavePlayMatrix(void);
public:
	~CWavePlayMatrix(void);
private:
	CMap<WPMHANDLE,WPMHANDLE,CWavePlayMatrixParam,CWavePlayMatrixParam&> m_matrix;
	WPMHANDLE m_handleCounter;
public:
	WPMHANDLE appendChannel(CWavePlayMethod * pMethod, WPMCHMAP uMap, double volume = 1.0);
	BOOL removeChannel(WPMHANDLE handle);
public:
	sint16 * getNextWaveFix16(int length);
public:
	f64 * getNextWaveFloat64(int length);
public:
	void setMaxChannels(int channels);
public:
	int m_channels;
public:
	f64 *m_pWaveBuf;
public:
	CWavePlayMethod * getPlayData(WPMHANDLE handle);
public:
	int getPosition(CWavePlayMethod * pMethod);
public:
	int getUpperBoundChannel(void);
public:
	void setVolume(WPMHANDLE handle, double newVolume);
public:
	void setChannelMap(WPMHANDLE handle, WPMCHMAP uNewMap);
public:
	BOOL getChannelMap(WPMHANDLE handle,WPMCHMAP &uMap);
public:
	BOOL getVolume(WPMHANDLE handle, double & volume);
public:
	CWavePlayMatrixHook *m_pHook;
public:
	void setHook(CWavePlayMatrixHook * pHook);
};

class CWavePlayMatrixHook
{
public:
	CWavePlayMatrixHook(void);
public:
	~CWavePlayMatrixHook(void);
public:
	virtual void matrixStart(CWavePlayMatrix * pMatrix) = 0;
public:
	virtual void matrixEnd(CWavePlayMatrix * pMatrix,f64 *pWav,int length) = 0;
};
