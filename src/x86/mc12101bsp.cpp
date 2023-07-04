#include "hal-core.h"
#include "library.h"
#include "hal/hal-options.h"
#include "mc12101bsp.h"
#include <iostream>
#include <cstring>

HalBoard *createBoard_MC12101(HalBoardOptions *options){
    return new HalBoardMC12101(options);
}

HalBoardMC12101::HalBoardMC12101(HalBoardOptions *options) {
    remoted = 0;
    is_opened = 0;
    handle = 0;
    if(options->server_enabled){
        handle = open_library("mc12101load_proxy");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12101load_proxy not found"; 
            return;
        }
        auto rplConnectToHost = (int (*)(const char *, int))library_get_addr(handle, "PL_ConnectToHost");
        int error = rplConnectToHost(options->server_ip, options->server_port);        
        remoted = 1;
    } else {
        handle = open_library("mc12101load");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12101load_proxy not found"; 
            return;
        }
    }

    board_type = HAL_MC12101;
    board_no = options->board_no;
    plGetCount = (int(*)(unsigned int*))library_get_addr(handle, "PL_GetBoardCount");
    plGetDesc = (int (*)(unsigned int, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
    plCloseDesc = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
    plGetAccess = (int (*)(PL_Board *, int, PL_Access**))library_get_addr(handle, "PL_GetAccess");
    plCloseAccess = (int (*)(PL_Access *))library_get_addr(handle, "PL_CloseAccess");
    plReadMemBlock = (int (*)(PL_Access *, void *, int addr, int))library_get_addr(handle, "PL_ReadMemBlock");
    plWriteMemBlock = (int (*)(PL_Access *, const void *, int addr, int))library_get_addr(handle, "PL_WriteMemBlock");
    plLoadProgramFile = (int (*)(PL_Access *, const char *))library_get_addr(handle, "PL_LoadProgramFile");
    plLoadProgramFileArgs = (int (*)(PL_Access *, const char *, const char *))library_get_addr(handle, "PL_LoadProgramFileArgs");
    plGetStatus = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetStatus");
    plGetResult = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetResult");
    plSync = (int (*)(PL_Access *, int, int *))library_get_addr(handle, "PL_Sync");
    plReset = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
    is_initialized = 1;
    
}

unsigned int HalBoardMC12101::count(int *error){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    int _error = plGetCount(&board_count);
    if(error != NULL) *error = _error;
    return board_count;
}


HalBoardMC12101::~HalBoardMC12101(){
    close();
    if(remoted){
        auto rplDisconnect = (int (*)())library_get_addr(handle, "RPL_DisconnectFromHost");
        rplDisconnect();
    } else {
        if(handle){
            close_library(handle);
        }
    }
}
PL_Board* HalBoardMC12101::native() {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return desc;
}

void* HalBoardMC12101::loadExtensionFunc(const char* function_name) {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    void* result = library_get_addr(handle, function_name);
    Log(LOG_DEBUG1).get() << "MC121.01 extension func " << function_name << ": " << result;
    return result;
}


int HalBoardMC12101::open(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    if(!is_opened){
        is_opened = 1;
        return plGetDesc(board_no, &desc);    
    } else {
        return HAL_ERROR;
    }
}

int HalBoardMC12101::close(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    if(is_opened){
        is_opened = 0;
        return plCloseDesc(desc);
    } else {
        return HAL_ERROR;
    }
}

int HalBoardMC12101::reset(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return plReset(desc);
}

HalAccess *HalBoardMC12101::getAccess(HalAccessOptions *options) {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    HalAccess *access = new HalAccessMC12101(this, options);
    //if(access)
    return access;
}


HalAccessMC12101::HalAccessMC12101(HalBoardMC12101 *board, HalAccessOptions *opt){
    _board = board;
    core = opt->core;
    access = 0;
    
    ops.plReadMemBlock = _board->plReadMemBlock;
    ops.plWriteMemBlock = _board->plWriteMemBlock;
}

int HalAccessMC12101::open(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return _board->plGetAccess(_board->desc, core, &access);
}

int HalAccessMC12101::close(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    if(access == 0) return HAL_OK;
    int error = _board->plCloseAccess(access);
    if(error == HAL_OK){
        access = 0;
    }    
    return error;
}

PL_Access *HalAccessMC12101::native(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return access;
}

int HalAccessMC12101::sync(int value, int *error){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    int result;
    int _error = _board->plSync(access, value, &result);
    if(error != NULL){
        *error = _error;
    }
    return result;
}

int HalAccessMC12101::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return _board->plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);
}

int HalAccessMC12101::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return _board->plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);
}

int HalAccessMC12101::getResult(int *error){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    unsigned int result;
    int _error = _board->plGetResult(access, &result);
    if(error != NULL){
        *error = _error;
    }
    return (int)result;
}

int HalAccessMC12101::loadProgramFile(const char* program_name){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    strcpy(program, program_name);
    return _board->plLoadProgramFile(access, program_name);
}

int HalAccessMC12101::loadProgramFile(const char* program_name, const char *mainArgs){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    strcpy(program, program_name);
    return _board->plLoadProgramFileArgs(access, program_name, mainArgs);
}

int HalAccessMC12101::getStatus(int *error){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    unsigned int result;
    int _error = _board->plGetStatus(access, &result);
    if(_error){
        std::cout << "Failed get status" << std::endl;
    }
    return (int)result;
}

void *HalAccessMC12101::getOpsForIO(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return &ops;
}


HalAccessMC12101::~HalAccessMC12101(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    close();
}