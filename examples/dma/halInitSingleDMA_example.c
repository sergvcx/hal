#include <stdio.h>
#include <hal.h>
#include <sleep.h>
#include "time.h"
#define SIZE_MEM   32000
#define MEM_OFFSET 0x40000
#define IASH       0x40000458
#define IASH_CLR   0x4000045C
#define IRRH       0x40000440
#define IMRH       0x40000448

#pragma data_section ".data_imu1"
	int arr2read[SIZE_MEM];
#pragma data_section ".data_shared"
	int buff[SIZE_MEM+16]; 	
 	int buff_ref[SIZE_MEM+16];
 	int status;
 	
 	void ref_DMA(int  src,  int  dst,  int  size32){
 		int i;
 		int amm = size32>>1;
 		amm<<=1;
 		int* pntr_src  = (int*)src;
 		int* pntr_dst  = (int*)dst;
 		for(i=0;i<amm;i++){
 			pntr_dst[i] = pntr_src[i];
 		}
 	}
 	
 	int cmp(int amm, int* arr_1, int* arr_2){
 		int i;
 		for(i=0;i<amm;i++){
 			if(arr_1[i] != arr_2[i]){
 				return i + 1;
 			}
 		}
 		return 0;
 	}


 int user_callback(){
 		status = 1;
 		return 0;
 } 


int main(){
	int i;
	clock_t t0,t1;
	
	halEnbExtInt();
	halMaskIntContMdma_mc12101();
	halInitDMA();
	halSetCallbackDMA((DmaCallback)callback);
	
	for(i=0;i<SIZE_MEM;i++){
		arr2read[i] = i;
		buff[i] = 0;
		buff_ref[i] = 0;
	}

	printf("SRC: %x DST: %x\n",arr2read,buff);
	status = 0;
	t0=clock();
	halInitSingleDMA((arr2read + MEM_OFFSET),buff,SIZE_MEM);

	while(1){
		if(status){
			t1 = clock();
			break;
		}
	}
	
	printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",SIZE_MEM,(t1-t0));
	
	for(i=0;i<10;i++){
		printf("buff[%d] = %d\n",i,buff[i]);
	}

//case aligned address
	printf("CASE address ALIGNED as 4 the LSB are 0\n");
	int* pntr2mem;
	int aligned = (int)(int(buff + 15) >> 4);
	aligned<<=4;
	pntr2mem = (int*)aligned; 	
	printf("SRC: %x DST: %x\n",arr2read,pntr2mem);
	status = 0;
	t0=clock();
	halInitSingleDMA((arr2read + MEM_OFFSET),pntr2mem,SIZE_MEM);

	while(1){
		if(status){
			t1 = clock();
			break;
		}
	}

	ref_DMA((int)(arr2read + MEM_OFFSET),(int)buff_ref,SIZE_MEM);
	index = cmp(SIZE_MEM,pntr2mem,buff_ref);
	if(index){
		printf("ERROR: mismatch btw ref function and true DMA at %d\n",index);
	}else{
		printf("DMA has finished correctly\n");
		printf("full time used to coppy %d int32 form SRC to DST is %d clk\n",SIZE_MEM,(t1-t0));
	}
	
	for(i=0;i<10;i++){
		printf("buff[%d] = %d\n",i,buff[i]);
	}
	return 0;
}