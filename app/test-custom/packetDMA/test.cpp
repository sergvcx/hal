#include "time.h"
#include "chain7707.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"

void initChain7707(int *buf);
extern chain7707 chain;


#define MAX_NUM_BUFFERS 16  // Maximum buffers in packet
#define MAX_BUFFER_SIZE 16	// Maximum size of each buffer 
#define ALIGN( addr, numInts) (((((unsigned)addr)+numInts-1)>>(numInts-1))<<(numInts-1)) // align "addr" address to boundary of numInts 32-bit words

int chainBuf[MAX_NUM_BUFFERS*16];
int main()
{ 
	clock_t t0,t1;
	initChain7707((int*)chainBuf); // ������ ������ ������� � ������ ������� chainBuf
	

	int numBuffers = 16; // counter of buffers in packet 1

	
			

	for (int srcBank = 0; srcBank < 1; srcBank++) {
		for (int dstBank = 0; dstBank < 3; dstBank++) {
			nmppsMallocSetRoute16(0xF00 | (dstBank<<4) | drcBank);
			//nm32s* src = (nm32s*)0x00040000; // (MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			//nm32s* dst = (nm32s*)0x00048000; // (MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			//nmppsMallocSetRoute16(0xF00);
			nm32s* src = nmppsMalloc_32s(MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			nm32s* dst = nmppsMalloc_32s(MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
	
			nmppsRandUniform_32s(src, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);
			nmppsSet_32s(dst, 0xcccccccc, MAX_NUM_BUFFERS*MAX_BUFFER_SIZE * 2);

			if (src == 0 || dst == 0)
				return -1;

			nm32s* srcAddrList[MAX_NUM_BUFFERS];
			nm32s* dstAddrList[MAX_NUM_BUFFERS];
			int    bufSizeList[MAX_NUM_BUFFERS + 1];


			unsigned crcSrc = 0;
			int bufSize = 16;
			for (int i = 0; i < MAX_NUM_BUFFERS; i++) {
				srcAddrList[i] = nmppsAddr_32s((nm32s*)ALIGN(src, 16), i*bufSize * 2);
				dstAddrList[i] = nmppsAddr_32s((nm32s*)ALIGN(dst, 16), i*bufSize * 2);
				bufSizeList[i] = bufSize;

				nmppsCrcAcc_32s(srcAddrList[i], bufSize, &crcSrc);
			}
			bufSizeList[MAX_NUM_BUFFERS] = 0;

			t0 = clock();
			int err = halInitPacketDMA((void**)srcAddrList, (void**)dstAddrList, (int*)bufSizeList);

			if (err)	return -2;

			int status;
			unsigned time = 0;
			do {
				halSleep(1);
				if (time++ > 2000)
					return 333;
				status = xdmac0();
			} while (status);
			t1 = clock();

			unsigned crcDst = 0;
			for (int i = 0; i < MAX_NUM_BUFFERS; i++) {
				nmppsCrcAcc_32s(dstAddrList[i], bufSize, &crcDst);
			}
			if (crcSrc - crcDst)
				return 9;
			
			nmppsFree(src);
			nmppsFree(dst);
			
		}
	}
	
	
	//return t1-t0;		
	
	return 777;		

}
