#include "hal/hal.h"
#include "hal/hal-options.h"
#include "hal-core.h"
#include <iostream>

#ifndef DEPRECATED
extern "C" {

     HalBoard *halGetBoard(const char* options){
        HalBoardOptions opt;
        halSetBoardOption(&opt, HAL_BOARD_NUMBER, 0);
        return halGetBoardOpt(&opt);
    }
    
    HalAccess *halGetAccess(HalBoard *board, const char* options){
        HalAccessOptions opt;
        halSetAccessOption(&opt, HAL_CORE, 0);
        return halGetAccessOpt(board, &opt);
    }

    void halCloseBoard(HalBoard *board){
        board->close();
        delete board;
    }

    void halResetBoard(HalBoard *board){
        board->reset();
    }

    int halGetLastError(){
        return 0;
    }
 

    void halCloseAccess(HalAccess *access){
        delete access;
    }

    void halLoadProgram(HalAccess *access, const char *program_name){
        access->loadProgram(program_name);
    }

    int halSync(HalAccess *access, int value){
        return access->sync(value);
    }
    
    uintptr_t halSyncAddr(HalAccess *access, uintptr_t value){
        return 0;
    }

    void halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32){
        access->readMemBlock(dstHostAddr, srcBoardAddr, size32);
    }

    void halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32){
        access->writeMemBlock(srcHostAddr, dstBoardAddr, size32);
    }

    int halGetResult(HalAccess *access){
        return access->getResult();
    }

    int halGetStatus(HalAccess *access){
        return access->getStatus();
    }

    void halSetTimeout(int msec){

    }       

    void* halMalloc32(size_t size32){
        return 0;
    }

    

#endif //DEPRECATED
}