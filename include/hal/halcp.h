#ifndef __HAL_CP__H_INCLUDED__
#define __HAL_CP__H_INCLUDED__
#include <stdlib.h>

typedef (*HalCpCallbackFuncT)(void *data);

#ifdef __cplusplus
extern "C" {
#endif

    int halcpOpenLink(int port);
    int halcpOpenDma();
    int halcpOpenShm(void *buffer, size_t len);
    int halcpOpenFile(const char *filename, const char *mode);

    int halcpWrite(int desc, void *data, size_t size);
    int halcpRead(int desc, void *data, size_t size);
	int halcpWriteM(int desc, void *data, size_t size, int width, int stride);
    int halcpReadM(int desc, void *data, size_t size, int width, int stride);
	int halcpWriteP(int desc, void *data, size_t size, int offset);
    int halcpReadP(int desc, void *data, size_t size, int offset);
	int halcpSetCallback(int desc, void *user_data);
    int halcpFlush(int desc);
    int halcpClose(int desc);

#ifdef __cplusplus
}
#endif

#endif //__HAL_CP__H_INCLUDED__