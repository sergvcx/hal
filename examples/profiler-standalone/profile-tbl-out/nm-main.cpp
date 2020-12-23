#include <stdio.h>

//#define DISABLE_STOPWATCH
#include "stopwatch.h"
#include "nmprofiler.h"
#include <malloc.h>
#include "hal.h"
#include "ringbuffert.h"
volatile int g=1;
extern "C"{
	 

int sum( int size){
	int sum=0;
	for(int i=0; i<size;i++){
		sum+=1;
	}
	g++;
	return sum;
}


int mycos(){
	for (int i=0; i<2; i++){
		g+=g*g;
		sum(10);
	}
	return 0;
}

	
	
int mysin(){
	for (int i=0; i<10/4; i++){
		g++;
		//sum(10);
	}
	mycos();
	return g;
}
	 
};


STOPWATCH_CREATE(tmr_all,"tmr_all");			//  tmr_all.	
STOPWATCH_CREATE(tmr_sin,"tmr_sin");			//  tmr_sin.	
STOPWATCH_CREATE(tmr_cos,"tmr_cos");			//  tmr_cos.

extern "C"{
	HalRingBufferData<TraceData,1024> nmprofiler_trace;
}




int  main(){

	
	//float d=4.4;
	//printf("%f \n",d);
	printf("head=%d\n",nmprofiler_trace.head);
	PROFILER_START();
	
	
	volatile void *a=halMapAddrTo(0,0);
	
	volatile void *b=halMapAddrFrom(0,0);
	nmprofiler_set_depth(10);
	mysin();
	PROFILER_STOP();						//  tmr_all.
	
	int *p=(int*)malloc(100);
	int *p1=(int*)malloc(100);
	//return
	printf("%x %x\n",p,p1);
	for(int i=0; i<10; i++){
	
		STOPWATCH_START(tmr_sin,"tmr_sin");			//  tmr_sin.	
		mysin();
		mysin();
		STOPWATCH_STOP(tmr_sin);					// tmr_sin.
	
		STOPWATCH_START(tmr_sin,"tmr_sin");			//  tmr_sin.	
		mysin();
		STOPWATCH_STOP(tmr_sin);					// tmr_sin.
	
	
		//PROFILER_STOP();						//  tmr_all.
		STOPWATCH_START(tmr_cos,"tmr_cos");			//  tmr_cos.
		mycos();
		mycos();
		mycos();
		STOPWATCH_STOP(tmr_cos);					//  tmr_cos.
		
	}
	PROFILER_STOP();						//  tmr_all.
	
	STOPWATCH_STOP(tmr_all);						//  tmr_all.
	
	
	PROFILER_PRINT2TBL();				//  std 
	STOPWATCH_PRINT2TBL();			
	for(;nmprofiler_trace.tail<nmprofiler_trace.head;nmprofiler_trace.tail++){
		TraceData* item=nmprofiler_trace.ptrTail();
		printf("t:%x\tfunc:%x\tdir:%d\thead:%d\n",item->time, item->func, item->dir, item->counter);
	}
	
	return (int)a;
	return 10;

}