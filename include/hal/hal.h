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

struct HalCore{
    int core;
    int cluster;
};

enum HalBoardType{
    NO_BOARD,
    MC12101,
    MB7707,
    MC7601,
    MC5103,
    VIRTUAL
};

enum HalError{
    HAL_OK,
    HAL_ERROR,
    HAL_BAD_ARGUMENT,
    HAL_WRONG_MEM,
    HAL_NOT_IMPLEMENTED
};



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
    HalBoard *halGetBoard(const char *options);
    HalBoard *halGetBoardOpt(HalBoardOptions *board_options);

    unsigned int halGetBoardCount(HalBoardOptions *board_options, int *error = NULL);

    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor);
    

    int halCloseBoard(HalBoard *board);
    int halResetBoard(HalBoard *board);

    int halLoadInitCode(HalBoard *board);

 
    HalAccess *halGetAccess(HalBoard *board, HalCore *core);
    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options);
    int halCloseAccess(HalAccess *access);

    //HalIO *halStartIO(HalAccess *access, FILE *file);
    //void halStopIO(HalIO *io);

    int halLoadProgramFile(HalAccess *access, const char *program_name);
    int halLoadProgramFileArgs(HalAccess *access, const char *program_name, const char *args);
    int halLoadProgram(HalAccess *access, const void *addrProgram, unsigned int sizeProgram);
    HalIO *halStartIO(HalAccess *access, const char *program_file, FILE *file);
    int halStopIO(HalIO *hal_io);


    int halSync(HalAccess *board, int value, int *error = NULL);
    uintptr_t halSyncAddr(HalAccess *board, uintptr_t value, int *error = NULL);

    int halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32);
    int halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32);

    int halGetResult(HalAccess *access, int *error = NULL);
    int halGetStatus(HalAccess *access, int *error = NULL);

    
    int halSetTimeout(int msec);

    void* halMalloc32(size_t size32);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H_INCLUDED