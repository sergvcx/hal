#ifdef _WIN32
#include "windows.h"
#endif

#ifdef unix
#include "unistd.h"
#endif


#ifdef __cplusplus
    extern "C" {
#endif

void halSleep(int msecs){

	#ifdef _WIN32
	::Sleep(msecs);
	#endif


	#ifdef unix
	usleep(msecs * 1000);
	#endif

}

#ifdef __cplusplus
	};
#endif
