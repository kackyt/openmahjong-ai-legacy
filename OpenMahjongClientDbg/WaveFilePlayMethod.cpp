#include "StdAfx.h"
#include "WaveFilePlayMethod.h"

CWaveFilePlayMethod::CWaveFilePlayMethod(void)
: m_pPlayer(NULL)
, m_hMio(NULL)
, m_pBufFix(NULL)
, m_pBufFloat(NULL)
, m_nDataSize(0)
, m_loop(FALSE)
{
}

CWaveFilePlayMethod::~CWaveFilePlayMethod(void)
{
	if(m_pBufFloat) delete[] m_pBufFloat;
	if(m_pBufFix) delete[] m_pBufFix;
}

sint16 * CWaveFilePlayMethod::getNextWaveFix16(int length)
{
	LONG res,size;

	size = sizeof(sint16)*length;
	if(m_pBufFix == NULL){
		m_pBufFix = new sint16[length];
	}
	ZeroMemory(m_pBufFix,size);

	m_mutex.Lock();
	res = mmioRead(m_hMio,(HPSTR)m_pBufFix,size);

	if(res < size && m_loop){
		size -= res;
		mmioSeek(m_hMio,m_nDataSize,SEEK_END);   // データの先頭に戻る
		mmioRead(m_hMio,(HPSTR)&m_pBufFix[res>>1],size); // 残りをリード
	}

	m_mutex.Unlock();

	return m_pBufFix;
}

double * CWaveFilePlayMethod::getNextWaveFloat64(int length)
{
	int i;
	LONG res,size;

	size = sizeof(sint16)*length;
	if(m_pBufFix == NULL){
		m_pBufFix = new sint16[length];
	}
	if(m_pBufFloat == NULL){
		m_pBufFloat = new f64[length];
	}

	ZeroMemory(m_pBufFix,sizeof(sint16)*length);
	ZeroMemory(m_pBufFloat,sizeof(f64)*length);

	m_mutex.Lock();
	res = mmioRead(m_hMio,(HPSTR)m_pBufFix,sizeof(sint16)*length);
	if(res < size && m_loop){
		size -= res;
		mmioSeek(m_hMio,m_nDataSize,SEEK_END);   // データの先頭に戻る
		mmioRead(m_hMio,(HPSTR)&m_pBufFix[res>>1],size); // 残りをリード
	}
	m_mutex.Unlock();

	for(i=0;i<length;i++){
		m_pBufFloat[i] = (double)m_pBufFix[i] / 32768.0; // 浮動小数点に変換
	}

	return m_pBufFloat;
}

// wavファイルを開く
BOOL CWaveFilePlayMethod::openWaveFile(LPCTSTR strFilePath)
{
	MMIOINFO	ioInfo;
	MMCKINFO	ciRiffChunk;
	MMCKINFO	ciSubChunk;
	MMRESULT ret = MMSYSERR_NOERROR;
	long nFmtSize;
	long nReadSize;

	m_mutex.Lock();
	if (m_hMio)
	{
		ret =  MMIOERR_CANNOTOPEN;
		goto excp;
	}

	//m_nDataSize = 0;

	// FilePath Check
	if (! _tcslen(strFilePath))
	{
		ret =  MMIOERR_FILENOTFOUND;
		goto excp;
	}

	// Open File
	ZeroMemory(&ioInfo, sizeof(MMIOINFO));

	m_hMio = mmioOpen((LPTSTR)(LPCTSTR)strFilePath, &ioInfo, MMIO_READ);

	if (! m_hMio)
	{
		ret = ioInfo.wErrorRet;
		goto excp;
	}

	// Find Riff Chunk
	ciRiffChunk.fccType = mmioFOURCC('W','A','V','E');
	ret = mmioDescend(m_hMio, &ciRiffChunk, NULL, MMIO_FINDRIFF);

	if (ret)
	{
		goto excp;
	}

	// Find Format Chunk
	ciSubChunk.ckid =mmioFOURCC ('f','m','t',' ');
	ret = mmioDescend(m_hMio, &ciSubChunk, &ciRiffChunk, MMIO_FINDCHUNK);

	if (ret)
	{
		goto excp;
	}

	// Read Format Chunk
	ZeroMemory(&m_wfxWaveForm, sizeof(WAVEFORMATEX));
	nFmtSize = min(ciSubChunk.cksize, sizeof(WAVEFORMATEX));
	nReadSize = mmioRead(m_hMio, (HPSTR)&m_wfxWaveForm, nFmtSize);

	if (nReadSize != nFmtSize)
	{
		ret = MMIOERR_CANNOTREAD;
		goto excp;
	}

	// Find Data Chunk
	mmioAscend(m_hMio, &ciSubChunk, 0);
	ciSubChunk.ckid = mmioFOURCC('d','a','t','a');
	ret = mmioDescend(m_hMio, &ciSubChunk, &ciRiffChunk, MMIO_FINDCHUNK);

	if (ret)
	{
		goto excp;
	}

	// Set Wave Info
	m_nDataSize = ciSubChunk.cksize;

	if(m_pPlayer != NULL){
		m_pPlayer->setPlayFormat(&m_wfxWaveForm); // 再生フォーマットを設定
	}

excp:
	m_mutex.Unlock();
	return ret;
}

void CWaveFilePlayMethod::setPlayer(CWavePlayer * pPlayer)
{
	m_pPlayer = pPlayer;
}

void CWaveFilePlayMethod::closeWaveFile(void)
{
	MMRESULT ret = MMSYSERR_NOERROR;

	m_mutex.Lock();

	if (m_hMio)
	{
		ret = mmioClose(m_hMio, 0);
		m_hMio = NULL;
		m_nDataSize = 0;
	}

	m_mutex.Unlock();

}

// ループの設定をする
void CWaveFilePlayMethod::setLoop(bool loop)
{
	m_loop = loop;
}

void CWaveFilePlayMethod::getPlayFormat(WAVEFORMATEX * pFormat)
{
	*pFormat = m_wfxWaveForm;
}

int CWaveFilePlayMethod::getWaveSamples(void)
{
	return m_nDataSize;
}
