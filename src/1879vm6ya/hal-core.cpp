#include <iostream>
#include "mc12101load_nm.h"
#include "hal/sleep.h"
#include "hal/utils.h"
#include "hal/section.h"
#include "logger.h"
#include "hal/hal.h"
#include "options-core.h"

static const int CORE_COUNT=2;

#ifdef __cplusplus
		extern "C" {
#endif

struct SyncBuf {
		int 	stateDMA;	// -1  free	
							// 0 used by core 0
							// 1 used by core 1
		//3 varibels below use for peterson algorithm in critical section
		int 	turn; 
		int 	flag0;
		int 	flag1;
		int		readCounter[2];
		int		writeCounter[2];
		int     sync0;
		int     sync1;
		int 	handshake[5];
		int 	ringBufferDMA;
		void*	syncAddr0;
		void*	syncAddr1;
	};

struct HalAccess{
    HalBoard *board;
    int core;
};
struct HalBoard{
    HalAccess cores[CORE_COUNT];
    SyncBuf *buf;
    int selfCoreNo;
};

#ifdef __GNUC__
/**************************NMC-GCC************************/
__attribute__((section(".data.hal.syncro"))) SyncBuf halSyncro={-1,0,0,0,0,0,0,0,0,0};
static __attribute__((section(".data.hal"))) HalBoard selfBoard;
static __attribute__((section(".data.hal"))) HalBoard hostBoard;

//__attribute__((section(".data"))) int procNo=-1;

#else
/***************************NMCC*************************/
#pragma data_section ".data_hal_syncro"
  SyncBuf halSyncro={-1,0,0,0,0,0,0,0,0,0};
#pragma data_section ".data"

#endif

    static void initHalBoard(HalBoard *board, int selfCore){
        board->buf = &halSyncro;
        board->cores[0].core = 0;
        board->cores[1].core = 1;
        board->cores[0].board = board;
        board->cores[1].board = board;
        board->selfCoreNo = selfCore;
    }


    
    HalBoard *halGetBoardOpt(HalBoardOptions *board_options, int *error){
        if(board_options == NULL){
            initHalBoard(&selfBoard, ncl_getProcessorNo());
            if(error) *error = 0;
            return &selfBoard;
        } else if(board_options->board_type == HAL_HOST){
            initHalBoard(&hostBoard, -1);
            if(error) *error = 0;
            return &hostBoard;
        } else{
            if(error) *error = HAL_NOT_IMPLEMENTED;
            return 0;
        }
        
    }

    unsigned int halGetBoardCount(HalBoardOptions *board_options, int *error){
        return 0;
    }

    int halGetFirmwareVersion(HalBoard *board, unsigned int *version_major, unsigned int *version_minor){
        return 0;
    }
    int halGetVersion(unsigned int *version_major, unsigned int *version_minor){
        return 0;
    }

    

    int halCloseBoard(HalBoard *board){
        return 0;
    }
    int halBoardReset(HalBoard *board){
        return 0;
    }

    int halLoadInitCode(HalBoard *board){
        return 0;
    }

    int halGetProcessorNo(){
        return ncl_getProcessorNo();
    }
 
    HalAccess *halGetAccess(HalBoard *board, HalCore *core, int *error){
        if(board == NULL){
            if(core == NULL){
                initHalBoard(&hostBoard, -1);
                return hostBoard.cores;
            } else {
                if(error) *error = HAL_BAD_ARGUMENT;
                return 0;
            }   
        }

        if(core == NULL){
            if(error) *error = HAL_BAD_ARGUMENT;
            return NULL;
        }

        if(core->core > 1 || core->core < 0){
            if(error) *error = HAL_BAD_ARGUMENT;
            return 0;
        } else {
            if(error) *error = HAL_OK;
            return selfBoard.cores + core->core;
        }
        
    }

    PL_Access *halGetNativeAccess(HalAccess *access){
        return 0;
    }

    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options, int *error){
        return 0;
    }
    int halAccessClose(HalAccess *access){
        return 0;
    }

    //HalIO *halStartIO(HalAccess *access, FILE *file);
    //void halStopIO(HalIO *io);
	void *halLoadExtensionFunction(HalBoard *board, const char* functionName){
        return 0;
    }

    int halLoadProgramFile(HalAccess *access, const char *program_name){
        return 0;
    }
    int halLoadProgramFileArgs(HalAccess *access, const char *program_name, const char *args){
        return 0;
    }
    int halLoadProgram(HalAccess *access, const void *addrProgram, unsigned int sizeProgram){
        return 0;
    }
    //HalIO *halStartIO(HalAccess *access, const char *program_file, FILE *file){
    //    return 0;
    //}
    //int halStopIO(HalIO *hal_io){
    //    return 0;
    //}


    int halSync(HalAccess *access, int value, int *error){
        int procNo = access->board->selfCoreNo;
        if(procNo == -1){
            int result = ncl_hostSync(value);
            if(error) *error = 0;
            return result;
        }

	    int sync = 0;
	    SyncBuf& syncro= *access->board->buf;
	    if (procNo==0){
    		while(syncro.writeCounter[0]>syncro.readCounter[0]){
			    //printf("%d,%d,%d,%d\n", syncro.writeCounter[0], syncro.readCounter[0], syncro.writeCounter[1], syncro.readCounter[1]);
			    halSleep(1);
		    }
		    syncro.sync0=value;
		    syncro.writeCounter[0]++;

		    while(syncro.readCounter[1]==syncro.writeCounter[1]){
    			//printf("%d,%d,%d,%d\n", syncro.writeCounter[0], syncro.readCounter[0], syncro.writeCounter[1], syncro.readCounter[1]);
			    halSleep(1);
		    }
		    sync=syncro.sync1;
		    syncro.readCounter[1]++;
	    }
	    else if (procNo==1){

		    while(syncro.writeCounter[1]>syncro.readCounter[1]){
    			//printf("%d,%d,%d,%d\n", syncro.writeCounter[0], syncro.readCounter[0], syncro.writeCounter[1], syncro.readCounter[1]);
			    halSleep(1);
		    }
		    syncro.sync1=value;
		    syncro.writeCounter[1]++;

		    while(syncro.readCounter[0]==syncro.writeCounter[0]){
    			//printf("%d,%d,%d,%d\n", syncro.writeCounter[0], syncro.readCounter[0], syncro.writeCounter[1], syncro.readCounter[1]);
			    halSleep(1);
		    }
		    sync=syncro.sync0;
		    syncro.readCounter[0]++;

	    }

	    return sync;
    }

    int halSyncArray(HalAccess *access, HalSyncArrayData *src, HalSyncArrayData *dst){
        int outAddr = src->addr;
        int outLength = src->length;
        dst->value = ncl_hostSyncArray(src->value, &outAddr, outLength, (void **)&dst->addr, &dst->length);
        return 0;
    }

    int halReadMemBlock (HalAccess *access, void* dstHostAddr, uintptr_t srcBoardAddr, size_t size32){
        return 0;
    }
    int halWriteMemBlock(HalAccess *access, const void* srcHostAddr, uintptr_t dstBoardAddr, size_t size32){
        return 0;
    }

    int halGetResult(HalAccess *access, int *error){
        return 0;
    }
    int halGetStatus(HalAccess *access, int *error){
        return 0;
    }

    
    int halSetTimeout(int msec){
        return 0;
    }


#ifdef __cplusplus
};
#endif

