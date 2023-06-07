#ifndef __HAL_CORE_H_INCLUDED__
#define __HAL_CORE_H_INCLUDED__
#include "hal/hal.h"
#include "library.h"

struct PL_Access;
struct IO_Service;
struct PL_Board;

class BoardHandle{
public:
    LibraryHandle handle;
    int (*plGetCount)(unsigned int*);
    int (*plGetDesc)(unsigned int, PL_Board **);
    int (*plCloseDesc)(PL_Board *);
    int (*plGetAccess_i)(PL_Board *, int, PL_Access**);
    int (*plGetAccess_ii)(PL_Board *, int *, PL_Access**);
    int (*plCloseAccess)(PL_Access *);
    int (*plReadMemBlock)(PL_Access *, void *, int addr, int);
    int (*plWriteMemBlock)(PL_Access *, const void *, int addr, int); 
    int (*plLoadProgramFile)(PL_Access *, const char *);
    int (*plGetStatus)(PL_Access *, unsigned int *);
    int (*plGetResult)(PL_Access *, unsigned int *);
    int (*plSync)(PL_Access *, int, int *);
    int (*plReset)(PL_Board *board);

    BoardHandle(const char *library);
};



struct HalBoardSetting{
    int board_no;
    int board_type;
};

struct HalBoard{
private:
    HalBoard();
public:
    PL_Board *desc;
    BoardHandle *board_handle;
    int board_type;
    int board_no;

    static HalBoard *createBoard_MC12101(int index);
    void open();
    void close();
    void reset();
    friend class HalAccess;
};

struct HalAccess{
private:
    HalAccess();
public:
    HalBoard *board;
    PL_Access *access;
    int core;
    int cluster;
    char program[256];

    static HalAccess *createAccess(HalBoard *board, int core, int cluster = 0);

    int sync(int value);
    void readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size);
    void writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size);
    int getResult();
    void loadProgram(const char* program_name);
    int getStatus();

    ~HalAccess();
};



#endif //__HAL_CORE_H_INCLUDED__