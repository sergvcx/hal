#ifndef __MC12101BSP_H_INCLUDED
#define __MC12101BSP_H_INCLUDED
#include "library.h"
#include "hal-core.h"
#include "hal/hal-options.h"

struct PL_Access;
struct IO_Service;
struct PL_Board;


struct HalBoardMC12101: public HalBoard{
public:
    PL_Board *desc;
    int board_no;
    LibraryHandle handle;
    int remoted;
    unsigned int board_count;
   
    int (*plGetCount)(unsigned int*);
    int (*plGetDesc)(unsigned int, PL_Board **);
    int (*plReset)(PL_Board *board);
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

    HalBoardMC12101(HalBoardOptions *board_options);
    ~HalBoardMC12101() override;

    int open() override;
    int close() override;
    int reset() override;
    HalAccess *getAccess(HalAccessOptions *options) override;
    unsigned int count() override;
};

struct HalAccessMC12101 : public HalAccess, public IHalAccessIO{
public:
    HalBoardMC12101 *_board;
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

    PL_Access *getBspAccess() override;
    int sync(int value) override;
    void readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size) override;
    void writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size) override;
    int getResult() override;
    void loadProgramFile(const char* program_name) override;
    void loadProgramFile(const char* program_name, const char *mainArgs) override;
    int getStatus() override;
    void *getOpsForIO() override;
    ~HalAccessMC12101() override;
};

#endif //__MC12101BSP_H_INCLUDED



// struct HalBoardMC5103 : public HalBoard{
//     LibraryHandle handle;
//     int (*plGetCount)(unsigned int*);
//     int (*plGetDesc)(unsigned int, PL_Board **);
//     int (*plReset)(PL_Board *board);
//     int (*plCloseDesc)(PL_Board *);
//     int (*plGetAccess)(PL_Board *, int, PL_Access**);
//     int (*plCloseAccess)(PL_Access *);
//     int (*plReadMemBlock)(PL_Access *, void *, int addr, int);
//     int (*plWriteMemBlock)(PL_Access *, const void *, int addr, int); 
//     int (*plLoadProgramFile)(PL_Access *, const char *);
//     int (*plGetStatus)(PL_Access *, unsigned int *);
//     int (*plGetResult)(PL_Access *, unsigned int *);
//     int (*plSync)(PL_Access *, int, int *);    

//     void open() override {

//     }
//     void close() override {

//     }
//     void reset() override {

//     }
// };

// struct HalBoardMB7707 : public HalBoard{
//     void open() override {

//     }
//     void close() override {

//     }
//     void reset() override {

//     }
    //int (*plGetCoreCount)()
//     #define WR_GetBoardDesc(channel_conf, board) PL_OK
// #define WR_CloseBoardDesc(board) PL_OK
// #define WR_GetVersion(board, version_major, version_minor) PL_GetVersion(version_major, version_minor)
// #define WR_GetCoreCount(board, count) PL_GetCoreCount(count)
// #define WR_GetAccess(board, coreNo, access) PL_GetAccess(coreNo, access)
// #define WR_CloseAccess(access) PL_CloseAccess(access)
// #define WR_ResetCore(access) PL_ResetCore(access)
// #define WR_LoadInitCode(access) PL_LoadInitCode(access)
// #define WR_LoadProgram(access, addrProgram, sizeProgram, timeOut) PL_LoadProgram(access, addrProgram, timeOut)
// #define WR_LoadProgramFile(access, filename, timeOut) PL_LoadProgramFile(access, filename, timeOut)
// #define WR_WaitEndProgram(access, returnValue, timeOut) PL_WaitEndProgram(access, returnValue, timeOut)
// #define WR_ReadMemBlock(access, block, address, len) PL_ReadMemBlock(access, block, address, len)
// #define WR_WriteMemBlock(access, block, address, len) PL_WriteMemBlock(access, block, address, len)
// #define WR_ReadMemBlockSafe(access, block, address, len) PL_ReadMemBlockSafe(access, block, address, len)
// #define WR_WriteMemBlockSafe(access, block, address, len) PL_WriteMemBlockSafe(access, block, address, len)
// #define WR_Sync(access, timeOut, value, returnValue) PL_Sync(access, timeOut, value, returnValue)
// #define WR_SyncArray(access, timeOut, value, outAddress, outLen, returnValue, inAddress, inLen) PL_SyncArray(access, timeOut, value, outAddress, outLen, returnValue, inAddress, inLen)
// #define WR_InterruptNMI(access) PL_InterruptNMI(access)
// #define WR_InterruptHPINT(access) PL_InterruptHPINT(access)
// #define WR_InterruptLPINT(access) PL_InterruptLPINT(access)
// #define WR_MaskInterruptHPINT(access, mask) PL_MaskInterruptHPINT(access, mask)
// #define WR_MaskInterruptLPINT(access, mask) PL_MaskInterruptLPINT(access, mask)
// #define WR_WaitInterruptHPINT(access, timeOut) PL_WaitInterruptHPINT(access, timeOut)
// #define WR_WaitInterruptLPINT(access, timeOut) PL_WaitInterruptLPINT(access, timeOut)
// #define WR_CancelWaitInterruptHPINT(access) PL_CancelWaitInterruptHPINT(access)
// #define WR_CancelWaitInterruptLPINT(access) PL_CancelWaitInterruptLPINT(access)
// #define WR_SetChannelTimeout(board, timeOut) PL_OK
// #define WR_GetChannelInf(board, chan, p1, p2, timeOut) PL_OK
//};