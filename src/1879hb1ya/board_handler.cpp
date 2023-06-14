#include "hal-core.h"
#include "hal/hal-options.h"
#include <iostream>


// HalBoard *HalBoard::createBoard_MB7707(char *addr){

// }

HalBoard *createBoard_MC12101(HalBoardOptions *board_options){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}

HalBoard *createBoard_MB7707(HalBoardOptions *board_options){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}

int HalBoard::open(){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
}

int HalBoard::close(){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
}

int HalBoard::reset(){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
}

unsigned int HalBoard::count(){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}

HalBoard::~HalBoard(){

}

HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << "get access not implemented" << std::endl;
    return 0;
}


int HalAccess::sync(int value){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}

void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
}

void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
}

int HalAccess::getResult(){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}

void HalAccess::loadProgram(const char* program_name){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
}

int HalAccess::getStatus(){
    std::cout << "Warning! " << __FUNCTION__ << " not implemented" << std::endl;
    return 2;
}

HalAccess::~HalAccess(){
    //std::cout << "close access not implemented" << std::endl;
}