#include "hal/hal.h"
#include "hal/hal-host.h"
#include <stdlib.h>
#include "stdio.h"


int main(){
    HalBoard *board = halAllocBoard();
    halBoardSetOption(board, HAL_BOARD_TYPE, HAL_MC12101);
    halBoardSetOption(board, HAL_BOARD_NUMBER, 0);

    HalCore core;
    core.core = 0;

    int error = halBoardOpen(board);
    if(error){
        printf("Failed get board\n");
        return 0;
    }    

    HalAccess *access = halGetAccess(board, &core, NULL);
    if(!access){
        printf("Failed get access\n");
        return 0;
    }

    error = halLoadProgramFile(access, "factorial_mc12101.abs");
    if(error){
        printf("Failed load program\n");
        halAccessClose(access);
        halBoardClose(board);
        return 1;
    }    
    
    while (1)
	{
        if (halGetStatus(access, NULL) & 2)
			break;
	}
    int result = halGetResult(access, NULL);
    printf("result: %d\n", result);

    halAccessClose(access);
    halBoardClose(board);
    return 0;
}