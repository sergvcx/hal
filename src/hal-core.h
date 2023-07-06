#ifndef __HAL_CORE_H_INCLUDED__
#define __HAL_CORE_H_INCLUDED__
#include "hal/hal.h"
#include <iostream>
#include "logger.h"


#ifdef __NM__
#define HAL_VIRTUAL_FUNC
#else
#define HAL_VIRTUAL_FUNC virtual
#endif

extern "C" int halOpenBoard(HalBoard *board);

struct IHalExtension {
    HAL_VIRTUAL_FUNC void* loadExtensionFunc(const char* function_name);
};

#ifdef __cplusplus
struct HalBoard : public IHalExtension {
protected:
    HalBoard(){};
public:
    int is_initialized;
    int board_type;
    HAL_VIRTUAL_FUNC int open();
    HAL_VIRTUAL_FUNC int loadInitCode();
    HAL_VIRTUAL_FUNC int close();
    HAL_VIRTUAL_FUNC int reset();    
    HAL_VIRTUAL_FUNC PL_Board *native();
    HAL_VIRTUAL_FUNC HalAccess *getAccess(HalAccessOptions *options);
    HAL_VIRTUAL_FUNC ~HalBoard();
    HAL_VIRTUAL_FUNC unsigned int count(int *error = NULL);
};

struct IHalAccessIO{
    HAL_VIRTUAL_FUNC void *getOpsForIO();
};



struct HalAccess{
protected:
    HalAccess(){};
public:
    HalBoard *board;

    //HAL_VIRTUAL uintptr_t getBspAccess();
    HAL_VIRTUAL_FUNC PL_Access *native();    
    HAL_VIRTUAL_FUNC int open();
    HAL_VIRTUAL_FUNC int close();
    HAL_VIRTUAL_FUNC int sync(int value, int *error = NULL);
    HAL_VIRTUAL_FUNC int readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size);
    HAL_VIRTUAL_FUNC int writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size);
    HAL_VIRTUAL_FUNC int getResult(int *error = NULL);
    HAL_VIRTUAL_FUNC int loadProgramFile(const char* filename);
    HAL_VIRTUAL_FUNC int loadProgramFile(const char* filename, const char *mainArgs);
    HAL_VIRTUAL_FUNC int getStatus(int *error = NULL);
    HAL_VIRTUAL_FUNC ~HalAccess();
};

#endif



struct HalBoardOptions{
    int board_no;
    int board_type;
    unsigned char host_mac_addr[7];
    char server_ip[16];
    int server_port;
    int server_enabled;
};

struct HalAccessOptions{
    int core;
    int cluster;
};


#endif //__HAL_CORE_H_INCLUDED__