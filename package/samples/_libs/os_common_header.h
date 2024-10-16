
// Operating system common headers

#pragma once

#ifdef UNIX
#elif defined(WIN32)
	// windows 32 and 64
	#include <Windows.h>
	#include <setupapi.h>
	#include <ObjBase.h>
#else
	#error No operating system defined
#endif

// all systems defines
#define MAX_DEVICE_PATH_SIZE	512
