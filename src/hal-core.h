#ifndef __HAL_CORE_H_INCLUDED__
#define __HAL_CORE_H_INCLUDED__
#include "hal/hal.h"
#include <iostream>

#ifdef __NM__
#define HAL_VIRTUAL
#else
#define HAL_VIRTUAL virtual
#endif

#ifdef DEBUG
#   define   INF_LOG(message) std::cout << message << std::endl
#else
#   define   INF_LOG(message)
#endif


HalBoard *createBoard_MB7707(const unsigned char *host_mac_addr);
HalBoard *createBoard_MC12101(int index);

#ifdef __cplusplus
struct HalBoard{
protected:
    HalBoard(){};
public:
    int is_initialized;
    int board_type;    
    //static HalBoard *createBoard_MC12705(int index);
    //static HalBoard *createBoard_MC5103(int index);
    //static HalBoard *createBoard_MC7601(int index);
    static HalBoard *createHost(int index);
    HAL_VIRTUAL int open();
    HAL_VIRTUAL int close();
    HAL_VIRTUAL int reset();
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
    unsigned char host_mac_addr[7];
};

struct HalAccessOptions{
    int core;
    int cluster;
};


#endif //__HAL_CORE_H_INCLUDED__