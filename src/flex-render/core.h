#pragma once
#define FM_BUILD_DLL

#ifdef FR_PLATFORM_WINDOWS
#	ifdef FR_BUILD_DLL
#		define FR_API _declspec(dllexport)
#	else 
#		define FR_API _declspec(dllimport)
#	endif
#else 
#	error Flex engine supports only Widnows
#endif
