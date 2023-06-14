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
    
    HalAccess *halGetAccess(HalBoard *board, HalCore *core){
        HalAccessOptions opt;
        halSetAccessOption(&opt, HAL_CORE, core->core);
        halSetAccessOption(&opt, HAL_CLUSTER, core->cluster);
        //board->getAccess();
        return halGetAccessOpt(board, &opt);
    }

    int halCloseBoard(HalBoard *board){
        board->close();
        delete board;
        return 0;
    }

    int halResetBoard(HalBoard *board){
        board->reset();
        return 0;
    }
    int halLoadInitCode(HalBoard *board){
        std::cout << "halLoadInitCode not implemented" << std::endl;
        return 0;
    }

    int halGetLastError(HalBoard *board){
        std::cout << "halGetLastError not implemented" << std::endl;
        return 0;
    }

    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor){
        std::cout << "WARNING: halGetFirmwareVersion not implemented" << std::endl;
        return HAL_NOT_IMPLEMENTED;
    }
 

    int halCloseAccess(HalAccess *access){
        delete access;
        return 0;
    }

    int halLoadProgram(HalAccess *access, const char *program_name){
        access->loadProgram(program_name);
        return 0;
    }

    int halLoadProgramArgs(HalAccess *access, const char *program_name, const char *args){
        access->loadProgram(program_name);
        return 0;
    }

    int halSync(HalAccess *access, int value, int *error){
        *error = 0;
        return access->sync(value);
    }
    
    uintptr_t halSyncAddr(HalAccess *access, uintptr_t value, int *error){
        *error = 0;
        return 0;
    }

    int halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32){
        access->readMemBlock(dstHostAddr, srcBoardAddr, size32);
        return 0;
    }

    int halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32){
        access->writeMemBlock(srcHostAddr, dstBoardAddr, size32);
        return 0;
    }

    int halGetResult(HalAccess *access, int* error){
        *error = 0;
        return access->getResult();
    }

    int halGetStatus(HalAccess *access, int* error){
        *error = 0;
        return access->getStatus();
    }

    int halSetTimeout(int msec){
        return 0;
    }       

    void* halMalloc32(size_t size32){
        return 0;
    }

    

#endif //DEPRECATED
}