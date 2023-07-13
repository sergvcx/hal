#include "hal/hal.h"
#include "hal/hal-options.h"
#include "stdio.h"

const int SIZE32=1024;
unsigned int srcHostArray[SIZE32];
unsigned int dstHostArray[SIZE32];

int main(){

	for(int i=0;i<SIZE32;i++)	
		srcHostArray[i]=i;
	
	unsigned srcBoardAddr;
	unsigned dstBoardAddr;
	unsigned sync;
	printf("Openning...\n");
	HalBoardOptions *options = halCreateBoardOptions();
	halSetBoardOption(options, HAL_BOARD_TYPE, HAL_MC12101);
	halSetBoardOption(options, HAL_BOARD_NUMBER, 0);

	HalBoard *board = halGetBoardOpt(options, NULL);
	if (board == 0){			
		return -1;
	}

	HalCore core;
	core.core = 0;

	HalAccess *access = halGetAccess(board, &core, NULL);
	if (access == 0){			
		return -1;
	}
	HalAccess *servAccess = halGetAccess(board, &core, NULL);
	if (servAccess == 0){			
		return -1;
	}

	if( halLoadProgramFile(access, "nm_sync_mc12101.abs")){
		printf("Failed load program\n");
		return 1;
	}	
	
	sync = halSync(access, 0x8086, NULL);			// Receive handshake (0x6406)
	srcBoardAddr=halSync(access, SIZE32, NULL);			// Receive array address to write  on board  , send size of array
	dstBoardAddr=halSync(access, 123, NULL);				// Receive array address to read from board  , send increment value
	halWriteMemBlock(access, srcHostArray, srcBoardAddr, SIZE32); // Write data to board 
	sync        =halSync(access, 0x600DB00F, NULL);		// Send end of write status
	// 										// ..... NMC working ...
	sync        =halSync(access, 0x600DBA1, NULL); 		// Wait ready to read status , send/receive last sync (G000dBuy)
	halReadMemBlock (access, dstHostArray, dstBoardAddr, SIZE32); // read data from board
	for(int i=0; i<SIZE32;i++) printf("%d\n",dstHostArray[i]);
	
	//sync        =halSync(0x6000DA1); 	// wait ready to read status , send/receive last sync (G000dBuy)
	dstBoardAddr  =halSync(access, 0x1, NULL); 		// Receive second array address to read from board 
	halReadMemBlock (access, dstHostArray, dstBoardAddr, SIZE32); // read data from board
	
											
	sync = halGetResult(access, NULL);					// get return value (0x600D)
	// printf("Return value:%X\n",sync);
	halCloseAccess(access);
	halCloseBoard(board);								// close board, disconect from shared memory
	printf("Sleep 5 sec...\n",sync);
	//::Sleep(5000);
	return sync;

}