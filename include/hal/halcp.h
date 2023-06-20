#ifndef __HAL_CP__H_INCLUDED__
#define __HAL_CP__H_INCLUDED__
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    int halcpOpenLink(int port);
    int halcpOpenDma();
    int halcpOpenShm(void *buffer, size_t len);
    int halcpOpenFile(const char *filename, const char *mode);

    int halcpWrite(int fd, void *data, size_t size);
    int halcpRead(int fd, void *data, size_t size);
    int halcpFlush(int cpd);
    int halcpClose(int cpd);

#ifdef __cplusplus
}
#endif

#endif //__HAL_CP__H_INCLUDED__