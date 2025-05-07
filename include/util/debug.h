#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef WITH_DEBUG_INFO
#define DEBUG_LINE(...) __VA_ARGS__ ;
#else
#define DEBUG_LINE(...)
#endif

#endif
