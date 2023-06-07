#include "hal/hal.h"
#include "hal-core.h"

extern "C" {

    HalBoardSetting *halGetBoardSettings(const char *str){
        
    }

    HalBoard *halOpenBoard(HalBoardSetting *board_settings){
        
    }

    void halCloseBoard(HalBoard *board){

    }

    int halGetLastError(){

    }
 
    HalAccess *halCreateAccess(HalBoard *board, int *attrib_list){

    }
    void halDestroyAccess(HalAccess *access){

    }

    void halLoadProgram(HalAccess *access, const char *filename){

    }

    int halSync(HalAccess *board, int value){

    }
    uintptr_t halSyncAddr(HalAccess *board, uintptr_t value){

    }

    void halReadMemBlock (HalAccess *access, const void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32){

    }

    void halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32){

    }

    int halGetResult(HalAccess *access){

    }

    void halSetTimeout(int msec){

    }       

    void* halMalloc32(size_t size32){
        
    }
}