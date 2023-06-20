#include "mb7707bsp.h"
#include "string.h"


HalBoard *createBoard_MB7707(HalBoardOptions *board_options){
    return new HalBoardMB7707(board_options->host_mac_addr);
}
    
HalBoardMB7707::HalBoardMB7707(const unsigned char* host_mac_addr){
    is_initialized = 0;
    is_opened = 0;
    strcpy((char*)mac_addr, (const char*)host_mac_addr);
    handle = open_library("mb7707load");
    
    if(handle == 0){
        INF_LOG("Library not found");
        return;
    }

    plGetVersion        = (int (*)(int *, int *))library_get_addr(handle, "PL_GetVersion");
    plGetBoardDesc      = (int (*)(const unsigned char *, PL_Board **))library_get_addr(handle, "PL_GetBoardDesc");
	plCloseBoardDesc    = (int (*)(PL_Board *))library_get_addr(handle, "PL_CloseBoardDesc");
	plResetBoard        = (int (*)(PL_Board *))library_get_addr(handle, "PL_ResetBoard");
	plLoadInitCode      = (int (*)(PL_Access *))library_get_addr(handle, "PL_LoadInitCode");
	plGetAccess         = (int (*)(PL_Board *, int procNo, PL_Access **))library_get_addr(handle, "PL_GetAccess");
	plCloseAccess       = (int (*)(PL_Access *))library_get_addr(handle, "PL_CloseAccess");
	plLoadProgramFile   = (int (*)(PL_Access *, const char *))library_get_addr(handle, "PL_LoadProgramFile");
	plReadMemBlock      = (int (*)(PL_Access *, PL_Word *, PL_Addr, PL_Word))library_get_addr(handle, "PL_ReadMemBlock");
	plWriteMemBlock     = (int (*)(PL_Access *, PL_Word *, PL_Addr, PL_Word))library_get_addr(handle, "PL_WriteMemBlock");
	plSync              = (int (*)(PL_Access *, PL_Word, PL_Word *))library_get_addr(handle, "PL_Sync");
	plSyncArray         = (int (*)(PL_Access *,PL_Word, PL_Addr ,PL_Word ,PL_Word *,PL_Addr *, PL_Word *))library_get_addr(handle, "PL_SyncArray");
	plSetTimeout        = (int (*)(int))library_get_addr(handle, "PL_SetTimeout");
	plGetStatus         = (int (*)(PL_Access *, PL_Word *))library_get_addr(handle, "PL_GetStatus");
	plGetResult         = (int (*)(PL_Access *, PL_Word *))library_get_addr(handle, "PL_GetResult");
	plFirstLightOn      = (int (*)(PL_Board *))library_get_addr(handle, "PL_FirstLightOn");
	plFirstLightOff     = (int (*)(PL_Board *))library_get_addr(handle, "PL_FirstLightOff");
	plSecondLightOn     = (int (*)(PL_Board *))library_get_addr(handle, "PL_SecondLightOn");
	plSecondLightOff    = (int (*)(PL_Board *))library_get_addr(handle, "PL_SecondLightOff");
    is_initialized = 1;
}

bool HalBoardMB7707::check(){
    return handle != 0;
}

HalBoardMB7707::~HalBoardMB7707(){
    if(!check()) return;
    close();
    close_library(handle);
}

int HalBoardMB7707::open(){
    if(!check()) return HAL_ERROR;
    int error = plGetBoardDesc(mac_addr, &desc);
    if(error == 0){
        is_opened = 1;
    }
    return error;
}
int HalBoardMB7707::close(){
    if(!check()) return HAL_ERROR;
    if(is_opened){
        return plCloseBoardDesc(desc);
    } else {
        return HAL_OK;
    }
}

int HalBoardMB7707::loadInitCode(){
    HalAccessOptions opt;
    opt.core = 0;
    HalAccessMB7707 *access = new HalAccessMB7707(this, &opt);
    if(!access) return HAL_ERROR;
    int error;
    error = access->open();
    if(error) {
        delete access;
        return error;
    }
    error = plLoadInitCode(access->access);
    int close_error = access->close();
    delete access;
    return error;
}

// unsigned int HalBoardMB7707::count(int *error){
//     if(error != NULL){
//         *error = 0;
//     }
//     return 0;
// }

int HalBoardMB7707::reset(){
    if(!check()) return HAL_ERROR;
    return plResetBoard(desc);
}

HalAccess *HalBoardMB7707::getAccess(HalAccessOptions *options){
    if(!check()) return 0;
    HalAccessMB7707 *access = new HalAccessMB7707(this, options);
    return access;
}

HalAccessMB7707::HalAccessMB7707(HalBoardMB7707 *board, HalAccessOptions *opt){
    if(!board->check()) return;
    core = opt->core;
    _board = board;
    
}

HalAccessMB7707::~HalAccessMB7707(){

}

int HalAccessMB7707::open(){
    return _board->plGetAccess(_board->desc, core, &access);
}
int HalAccessMB7707::close(){
    if(!_board->check()) return HAL_ERROR;
    return _board->plCloseAccess(access);    
}

int HalAccessMB7707::sync(int value, int *error){
    if(!_board->check()) return 0;
    PL_Word result = 0;
    _board->plSync(access, value, &result);
    return result;
}

int HalAccessMB7707::readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size){
    if(!_board->check()) return HAL_ERROR;
    return _board->plReadMemBlock(access, (PL_Word *)dstHostAddr, srcBoardAddr, size);
}

int HalAccessMB7707::writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size){
    if(!_board->check()) return HAL_ERROR;
    return _board->plWriteMemBlock(access, (PL_Word *)srcHostAddr, dstBoardAddr, size);
}

int HalAccessMB7707::getResult(int *error){
    PL_Word result;
    int _error = _board->plGetResult(access, &result);
    if(error != NULL){
        *error = _error;
    }
    return (int)result;
}


int HalAccessMB7707::loadProgramFile(const char* program_name){
    return _board->plLoadProgramFile(access, program_name);
}

int HalAccessMB7707::getStatus(int *error){
    PL_Word result;
    int _error = _board->plGetStatus(access, &result);
    return (int)result;
}
