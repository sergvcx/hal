#ifndef __HAL_H_INCLUDED
#define __HAL_H_INCLUDED
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"

typedef struct PL_Board PL_Board;
typedef struct PL_Access PL_Access;


typedef struct HalBoard HalBoard;        //for nm and x86 different realizations
typedef struct HalAccess HalAccess;       //for nm and x86 different realizations
typedef struct HalBoardOptions HalBoardOptions;
typedef struct HalAccessOptions HalAccessOptions;
typedef struct HalIO HalIO;

typedef struct{
    int core;
    int cluster;
}  HalCore;

enum HAL_ACCESS_OPTIONS{
    HAL_CORE,
    HAL_CLUSTER
};

enum HalBoardType{
    HAL_NO_BOARD,
    HAL_MC12101,
    HAL_MB7707,
    HAL_MC7601,
    HAL_MC5103,
    HAL_MC12705,
    HAL_HOST
};

enum HAL_BOARD_OPTIONS{
    HAL_BOARD_NUMBER,
    HAL_BOARD_TYPE,
    HAL_BOARD_MAC_ADDR,
    HAL_SERVER_IP,
    HAL_SERVER_PORT,
    HAL_SERVER_ENABLED
};

enum HalError{
    HAL_OK,
    HAL_ERROR,
    HAL_FILE,
    HAL_BAD_ARGUMENT,
    HAL_WRONG_MEM,
    HAL_NOT_IMPLEMENTED
};

#define HAL_PROGRAM_FINISHED 2

typedef struct {
    int value;
    uintptr_t addr;
    size_t length;
} HalSyncArrayData;



#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor);
    int halGetVersion(unsigned int *version_major, unsigned int *version_minor);

    
    HalBoard *halAllocBoard();
    int halBoardSetOption(HalBoard * board, int option, ...);
    int halBoardOpen(HalBoard *board);
    int halBoardClose(HalBoard *board);
    int halBoardReset(HalBoard *board);
    int halFreeBoard(HalBoard *board);

    //HalAccess *halAllocAccess(HalBoard *board);
    //int halAccessSetOption(HalAccess *access, int option, ...);
    //int halFreeAccess(HalAccess *access);
 
    HalAccess *halGetAccess(HalBoard *access, HalCore *core, int *error);
    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options, int *error);
    int halAccessClose(HalAccess *access);


    int halSync(HalAccess *board, int value, int *error);
    //uintptr_t halSyncAddr(HalAccess *board, uintptr_t value, int *error);

    int halSyncArray(HalAccess *access, HalSyncArrayData *src, HalSyncArrayData *dst);

    int halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32);
    int halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32);

    int halGetResult(HalAccess *access, int *error);
    int halGetStatus(HalAccess *access, int *error);

    
    int halSetTimeout(int msec);

    int halGetProcessorNo();


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H_INCLUDED