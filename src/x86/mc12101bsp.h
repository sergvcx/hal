#ifndef __MC12101BSP_H_INCLUDED
#define __MC12101BSP_H_INCLUDED
#include "library.h"
#include "hal-core.h"
#include "hal/hal-options.h"

struct PL_Access;
struct IO_Service;
struct PL_Board;

struct BoardInterfaceMC12101 : public IHalBoard{
    PL_Board *desc;
    unsigned int index;
    int (*plGetCount)(unsigned int*);
    int (*plGetDesc)(unsigned int, PL_Board **);
    int (*plReset)(PL_Board *);
    int (*plLoadInitCode)(PL_Board*);
    int (*plCloseDesc)(PL_Board *);
    int (*plGetAccess)(PL_Board *, int, PL_Access**);
    int (*plCloseAccess)(PL_Access *);
    int (*plReadMemBlock)(PL_Access *, void *, int, int);
    int (*plWriteMemBlock)(PL_Access *, const void *, int, int); 
    int (*plLoadProgramFile)(PL_Access *, const char *);
    int (*plLoadProgramFileArgs)(PL_Access *, const char *, const char *);
    int (*plGetStatus)(PL_Access *, unsigned int *);
    int (*plGetResult)(PL_Access *, unsigned int *);
    int (*plSync)(PL_Access *, int, int *);
    int (*plSyncArray)(PL_Access *, int, int, int, int *, int *, int *);
    void init(LibraryHandle handle);
    unsigned int count(int *error) override;
    int open() override;
    int close() override;
    int reset() override;
    int loadInitCode() override;
};


struct HalBoardMC12101: public HalBoard{
public:
    int board_no;
    LibraryHandle handle;
    int remoted;
    unsigned int board_count;
    int is_opened;
   
    BoardInterfaceMC12101 interface;

    HalBoardMC12101(HalBoardOptions *board_options);
    ~HalBoardMC12101() override;

    HalAccess *getAccess(HalAccessOptions *options) override;    
    void* loadExtensionFunc(const char* function_name) override;
    PL_Board* native() override;
};

struct HalAccessMC12101 : public HalAccess, public IHalAccessIO{
public:
    BoardInterfaceMC12101 *interface;    
    PL_Access *access;
    IO_Service *io; 
    int core;
    char program[256];
     
    struct {
        int (*plReadMemBlock)(PL_Access *, void *, int, int);
        int (*plWriteMemBlock)(PL_Access *, const void *, int, int); 
    } ops;
    HalAccessMC12101(HalBoardMC12101 *board, HalAccessOptions *opt);
    // int (*plReadMemBlock)(PL_Access *, void *, int, int);
    // int (*plWriteMemBlock)(PL_Access *, const void *, int, int); 
    // int (*plLoadProgramFile)(PL_Access *, const char *);
    // int (*plLoadProgramFileArgs)(PL_Access *, const char *, const char *);
    // int (*plGetStatus)(PL_Access *, unsigned int *);
    // int (*plGetResult)(PL_Access *, unsigned int *);
    // int (*plSync)(PL_Access *, int, int *);     

    PL_Access *native() override;
    int open() override;
    int close() override;
    int sync(int value, int *error) override;
    int syncArray(HalSyncArrayData *src, HalSyncArrayData *dst) override;
    int readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size) override;
    int writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size) override;
    int getResult(int *error) override;
    int loadProgramFile(const char* program_name) override;
    int loadProgramFile(const char* program_name, const char *mainArgs) override;
    int getStatus(int *error) override;
    void *getOpsForIO() override;
    ~HalAccessMC12101() override;
};

#endif //__MC12101BSP_H_INCLUDED

