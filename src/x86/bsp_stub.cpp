#include "hal-core.h"
#include <iostream>

// HalBoard:HalBoard(){
//     std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
// }


int HalBoard::loadInitCode(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::open(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
};
int HalBoard::close(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
int HalBoard::reset(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}
HalBoard::~HalBoard() {
    //std::cout << __FUNCTION__ << ": HalBoard not reailized! Use derived structs" << std::endl;
}

unsigned int HalBoard::count(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}

// HalAccess::HalAccess(){

// }

int HalAccess::getError(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
int HalAccess::sync(int value){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}

PL_Access *HalAccess::getBspAccess(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}

void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
}
void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
}
int HalAccess::getResult(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}
void HalAccess::loadProgramFile(const char* program_name){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
}
void HalAccess::loadProgramFile(const char* program_name, const char *mainArgs){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
}


int HalAccess::getStatus(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}

HalAccess::~HalAccess() {
    //std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
};

void *IHalAccessIO::getOpsForIO(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return NULL;
}


