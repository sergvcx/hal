#include "hal-core.h"
#include <iostream>

// HalBoard:HalBoard(){
//     std::cout << "HalBoard not reailized! Use derived structs" << std::endl;
// }

int HalBoard::loadInitCode(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}

PL_Board *HalBoard::native(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
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
void* IHalExtension::loadExtensionFunc(const char *function_name) {
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}
HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}
HalBoard::~HalBoard() {
    //std::cout << __FUNCTION__ << ": HalBoard not reailized! Use derived structs" << std::endl;
}

unsigned int HalBoard::count(int *error){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    return 0;
}

// HalAccess::HalAccess(){

// }

int HalAccess::open(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
int HalAccess::close(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}

PL_Access *HalAccess::native(){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}

int HalAccess::sync(int value, int *error){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}

int HalAccess::syncArray(HalSyncArrayData *src, HalSyncArrayData *dst){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
int HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
int HalAccess::getResult(int *error){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return 0;
}
int HalAccess::loadProgramFile(const char* program_name){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}
int HalAccess::loadProgramFile(const char* program_name, const char *mainArgs){
    std::cout << "Warning: " << __FUNCTION__ << " not reailized!" << std::endl;
    return HAL_NOT_IMPLEMENTED;
}


int HalAccess::getStatus(int *error){
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


