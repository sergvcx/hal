#include "hal.h"
static int size32_loc;
static int height_loc;
static int* src_loc;
static int* dst_loc;
static int stride_src_loc;
static int stride_dst_loc;
static DmaCallback user_callback_loc;


void halInitMatrixDMA_asm(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
DmaCallback readCallback();	

static int own_callback(){
	if(--height_loc){
		halInitSingleDMA(src_loc,dst_loc,size32_loc);
	}else{
		halSetCallbackDMA(user_callback_loc);	
		halInitSingleDMA(src_loc,dst_loc,size32_loc);
		return 0;
	}
	src_loc += stride_src_loc + size32_loc;
	dst_loc += stride_dst_loc + size32_loc;
	return 0;
}


int halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32){
	int test_src         = (int)src;
	int test_width       = (int)width;
	int test_srcStride32 = (int)srcStride32;
	int test_dstStride32 = (int)dstStride32;
	int test_dst         = (int)dst;
	int check = test_src | test_width | test_srcStride32 | test_dstStride32 | test_dst; 
	if(check<<28){
		if(height == 0 || width ==0){
			halInitSingleDMA(src,dst,0);
			return 0;
		}
		if(height == 1){
			halInitSingleDMA(src,dst,width);
		}else{
			user_callback_loc = readCallback();
			halSetCallbackDMA(own_callback);
			///wrt param for nex call back
			size32_loc = width;
			height_loc = height - 1;
			src_loc = (int*)((int)src + srcStride32 + width);
			dst_loc = (int*)((int)dst + dstStride32 + width);
			stride_src_loc = srcStride32;
			stride_dst_loc = dstStride32;
			halInitSingleDMA(src,dst,width);
		}
	}else{
		if(height == 1){
			halInitSingleDMA(src,dst,width);
		}else {
			halInitMatrixDMA_asm(src,width,height,srcStride32,dst,dstStride32);
		}
	}
	return 0;
}