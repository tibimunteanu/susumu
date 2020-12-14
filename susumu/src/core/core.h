#pragma once

#ifdef SU_PLATFORM_WINDOWS
	#ifdef SU_BUILD_DLL
		#define SU_API __declspec(dllexport)
	#else
		#define SU_API __declspec(dllimport)
	#endif
#else
	#error susumu only runs on windows for now!
#endif