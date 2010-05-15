#pragma once
#include "libtype.h"

class CWavePlayMethod
{
public:
	CWavePlayMethod(void);
public:
	~CWavePlayMethod(void);
public:
	// 演奏する次のバッファを獲得する(16bit固定小数点) (NULLを返してもよい)
	virtual sint16 * getNextWaveFix16(int length) = 0;
public:
	// 演奏する次のバッファを獲得する(64bit浮動小数点) (NULLを返してもよい)
	virtual double * getNextWaveFloat64(int length) = 0;
};
