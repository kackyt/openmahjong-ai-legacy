#ifndef MAHJONGAI_GLOBAL_H
#define MAHJONGAI_GLOBAL_H

#ifdef QT
#include <QtCore/qglobal.h>


#if defined(MAHJONGAI_LIBRARY)
#  define MAHJONGAISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAHJONGAISHARED_EXPORT Q_DECL_IMPORT
#endif

#else

#if defined(MAHJONGAI_LIBRARY)
#  define MAHJONGAISHARED_EXPORT __declspec(dllexport)
#else
#  define MAHJONGAISHARED_EXPORT __declspec(dllimport)
#endif
#endif

#endif // MAHJONGAI2_GLOBAL_H
