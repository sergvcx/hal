#include "hal/hal.h"
#include "hal/hal-options.h"
#include <stdlib.h>
#include "stdio.h"

int main(){
    HalBoardOptions *options = halCreateBoardOptions();
    HalAccessOptions *accessOptions = halCreateAccessOptions();

    //options->type(MC12101)->number(0)->ip("localhost:5557");
    halSetBoardOption(options, HAL_BOARD_TYPE, MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);

    halSetAccessOption(accessOptions, HAL_CORE, 0);
    halSetAccessOption(accessOptions, HAL_CLUSTER, 0);


    HalBoard *board = halGetBoardOpt(options);
    HalAccess *access = halGetAccessOpt(board, accessOptions);

    halDestroyBoardOptions(options); 
    halDestroyAccessOptions(accessOptions);
    

    halLoadProgram(access, "simple.abs");
    //halStartIOService(access, stdout);

    
    while (1)
	{
        if (halGetStatus(access) & 2)
			break;
	}
    int result = halGetResult(access);
    printf("result: %d\n", result);

    halCloseAccess(access);
    halCloseBoard(board);
    return 0;
}