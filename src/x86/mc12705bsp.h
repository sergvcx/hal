#ifndef __MC12705BSP_H_INCLUDED
#define __MC12705BSP_H_INCLUDED
#include "library.h"
#include "hal-core.h"
#include "hal/hal-options.h"

struct PL_Access;
struct IO_Service;
struct PL_Board;
struct PL_Core;

typedef unsigned long PL_Word;
typedef unsigned long PL_Addr;


struct HalBoardMC12705: public HalBoard{
public:
    PL_Board *desc;
    int board_no;
    LibraryHandle handle;
    int remoted;
    unsigned int board_count;
   
    int (*plGetCount)(unsigned int*);
    int (*plGetVersion)(unsigned int *, unsigned int *);
    int (*plGetDesc)(unsigned int, PL_Board **);
    int (*plReset)(PL_Board *board);
    int (*plCloseDesc)(PL_Board *);
    int (*plGetSerialNumber)(PL_Board *, unsigned long *);
    int (*plGetFirwareVersion)(PL_Board *, unsigned int *, unsigned int *);
    int (*plLoadInitCode)(PL_Board *board);
    int (*plGetAccess)(PL_Board *, PL_Core *, PL_Access**);
    int (*plCloseAccess)(PL_Access *);
    int (*plReadMemBlock)(PL_Access *, void *, int, int);
    int (*plWriteMemBlock)(PL_Access *, const void *, int, int); 
    int (*plLoadProgram)(PL_Access *, const void *, unsigned int);
    int (*plLoadProgramFile)(PL_Access *, const char *);
    int (*plLoadProgramFileArgs)(PL_Access *, const char *, const char *);
    int (*plReadRegister)(PL_Access *, PL_Word *, PL_Addr);
    int (*plWriteRegister)(PL_Access *, PL_Word, PL_Addr);
    int (*plGetStatus)(PL_Access *, PL_Word *);
    int (*plGetResult)(PL_Access *, PL_Word *);
    int (*plSync)(PL_Access *, int, int *);    

    HalBoardMC12705(HalBoardOptions *board_options);
    ~HalBoardMC12705() override;

    int open() override;
    int loadInitCode() override;
    int close() override;
    int reset() override;
    HalAccess *getAccess(HalAccessOptions *options) override;
    unsigned int count(int *error) override;
    PL_Board* native() override;
    void* loadExtensionFunc(const char* function_name) override;
};

struct HalAccessMC12705 : public HalAccess{
public:
    HalBoardMC12705 *_board;
    PL_Access *access;
    IO_Service *io; 
    struct {
        int core;
        int cluster;
    } core;
    char program[256];
     
    struct {
        int (*plReadMemBlock)(PL_Access *, void *, int, int);
        int (*plWriteMemBlock)(PL_Access *, const void *, int, int); 
    } ops;
    HalAccessMC12705(HalBoardMC12705 *board, HalAccessOptions *opt);

    PL_Access *native() override;
    int open() override;
    int close() override;
    int sync(int value, int *error) override;
    int readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size) override;
    int writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size) override;
    int getResult(int *error) override;
    int loadProgramFile(const char* program_name) override;
    int loadProgramFile(const char* program_name, const char *mainArgs) override;
    int getStatus(int *error) override;
    ~HalAccessMC12705() override;
};

#endif //__MC12705BSP_H_INCLUDED
