#ifndef __HAL_CORE_H_INCLUDED__
#define __HAL_CORE_H_INCLUDED__
#include "hal/hal.h"
#include "library.h"

#ifdef __NM__
#define HAL_VIRTUAL
#else
#define HAL_VIRTUAL virtual
#endif

#ifdef __cplusplus
struct HalBoard{
protected:
    HalBoard(){};
public:
    int board_type;
    static HalBoard *createBoard_MC12101(int index);
    //static HalBoard *createBoard(HalAccessOptions *options);
    static HalBoard *createHost();
    HAL_VIRTUAL void open();
    HAL_VIRTUAL void close();
    HAL_VIRTUAL void reset();
    HAL_VIRTUAL HalAccess *getAccess(HalAccessOptions *options);
    HAL_VIRTUAL ~HalBoard();
};

struct HalAccess{
protected:
    HalAccess(){};
public:
    HalBoard *board;

    HAL_VIRTUAL int sync(int value);
    HAL_VIRTUAL void readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size);
    HAL_VIRTUAL void writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size);
    HAL_VIRTUAL int getResult();
    HAL_VIRTUAL void loadProgram(const char* program_name);
    HAL_VIRTUAL int getStatus();

    HAL_VIRTUAL ~HalAccess();
};
#endif



struct HalBoardOptions{
    int board_no;
    int board_type;
};

struct HalAccessOptions{
    int core;
    int cluster;
};


#endif //__HAL_CORE_H_INCLUDED__