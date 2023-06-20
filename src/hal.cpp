#include "hal/hal.h"
#include "hal/hal-options.h"
#include "hal-core.h"
#include <iostream>

#define HAL_SET_ERROR(pointer, err) if(pointer != NULL) *pointer = err
//#define HAL_SET_ERROR(pointer, err) 

HalBoard *createBoard_MB7707(HalBoardOptions *board_options);
HalBoard *createBoard_MC12101(HalBoardOptions *board_options);
HalBoard *createBoard_MC12705(HalBoardOptions *board_options);


#ifndef DEPRECATED
extern "C" {

    HalBoard *halCreateBoard(HalBoardOptions *board_options){
        HalBoard *board = NULL;
        switch (board_options->board_type)
        {
        case HAL_MC12101:
            board = createBoard_MC12101(board_options);
            break;
        case HAL_MB7707:
            board = createBoard_MB7707(board_options);
            break;
        case HAL_MC12705:
            board = createBoard_MC12705(board_options);
            break;
        default:
            break;
        }
        return board;
    }

    HalBoard *halGetBoardOpt(HalBoardOptions *board_options, int *error){
        HalBoard *board = halCreateBoard(board_options);
        if(!board) {
            HAL_SET_ERROR(error, HAL_BAD_ARGUMENT);
            return NULL;
        }
        if(board->is_initialized){
            int _error = board->open();
            HAL_SET_ERROR(error, _error);
            if(_error){
                delete board;
                return 0;
            }
            return board;
        } else {
            HAL_SET_ERROR(error, HAL_ERROR);
            delete board;
            return NULL;
        }
    }


    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options, int *error){
        int get_access_error = HAL_NOT_IMPLEMENTED;
        HalAccess *access = board->getAccess(access_options);
        if(access == 0) {
            HAL_SET_ERROR(error, HAL_ERROR);
            return 0;
        }
        int _error = access->open();
        HAL_SET_ERROR(error, _error);
        return access;
    }

     HalBoard *halGetBoard(const char* options, int *error){
        HalBoardOptions opt;
        halSetBoardOption(&opt, HAL_BOARD_NUMBER, 0);
        return halGetBoardOpt(&opt, error);
    }

    int halOpenBoard(HalBoard *board){
        return board->open();
    }
    
    HalAccess *halGetAccess(HalBoard *board, HalCore *core, int *error){
        HalAccessOptions opt;
        halSetAccessOption(&opt, HAL_CORE, core->core);
        halSetAccessOption(&opt, HAL_CLUSTER, core->cluster);        
        return halGetAccessOpt(board, &opt, error);
    }

    int halCloseBoard(HalBoard *board){
        int error = board->close();
        delete board;
        return error;
    }

    int halResetBoard(HalBoard *board){
        return board->reset();
    }
    int halLoadInitCode(HalBoard *board){
        return board->loadInitCode();
    }


    unsigned int halGetBoardCount(HalBoardOptions *board_options, int *error){
        int getBoardError;
        HalBoard *board = halCreateBoard(board_options);
        if(!board) {
            HAL_SET_ERROR(error, HAL_BAD_ARGUMENT);
            return 0;
        }
        unsigned int count = board->count(error);
        halCloseBoard(board);
        return count;
    }

    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor){
        std::cout << "WARNING: halGetFirmwareVersion not implemented" << std::endl;
        return HAL_NOT_IMPLEMENTED;
    }
 

    int halCloseAccess(HalAccess *access){
        access->close();
        delete access;
        //return access->getError();
        return 0;
    }

    int halLoadProgramFile(HalAccess *access, const char *program_name){
        int error = access->loadProgramFile(program_name);
        if(error != 0){
            return HAL_FILE;
        }
        return HAL_OK;
    }

    int halLoadProgram(HalAccess *access, const void *addrProgram, unsigned int sizeProgram){
        return HAL_NOT_IMPLEMENTED;
    }

    int halLoadProgramFileArgs(HalAccess *access, const char *program_name, const char *args){
        access->loadProgramFile(program_name, args);
        return 0;
    }

    int halSync(HalAccess *access, int value, int *error){
        if(error != NULL){
            *error = 0;
        }
        return access->sync(value);
    }
    
    uintptr_t halSyncAddr(HalAccess *access, uintptr_t value, int *error){
        if(error != NULL){
            *error = 0;
        }
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
        int _error;
        int result = access->getResult(&_error);
        HAL_SET_ERROR(error, _error);
        return result;
    }

    int halGetStatus(HalAccess *access, int* error){
        if(error != NULL){
            *error = 0;
        }
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