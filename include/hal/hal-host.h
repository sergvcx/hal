#ifndef __HAL_HOST_H_INCLUDED
#define __HAL_HOST_H_INCLUDED
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"
#include "hal/hal.h"

typedef struct PL_Board PL_Board;
typedef struct PL_Access PL_Access;


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

    /**
     * @brief open board
     * 
     * @param board_settings
     * @return HalBoard* 
     * @details 
     */
    PL_Board *halGetNativeBoard(HalBoard *board);

    int halBoardGetCount(HalBoard *board);

    //int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor);
    //int halGetVersion(unsigned int *version_major, unsigned int *version_minor);

    
    int halBoardReset(HalBoard *board);
    int halLoadInitCode(HalBoard *board);


 
    
    PL_Access *halGetNativeAccess(HalAccess *access);

    
	void *halLoadExtensionFunction(HalBoard *board, const char* functionName);

    int halLoadProgramFile(HalAccess *access, const char *program_name);
    int halLoadProgramFileArgs(HalAccess *access, const char *program_name, const char *args);
    int halLoadProgram(HalAccess *access, const void *addrProgram, unsigned int sizeProgram);
    HalIO *halStartIO(HalAccess *access, const char *program_file, FILE *file);
    int halStopIO(HalIO *hal_io);    

    

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H_INCLUDED