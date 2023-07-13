#include "hal/hal.h"
#include "hal/hal-options.h"
#undef NDEBUG
#include "hal/hassert.h"

#define PRINT_FUNC() printf("Test: %s\n", __FUNCTION__); fflush(stdout)

void test_halGetBoardDesc_whenNeededSelfBoard_shouldReturnHalBoard(){
    PRINT_FUNC();
    // Arrange
    int error = 1;

    // Act
    HalBoard *board = halGetBoardOpt(NULL, &error);

    // Asserts
    hassert(board);
    hassert(error == 0);
}

void test_halGetAccess_whenSelectedSelfBoardNegativeCore_shouldReturnError(){
    PRINT_FUNC();
    // Arrange
    int error = 1;
    HalBoard *board = halGetBoardOpt(NULL, &error);
    HalCore core;
    core.core = -1;

    // Act
    HalAccess *access = halGetAccess(board, &core, &error);

    // Asserts
    hassert(access == 0);
    hassert(error == HAL_BAD_ARGUMENT);
}

void test_halGetAccess_whenSelectedSelfBoardGt1_shouldReturnError(){
    PRINT_FUNC();
    // Arrange
    int error = 1;
    HalBoard *board = halGetBoardOpt(NULL, &error);
    HalCore core;
    core.core = 2;

    // Act
    HalAccess *access = halGetAccess(board, &core, &error);

    // Asserts
    hassert(access == 0);
    hassert(error == HAL_BAD_ARGUMENT);
}

void test_halGetAccess_whenSelectedSelfBoardZeroCore_shouldReturnHalAccess(){
    PRINT_FUNC();
    // Arrange
    int error = 1;
    HalBoard *board = halGetBoardOpt(NULL, &error);
    HalCore core;
    core.core = 0;

    // Act
    HalAccess *access = halGetAccess(board, &core, &error);

    // Asserts
    hassert(access);
    hassert(error == 0);
}

void test_halGetAccess_whenSelectedSelfBoardFirstCore_shouldReturnHalAccess(){
    PRINT_FUNC();
    // Arrange
    int error = 1;
    HalBoard *board = halGetBoardOpt(NULL, &error);
    HalCore core;
    core.core = 1;

    // Act
    HalAccess *access = halGetAccess(board, &core, &error);

    // Asserts
    hassert(access);
    hassert(error == 0);
}

void test_halSync_whenSelectedSelfBoard_shouldReturnValue(){
    PRINT_FUNC();
    // Arrange
    int error = 1;
    HalBoard *board = halGetBoardOpt(NULL, &error);
    HalCore other;

    hassert(CORE >= 0 && CORE < 2);
    if(CORE == 0){
        other.core = 1;
    } else {
        other.core = 0;
    }
    HalAccess *access = halGetAccess(board, &other, &error);
    

    // Act
    int handshake = halSync(access, 0xC0DE0000 | CORE, &error);

    // Asserts
    hassert(handshake == (0xC0DE0000 | other.core));

}


int main(int argc, char *argv[]){
    test_halGetBoardDesc_whenNeededSelfBoard_shouldReturnHalBoard();
    test_halGetAccess_whenSelectedSelfBoardNegativeCore_shouldReturnError();
    test_halGetAccess_whenSelectedSelfBoardGt1_shouldReturnError();
    test_halGetAccess_whenSelectedSelfBoardZeroCore_shouldReturnHalAccess();
    test_halGetAccess_whenSelectedSelfBoardFirstCore_shouldReturnHalAccess();
    test_halSync_whenSelectedSelfBoard_shouldReturnValue();
    return 0;
}