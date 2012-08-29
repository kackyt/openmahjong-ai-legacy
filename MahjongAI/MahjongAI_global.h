#ifndef MAHJONGAI_GLOBAL_H
#define MAHJONGAI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MAHJONGAI_LIBRARY)
#  define MAHJONGAISHARED_EXPORT Q_DECL_EXPORT
#else
#  define MAHJONGAISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MAHJONGAI2_GLOBAL_H
