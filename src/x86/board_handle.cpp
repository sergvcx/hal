#include "hal-core.h"
#include "hal/hal-options.h"
#include <iostream>

BoardHandle::BoardHandle(const char *library_name){
    handle = open_library(library_name);
    plGetCount = (int(*)(unsigned int*))library_get_addr(handle, "PL_GetBoardCount");
    plGetDesc = (int (*)(unsigned int, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
    plCloseDesc = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
    plGetAccess_i = (int (*)(PL_Board *, int, PL_Access**))library_get_addr(handle, "PL_GetAccess");
    plGetAccess_ii = (int (*)(PL_Board *, int *, PL_Access**))library_get_addr(handle, "PL_GetAccess");
    plCloseAccess = (int (*)(PL_Access *))library_get_addr(handle, "PL_CloseAccess");
    plReadMemBlock = (int (*)(PL_Access *, void *, int addr, int))library_get_addr(handle, "PL_ReadMemBlock");
    plWriteMemBlock = (int (*)(PL_Access *, const void *, int addr, int))library_get_addr(handle, "PL_WriteMemBlock");
    plLoadProgramFile = (int (*)(PL_Access *, const char *))library_get_addr(handle, "PL_LoadProgramFile");
    plGetStatus = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetStatus");
    plGetResult = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetResult");
    plSync = (int (*)(PL_Access *, int, int *))library_get_addr(handle, "PL_Sync");
    plReset = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
}

HalBoard::HalBoard(){

}

// HalBoard *HalBoard::createBoard_MB7707(char *addr){

// }

HalBoard *HalBoard::createBoard_MC12101(int i){
    HalBoard *board = new HalBoard();
    board->board_handle = new BoardHandle("mc12101load.dll");
    board->board_type = MC12101;
    board->board_no = i;
    return board;
}

void HalBoard::open(){
    board_handle->plGetDesc(board_no, &desc);
}

void HalBoard::close(){
    board_handle->plCloseDesc(desc);
}

void HalBoard::reset(){
    board_handle->plReset(desc);
}

HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    if(board_type == MC12101){
        HalAccess *access = new HalAccess();
        access->board = this;
        board_handle->plGetAccess_i(desc, options->core, &access->access);
        return access;
    }
    return 0;
}

void HalBoard::closeAccess(HalAccess *access){
    if(access->board == this){
        std::cout << "Warning" << "Mismatch access and board";
    }
    delete access;
}

HalAccess::HalAccess(){
    
}


int HalAccess::sync(int value){
    int result;
    int error = board->board_handle->plSync(access, value, &result);
    if(error){
        std::cout << "Failed sync";
    }
    return result;
}

void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    int error = board->board_handle->plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);
    if(error){
        std::cout << "Failed read mem block";
    }
}

void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    int error = board->board_handle->plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);
    if(error){
        std::cout << "Failed write mem block";
    }
}

int HalAccess::getResult(){
    unsigned int result;
    int error = board->board_handle->plGetResult(access, &result);
    if(error){
        std::cout << "Failed get result";
    }
    return (int)result;
}

void HalAccess::loadProgram(const char* program_name){
    int error = board->board_handle->plLoadProgramFile(access, program_name);
    if(error){
        std::cout << "Failed load program";
    }
}

int HalAccess::getStatus(){
    unsigned int result;
    int error = board->board_handle->plGetStatus(access, &result);
    if(error){
        std::cout << "Failed get status";
    }
    return (int)result;
}

HalAccess::~HalAccess(){
    board->board_handle->plCloseAccess(access);
}