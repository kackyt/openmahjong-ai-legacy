#include "StdAfx.h"
#include "WavePlayMatrix.h"

CWavePlayMatrixHook::CWavePlayMatrixHook(void)
{
}

CWavePlayMatrixHook::~CWavePlayMatrixHook(void)
{
}


CWavePlayMatrix::CWavePlayMatrix(void)
: m_channels(0)
, m_pWaveBuf(NULL)
, m_pHook(NULL)
{
	m_handleCounter = 1;

	m_matrix.InitHashTable(20); // テキトーにハッシュテーブルを初期化する
}

CWavePlayMatrix::~CWavePlayMatrix(void)
{
	if(m_pWaveBuf) delete[] m_pWaveBuf;
}

CWavePlayMatrix::WPMHANDLE CWavePlayMatrix::appendChannel(CWavePlayMethod * pMethod, WPMCHMAP uMap, double volume)
{
	CWavePlayMatrixParam param;
	int i;

	param.pMethod = pMethod;
	param.uMap = uMap;
	param.volume = volume;
	param.channels = 0;

	for(i=0;i<32;i++){
		if(param.uMap & (1 << i)){
			param.channels++;
		}
	}

	m_matrix.SetAt(m_handleCounter,param);

	return m_handleCounter++;
}

BOOL CWavePlayMatrix::removeChannel(WPMHANDLE handle)
{
	return m_matrix.RemoveKey(handle);
}

sint16 * CWavePlayMatrix::getNextWaveFix16(int length)
{
	// 16bit 固定小数点はサポート外
	return NULL;
}

f64 * CWavePlayMatrix::getNextWaveFloat64(int length)
{
	POSITION pos;
	WPMHANDLE handle;
	CWavePlayMatrixParam param;
	f64 *pBuf;
	int mCh,ch,i,samples;

	if(m_channels == 0) return NULL;

	if(!m_pWaveBuf){
		m_pWaveBuf = new f64[length];
	}

	samples = length / m_channels;

	/* Mapの中を全部なめる */
	pos = m_matrix.GetStartPosition();

	ZeroMemory(m_pWaveBuf,sizeof(f64)*length);

	if(m_pHook){
		/* フックルーチンを呼び出す */
		m_pHook->matrixStart(this);
	}

	while(pos != NULL){
		m_matrix.GetNextAssoc(pos,handle,param);
		pBuf = param.pMethod->getNextWaveFloat64(param.channels*samples);
		if(pBuf != NULL){
			ch = 0;
			for(mCh=0;mCh<m_channels;mCh++){
				if(param.uMap & (1 << mCh)){
					for(i=0;i<samples;i++){
						m_pWaveBuf[i*m_channels + mCh] += pBuf[i*param.channels + ch] * param.volume;
					}
					ch++;
				}
			}
		}
	}

	if(m_pHook){
		/* フックルーチンを呼び出す */
		m_pHook->matrixEnd(this,m_pWaveBuf,length);
	}

	return m_pWaveBuf;
}

void CWavePlayMatrix::setMaxChannels(int channels)
{
	f64 * pBuf = m_pWaveBuf;
	m_channels = channels;
	if(pBuf){
		m_pWaveBuf = NULL;
		delete[] pBuf;
	}
}

CWavePlayMethod * CWavePlayMatrix::getPlayData(WPMHANDLE handle)
{
	CWavePlayMatrixParam methodParam;
	BOOL success;

	success = m_matrix.Lookup(handle,methodParam);

	return success ? methodParam.pMethod : NULL; 
}

int CWavePlayMatrix::getPosition(CWavePlayMethod * pMethod)
{
	POSITION pos;
	WPMHANDLE handle;
	CWavePlayMatrixParam param;
	int ret = 0;
	/* Mapの中を全部なめる */
	pos = m_matrix.GetStartPosition();

	while(pos != NULL){
		m_matrix.GetNextAssoc(pos,handle,param);
		if(param.pMethod == pMethod){
			return ret;
		}
		ret++;		
	}
	return -1;
}

int CWavePlayMatrix::getUpperBoundChannel(void)
{
	POSITION pos;
	WPMHANDLE handle;
	CWavePlayMatrixParam param;
	int max = -1,mCh;

	/* Mapの中を全部なめて、一番上のチャンネル番号を返す */
	pos = m_matrix.GetStartPosition();

	while(pos != NULL){
		m_matrix.GetNextAssoc(pos,handle,param);
		for(mCh=max;mCh<32;mCh++){
			if(param.uMap & (1 << mCh)){
				max = mCh;
			}
		}
	}

	return max;
}

void CWavePlayMatrix::setVolume(WPMHANDLE handle, double volume)
{
	CWavePlayMatrixParam methodParam;
	BOOL success;

	success = m_matrix.Lookup(handle,methodParam);

	if(success){
		methodParam.volume = volume;
		m_matrix.SetAt(handle,methodParam);
	}
}

void CWavePlayMatrix::setChannelMap(WPMHANDLE handle, WPMCHMAP uNewMap)
{
	CWavePlayMatrixParam methodParam;
	BOOL success;

	success = m_matrix.Lookup(handle,methodParam);

	if(success){
		methodParam.uMap = uNewMap;
		m_matrix.SetAt(handle,methodParam);
	}
}

BOOL CWavePlayMatrix::getChannelMap(WPMHANDLE handle,WPMCHMAP &uMap)
{
	CWavePlayMatrixParam methodParam;
	BOOL success;

	success = m_matrix.Lookup(handle,methodParam);

	if(success){
		uMap = methodParam.uMap;
	}

	return success;
}

BOOL CWavePlayMatrix::getVolume(WPMHANDLE handle, double & volume)
{
	CWavePlayMatrixParam methodParam;
	BOOL success;

	success = m_matrix.Lookup(handle,methodParam);

	if(success){
		volume = methodParam.volume;
	}

	return success;
}

void CWavePlayMatrix::setHook(CWavePlayMatrixHook * pHook)
{
	m_pHook = pHook;
}
