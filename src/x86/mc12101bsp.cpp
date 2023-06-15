#include "hal-core.h"
#include "library.h"
#include "hal/hal-options.h"
#include "mc12101bsp.h"
#include <iostream>
#include "io_host.h"

HalBoard *createBoard_MC12101(HalBoardOptions *options){
    return new HalBoardMC12101(options);
}

HalBoardMC12101::HalBoardMC12101(HalBoardOptions *options) {
    remoted = 0;
    if(options->server_enabled){
        remoted = 1;
        handle = open_library("mc12101load_proxy");
        if(handle == 0){
           INF_LOG("Library mc12101load_proxy not found");
            return;
        }
        auto rplConnectToHost = (int (*)(const char *, int))library_get_addr(handle, "PL_ConnectToHost");
        int error = rplConnectToHost(options->server_ip, options->server_port);        
    } else {
        handle = open_library("mc12101load");
        if(handle == 0){
            INF_LOG("Library mc12101load not found");
            return;
        }
    }

    board_type = MC12101;
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

unsigned int HalBoardMC12101::count(){
    plGetCount(&board_count);
    return board_count;
}


HalBoardMC12101::~HalBoardMC12101(){
    close();
    if(remoted){
        auto rplDisconnect = (int (*)())library_get_addr(handle, "RPL_DisconnectFromHost");
        rplDisconnect();
    } else {
        close_library(handle);
    }
}


int HalBoardMC12101::open(){
    //INF_LOG
    return plGetDesc(board_no, &desc);    
}

int HalBoardMC12101::close(){
    return plCloseDesc(desc);
}

int HalBoardMC12101::reset(){
    return plReset(desc);
}

HalAccess *HalBoardMC12101::getAccess(HalAccessOptions *options) {
    HalAccess *access = new HalAccessMC12101(this, options);
    return access;
}


HalAccessMC12101::HalAccessMC12101(HalBoardMC12101 *board, HalAccessOptions *opt){
    _board = board;
    core = opt->core;
    
    ops.plReadMemBlock = _board->plReadMemBlock;
    ops.plWriteMemBlock = _board->plWriteMemBlock;

    board->plGetAccess(board->desc, opt->core, &access);    
}

PL_Access *HalAccessMC12101::getBspAccess(){
    return access;
}


int HalAccessMC12101::sync(int value){
    int result;
    int error = _board->plSync(access, value, &result);
    if(error){
        std::cout << "Failed sync" << std::endl;
    }
    return result;
}

void HalAccessMC12101::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    int error = _board->plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);
    if(error){
        std::cout << "Failed read mem block" << std::endl;
    }
}

void HalAccessMC12101::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    int error = _board->plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);
    if(error){
        std::cout << "Failed write mem block";
    }
}

int HalAccessMC12101::getResult(){
    unsigned int result;
    int error = _board->plGetResult(access, &result);
    if(error){
        std::cout << "Failed get result" << std::endl;
    }
    return (int)result;
}

void HalAccessMC12101::loadProgramFile(const char* program_name){
    strcpy(program, program_name);
    int error = _board->plLoadProgramFile(access, program_name);
    if(error){
        std::cout << "Failed load program" << std::endl;
    }
}

void HalAccessMC12101::loadProgramFile(const char* program_name, const char *mainArgs){
    strcpy(program, program_name);
    int error = _board->plLoadProgramFileArgs(access, program_name, mainArgs);
    if(error){
        std::cout << "Failed load program" << std::endl;
    }
}

int HalAccessMC12101::getStatus(){
    unsigned int result;
    int error = _board->plGetStatus(access, &result);
    if(error){
        std::cout << "Failed get status" << std::endl;
    }
    return (int)result;
}

void *HalAccessMC12101::getOpsForIO(){
    return &ops;
}


HalAccessMC12101::~HalAccessMC12101(){
    _board->plCloseAccess(access);
}