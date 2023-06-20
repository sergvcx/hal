#include <assert.h>
#include "hal/hal.h"
#include "hal/hal-options.h"
#include "string.h"
#include "stdlib.h"

static unsigned char MAC_ADDRESS[] = { 0x10,0x62,0xEB,0xDF,0x42,0x74};

HalBoard *arrangeBoardMB7707(){
    
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MB7707);
    //char *getenv("MB7707_IP");
    const char *mac_addr = MAC_ADDRESS;
    halSetBoardOption(options, HAL_BOARD_MAC_ADDR, mac_addr);

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
    int count = halGetBoardCount(options, &error);

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

void test_halGetBoardCount_whenChooseMB7707_shouldGetCountOfBoards(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MB7707);
    //char *getenv("MB7707_IP");
    const char *mac_addr = MAC_ADDRESS;
    halSetBoardOption(options, HAL_BOARD_MAC_ADDR, mac_addr);
    int error = 1;

    // Act
    int count = halGetBoardCount(options, &error);

    // Assert
    assert(error == HAL_NOT_IMPLEMENTED);
    assert(count == 0);
    
    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halGetBoard_whenChooseFirstBoardOfMB7707_shouldGetBoard(){
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MB7707);
    //char *getenv("MB7707_IP");
    const char *mac_addr = MAC_ADDRESS;
    halSetBoardOption(options, HAL_BOARD_MAC_ADDR, mac_addr);
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

void test_halGetAccess_whenMB7707FirstBoardOpened_shouldGetAccessZeroCore(){
    // Arrange
    HalBoard *board = arrangeBoardMB7707();
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

void test_halGetAccess_whenMB7707FirstBoardOpened_shouldFailedGetAccess(){
    // Arrange
    HalBoard *board = arrangeBoardMB7707();
    HalCore core;
    core.core = 1;
    int error = 1;

    // Act
    HalAccess *access = halGetAccess(board, &core, &error);

    // Assert
    assert(error == HAL_ERROR);

    // Free
    error = halCloseBoard(board);
    assert(error == HAL_OK);

    printf("[ OK ] %s\n", __FUNCTION__);
}

void test_halLoadInitCode_whenMB7707FirstBoardOpened_shouldReturnSuccessful(){
    // Arrange
    HalBoard *board = arrangeBoardMB7707();
    HalCore core;
    core.core = 0;
    int error = 1;

    // Act
    error = halLoadInitCode(board);

    // Assert
    assert(error == HAL_OK);

    // Free
    error = halCloseBoard(board);
    assert(error == HAL_OK);
}


void test_halLoadProgramFile_whenOpenedBoardMb7707ZeroCoreWrongProgramFile_shouldReturnFailed(){
    // Arrange
    HalBoard *board = arrangeBoardMB7707();
    HalCore core;
    core.core = 0;
    int error = 1;
    error = halLoadInitCode(board);
    assert(error == HAL_OK);
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


void test_halResult_whenOpenedBoardMb7707ZeroCoreLoadFactorial_shouldGetCorrectResult(){
    // Arrange
    HalBoard *board = arrangeBoardMB7707();
    HalCore core;
    core.core = 0;
    int error = 1;
    error = halLoadInitCode(board);
    assert(error == HAL_OK);
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);
    error = halLoadProgramFile(access, "mb7707/factorial.abs");
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
    test_halGetBoardCount_whenChooseMB7707_shouldGetCountOfBoards();  
    test_halGetBoard_whenChooseFirstBoardOfMB7707_shouldGetBoard();      
    test_halGetAccess_whenMB7707FirstBoardOpened_shouldGetAccessZeroCore();    
    test_halGetAccess_whenMB7707FirstBoardOpened_shouldFailedGetAccess();
    test_halLoadInitCode_whenMB7707FirstBoardOpened_shouldReturnSuccessful();
    test_halLoadProgramFile_whenOpenedBoardMb7707ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halResult_whenOpenedBoardMb7707ZeroCoreLoadFactorial_shouldGetCorrectResult();
    return 0;
}