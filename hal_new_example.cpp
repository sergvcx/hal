#include "hal.h"
#include "stdio.h"

int main_host(){
    HalBoard *board = halOpenBoard("mc12101:0");
    if(!board){
        printf("Open board failed\n");
        return 1;
    }


    int attrib_list[] = {
        HAL_CORE_NUMBER, 0, //connect to core 0
        HAL_NONE
    };
    HalAccess *access = halCreateAccess(board, attrib_list);
    if(!access){
        printf("Get access failed\n");
        halCloseBoard(board);
        return 1;
    }


    halLoadProgram(access, "simple.abs"); // work only on host
    int ok = halGetLastError();
    if(ok){
        printf("Load program error\n")
        halDestroyAccess(access);
        halCloseBoard(board);
        return 1;
    }


    int result = halGetResult(access);
    printf("result: %d\n", result);


    halDestroyAccess(access);
    halCloseBoard(board);
    return 0;
}


int main_nm0(){
    HalBoard *board = halOpenBoard(NULL);
    //HalBoard *board = halOpenBoard("x86:usb");
    if(!board){
        printf("Open board failed\n");
        return 1;
    }


    int attrib_list[] = {
        HAL_CORE_NUMBER, 1, //connect to core 1
        HAL_NONE
    };
    HalAccess *access = halCreateAccess(board, attrib_list);
    //HalAccess *access = halCreateAccess(NULL, attrib_list);
    if(!access){
        printf("Get access failed\n");
        halCloseBoard(board);
        return 1;
    }


    int sync = halSync(access, 0xC0DE00001);
    int ok = halGetLastError();
    if(ok){
        printf("Sync error\n")
        halDestroyAccess(access);
        halCloseBoard(board);
        return 1;
    }


    halDestroyAccess(access);
    halCloseBoard(board);
    return 0;
}