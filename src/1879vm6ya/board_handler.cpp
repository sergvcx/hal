#include "hal-core.h"
#include "hal/hal-options.h"
#include <iostream>




HalBoard *createBoard_MC12101(HalBoardOptions *options){
    std::cout << "open board mc12101 not implemented" << std::endl;
    return 0;
}

HalBoard *createBoard_MB7707(HalBoardOptions *options){
    std::cout << "open board mb7707 not implemented" << std::endl;
    return 0;
}

int HalBoard::open(){
    std::cout << "close not implemented" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}

HalBoard::~HalBoard(){
    
}

unsigned int HalBoard::count(){
    return 0;
}

int HalBoard::close(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::reset(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}

HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}


int HalAccess::sync(int value){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}

void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
}

void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
}

int HalAccess::getResult(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}

void HalAccess::loadProgram(const char* program_name){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
}

int HalAccess::getStatus(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return 2;
}

HalAccess::~HalAccess(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
}

void *IHalAccessIO::getOpsForIO(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
}