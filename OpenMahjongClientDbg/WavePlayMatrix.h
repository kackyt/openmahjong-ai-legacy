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
