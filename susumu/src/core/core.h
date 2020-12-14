#pragma once

#ifdef SUSUMU_PLATFORM_WINDOWS
	#ifdef SUSUMU_BUILD_DLL
		#define SUSUMU_API __declspec(dllexport)
	#else
		#define SUSUMU_API __declspec(dllimport)
	#endif
#else
	#error susumu only runs on windows for now!
#endif