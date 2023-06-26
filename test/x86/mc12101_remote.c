#include <assert.h>
#include "hal/hal.h"
#include "hal/hal-options.h"
#include "string.h"
#include "stdlib.h"

void test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithCorrectIP_shouldGetBoard(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);
    char *ip = getenv("NMSERVER_IP");
    //assert(ip != NULL);    
    ip = "proton";
    halSetBoardOption(options, HAL_SERVER_IP, ip);
    halSetBoardOption(options, HAL_SERVER_PORT, 5557);
    halSetBoardOption(options, HAL_SERVER_ENABLED, 1);
    int error = 1;

    // Act
    HalBoard *board = halGetBoardOpt(options, &error);

    // Assert
    assert(error == HAL_OK);
    assert(board != 0);
    
    // Free
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithIncorrectIP_shouldReturnFailed(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);
    halSetBoardOption(options, HAL_SERVER_IP, "this.is.not.ip");
    halSetBoardOption(options, HAL_SERVER_PORT, 5557);
    halSetBoardOption(options, HAL_SERVER_ENABLED, 1);
    int error = 1;

    // Act
    HalBoard *board = halGetBoardOpt(options, &error);

    // Assert
    assert(error != HAL_OK);
    assert(board == 0);
    
    // Free

    printf("[ OK ] %s\n", __FUNCTION__);
}

int main(int argc, char *argv[]){
    test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithCorrectIP_shouldGetBoard();    
    test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithIncorrectIP_shouldReturnFailed();
    return 0;
}