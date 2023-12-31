#ifndef __MB7707BSP_H_INCLUDED
#define __MB7707BSP_H_INCLUDED
#include "library.h"
#include "hal-core.h"

struct PL_Access;
struct IO_Service;
struct PL_Board;
typedef unsigned long PL_Word;
typedef unsigned long PL_Addr;

struct BoardInterfaceMB7707 : public IPLoadInterface{
    int (*plGetVersion)(int *, int *);
    int (*plGetBoardDesc)(const unsigned char *, PL_Board **);
	int (*plCloseBoardDesc)(PL_Board *);
	int (*plResetBoard)(PL_Board *);
	int (*plLoadInitCode)(PL_Access *);
	int (*plGetAccess)(PL_Board *, int procNo, PL_Access **);
	int (*plCloseAccess)(PL_Access *);
	int (*plLoadProgramFile)(PL_Access *, const char *);
	int (*plReadMemBlock)(PL_Access *, PL_Word *, PL_Addr, PL_Word);
	int (*plWriteMemBlock)(PL_Access *, PL_Word *, PL_Addr, PL_Word);
	int (*plSync)(PL_Access *, PL_Word, PL_Word *);
	int (*plSyncArray)(PL_Access *,PL_Word, PL_Addr ,PL_Word ,PL_Word *,PL_Addr *, PL_Word *);
	int (*plSetTimeout)(int);
	int (*plGetStatus)(PL_Access *, PL_Word *);
	int (*plGetResult)(PL_Access *, PL_Word *);
	int (*plFirstLightOn)(PL_Board *);
	int (*plFirstLightOff)(PL_Board *);
	int (*plSecondLightOn)(PL_Board *);
	int (*plSecondLightOff)(PL_Board *);
    void init(LibraryHandle handle);

    unsigned int count(int *error) override;
    int open() override;
};

struct HalBoardMB7707: public HalBoard{
private:
    bool check();
    int is_opened;
public:
    PL_Board *desc;
    LibraryHandle handle;
    unsigned char mac_addr[7];
    BoardInterfaceMB7707 binterface;
    int (*plGetVersion)(int *, int *);
    int (*plGetBoardDesc)(const unsigned char *, PL_Board **);
	int (*plCloseBoardDesc)(PL_Board *);
	int (*plResetBoard)(PL_Board *);
	int (*plLoadInitCode)(PL_Access *);
	int (*plGetAccess)(PL_Board *, int procNo, PL_Access **);
	int (*plCloseAccess)(PL_Access *);
	int (*plLoadProgramFile)(PL_Access *, const char *);
	int (*plReadMemBlock)(PL_Access *, PL_Word *, PL_Addr, PL_Word);
	int (*plWriteMemBlock)(PL_Access *, PL_Word *, PL_Addr, PL_Word);
	int (*plSync)(PL_Access *, PL_Word, PL_Word *);
	int (*plSyncArray)(PL_Access *,PL_Word, PL_Addr ,PL_Word ,PL_Word *,PL_Addr *, PL_Word *);
	int (*plSetTimeout)(int);
	int (*plGetStatus)(PL_Access *, PL_Word *);
	int (*plGetResult)(PL_Access *, PL_Word *);
	int (*plFirstLightOn)(PL_Board *);
	int (*plFirstLightOff)(PL_Board *);
	int (*plSecondLightOn)(PL_Board *);
	int (*plSecondLightOff)(PL_Board *);


    HalBoardMB7707(const unsigned char *host_mac_addr);
    ~HalBoardMB7707() override;
    
    void setInterface(const char *libraryname);
    int loadInitCode() override;
    //unsigned int count(int *error) override;
    int close() override;
    int reset() override;
    HalAccess *getAccess(HalAccessOptions *options) override;
    friend struct HalAccessMB7707;
};

struct HalAccessMB7707 : public HalAccess{
public:
    HalBoardMB7707 *_board;
    PL_Access *access;    
    int core;
    int error;
    char program[256];
    HalAccessMB7707(HalBoardMB7707 *board, HalAccessOptions *opt);
    int (*plReadMemBlock)(PL_Access *, void *, int, int);
    int (*plWriteMemBlock)(PL_Access *, const void *, int, int); 
    int (*plLoadProgramFile)(PL_Access *, const char *);
    int (*plGetStatus)(PL_Access *, unsigned int *);
    int (*plGetResult)(PL_Access *, unsigned int *);
    int (*plSync)(PL_Access *, int, int *); 

    int open() override;
    int close() override;
    int sync(int value, int *error) override;
    int readMemBlock(void *dstHostAddr, uintptr_t srcBoardAddr, int size) override;
    int writeMemBlock(const void *srcHostAddr, uintptr_t dstBoardAddr, int size) override;
    int getResult(int *error) override;
    int loadProgramFile(const char* program_name) override;
    int getStatus(int *error) override;
    ~HalAccessMB7707() override;
};

#endif //__MB7707BSP_H_INCLUDED