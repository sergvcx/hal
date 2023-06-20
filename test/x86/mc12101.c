#include <assert.h>
#include "hal/hal.h"
#include "hal/hal-options.h"
#include "string.h"
#include "stdlib.h"

#define SERVER_IP 
#define SERVER_PORT 

HalBoard *arrangeFirstBoardMC12101(){
    
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);

    int error = 1;    
    HalBoard *board = halGetBoardOpt(options, &error);

    halDestroyBoardOptions(options);
    return board;
}

HalBoard *arrangeRemoteBoardMC12101(){
    
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);
    halSetBoardOption(options, HAL_SERVER_IP, "");
    halSetBoardOption(options, HAL_SERVER_PORT, 5557);
    halSetBoardOption(options, HAL_SERVER_ENABLED, 1);

    int error = 1;    
    HalBoard *board = halGetBoardOpt(options, &error);

    halDestroyBoardOptions(options);
    return board;
}

void test_halGetBoardCount_whenNoBoard_shouldOccurError(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    int error = 0;

    // Act
    int count = halGetBoardOpt(options, &error);

    // Assert
    assert(error == HAL_BAD_ARGUMENT);

    // Free
    halDestroyBoardOptions(options);
    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetBoard_whenNoBoard_shouldOccurError(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    int error = 0;

    // Act
    HalBoard *board = halGetBoardOpt(options, &error);

    // Assert
    assert(error == HAL_BAD_ARGUMENT);
    assert(board == 0);

    // Free
    halDestroyBoardOptions(options);
    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetBoardCount_whenChoosedMC12101_shouldGetCountOfBoards(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    int error = 1;

    // Act
    int count = halGetBoardCount(options, &error);

    // Assert
    assert(error == HAL_OK);
    assert(count >=0);

    // Free
    halDestroyBoardOptions(options);
    printf("mc12101 counts: %d\n", count);  
    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetBoard_whenChoosedFirstBoardOfMC12101_shouldGetBoard(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);
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

void test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithCorrectIP_shouldGetBoard(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);
    char *ip = getenv("NMSERVER_IP");
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
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessZeroCore(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;

    // Act
    HalAccess *access = halGetAccess(board, &core, &error);

    // Assert
    assert(error == HAL_OK);

    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessFirstCore(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 1;
    int error = 1;

    // Act
    HalAccess *access = halGetAccess(board, &core, &error);

    // Assert
    assert(error == HAL_OK);

    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessBothCores(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core0;
    HalCore core1;
    int errorGetAccess0;
    int errorGetAccess1;
    core0.core = 0;
    core1.core = 1;

    // Act
    HalAccess *access0 = halGetAccess(board, &core0, &errorGetAccess0);
    HalAccess *access1 = halGetAccess(board, &core1, &errorGetAccess1);
    
    // Assert
    assert(errorGetAccess0 == HAL_OK);
    assert(errorGetAccess1 == HAL_OK);

    // Free
    int error;
    error = halCloseAccess(access0);
    assert(error == HAL_OK);
    error = halCloseAccess(access1);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);

    // Act
    error = halLoadProgramFile(access, "not_existing_file.abs");

    // Assert
    assert(error == HAL_FILE);

    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);

    // Act
    error = halLoadProgramFile(access, "mc12101/dummy.abs");

    // Assert
    assert(error == HAL_OK);

    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);

    // Act
    error = halLoadProgramFileArgs(access, "not_existing_file.abs", "");

    // Assert
    assert(error == HAL_FILE);

    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);

    // Act
    error = halLoadProgramFileArgs(access, "mc12101/dummy.abs", "");

    // Assert
    assert(error == HAL_OK);

    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halResult_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldGetResult(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);
    error = halLoadProgramFile(access, "mc12101/dummy.abs");
    assert(error == HAL_OK);

    // Act
    int value = halGetResult(access, &error);

    // Assert
    assert(value == 0);
    
    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halResult_whenOpenedFirstBoardMC12101ZeroCoreLoadFactorial_shouldGetCorrectResult(){
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);
    error = halLoadProgramFile(access, "mc12101/factorial.abs");
    assert(error == HAL_OK);
    
    // Act
    while((halGetStatus(access, &error) & HAL_PROGRAM_FINISHED) == 0){
        assert(error == HAL_OK);
    }
    int result = halGetResult(access, &error);

    // Assert
    assert(error == HAL_OK);
    assert(result == 3628800);

    // Free
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

int check_param(int argc, char *argv[], const char *board){
    for(int i = 0; i < argc; i++){
        if(strcmp(board, argv[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]){
    test_halGetBoardCount_whenNoBoard_shouldOccurError();
    test_halGetBoard_whenNoBoard_shouldOccurError();
    test_halGetBoardCount_whenChoosedMC12101_shouldGetCountOfBoards();    
    test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithCorrectIP_shouldGetBoard();    
    test_halGetBoard_whenChoosedRemoteBoardOfMC12101WithIncorrectIP_shouldReturnFailed();
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessZeroCore();    
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessFirstCore();   
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessBothCores();
    test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful();
    test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful();
    test_halResult_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldGetResult();
    test_halResult_whenOpenedFirstBoardMC12101ZeroCoreLoadFactorial_shouldGetCorrectResult();
    return 0;
}