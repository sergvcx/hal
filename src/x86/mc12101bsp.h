#ifndef __MC12101BSP_H_INCLUDED
#define __MC12101BSP_H_INCLUDED
#include "library.h"
#include "hal-core.h"

struct PL_Access;
struct IO_Service;
struct PL_Board;

struct BoardInterfaceMC12101 : public IPLoadInterface{
private:
    void init(LibraryHandle handle);
    bool remoted;
public:
    PL_Board *desc;
    unsigned int index;
    LibraryHandle handle;
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
    
    bool init(const char *library_name);
    unsigned int count(int *error) override;
    int open() override;
    int close() override;
    int reset() override;
    int loadInitCode() override;
    PL_Board* native() override;

    IHalAccess *getAccess(HalCore *core) override;

    void *loadExtensionFunc(const char *funcname) override;

    ~BoardInterfaceMC12101() override;
};


struct HalAccessMC12101 : public IHalAccess, public IHalAccessIO{
public:
    BoardInterfaceMC12101 *interface;
    IPLoadInterface *pload_interface;

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
    PL_Access *access;
    IO_Service *io; 
    int core;
    char program[256];
     
    struct {
        int (*plReadMemBlock)(PL_Access *, void *, int, int);
        int (*plWriteMemBlock)(PL_Access *, const void *, int, int); 
    } ops;
    HalAccessMC12101(BoardInterfaceMC12101 *board, HalCore *core);
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
    ~HalAccessMC12101();
};

#endif //__MC12101BSP_H_INCLUDED

