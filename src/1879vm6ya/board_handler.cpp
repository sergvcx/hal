#include "hal-core.h"
#include "hal/hal-options.h"
#include <iostream>

HalBoard *createBoard_MB7707(HalBoardOptions *board_options){
    std::cout << "open board mb7707 not implemented" << std::endl;
    return 0;
}
HalBoard *createBoard_MC12101(HalBoardOptions *board_options){
    std::cout << "open board mc12101 not implemented" << std::endl;
    return 0;
}
HalBoard *createBoard_MC12705(HalBoardOptions *board_options){
    std::cout << "open board mc12705 not implemented" << std::endl;
    return 0;
}

HalBoard::~HalBoard(){
    
}

int HalBoard::open(){
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::loadInitCode(){
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::close(){
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::reset(){
    return HAL_NOT_IMPLEMENTED;
}

unsigned int HalBoard::count(int *error){
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    return 0;
}

HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    return 0;
}

int HalAccess::open(){
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::close(){
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::sync(int value, int *error){
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    return 0;
}

int HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::getResult(int *error){    
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    return 0;
}

int HalAccess::loadProgramFile(const char* program_name){
    return HAL_NOT_IMPLEMENTED;
}
int HalAccess::loadProgramFile(const char* program_name, const char *mainArgs){    
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::getStatus(int *error){
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    return 2;
}

HalAccess::~HalAccess(){
    
}

void *IHalAccessIO::getOpsForIO(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}