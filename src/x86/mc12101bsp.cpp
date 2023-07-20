#include "hal-core.h"
#include "library.h"
#include "mc12101bsp.h"
#include <iostream>
#include <cstring>
#undef NDEBUG
#include <assert.h>


void BoardInterfaceMC12101::init(LibraryHandle _handle){
    handle = _handle;
    plGetCount = (int(*)(unsigned int*))library_get_addr(handle, "PL_GetBoardCount");
    plGetDesc = (int (*)(unsigned int, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
    plCloseDesc = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
    plReset = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
    plLoadInitCode = (int (*)(PL_Board *))library_get_addr(handle, "PL_LoadInitCode");
    plGetAccess = (int (*)(PL_Board *, int, PL_Access**))library_get_addr(handle, "PL_GetAccess");
    plCloseAccess = (int (*)(PL_Access *))library_get_addr(handle, "PL_CloseAccess");
    plReadMemBlock = (int (*)(PL_Access *, void *, int addr, int))library_get_addr(handle, "PL_ReadMemBlock");
    plWriteMemBlock = (int (*)(PL_Access *, const void *, int addr, int))library_get_addr(handle, "PL_WriteMemBlock");
    plLoadProgramFile = (int (*)(PL_Access *, const char *))library_get_addr(handle, "PL_LoadProgramFile");
    plLoadProgramFileArgs = (int (*)(PL_Access *, const char *, const char *))library_get_addr(handle, "PL_LoadProgramFileArgs");
    plGetStatus = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetStatus");
    plGetResult = (int (*)(PL_Access *, unsigned int *))library_get_addr(handle, "PL_GetResult");
    plSync = (int (*)(PL_Access *, int, int *))library_get_addr(handle, "PL_Sync");
    plSyncArray = (int (*)(PL_Access *, int, int, int, int *, int *, int*))library_get_addr(handle, "PL_SyncArray");
    
}

BoardInterfaceMC12101::~BoardInterfaceMC12101(){
    close_library(handle);
}

bool BoardInterfaceMC12101::init(const char *library_name){
    handle = open_library(library_name);
    if(handle == 0) return false;
    init(handle);
    return true;
}

unsigned int BoardInterfaceMC12101::count(int *error){
    unsigned int result;
    int _error = plGetCount(&result);
    if(error != 0) *error = _error;
    return result;
}

int BoardInterfaceMC12101::open(){
    return plGetDesc(index, &desc);
}

int BoardInterfaceMC12101::close(){
    return plCloseDesc(desc);    
}


int BoardInterfaceMC12101::reset(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return plReset(desc);
}

int BoardInterfaceMC12101::loadInitCode() {
    return plLoadInitCode(desc);
}


PL_Board* BoardInterfaceMC12101::native() {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return desc;
}

void *BoardInterfaceMC12101::loadExtensionFunc(const char *funcname){
    return library_get_addr(handle, funcname);
}

IHalAccess *BoardInterfaceMC12101::getAccess(HalCore *core){
    return new HalAccessMC12101(this, core);
}


HalAccessMC12101::HalAccessMC12101(BoardInterfaceMC12101 *board, HalCore *_core){
    //interface = &board->board_interface;
    pload_interface = board;
    core = _core->core;
    access = 0;
    io = nullptr;
    
    plGetAccess = (int (*)(PL_Board *, int, PL_Access**))board->loadExtensionFunc("PL_GetAccess");
    plCloseAccess = (int (*)(PL_Access *))board->loadExtensionFunc("PL_CloseAccess");
    plReadMemBlock = (int (*)(PL_Access *, void *, int addr, int))board->loadExtensionFunc("PL_ReadMemBlock");
    plWriteMemBlock = (int (*)(PL_Access *, const void *, int addr, int))board->loadExtensionFunc("PL_WriteMemBlock");
    plLoadProgramFile = (int (*)(PL_Access *, const char *))board->loadExtensionFunc("PL_LoadProgramFile");
    plLoadProgramFileArgs = (int (*)(PL_Access *, const char *, const char *))board->loadExtensionFunc("PL_LoadProgramFileArgs");
    plGetStatus = (int (*)(PL_Access *, unsigned int *))board->loadExtensionFunc("PL_GetStatus");
    plGetResult = (int (*)(PL_Access *, unsigned int *))board->loadExtensionFunc("PL_GetResult");
    plSync = (int (*)(PL_Access *, int, int *))board->loadExtensionFunc("PL_Sync");
    plSyncArray = (int (*)(PL_Access *, int, int, int, int *, int *, int*))board->loadExtensionFunc("PL_SyncArray");

    //ops.plReadMemBlock = interface->plReadMemBlock;
    //ops.plWriteMemBlock = interface->plWriteMemBlock;
    ops.plReadMemBlock = plReadMemBlock;
    ops.plWriteMemBlock = plWriteMemBlock;
}

int HalAccessMC12101::open(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return plGetAccess(pload_interface->native(), core, &access);
}

int HalAccessMC12101::close(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    if(access == 0) return HAL_OK;
    int error = plCloseAccess(access);
    if(error == HAL_OK){
        access = 0;
    }    
    return error;
}

PL_Access *HalAccessMC12101::native(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return access;
}

int HalAccessMC12101::sync(int value, int *error){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    assert(plSync);
    int result;
    int _error = plSync(access, value, &result);
    if(error != NULL){
        *error = _error;
    }
    return result;
}

int HalAccessMC12101::syncArray(HalSyncArrayData *src, HalSyncArrayData *dst){
    assert(plSyncArray);
    int srcAddr = src->addr;
    int dstAddr;
    int dstLength;
    int error = plSyncArray(access, src->value, src->addr, src->length, &dst->value, &dstAddr, &dstLength);
    dst->addr = dstAddr; 
    dst->length = dstLength;
    return error;
}

int HalAccessMC12101::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    assert(plReadMemBlock);
    return plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);
}

int HalAccessMC12101::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    assert(plWriteMemBlock);
    return plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);
}

int HalAccessMC12101::getResult(int *error){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    unsigned int result;
    int _error = plGetResult(access, &result);
    if(error != NULL){
        *error = _error;
    }
    return (int)result;
}

int HalAccessMC12101::loadProgramFile(const char* program_name){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    
    strcpy(program, program_name);
    Log(LOG_DEBUG2).get() << "Loading " << program_name;
    if ( FILE *file = fopen(program_name, "r") ){
        fclose(file);
        Log(LOG_DEBUG2).get() << program_name << " exist: " << true;
    } else {
        Log(LOG_DEBUG2).get() << program_name << " exist: " << false;
    }
    assert(plLoadProgramFile);
    return plLoadProgramFile(access, program_name);
}

int HalAccessMC12101::loadProgramFile(const char* program_name, const char *mainArgs){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    strcpy(program, program_name);
    Log(LOG_DEBUG2).get() << "Loading " << program_name;
    if ( FILE *file = fopen(program_name, "r") ){
        fclose(file);
        Log(LOG_DEBUG2).get() << program_name << " exist: " << true;
    } else {
        Log(LOG_DEBUG2).get() << program_name << " exist: " << false;
    }
    
    assert(plLoadProgramFileArgs);
    return plLoadProgramFileArgs(access, program_name, mainArgs);
}

int HalAccessMC12101::getStatus(int *error){
    //Log(LOG_DEBUG1).get() << __FUNCTION__;
    unsigned int result;
    int _error = plGetStatus(access, &result);
    if (error) *error = _error;
    return (int)result;
}

void *HalAccessMC12101::getOpsForIO(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return &ops;
}


HalAccessMC12101::~HalAccessMC12101(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    close();
}