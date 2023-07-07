#include <assert.h>
#include "hal/hal.h"
#include "hal/hal-options.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#define PRINT_TEST_NAME() printf("[TEST]: %s\n", __FUNCTION__); fflush(stdout);

HalBoard *arrangeFirstBoardMC12705(){
    
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12705);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);

    int error = 1;    
    HalBoard *board = halGetBoardOpt(options, &error);
    halResetBoard(board);

    halDestroyBoardOptions(options);
    return board;
}


void test_halGetBoardCount_whenNoBoard_shouldOccurError(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    int error = 0;

    // Act
    int count = halGetBoardOpt(options, &error);

    // Assert
    assert(error == HAL_BAD_ARGUMENT);

    // Free
    halDestroyBoardOptions(options);
}

void test_halGetBoard_whenNoBoard_shouldOccurError(){
    PRINT_TEST_NAME();
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

void test_halGetBoardCount_whenChoosedMC12705_shouldGetCountOfBoards(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12705);
    int error = 1;

    // Act
    int count = halGetBoardCount(options, &error);

    // Assert
    assert(error == HAL_OK);
    assert(count >=0);

    // Free
    halDestroyBoardOptions(options);
    printf("MC12705 counts: %d\n", count);  
}

void test_halGetBoard_whenChoosedFirstBoardOfMC12705_shouldGetBoard(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12705);
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


void test_halGetAccess_whenMC12705FirstBoardOpened_shouldGetAccessZeroCore(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12705();
	assert(board !=0 );
    HalCore core;
    core.core = 0;
    core.cluster = 0;
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

}

void test_halGetAccess_whenMC12705FirstBoardOpened_shouldGetAccessFirstCore(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12705();
    HalCore core;
    core.core = 1;
    core.cluster = 0;
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

void test_halGetAccess_whenMC12705FirstBoardOpened_shouldGetAccessBothCores(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12705();
    HalCore core0;
    HalCore core1;
    int errorGetAccess0;
    int errorGetAccess1;
    core0.core = 0;
    core0.cluster = 0;
    core1.core = 1;
    core1.cluster = 0;

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

void test_halLoadProgramFile_whenOpenedFirstBoardMC12705ZeroCoreWrongProgramFile_shouldReturnFailed(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12705();
    HalCore core;
    core.core = 0;
    core.cluster = 0;   
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


void test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12705ZeroCoreWrongProgramFile_shouldReturnFailed(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12705();
    HalCore core;
    core.core = 0;
    core.cluster = 0;
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

void test_halResult_whenOpenedFirstBoardMC12705ZeroCoreLoadFactorial_shouldGetCorrectResult(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12705();
    assert(board);
    HalCore core;
    core.core = 0;
    core.cluster = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);
    error = halLoadProgramFile(access, "mc12705/factorial.abs");
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

void test_printf_whenUsedHalIO_shouldGetDoneCorrectly(){
    // PRINT_TEST_NAME();
    // HalCore core;
    // core.core = 0;
    // core.cluster = 0;
    // HalIO *io_service;
	// unsigned int boardCount;
    // const char *program = "mc12705/main_arguments.abs";
    // int error = 1;

    // HalBoard *board = arrangeFirstBoardMC12705();

	// HalAccess *access = halGetAccess(board, &core, &error);
    // assert(error == 0);

	// HalAccess *servAccess = halGetAccess(board, &core, &error);
    // assert(error == 0);
	
	// error = halLoadProgramFile(access, program);
    // assert(error == 0);

    // HalIO *halIO = halStartIO(servAccess, program, NULL);
    // assert(halIO);

	// int status = 0x0;

	// while (1)
	// {
	// 	Sleep(1);

	// 	error = halGetStatus(access, &status);
    //     assert(error == 0);

	// 	if (status & HAL_PROGRAM_FINISHED);
	// 		break;
	// }

    // int result = halGetResult(access, &error);
    // assert(result == 4);

    // halStopIO(halIO);

	// error = halCloseAccess(servAccess);
    // assert(error == 0);

	// error = halCloseAccess(access);
	// assert(error == 0);

	// error = halCloseBoard(board);
	// assert(error == 0);

	// printf("Test Ok (if you can see print from NM).\n");
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
    //setvbuf(stdout, NULL, _IOLBF, 0);
    test_halGetBoardCount_whenNoBoard_shouldOccurError();
    test_halGetBoard_whenNoBoard_shouldOccurError();
    test_halGetBoardCount_whenChoosedMC12705_shouldGetCountOfBoards();    
    test_halGetAccess_whenMC12705FirstBoardOpened_shouldGetAccessZeroCore();    
    test_halGetAccess_whenMC12705FirstBoardOpened_shouldGetAccessFirstCore();   
    test_halGetAccess_whenMC12705FirstBoardOpened_shouldGetAccessBothCores();
    test_halLoadProgramFile_whenOpenedFirstBoardMC12705ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12705ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halResult_whenOpenedFirstBoardMC12705ZeroCoreLoadFactorial_shouldGetCorrectResult();
    test_printf_whenUsedHalIO_shouldGetDoneCorrectly();
    return 0;
}