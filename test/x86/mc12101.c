#include "hal/hal.h"
#include "string.h"
#include "stdlib.h"
#include "hal/hal-host.h"
//#include "io_host.h"
#undef NDEBUG
#include <assert.h>

#define PRINT_TEST_NAME() printf("[TEST]: %s\n", __FUNCTION__); fflush(stdout);

HalBoard *arrangeFirstBoardMC12101(){
    HalBoard *board = halAllocBoard();
    halBoardSetOption(board, HAL_BOARD_TYPE, HAL_MC12101);
    halBoardSetOption(board, HAL_BOARD_NUMBER, 0);

    //int count = halGetBoardCount(options, &error);

    int error = halBoardOpen(board);    

    return board;
}


void test_halBoardGetCount_whenNoBoard_shouldOccurError(){
    PRINT_TEST_NAME();
    
    // Arrange
    int error = 0;

    HalBoard *board = halAllocBoard();

    // Act
    int count = halBoardGetCount(board);

    // Assert
    assert(count < 0);

    // Free
    halFreeBoard(board);    
}


void test_halBoardOpen_whenChoosedNoBoard_shoudReturnError(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = halAllocBoard();

    // Act
    int error = halBoardOpen(board);

    // Assert
    assert(error == HAL_ERROR);

    // Free
}

void test_halBoardOpen_whenChoosedMC12101_shoudReturnSuccess(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = halAllocBoard();
    halBoardSetOption(board, HAL_BOARD_TYPE, HAL_MC12101);

    // Act
    int error = halBoardOpen(board);

    // Assert
    assert(error == 0);

    // Free
    halBoardClose(board);
}

void test_halGetBoard_whenChoosedFirstBoardOfMC12101_shouldGetBoard(){
    PRINT_TEST_NAME();
    // Arrange
    HalBoard *board = halAllocBoard();
    halBoardSetOption(board, HAL_BOARD_TYPE, HAL_MC12101);
    halBoardSetOption(board, HAL_BOARD_NUMBER, 0);
    int error = 1;

    // Act
    error = halBoardOpen(board);

    // Assert
    assert(error == HAL_OK);
    assert(board != 0);
    
    // Free
    error = halBoardClose(board);
    halFreeBoard(board);
    assert(error == HAL_OK);
}

void test_halGetNativeAccess_whenMC12101FirstBoardOpened_shouldNonZeroValue(){
    PRINT_TEST_NAME();
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
    halAccessClose(access);
    halBoardClose(board);
}  

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessZeroCore(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessFirstCore(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessBothCores(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access0);
    assert(error == HAL_OK);
    error = halAccessClose(access1);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_halLoadProgramFile_shouldLoadNmPart(){
    PRINT_TEST_NAME();
    HalBoard *board = arrangeFirstBoardMC12101();
    HalCore core0;
    int errorGetAccess0 = -1;
    HalAccess *access0 = halGetAccess(board, &core0, &errorGetAccess0);
    assert(errorGetAccess0 == HAL_OK);
}

void test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_halResult_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldGetResult(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
    assert(error == HAL_OK);
}

void test_factorial_whenOpenedFirstBoardMC12101ZeroCoreLoadFactorial_shouldGetCorrectResult(){
    PRINT_TEST_NAME();
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
    error = halAccessClose(access);
    assert(error == HAL_OK);
    error = halBoardClose(board);
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

// 	error = halBoardClose(board);
// 	assert(error == 0);

// 	printf("Test Ok (if you can see print from NM).\n");
// }

/* void test_printf_whenUsedHalIO_shouldGetDoneCorrectly(){
    PRINT_TEST_NAME();
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

	error = halBoardClose(board);
	assert(error == 0);

	printf("Test Ok (if you can see print from NM).\n");
}

void test_mainArguments_shouldDoneCorrectly(){
    PRINT_TEST_NAME();
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
    error = halBoardClose(board);
    assert(error == HAL_OK);
}
*/

void test_sync_whenOpenedFirestBoardZeroCodeLoadSyncProgram_shoudlDoneCorrectly(){
    PRINT_TEST_NAME();
    // Arrange
	unsigned int boardCount;
    HalCore core;
    core.core = 0;
    int error = 1;

    HalBoard *board = arrangeFirstBoardMC12101();
    HalAccess *access = halGetAccess(board, &core, NULL);

    error = halLoadProgramFile(access, "mc12101/sync.abs");
    assert(error == 0);


	
	int *Buffer = NULL;
	int AddrLoad = 0;
	int AddrStore = 0;
	int syncValue = 0;
    int Length = 0;
    HalSyncArrayData srcArray;
    HalSyncArrayData dstArray;    

    // Act
    srcArray.value = 0;
    srcArray.addr = 0;
    srcArray.length = 0;
    error = halSyncArray(access, &srcArray, &dstArray);
    assert(error == 0);
    syncValue = dstArray.value;
    AddrLoad = dstArray.addr;
    Length = dstArray.length;
    assert(syncValue == 1);

    Buffer = (int*)malloc(Length * sizeof(int));
    assert(Buffer != 0);

	for (int i = 1; i <= 10; i++)
	{
		// Sync for receive array and address of output array
        srcArray.value = 1;
        srcArray.addr = 0;
        srcArray.length = 0;
        error = halSyncArray(access, &srcArray, &dstArray);
        assert(error == 0);
        
        syncValue = dstArray.value;
        AddrStore = dstArray.addr;

        assert(syncValue == 2);

        error = halReadMemBlock(access, Buffer, AddrLoad, Length);
        assert(error == 0);

		for (int j = 0; j < Length; j++)
		{
            assert(Buffer[j] == i);
			// New fill of array
			Buffer[j] = i + 1;
		}

		// Send array
		int error = halWriteMemBlock(access, Buffer, AddrStore, Length);
        assert(error == 0);

		// Sync for send array
		syncValue = halSync(access, 2, &error);
        assert(error == 0);
        assert(syncValue == 3);
	}

    free(Buffer);

    halSync(access, 17, &error);
	assert(error == 0);

    error = halAccessClose(access);
    assert(error == 0);

    error = halBoardClose(board);
    assert(error == 0);


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
    test_halBoardGetCount_whenNoBoard_shouldOccurError();
    //test_halGetBoard_whenNoBoard_shouldOccurError();
    //test_halGetBoardCount_whenChoosedMC12101_shouldGetCountOfBoards();    
    test_halBoardOpen_whenChoosedNoBoard_shoudReturnError();
    test_halBoardOpen_whenChoosedMC12101_shoudReturnSuccess();
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessZeroCore();    
    test_halGetNativeAccess_whenMC12101FirstBoardOpened_shouldNonZeroValue();    
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessFirstCore();   
    test_halGetAccess_whenMC12101FirstBoardOpened_shouldGetAccessBothCores();
    test_halLoadProgramFile_shouldLoadNmPart();
    test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halLoadProgramFile_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful();
    test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreWrongProgramFile_shouldReturnFailed();
    test_halLoadProgramFileArgs_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldReturnSuccesful();
    test_halResult_whenOpenedFirstBoardMC12101ZeroCoreDummyFile_shouldGetResult();
    test_factorial_whenOpenedFirstBoardMC12101ZeroCoreLoadFactorial_shouldGetCorrectResult();
    //test_printf_whenIOService_shouldGetDoneCorrectly();
    //test_printf_whenUsedHalIO_shouldGetDoneCorrectly();
    //test_mainArguments_shouldDoneCorrectly();
    test_sync_whenOpenedFirestBoardZeroCodeLoadSyncProgram_shoudlDoneCorrectly();
    printf("[TEST] ALL DONE\n");
    return 0;
}