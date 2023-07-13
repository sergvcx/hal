#include "hal/hal.h"
#include "hal/hal-options.h"
#include <stdlib.h>
#include "stdio.h"


int main(){
    HalBoardOptions *options = halCreateBoardOptions();    
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);

    HalCore core;
    core.core = 0;

    HalBoard *board = halGetBoardOpt(options, NULL);
    if(!board){
        printf("Failed get board\n");
        return 0;
    }
    halDestroyBoardOptions(options); 

    HalAccess *access = halGetAccess(board, &core, NULL);
    if(!access){
        printf("Failed get access\n");
        return 0;
    }

    int error = halLoadProgramFile(access, "factorial_mc12101.abs");
    if(error){
        printf("Failed load program\n");
        halCloseAccess(access);
        halCloseBoard(board);
        return 1;
    }    
    
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