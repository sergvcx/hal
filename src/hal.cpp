#include "hal/hal.h"
#include "hal-core.h"
#include <iostream>

#ifndef DEPRECARED
extern "C" {

    HalBoardSetting *halGetBoardSettings(const char *str){
        HalBoardSetting *settings = new HalBoardSetting();
        settings->board_type = MC12101;
        settings->board_no = 0;
        return settings;
    }

    HalBoard *halOpenBoard(HalBoardSetting *board_settings){
        HalBoard *board = 0;
        if(board_settings->board_type == MC12101){
            board = HalBoard::createBoard_MC12101(board_settings->board_no);
        } else {
            std::cout << "Not implemented";
        }   
        board->open();
        return board;
    }

    void halCloseBoard(HalBoard *board){
        board->close();
    }

    void halResetBoard(HalBoard *board){
        board->reset();
    }

    int halGetLastError(){
        return 0;
    }
 
    HalAccess *halCreateAccess(HalBoard *board, int *attrib_list){
        int i = 0;
        int core, cluster;
        while(attrib_list[i] != HAL_NONE){
            if(attrib_list[i] == HAL_CORE_NUMBER){
                core = attrib_list[i + 1];
            }
            if(attrib_list[i] == HAL_CLUSTER_NUMBER){
                cluster = attrib_list[i + 1];
            }
            i += 2;
        }
        if(board->board_type == MC12101){
            return HalAccess::createAccess(board, core);
        }
    }

    void halDestroyAccess(HalAccess *access){
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

    

#endif DEPRECARED
}