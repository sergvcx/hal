#include "hal-core.h"
#include "library.h"
#include "hal/hal-options.h"
#include "mc12101_bsp.h"
#include <iostream>

HalBoard *HalBoard::createBoard_MC12101(int i){
    return new HalBoardMC12101(i);
}

HalBoardMC12101::HalBoardMC12101(int i) {
    handle = open_library("mc12101load.dll");
    board_type = MC12101;
    board_no = i;
    plGetCount = (int(*)(unsigned int*))library_get_addr(handle, "PL_GetBoardCount");
    plGetDesc = (int (*)(unsigned int, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
    plCloseDesc = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
    plGetAccess = (int (*)(PL_Board *, int, PL_Access**))library_get_addr(handle, "PL_GetAccess");
    plCloseAccess = (int (*)(PL_Access *))library_get_addr(handle, "PL_CloseAccess");
    plReadMemBlock = (int (*)(PL_Access *, void *, int addr, int))library_get_addr(handle, "PL_ReadMemBlock");
    plWriteMemBlock = (int (*)(PL_Access *, const void *, int addr, int))library_get_addr(handle, "PL_WriteMemBlock");
    plLoadProgramFile = (int (*)(PL_Access *, const char *))library_get_addr(handle, "PL_LoadProgramFile");
    plGetStatus = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetStatus");
    plGetResult = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetResult");
    plSync = (int (*)(PL_Access *, int, int *))library_get_addr(handle, "PL_Sync");
    plReset = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
}

HalBoardMC12101::~HalBoardMC12101(){
    close();
    close_library(handle);
}


void HalBoardMC12101::open(){
    plGetDesc(board_no, &desc);
}

void HalBoardMC12101::close(){
    plCloseDesc(desc);
}

void HalBoardMC12101::reset(){
    plReset(desc);
}

HalAccess *HalBoardMC12101::getAccess(HalAccessOptions *options) {
    HalAccess *access = new HalAccessMC12101(this, options);
    return access;
}


HalAccessMC12101::HalAccessMC12101(HalBoardMC12101 *board, HalAccessOptions *opt){
    plGetResult = board->plGetResult;
    plGetStatus = board->plGetStatus;
    plReadMemBlock = board->plReadMemBlock;
    plWriteMemBlock = board->plWriteMemBlock;
    plLoadProgramFile = board->plLoadProgramFile;
    board->plGetAccess(board->desc, opt->core, &access);    
}


int HalAccessMC12101::sync(int value){
    int result;
    int error = plSync(access, value, &result);
    if(error){
        std::cout << "Failed sync";
    }
    return result;
}

void HalAccessMC12101::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    int error = plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);
    if(error){
        std::cout << "Failed read mem block";
    }
}

void HalAccessMC12101::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    int error = plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);
    if(error){
        std::cout << "Failed write mem block";
    }
}

int HalAccessMC12101::getResult(){
    unsigned int result;
    int error = plGetResult(access, &result);
    if(error){
        std::cout << "Failed get result";
    }
    return (int)result;
}

void HalAccessMC12101::loadProgram(const char* program_name){
    int error = plLoadProgramFile(access, program_name);
    if(error){
        std::cout << "Failed load program";
    }
}

int HalAccessMC12101::getStatus(){
    unsigned int result;
    int error = plGetStatus(access, &result);
    if(error){
        std::cout << "Failed get status";
    }
    return (int)result;
}

HalAccessMC12101::~HalAccessMC12101(){
    plCloseAccess(access);
}