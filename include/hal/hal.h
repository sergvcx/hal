#ifndef __HAL_H_INCLUDED
#define __HAL_H_INCLUDED
#include <stddef.h>
#include <stdint.h>


typedef struct HalBoard HalBoard;        //for nm and x86 different realizations
typedef struct HalAccess HalAccess;       //for nm and x86 different realizations
typedef struct HalBoardOptions HalBoardOptions;
typedef struct HalAccessOptions HalAccessOptions;

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
    

    void halCloseBoard(HalBoard *board);
    void halResetBoard(HalBoard *board);

    int halGetLastError();
 
    HalAccess *halGetAccess(HalBoard *board, const char *options);
    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options);
    void halCloseAccess(HalAccess *access);

    //HalIO *halStartIO(HalAccess *access, FILE *file);
    //void halStopIO(HalIO *io);

    void halLoadProgram(HalAccess *access, const char *filename); // work only on host

    int halSync(HalAccess *board, int value);
    uintptr_t halSyncAddr(HalAccess *board, uintptr_t value);

    void halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32);
    void halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32);

    int halGetResult(HalAccess *access);
    int halGetStatus(HalAccess *access);
    void halSetTimeout(int msec);

    void* halMalloc32(size_t size32);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H_INCLUDED