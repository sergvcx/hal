#include "hal-core.h"
#include "library.h"
#include "mc12705bsp.h"
#include <iostream>
#include "assert.h"

#include <cstring>

void BoardInterfaceMC12705::init(LibraryHandle handle){
    plGetCount = (int(*)(unsigned int*))library_get_addr(handle, "PL_GetBoardCount");
    plGetDesc = (int (*)(unsigned int, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
    plCloseDesc = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
    plGetAccess = (int (*)(PL_Board *, PL_Core*, PL_Access**))library_get_addr(handle, "PL_GetAccess");
    plCloseAccess = (int (*)(PL_Access *))library_get_addr(handle, "PL_CloseAccess");
    plReadMemBlock = (int (*)(PL_Access *, void *, int addr, int))library_get_addr(handle, "PL_ReadMemBlock");
    plWriteMemBlock = (int (*)(PL_Access *, const void *, int addr, int))library_get_addr(handle, "PL_WriteMemBlock");
    plLoadProgramFile = (int (*)(PL_Access *, const char *))library_get_addr(handle, "PL_LoadProgramFile");
    plLoadProgramFileArgs = (int (*)(PL_Access *, const char *, const char *))library_get_addr(handle, "PL_LoadProgramFileArgs");
    plGetStatus = (int (*)(PL_Access *, PL_Word *))library_get_addr(handle, "PL_GetStatus");
    plGetResult = (int (*)(PL_Access *, PL_Word *))library_get_addr(handle, "PL_GetResult");
    plSync = (int (*)(PL_Access *, int, int *))library_get_addr(handle, "PL_Sync");
    plReset = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
    plLoadInitCode = (int (*)(PL_Board*))library_get_addr(handle, "PL_LoadInitCode");
}

unsigned int BoardInterfaceMC12705::count(int *error){
    unsigned int result;
    int _error = plGetCount(&result);
    if(error) *error = _error;
    return result;
}

HalBoard *createBoard_MC12705(HalBoardOptions *options){
    return new HalBoardMC12705(options);
}

HalBoardMC12705::HalBoardMC12705(HalBoardOptions *options) {
    remoted = 0;
    desc = 0;
    is_initialized = 0;
    if(options->server_enabled){
        remoted = 1;
        handle = open_library("mc12705load_proxy");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12705load_proxy not found";
            return;
        }
        auto rplConnectToHost = (int (*)(const char *, int))library_get_addr(handle, "PL_ConnectToHost");
        int error = rplConnectToHost(options->server_ip, options->server_port);        
    } else {
        handle = open_library("mc12705load");
        if(handle == 0){
            Log(LOG_ERROR).get() << "Library mc12705load not found";
            return;
        }
    }
    binterface.init(handle);
    board_interface = dynamic_cast<IPLoadInterface *>(&binterface);

    board_type = HAL_MC12705;
    board_no = options->board_no;

    is_initialized = 1;

    
}

int BoardInterfaceMC12705::open(){
    //INF_LOG
    assert(plGetDesc);
    return plGetDesc(index, &desc);    
}

int BoardInterfaceMC12705::close(){
    assert(plCloseDesc);
    if(desc){
        return plCloseDesc(desc);
    } else {
        return 0;
    }
}

int BoardInterfaceMC12705::reset(){
    assert(plReset);
    return plReset(desc);
}

int BoardInterfaceMC12705::loadInitCode(){
    assert(plLoadInitCode);
    return plLoadInitCode(desc);
}


PL_Board* BoardInterfaceMC12705::native()
{
    return desc;
}


HalBoardMC12705::~HalBoardMC12705(){
    close();
    if(remoted){
        auto rplDisconnect = (int (*)())library_get_addr(handle, "RPL_DisconnectFromHost");
        rplDisconnect();
    } else {
        close_library(handle);
    }
}




void* HalBoardMC12705::loadExtensionFunc(const char* function_name) {
    return library_get_addr(handle, function_name);
}

HalAccess *HalBoardMC12705::getAccess(HalAccessOptions *options) {
    HalAccess *access = new HalAccessMC12705(this, options);
    return access;
}

int HalAccessMC12705::open() {
    assert(_board->binterface.plGetAccess);
    return _board->binterface.plGetAccess(_board->desc, (PL_Core*)&core, &access);
}
int HalAccessMC12705::close() {
    assert(_board->binterface.plCloseAccess);
    return _board->binterface.plCloseAccess(access);
}


HalAccessMC12705::HalAccessMC12705(HalBoardMC12705 *board, HalAccessOptions *opt){
    _board = board;
    
    assert(_board->binterface.plReadMemBlock);
    assert(_board->binterface.plWriteMemBlock);
    ops.plReadMemBlock = _board->binterface.plReadMemBlock;
    ops.plWriteMemBlock = _board->binterface.plWriteMemBlock;

    core.core = opt->core;
    core.cluster = opt->cluster;    
}

PL_Access *HalAccessMC12705::native(){
    return access;
}


int HalAccessMC12705::sync(int value, int *error){
    int result = 0;
    int _error = _board->binterface.plSync(access, value, &result);
    if(error != NULL){
        *error = _error;
    }
    return result;
}

int HalAccessMC12705::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    assert(_board->binterface.plReadMemBlock);
    return _board->binterface.plReadMemBlock(access, dstHostAddr, (int)srcBoardAddr, size);    
}

int HalAccessMC12705::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    assert(_board->binterface.plWriteMemBlock);
    return _board->binterface.plWriteMemBlock(access, srcHostAddr, (int)dstBoardAddr, size);    
}

int HalAccessMC12705::getResult(int *error){
    PL_Word result;
    assert(_board->binterface.plGetResult);
    int _error = _board->binterface.plGetResult(access, &result);
    if (error) *error = _error;
    if(_error){
        std::cout << "Failed get result" << std::endl;
    }
    return (int)result;
}

int HalAccessMC12705::loadProgramFile(const char* program_name){
    strcpy(program, program_name);
    assert(_board->binterface.plLoadProgramFile);
    return _board->binterface.plLoadProgramFile(access, program_name);
}

int HalAccessMC12705::loadProgramFile(const char* program_name, const char *mainArgs){
    strcpy(program, program_name);
    assert(_board->binterface.plLoadProgramFileArgs);
    return _board->binterface.plLoadProgramFileArgs(access, program_name, mainArgs);
}

int HalAccessMC12705::getStatus(int *error){
    PL_Word result;
    assert(_board->binterface.plGetStatus);
    int _error = _board->binterface.plGetStatus(access, &result);
    if (error) *error = _error;
    if(_error){
        std::cout << "Failed get status" << std::endl;
    }
    return (int)result;
}

void* HalAccessMC12705::getOpsForIO() {
    Log(LOG_DEBUG1).get() << __FUNCTION__;
    return &ops;
}


HalAccessMC12705::~HalAccessMC12705(){
    close();
}