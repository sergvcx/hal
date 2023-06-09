#include "hal-core.h"
#include <iostream>

// HalBoard:HalBoard(){
//     std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
// }

int HalBoard::open(){
    std::cout << __FUNCTION__ << ": HalBoard not reailized! Use derived structs" << std::endl;
    return HAL_NOT_IMPLEMENTED;
};
int HalBoard::close(){
    std::cout << __FUNCTION__ << ": HalBoard not reailized! Use derived structs" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
int HalBoard::reset(){
    std::cout << __FUNCTION__ << ": HalBoard not reailized! Use derived structs" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << __FUNCTION__ << ": HalBoard not reailized! Use derived structs" << std::endl;
    return 0;
}
HalBoard::~HalBoard() {
    //std::cout << __FUNCTION__ << ": HalBoard not reailized! Use derived structs" << std::endl;
}

// HalAccess::HalAccess(){

// }

int HalAccess::sync(int value){
    std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
    return 0;
}
void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
}
void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
}
int HalAccess::getResult(){
    std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
    return 0;
}
void HalAccess::loadProgram(const char* program_name){
    std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
}
int HalAccess::getStatus(){
    std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
    return 0;
}

HalAccess::~HalAccess() {
    //std::cout << __FUNCTION__ << ": HalAccess not reailized! Use derived structs" << std::endl;
};


