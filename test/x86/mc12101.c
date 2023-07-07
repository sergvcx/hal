#include <assert.h>
#include "hal/hal.h"
#include "hal/hal-options.h"
#include "string.h"
#include "stdlib.h"
//#include "io_host.h"

#define PRINT_TEST() printf("[TEST]: %s\n", __FUNCTION__); fflush(stdout);

HalBoard *arrangeFirstBoardMC12101(){
    
    HalBoardOptions *options = halCreateBoardOptions();
    halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
    halSetBoardOption(options, HAL_BOARD_NUMBER, 0);

    //int count = halGetBoardCount(options, &error);

    int error = 1;    
    HalBoard *board = halGetBoardOpt(options, &error);

    halDestroyBoardOptions(options);
    return board;
}


void test_halGetBoardCount_whenNoBoard_shouldOccurError(){
    PRINT_TEST();
    
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
    PRINT_TEST();
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
}

void test_halGetBoardCount_whenChoosedMC12101_shouldGetCountOfBoards(){
    PRINT_TEST();
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
    
}

void test_halGetBoard_whenChoosedFirstBoardOfMC12101_shouldGetBoard(){
    PRINT_TEST();
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
}

void test_halGetNativeAccess_whenMC12101FirstBoardOpened_shouldNonZeroValue(){
    PRINT_TEST();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
	assert(board !=0 );
    HalCore core;
    core.core = 0;
    int error = 1;
    HalAccess *access = halGetAccess(board, &core, &error);

    // Act
    PL_Access *native_access = halGetNativeAccess(access);

    // Assert
    assert(native_access);

    //Free
    halCloseAccess(access);
    halCloseBoard(board);
}  

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessZeroCore(){
    PRINT_TEST();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
	assert(board !=0 );
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
}

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessFirstCore(){
    PRINT_TEST();
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
}

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessBothCores(){
    PRINT_TEST();
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
}

void test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed(){
    PRINT_TEST();
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
}

void test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful(){
    PRINT_TEST();
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
}

void test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed(){
    PRINT_TEST();
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
}

void test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful(){
    PRINT_TEST();
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
}

void test_halResult_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldGetResult(){
    PRINT_TEST();
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
}

void test_factorial_whenOpenedFirstBoardMC12101ZeroCoreLoadFactorial_shouldGetCorrectResult(){
    PRINT_TEST();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;

    // Act        
    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);
    error = halLoadProgramFile(access, "mc12101/factorial.abs");
    assert(error == HAL_OK);
    
    
    while(1){
        int status = halGetStatus(access, &error);
        if(status & HAL_PROGRAM_FINISHED) break;
        //printf("status = %d\n", status);
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
}

// void test_printf_whenIOService_shouldGetDoneCorrectly(){
//     printf("[TEST]: %s\n", __FUNCTION__);    
//     HalCore core;
//     core.core = 0;
//     HalIO *io_service;
// 	unsigned int boardCount;
//     const char *program = "mc12101/printf_nm_part.abs";
//     int error = 1;

//     HalBoard *board = arrangeFirstBoardMC12101();

// 	HalAccess *access = halGetAccess(board, &core, &error);
//     assert(error == 0);

// 	HalAccess *servAccess = halGetAccess(board, &core, &error);
//     assert(error == 0);
	
// 	error = halLoadProgramFile(access, program);
//     assert(error == 0);

//     IO_Service *io_service = IO_ServiceStart(program, halGetNativeAccess(access));
//     assert(io_service);

// 	int status = 0x0;

// 	while (1)
// 	{
// 		Sleep(1);

// 		error = halGetStatus(access, &status);
//         assert(error == 0);

// 		if (status & HAL_PROGRAM_FINISHED);
// 			break;
// 	}

//     int result = halGetResult(access, &error);
//     assert(result == 4);

//     IO_ServiceStop(&io_service);

// 	error = halCloseAccess(servAccess);
//     assert(error == 0);

// 	error = halCloseAccess(access);
// 	assert(error == 0);

// 	error = halCloseBoard(board);
// 	assert(error == 0);

// 	printf("Test Ok (if you can see print from NM).\n");
// }

void test_printf_whenUsedHalIO_shouldGetDoneCorrectly(){
    PRINT_TEST();
    HalCore core;
    core.core = 0;
    HalIO *io_service;
	unsigned int boardCount;
    const char *program = "mc12101/printf_nm_part.abs";
    int error = 1;

    HalBoard *board = arrangeFirstBoardMC12101();

	HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == 0);

	HalAccess *servAccess = halGetAccess(board, &core, &error);
    assert(error == 0);
	
	error = halLoadProgramFile(access, program);
    assert(error == 0);

    HalIO *halIO = halStartIO(servAccess, program, NULL);
    assert(halIO);

	int status = 0x0;

	while (1)
	{
		Sleep(1);

		error = halGetStatus(access, &status);
        assert(error == 0);

		if (status & HAL_PROGRAM_FINISHED);
			break;
	}

    int result = halGetResult(access, &error);
    assert(result == 4);

    halStopIO(halIO);

	error = halCloseAccess(servAccess);
    assert(error == 0);

	error = halCloseAccess(access);
	assert(error == 0);

	error = halCloseBoard(board);
	assert(error == 0);

	printf("Test Ok (if you can see print from NM).\n");
}

void test_mainArguments_shouldDoneCorrectly(){
    PRINT_TEST();
    // Arrange
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core;
    core.core = 0;
    int error = 1;
    const char *program = "mc12101/print_main_arguments.abs";
    const char *args = "one two three";

    HalAccess *access = halGetAccess(board, &core, &error);
    assert(error == HAL_OK);
    error = halLoadProgramFileArgs(access, "mc12101/print_main_arguments.abs", "one two three");
    assert(error == HAL_OK);

    HalAccess *servAccess = halGetAccess(board, &core, &error);
    assert(error == 0);
    HalIO *halIO = halStartIO(servAccess, program, NULL);
    assert(halIO);
    
    
    // Act
    while((halGetStatus(access, &error) & HAL_PROGRAM_FINISHED) == 0){
        assert(error == HAL_OK);
    }
    int result = halGetResult(access, &error);

    // Assert
    assert(error == HAL_OK);
    assert(result == 0);

    // Free
    halStopIO(halIO);

	error = halCloseAccess(servAccess);
    assert(error == 0);
    error = halCloseAccess(access);
    assert(error == HAL_OK);
    error = halCloseBoard(board);
    assert(error == HAL_OK);
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
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessZeroCore();    
    test_halGetNativeAccess_whenMC12101FirstBoardOpened_shouldNonZeroValue();    
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessFirstCore();   
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessBothCores();
    test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful();
    test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful();
    test_halResult_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldGetResult();
    test_factorial_whenOpenedFirstBoardMC12101ZeroCoreLoadFactorial_shouldGetCorrectResult();
    //test_printf_whenIOService_shouldGetDoneCorrectly();
    test_printf_whenUsedHalIO_shouldGetDoneCorrectly();
    test_mainArguments_shouldDoneCorrectly();
    printf("[TEST] ALL DONE\n");
    return 0;
}