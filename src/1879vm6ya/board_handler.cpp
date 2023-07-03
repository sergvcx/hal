#include "hal-core.h"
#include "hal/hal-options.h"
#include <iostream>
#include "mc12101load_nm.h"

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
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::loadInitCode(){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::close(){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalBoard::reset(){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

void *HalBoard::loadExtensionFunc(const char *funcname){
    return 0;
}

PL_Board *HalBoard::native(){
    return 0;
}

unsigned int HalBoard::count(int *error){
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    return 0;
}

HalAccess *HalBoard::getAccess(HalAccessOptions *options){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return 0;
}

PL_Access *HalAccess::native(){
    return 0;
}


int HalAccess::open(){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::close(){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::sync(int value, int *error){
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return 0;
}

int HalAccess::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::getResult(int *error){    
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return 0;
}

int HalAccess::loadProgramFile(const char* program_name){
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::loadProgramFile(const char* program_name, const char *mainArgs){    
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return HAL_NOT_IMPLEMENTED;
}

int HalAccess::getStatus(int *error){
    if(error != NULL){
        *error = HAL_NOT_IMPLEMENTED;
    }
    Log(LOG_WARNING).get() << __FUNCTION__ << " not realized"; 
    return 2;
}

HalAccess::~HalAccess(){
    
}

void *IHalAccessIO::getOpsForIO(){
    std::cout << "Warning: " << __FUNCTION__ << " not implemented" << std::endl;
    return 0;
}