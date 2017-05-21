#pragma once

#include <QtGlobal>

#if defined(GO_LIBRARY)
#define GOSHARED_EXPORT Q_DECL_EXPORT
#else
#define GOSHARED_EXPORT Q_DECL_IMPORT
#endif
