#ifndef __HAL_HOST_H_INCLUDED
#define __HAL_HOST_H_INCLUDED
#include <stddef.h>
#include <stdint.h>

#include "hal/hal.h"

typedef struct PL_Board PL_Board;
typedef struct PL_Access PL_Access;


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


    HalBoard *halAllocBoard();
    int halBoardSetOption(HalBoard * board, int option, ...);
    int halBoardOpen(HalBoard *board);
    int halBoardClose(HalBoard *board);
    int halBoardReset(HalBoard *board);
    int halBoardGetCount(HalBoard *board);
    int halFreeBoard(HalBoard *board);
    int halLoadInitCode(HalBoard *board);
    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor);
    int halGetVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor);
    
    PL_Board *halGetNativeBoard(HalBoard *board);
    PL_Access *halGetNativeAccess(HalAccess *access);
    
	void *halLoadExtensionFunction(HalBoard *board, const char* functionName);

    int halLoadProgramFile(HalAccess *access, const char *program_name);
    int halLoadProgramFileArgs(HalAccess *access, const char *program_name, const char *args);
    //int halLoadProgramFileArgs(HalAccess *access, const char *program_name, int argc, char **argv);
    int halLoadProgram(HalAccess *access, const void *addrProgram, unsigned int sizeProgram);
    

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H_INCLUDED