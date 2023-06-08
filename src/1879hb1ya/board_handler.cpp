#include "hal-core.h"
#include "hal/hal-options.h"


HalBoard::HalBoard(){

}

// HalBoard *HalBoard::createBoard_MB7707(char *addr){

// }

HalBoard *HalBoard::createBoard_MC12101(int i){
    std::cout << "optn board mc12101 not implemented" << std::endl;
    return board;
}

void HalBoard::open(){
    std::cout << "close not implemented" << std::endl;
}

void HalBoard::close(){
    std::cout << "close not implemented" << std::endl;
}

void HalBoard::reset(){
    std::cout << "reset not implemented" << std::endl;
}

HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << "get access not implemented" << std::endl;
    return 0;
}


HalAccess::HalAccess(){
    
}


int HalAccess::sync(int value){
    std::cout << "sync not implemented" << std::endl;
    return 0;
}

void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << "read not implemented" << std::endl;
}

void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << "write not implemented" << std::endl;
}

int HalAccess::getResult(){
    std::cout << "get result not implemented" << std::endl;
    return 0;
}

void HalAccess::loadProgram(const char* program_name){
    std::cout << "load program not implementer" << std::endl;
}

int HalAccess::getStatus(){
    std::cout << "get status not implementer" << std::endl;
    return 0;
}

HalAccess::~HalAccess(){
    std::cout << "close access not implementer" << std::endl;
}