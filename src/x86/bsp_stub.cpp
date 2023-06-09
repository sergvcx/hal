#include "hal-core.h"
#include <iostream>

// HalBoard:HalBoard(){
//     std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
// }

void HalBoard::open(){
    std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
};
void HalBoard::close(){
    std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
}
void HalBoard::reset(){
    std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
}
HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
    return 0;
}
HalBoard::~HalBoard() {
    std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
}

// HalAccess::HalAccess(){

// }

int HalAccess::sync(int value){
    std::cout << "HalAccess not reailized! Use derived structs" << std::endl;
    return 0;
}
void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << "HalAccess not reailized! Use derived structs" << std::endl;
}
void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << "HalAccess not reailized! Use derived structs" << std::endl;
}
int HalAccess::getResult(){
    std::cout << "HalAccess not reailized! Use derived structs" << std::endl;
    return 0;
}
void HalAccess::loadProgram(const char* program_name){
    std::cout << "HalAccess not reailized! Use derived structs" << std::endl;
}
int HalAccess::getStatus(){
    std::cout << "HalAccess not reailized! Use derived structs" << std::endl;
    return 0;
}

HalAccess::~HalAccess() {
    std::cout << "HalAccess not reailized! Use derived structs" << std::endl;    
};


