#include "hal-core.h"
#include "hal/hal-options.h"
#include <iostream>


// HalBoard *HalBoard::createBoard_MB7707(char *addr){

// }

inline HalBoard *HalBoard::createBoard_MC12101(int i){
    std::cout << "open board mc12101 not implemented" << std::endl;
    return 0;
}

inline void HalBoard::open(){
    std::cout << "close not implemented" << std::endl;
}

inline void HalBoard::close(){
    std::cout << "close not implemented" << std::endl;
}

inline void HalBoard::reset(){
    std::cout << "reset not implemented" << std::endl;
}

inline HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << "get access not implemented" << std::endl;
    return 0;
}


inline int HalAccess::sync(int value){
    std::cout << "sync not implemented" << std::endl;
    return 0;
}

inline void HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << "read not implemented" << std::endl;
}

inline void HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << "write not implemented" << std::endl;
}

inline int HalAccess::getResult(){
    std::cout << "get result not implemented" << std::endl;
    return 0;
}

inline void HalAccess::loadProgram(const char* program_name){
    std::cout << "load program not implemented" << std::endl;
}

inline int HalAccess::getStatus(){
    std::cout << "get status not implemented" << std::endl;
    return 2;
}

inline HalAccess::~HalAccess(){
    std::cout << "close access not implemented" << std::endl;
}