#include "hal-core.h"
#include "library.h"
#include "hal/hal-options.h"
#include "mc12705bsp.h"
#include <iostream>
#include "assert.h"

#include <cstring>

HalBoard *createBoard_MC12705(HalBoardOptions *options){
    return new HalBoardMC12705(options);
}

HalBoardMC12705::HalBoardMC12705(HalBoardOptions *options) {
    remoted = 0;
    if(options->server_enabled){
        remoted = 1;
        handle = open_library("mc12705load_proxy");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12705load_proxy not found";
            return;
        }
        auto rplConnectToHost = (int (*)(const char *, int))library_get_addr(handle, "PL_ConnectToHost");
        int error = rplConnectToHost(options->server_ip, options->server_port);        
    } else {
        handle = open_library("mc12705load");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12705load not found";
            return;
        }
    }

    board_type = HAL_MC12705;
    board_no = options->board_no;
    plGetCount = (int(*)(unsigned int*))library_get_addr(handle, "PL_GetBoardCount");
    plGetDesc = (int (*)(unsigned int, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
    plCloseDesc = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
    plGetAccess = (int (*)(PL_Board *, PL_Core*, PL_Access**))library_get_addr(handle, "PL_GetAccess");
    plCloseAccess = (int (*)(PL_Access *))library_get_addr(handle, "PL_CloseAccess");
    plReadMemBlock = (int (*)(PL_Access *, void *, int addr, int))library_get_addr(handle, "PL_ReadMemBlock");
    plWriteMemBlock = (int (*)(PL_Access *, const void *, int addr, int))library_get_addr(handle, "PL_WriteMemBlock");
    plLoadProgramFile = (int (*)(PL_Access *, const char *))library_get_addr(handle, "PL_LoadProgramFile");
    plLoadProgramFileArgs = (int (*)(PL_Access *, const char *, const char *))library_get_addr(handle, "PL_LoadProgramFileArgs");
    plGetStatus = (int (*)(PL_Access *, PL_Word *))library_get_addr(handle, "PL_GetStatus");
    plGetResult = (int (*)(PL_Access *, PL_Word *))library_get_addr(handle, "PL_GetResult");
    plSync = (int (*)(PL_Access *, int, int *))library_get_addr(handle, "PL_Sync");
    plReset = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
    is_initialized = 1;

    
}

unsigned int HalBoardMC12705::count(int *error){
    plGetCount(&board_count);
    return board_count;
}


HalBoardMC12705::~HalBoardMC12705(){
    close();
    if(remoted){
        auto rplDisconnect = (int (*)())library_get_addr(handle, "RPL_DisconnectFromHost");
        rplDisconnect();
    } else {
        close_library(handle);
    }
}


int HalBoardMC12705::open(){
    //INF_LOG
    assert(plGetDesc);
    return plGetDesc(board_no, &desc);    
}

int HalBoardMC12705::close(){
    assert(plCloseDesc);
    return plCloseDesc(desc);
}

int HalBoardMC12705::reset(){
    assert(plReset);
    return plReset(desc);
}

int HalBoardMC12705::loadInitCode(){
    assert(plLoadInitBoard);
    return plLoadInitBoard(desc); 
}

HalAccess *HalBoardMC12705::getAccess(HalAccessOptions *options) {
    HalAccess *access = new HalAccessMC12705(this, options);
    return access;
}


HalAccessMC12705::HalAccessMC12705(HalBoardMC12705 *board, HalAccessOptions *opt){
    _board = board;
    
    assert(_board->plReadMemBlock);
    assert(_board->plWriteMemBlock);
    ops.plReadMemBlock = _board->plReadMemBlock;
    ops.plWriteMemBlock = _board->plWriteMemBlock;

    core.core = opt->core;
    core.cluster = opt->cluster;

    board->plGetAccess(board->desc, (PL_Core *)&core, &access);    
}

PL_Access *HalAccessMC12705::getBspAccess(){
    return access;
}


int HalAccessMC12705::sync(int value, int *error){
    int result = 0;
    // int _error = board->plSync(access, value, &result);
    // if(error != NULL){
    //     *error = _error;
    // }
    return result;
}

int HalAccessMC12705::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    return _board->plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);    
}

int HalAccessMC12705::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    return _board->plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);    
}

int HalAccessMC12705::getResult(int *error){
    PL_Word result;
    int _error = _board->plGetResult(access, &result);
    if(_error){
        std::cout << "Failed get result" << std::endl;
    }
    return (int)result;
}

int HalAccessMC12705::loadProgramFile(const char* program_name){
    strcpy(program, program_name);
    return _board->plLoadProgramFile(access, program_name);
}

int HalAccessMC12705::loadProgramFile(const char* program_name, const char *mainArgs){
    strcpy(program, program_name);
    return _board->plLoadProgramFileArgs(access, program_name, mainArgs);
}

int HalAccessMC12705::getStatus(int *error){
    PL_Word result;
    int _error = _board->plGetStatus(access, &result);
    if(_error){
        std::cout << "Failed get status" << std::endl;
    }
    return (int)result;
}


HalAccessMC12705::~HalAccessMC12705(){
    _board->plCloseAccess(access);
}