#include "mc5103bsp.h"
#include "string.h"


HalBoard *createBoard_MC5103(const unsigned char* host_mac_addr){
    return new HalBoardMC5103(host_mac_addr);
}
    
HalBoardMC5103::HalBoardMC5103(const unsigned char* host_mac_addr){
    is_initialized = 0;
    //strcpy((char*)mac_addr, (const char*)host_mac_addr);
    handle = open_library("mc5103load");
    char *path = getenv("PATH");
    INF_LOG(path);
    path = getenv("MC5103");
    INF_LOG(path);
    if(handle == 0){
        INF_LOG("Library not found");
        return;
    }

    is_initialized = 1;
}

bool HalBoardMC5103::check(){
    return handle != 0;
}

HalBoardMC5103::~HalBoardMC5103(){
    if(!check()) return;
    close();
    close_library(handle);
}

int HalBoardMC5103::open(){
    if(!check()) return HAL_ERROR;
    return 1;
    //return plGetBoardDesc(mac_addr, &desc);
}
int HalBoardMC5103::close(){
    if(!check()) return HAL_ERROR;
    return 1;
    //return plCloseBoardDesc(desc);
}

int HalBoardMC5103::reset(){
    if(!check()) return HAL_ERROR;
    return 1;
    //return plResetBoard(desc);
}

HalAccess *HalBoardMC5103::getAccess(HalAccessOptions *options){
    if(!check()) return 0;
    HalAccessMC5103 *access = new HalAccessMC5103(this, options);
    return access;
}

HalAccessMC5103::HalAccessMC5103(HalBoardMC5103 *board, HalAccessOptions *opt){
    if(!_board->check()) return;
    
}

int HalAccessMC5103::sync(int value, int *error){
    if(!_board->check()) return 0;
    PL_Word result = 0;
    //_board->plSync(access, value, &result);
    return result;
}

int HalAccessMC5103::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    if(!_board->check()) return HAL_ERROR;
    //return _board->plReadMemBlock(access, (PL_Word *)dstHostAddr, srcBoardAddr, size);
    return HAL_NOT_IMPLEMENTED;
}

int HalAccessMC5103::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    if(!_board->check()) return HAL_ERROR;
    //return _board->plWriteMemBlock(access, (PL_Word *)srcHostAddr, dstBoardAddr, size);
    return HAL_NOT_IMPLEMENTED;
}

int HalAccessMC5103::getResult(int *error){
    return 0;
}

int HalAccessMC5103::loadProgramFile(const char* program_name){
    return HAL_NOT_IMPLEMENTED;

}

int HalAccessMC5103::getStatus(int *error){
    return 0;
}

HalAccessMC5103::~HalAccessMC5103(){
    if(!_board->check()) return;
    //_board->plCloseAccess(access);
}