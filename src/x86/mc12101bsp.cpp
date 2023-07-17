#include "hal-core.h"
#include "library.h"
#include "hal/hal-options.h"
#include "mc12101bsp.h"
#include <iostream>
#include <cstring>
#undef NDEBUG
#include <assert.h>

HalBoard *createBoard_MC12101(HalBoardOptions *options){
    return new HalBoardMC12101(options);
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

void BoardInterfaceMC12101::init(LibraryHandle handle){
    plGetCount = (int(*)(unsigned int*))library_get_addr(handle, "PL_GetBoardCount");
    plGetDesc = (int (*)(unsigned int, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
    plCloseDesc = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
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
    plReset = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
    plLoadInitCode = (int (*)(PL_Board *))library_get_addr(handle, "PL_LoadInitCode");
}

HalBoardMC12101::HalBoardMC12101(HalBoardOptions *options) {
    remoted = 0;
    is_opened = 0;
    handle = 0;
    is_initialized = 0;
    if(options->server_enabled){
        handle = open_library("mc12101load_proxy");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12101load_proxy not found"; 
            return;
        }
        auto rplConnectToHost = (int (*)(const char *, int))library_get_addr(handle, "PL_ConnectToHost");
        int error = rplConnectToHost(options->server_ip, options->server_port);        
        remoted = 1;
    } else {
        handle = open_library("mc12101load");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12101load not found"; 
            return;
        }
    }
    interface.init(handle);
    board_interface = dynamic_cast<IHalBoard *>(&interface);

    board_type = HAL_MC12101;
    board_no = options->board_no;
    
    is_initialized = 1;
    
}

HalBoardMC12101::~HalBoardMC12101(){
    close();
    if(remoted){
        auto rplDisconnect = (int (*)())library_get_addr(handle, "RPL_DisconnectFromHost");
        rplDisconnect();
    } else {
        if(handle){
            close_library(handle);
        }
    }
}
PL_Board* HalBoardMC12101::native() {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return interface.desc;
}



void* HalBoardMC12101::loadExtensionFunc(const char* function_name) {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    void* result = library_get_addr(handle, function_name);
    Log(LOG_DEBUG1).get() << "MC121.01 extension func " << function_name << ": " << result;
    return result;
}


HalAccess *HalBoardMC12101::getAccess(HalAccessOptions *options) {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    HalAccess *access = new HalAccessMC12101(this, options);
    //if(access)
    return access;
}


HalAccessMC12101::HalAccessMC12101(HalBoardMC12101 *board, HalAccessOptions *opt){
    interface = &board->interface;
    core = opt->core;
    access = 0;
    io = nullptr;
    
    ops.plReadMemBlock = interface->plReadMemBlock;
    ops.plWriteMemBlock = interface->plWriteMemBlock;
}

int HalAccessMC12101::open(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return interface->plGetAccess(interface->desc, core, &access);
}

int HalAccessMC12101::close(){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    if(access == 0) return HAL_OK;
    int error = interface->plCloseAccess(access);
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
    assert(interface->plSync);
    int result;
    int _error = interface->plSync(access, value, &result);
    if(error != NULL){
        *error = _error;
    }
    return result;
}

int HalAccessMC12101::syncArray(HalSyncArrayData *src, HalSyncArrayData *dst){
    assert(interface->plSyncArray);
    int srcAddr = src->addr;
    int dstAddr;
    int dstLength;
    int error =  interface->plSyncArray(access, src->value, src->addr, src->length, &dst->value, &dstAddr, &dstLength);
    dst->addr = dstAddr; 
    dst->length = dstLength;
    return error;
}

int HalAccessMC12101::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    assert(interface->plReadMemBlock);
    return interface->plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);
}

int HalAccessMC12101::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    assert(interface->plWriteMemBlock);
    return interface->plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);
}

int HalAccessMC12101::getResult(int *error){
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    unsigned int result;
    int _error = interface->plGetResult(access, &result);
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
    assert(interface->plLoadProgramFile);
    return interface->plLoadProgramFile(access, program_name);
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
    
    assert(interface->plLoadProgramFileArgs);
    return interface->plLoadProgramFileArgs(access, program_name, mainArgs);
}

int HalAccessMC12101::getStatus(int *error){
    //Log(LOG_DEBUG1).get() << __FUNCTION__;
    unsigned int result;
    int _error = interface->plGetStatus(access, &result);
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