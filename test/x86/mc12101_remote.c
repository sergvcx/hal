#include <assert.h>
#include "hal/hal.h"
#include "string.h"
#include "stdlib.h"
#include "hal/hal-host.h"

void test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithCorrectIP_shouldGetBoard(){
    // Arrange
    HalBoard *board = halAllocBoard();
    halBoardSetOption(board, HAL_BOARD_TYPE, HAL_MC12101);
    halBoardSetOption(board, HAL_BOARD_NUMBER, 0);
    char *ip = getenv("NMSERVER_IP");
    //assert(ip != NULL);    
    ip = "proton";
    halBoardSetOption(board, HAL_SERVER_IP, ip);
    halBoardSetOption(board, HAL_SERVER_PORT, 5557);
    halBoardSetOption(board, HAL_SERVER_ENABLED, 1);
    int error = 1;

    // Act
    error = halBoardOpen(board);    

    // Assert
    assert(error == HAL_OK);
    assert(board != 0);
    
    // Free
    error = halBoardClose(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithIncorrectIP_shouldReturnFailed(){
    // Arrange
    HalBoard *board = halAllocBoard();
    halBoardSetOption(board, HAL_BOARD_TYPE, HAL_MC12101);
    halBoardSetOption(board, HAL_BOARD_NUMBER, 0);
    halBoardSetOption(board, HAL_SERVER_IP, "this.is.not.ip");
    halBoardSetOption(board, HAL_SERVER_PORT, 5557);
    halBoardSetOption(board, HAL_SERVER_ENABLED, 1);
    int error = 1;

    // Act
    error = halBoardOpen(board);

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