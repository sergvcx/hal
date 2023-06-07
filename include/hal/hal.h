#ifndef __HAL_H_INCLUDED
#define __HAL_H_INCLUDED
#include <stddef.h>
#include <stdint.h>

typedef struct HalBoard HalBoard;        //for nm and x86 different realizations
typedef struct HalAccess HalAccess;       //for nm and x86 different realizations
typedef struct HalCore HalCore;
typedef struct HalBoardSetting HalBoardSetting;

typedef enum{
    MC12101,
    MB7707,
    MC7601,
    MC5103
} HalBoardType;

enum HalError{
    HAL_OK,
    HAL_ERROR,
    HAL_BAD_ARGUMENT,
    HAL_WRONG_MEM,
    HAL_NOT_IMPLEMENTED
};

#define HAL_NONE            0
#define HAL_CORE_NUMBER     0xB000
#define HAL_CLUSTER_NUMBER  0xB001
#define HAL_CORE_NUMBER     0xB002

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

    HalBoardSetting *halGetBoardSettings(const char *str);
    //HalBoardSetting *halSetIPAddr(HalBoardSetting *setting, const char *addr);
    //HalBoardSetting *halSetBoardNumber(HalBoardSetting *setting, int i);

    /**
     * @brief open board
     * 
     * @param board_settings
     * @return HalBoard* 
     * @details 
     */
    HalBoard *halOpenBoard(HalBoardSetting *board_settings);

    void halCloseBoard(HalBoard *board);

    int halGetLastError();
 
    HalAccess *halCreateAccess(HalBoard *board, int *attrib_list);
    void halDestroyAccess(HalAccess *access);

    void halLoadProgram(HalAccess *access, const char *filename); // work only on host

    int halSync(HalAccess *board, int value);
    uintptr_t halSyncAddr(HalAccess *board, uintptr_t value);

    void halReadMemBlock (HalAccess *access, const void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32);
    void halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32);

    int halGetResult(HalAccess *access);
    void halSetTimeout(int msec);

    void* halMalloc32(size_t size32);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H_INCLUDED