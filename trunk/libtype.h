/*
  libtype.h

  Copyright(C) 2008 YAMAHA CORPORATION
  All rights reserved.

  Description:
  このライブラリで使う型定義

  Revision history:
  Mon Sep 08 19:57:06 2008 JST : v1.00 1st coded by Kakizaki(Soundnet)
 */
#ifndef _LIBTYPE_
#define _LIBTYPE_

/* BEGIN */
/* 型宣言 */
typedef signed char sint8;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef signed int sint32;
typedef signed short sint16;
typedef float f32;
typedef double f64;

/* Windowに送信するユーザーイベントID */
enum {
	WM_MMEASURE_PROCESS_COMPLETE = (WM_USER + 100),
	WM_CPUTIME_ALERT,
	WM_MFPROPERTY_PROCESS_COMPLETE,
	WM_MMEASURE_SETTING_COMPLETE,
	WM_HOWLING_DETECT,
	WM_DATA_READY,
	WM_LIBEVENT_END,
};

/* END */
#endif /* _LIBTYPE_ */
