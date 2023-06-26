#include "hal/halcp.h"

typedef size_t(*HalCpWriteFuncT)(void *user_data, const void *buf, size_t size);
typedef size_t(*HalCpReadFuncT)(void *user_data, void *buf, size_t size);

#define MAX_SIZE 16

enum HalcpType{
    HALCP_NONE,
    HALCP_LINK,
    HALCP_SHM
};

struct HalCpObject{
    int fd;
    int type;
    int is_enabled;
    void *user_data;
    HalCpWriteFuncT writeFunc;
    HalCpReadFuncT readFunc;
    HalCpObject(){
        is_enabled = 0;
    }
};

struct HalCpShmData{
    int pos;
    void *buffer;
};

static size_t shmemWrite(void *user_data, const void *buf, size_t size){
    HalCpShmData *data = (HalCpShmData *)user_data;
    const int *src = (const int *)buf;
    int *dst = (int *)data->buffer;
    size /= 4;
    for(size_t i = 0; i < size; i++){
        dst[i] = src[i];
    }
    return 0;
}

static size_t shmemRead(void *user_data, void *buf, size_t size){
    //HalCpShmData *data = (HalCpShmData *)user_data;
    return 0;
}


__attribute__ ((section (".hal.cp"))) HalCpObject objects[MAX_SIZE];
extern "C"{
    int halcpOpenLink(int port){
        return -1;
    }
    int halcpOpenDma(){
        return -1;
    }
    int halcpOpenShm(void *buffer, size_t len){
        for(int i = 0; i < MAX_SIZE; i++){
            if(objects[i].is_enabled == 0){
                objects[i].fd = i + 1;
                HalCpShmData *data = new HalCpShmData();
                if(data == 0){
                    return -1;
                }
                data->pos = 0;
                data->buffer = buffer;
                objects[i].user_data = data;
                objects[i].type = HALCP_SHM;
                objects[i].readFunc = shmemRead;
                objects[i].writeFunc = shmemWrite;
                return objects[i].fd;
            }
        }
        return -1;
    }
    int halcpOpenFile(const char *filename, const char *mode){
        return -1;
    }

    int halcpWrite(int fd, void *data, size_t size){
        return -1;
    }
    int halcpRead(int fd, void *data, size_t size){
        return -1;
    }
	int halcpWriteM(int desc, void *data, size_t size, int width, int stride){
		return -1;
	}
    int halcpReadM(int desc, void *data, size_t size, int width, int stride){
		return -1;
	}
	int halcpWriteP(int desc, void *data, size_t size, int offset){
		return -1;
	}
    int halcpReadP(int desc, void *data, size_t size, int offset){
		return -1;
	}
	
    int halcpFlush(int cpd){
        return -1;
    }

    int halcpClose(int cpd){
        return -1;
    }
}
    