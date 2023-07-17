#ifndef __MC5103BSP_H_INCLUDED
#define __MC5103BSP_H_INCLUDED
#include "library.h"
#include "hal-core.h"

struct PL_Access;
struct IO_Service;
struct PL_Board;
typedef unsigned long PL_Word;
typedef unsigned long PL_Addr;




struct HalBoardMC5103: public HalBoard{
private:
    bool check();
public:
    PL_Board *desc;
    LibraryHandle handle;
    int index;


    HalBoardMC5103(const unsigned char *host_mac_addr);
    ~HalBoardMC5103() override;
    

    int open() override;
    int close() override;
    int reset() override;
    HalAccess *getAccess(HalAccessOptions *options) override;
    friend struct HalAccessMC5103;
};

struct HalAccessMC5103 : public HalAccess{
public:
    HalBoardMC5103 *_board;
    PL_Access *access;    
    HalAccessMC5103(HalBoardMC5103 *board, HalAccessOptions *opt);


    int sync(int value, int *error) override;
    int readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size) override;
    int writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size) override;
    int getResult(int *error) override;
    int loadProgramFile(const char* program_name) override;
    int getStatus(int *error) override;
    ~HalAccessMC5103() override;
};

#endif //__MC5103BSP_H_INCLUDED