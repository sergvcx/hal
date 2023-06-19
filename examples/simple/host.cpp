#include "hal/hal.h"
#include "hal/hal-options.h"
#include <stdlib.h>
#include "stdio.h"

//unsigned char MAC_ADDRESS[] = { 0x1A,0x2B,0x3C,0x4D,0x5E,0x6F,0,0,0,0 };
unsigned char MAC_ADDRESS[] = { 0x10,0x62,0xEB,0xDF,0x42,0x74};
//10-62-EB-DF-42-74

int main(){
    HalBoardOptions *options = halCreateBoardOptions();
    HalAccessOptions *accessOptions = halCreateAccessOptions();

    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    //halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MB7707);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);
    halSetBoardOption(options, HAL_BOARD_MAC_ADDR, MAC_ADDRESS);

    halSetAccessOption(accessOptions, HAL_CORE, 0);
    halSetAccessOption(accessOptions, HAL_CLUSTER, 0);


    HalBoard *board = halGetBoardOpt(options, NULL);
    if(!board){
        printf("Failed get board\n");
        return 0;
    }
    HalAccess *access = halGetAccessOpt(board, accessOptions, NULL);
    if(!access){
        printf("Failed get access\n");
        return 0;
    }

    halDestroyBoardOptions(options); 
    halDestroyAccessOptions(accessOptions);
    
    //int error = halLoadProgramFile(access, "simple.abs");
    int error = halLoadProgramFile(access, "e:\\Win10\\git\\hal\\examples\\simple\\factorial_mb7707.abs");
    if(error){
        printf("Failed load program\n");
        halCloseAccess(access);
        halCloseBoard(board);
        return 1;
    }
    
    // if(halGetLastError()){
    //     printf("Error occured\n");
    //     return 0;
    // }
    //halStartIOService(access, stdout);

    
    while (1)
	{
        if (halGetStatus(access, NULL) & 2)
			break;
	}
    int result = halGetResult(access, NULL);
    printf("result: %d\n", result);

    halCloseAccess(access);
    halCloseBoard(board);
    return 0;
}