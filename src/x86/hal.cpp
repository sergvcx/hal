#include "hal/hal.h"
#include "hal-core.h"
#include <iostream>
#include "version.h"
#include "assert.h"
#include <stdarg.h>
#include "string.h"

#include "mc12101bsp.h"
#include "mc12705bsp.h"
#include "mb7707bsp.h"
#include "mc5103bsp.h"

#include "library.h"

#define HAL_SET_ERROR(pointer, err) if(pointer != NULL) *pointer = err
//#define HAL_SET_ERROR(pointer, err) 

HalBoard *createBoard_MB7707(HalBoardOptions *board_options);
HalBoard *createBoard_MC12101(HalBoardOptions *board_options);
HalBoard *createBoard_MC12705(HalBoardOptions *board_options);

static BoardInterfaceMC12101 board_interface_mc12101;
static BoardInterfaceMC12705 board_interface_mc12705;
static BoardInterfaceMB7707 board_interface_mb7707;
//static BoardInterfaceMC5103 board_interface_mc5103;

#ifndef DEPRECATED
extern "C" {
    HalBoard *halAllocBoard(){
        return new HalBoard();
    }

    int halFreeBoard(HalBoard *board){
        delete board;
        return 0;
    }

    HalAccess *halAllocAccess(){
        //return new HalAccess();
        return 0;
    }
    int halAccessSetOption(HalAccess *access, int opt, ...){
        return HAL_NOT_IMPLEMENTED;
    }
    int halAccessOpen(HalAccess *access){
        return HAL_NOT_IMPLEMENTED;
    }    
    int halFreeAccess(HalAccess *access){
        //delete access;
        return HAL_NOT_IMPLEMENTED;
    }

    int halBoardSetOption(HalBoard *board, int option, ...){
        va_list ap;
        va_start(ap, option);
        switch (option)
        {
        case HAL_BOARD_TYPE:
            board->options.board_type = va_arg(ap, int);
            board->board_type = board->options.board_type;
            //board->in
            break;
        case HAL_BOARD_NUMBER:
            board->options.board_no = va_arg(ap, int);
            break;
        case HAL_BOARD_MAC_ADDR:{
            const char *addr = va_arg(ap, const char*);
            memcpy(board->options.host_mac_addr, addr, 6);
            break;
        }
        case HAL_SERVER_IP:{
            const char *server_ip = va_arg(ap, const char*);
            strcpy(board->options.server_ip, server_ip);
            break;
        }
        case HAL_SERVER_PORT:
            board->options.server_port = va_arg(ap, int);
            break;
        case HAL_SERVER_ENABLED:
            board->options.server_enabled = va_arg(ap, int);
            break;
        default:
            return HAL_BAD_ARGUMENT;
        }
        va_end(ap);
        return 0;
    }

    
    void *halLoadExtensionFunction(HalBoard *board, const char* functionName){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        return board->board_interface->loadExtensionFunc(functionName);
    }

    PL_Board *halGetNativeBoard(HalBoard *board){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        return board->board_interface->native();
    }
    
    PL_Access *halGetNativeAccess(HalAccess *access){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        return access->access_interface->native();
    }

    int halGetVersion(unsigned int *version_major, unsigned int *version_minor){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        if(version_major != NULL){
            *version_major = HAL_VERSION_MAJOR;
        }
        if(version_minor != NULL){
            *version_minor = HAL_VERSION_MINOR;
        }
        return 0;
    }

    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options, int *error){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        int get_access_error = HAL_NOT_IMPLEMENTED;
        if(board->board_type == HAL_NO_BOARD){
            HAL_SET_ERROR(error, HAL_ERROR);
            return 0;
        }
        
        HalAccess *access = new HalAccess(board);
        HalCore core;
        core.cluster = access_options->cluster;
        core.core = access_options->core;
        access->access_interface = board->board_interface->getAccess(&core);
        int _error = access->access_interface->open();
        HAL_SET_ERROR(error, _error);
        return access;
    }

    int halSyncArray(HalAccess *access, HalSyncArrayData *src, HalSyncArrayData *dst){
        return access->access_interface->syncArray(src, dst);
    }


    int halBoardOpen(HalBoard *board){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        switch (board->board_type){
            {
            case HAL_MC12101:
                board->board_interface = &board_interface_mc12101;
                board_interface_mc12101.init("mc12101load");
                break;
            case HAL_MC12705:
                //board->board_interface = &board_interface_mc12101;
                //board_interface_mc12705.init("mc12705load");      
            case HAL_MB7707:
            case HAL_MC5103:
                return HAL_NOT_IMPLEMENTED;
            case HAL_NO_BOARD:
            default:
                return HAL_ERROR;
                break;
            }
        }
        return board->board_interface->open();
    }
    
    HalAccess *halGetAccess(HalBoard *board, HalCore *core, int *error){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        HalAccessOptions opt;
        opt.core = core->core;
        opt.cluster = core->cluster;
        return halGetAccessOpt(board, &opt, error);
    }

    int halBoardClose(HalBoard *board){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        if(board->board_type == HAL_NO_BOARD) return HAL_ERROR;
        int error = board->board_interface->close();
        return error;
    }

    int halBoardReset(HalBoard *board){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        return board->board_interface->reset();
    }

    int halLoadInitCode(HalBoard *board){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        return board->board_interface->loadInitCode();
    }

    int halBoardGetCount(HalBoard *board){
        if(board->board_type == HAL_NO_BOARD){
            return -1;
        } else {
            int error;
            int result = board->board_interface->count(&error);
            if(error) return -1;
            else return result;
        }
    }


    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        std::cout << "WARNING: halGetFirmwareVersion not implemented" << std::endl;
        return HAL_NOT_IMPLEMENTED;
    }
 

    int halAccessClose(HalAccess *access){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        access->access_interface->close();
        delete access;
        //return access->getError();
        return 0;
    }

    int halLoadProgramFile(HalAccess *access, const char *program_name){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        int error = access->access_interface->loadProgramFile(program_name);
        if(error != 0){
            return HAL_FILE;
        }
        return HAL_OK;
    }

    int halLoadProgram(HalAccess *access, const void *addrProgram, unsigned int sizeProgram){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        return HAL_NOT_IMPLEMENTED;
    }

    int halLoadProgramFileArgs(HalAccess *access, const char *program_name, const char *args){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        int error = access->access_interface->loadProgramFile(program_name, args);
        if(error != 0){
            return HAL_FILE;
        }
        return HAL_OK;
    }

    int halSync(HalAccess *access, int value, int *error){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        if(error != NULL){
            *error = 0;
        }
        return access->access_interface->sync(value);
    }
    
    uintptr_t halSyncAddr(HalAccess *access, uintptr_t value, int *error){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        if(error != NULL){
            *error = 0;
        }
        return 0;
    }

    int halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        access->access_interface->readMemBlock(dstHostAddr, srcBoardAddr, size32);
        return 0;
    }

    int halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        access->access_interface->writeMemBlock(srcHostAddr, dstBoardAddr, size32);
        return 0;
    }

    int halGetResult(HalAccess *access, int* error){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        int _error;
        int result = access->access_interface->getResult(&_error);
        HAL_SET_ERROR(error, _error);
        return result;
    }

    int halGetStatus(HalAccess *access, int* error){
        //Log(LOG_DEBUG).get() << __FUNCTION__;
        return access->access_interface->getStatus(error);
    }

    int halSetTimeout(int msec){
        return 0;
    }       

    void* halMalloc32(size_t size32){
        return 0;
    }

    

#endif //DEPRECATED
}