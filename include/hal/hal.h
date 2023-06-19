#ifndef __HAL_H_INCLUDED
#define __HAL_H_INCLUDED
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"


typedef struct HalBoard HalBoard;        //for nm and x86 different realizations
typedef struct HalAccess HalAccess;       //for nm and x86 different realizations
typedef struct HalBoardOptions HalBoardOptions;
typedef struct HalAccessOptions HalAccessOptions;
typedef struct HalIO HalIO;

typedef struct{
    int core;
    int cluster;
}  HalCore;

enum HalBoardType{
    HAL_NO_BOARD,
    HAL_MC12101,
    HAL_MB7707,
    HAL_MC7601,
    HAL_MC5103,
    HAL_MC12705,
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



#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

    /**
     * @brief open board
     * 
     * @param board_settings
     * @return HalBoard* 
     * @details 
     */
    HalBoard *halGetBoard(const char *options, int *error);
    HalBoard *halGetBoardOpt(HalBoardOptions *board_options, int *error);

    unsigned int halGetBoardCount(HalBoardOptions *board_options, int *error);

    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor);
    

    int halCloseBoard(HalBoard *board);
    int halResetBoard(HalBoard *board);

    int halLoadInitCode(HalBoard *board);

 
    HalAccess *halGetAccess(HalBoard *board, HalCore *core, int *error);
    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options, int *error);
    int halCloseAccess(HalAccess *access);

    //HalIO *halStartIO(HalAccess *access, FILE *file);
    //void halStopIO(HalIO *io);

    int halLoadProgramFile(HalAccess *access, const char *program_name);
    int halLoadProgramFileArgs(HalAccess *access, const char *program_name, const char *args);
    int halLoadProgram(HalAccess *access, const void *addrProgram, unsigned int sizeProgram);
    HalIO *halStartIO(HalAccess *access, const char *program_file, FILE *file);
    int halStopIO(HalIO *hal_io);


    int halSync(HalAccess *board, int value, int *error);
    uintptr_t halSyncAddr(HalAccess *board, uintptr_t value, int *error);

    int halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32);
    int halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32);

    int halGetResult(HalAccess *access, int *error);
    int halGetStatus(HalAccess *access, int *error);

    
    int halSetTimeout(int msec);

    void* halMalloc32(size_t size32);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H_INCLUDED