#include "hal/hal.h"
#include <stdlib.h>
#include <iostream>
struct HalCore{
    int core;
    int cluster;
};

int main(){
    HalBoardSetting *settings = halGetBoardSettings("mc12101:0");

    HalBoard *board = halOpenBoard(settings);
    int attrib[] = {
        HAL_CLUSTER_NUMBER, 0,
        HAL_CORE_NUMBER, 0,       
        HAL_NONE};    
        //"cluster:0,core:1,io:on"
    HalCore core = {0,0};
    HalAccess *access = halCreateAccess(board, (int*)&core);
    

    halLoadProgram(access, "simple.abs");
    //halStartIOService(access, stdout);

    
    while (1)
	{
        if (halGetStatus(access) & 2)
			break;
	}
    int result = halGetResult(access);
    std::cout << "result: " << result << std::endl;

    halDestroyAccess(access);
    halCloseBoard(board);
    return 0;
}