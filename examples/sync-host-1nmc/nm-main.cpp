//------------------------------------------------------------------------
//
//
//  Copyright (c) RC Module Inc.
//
//!
//! \file   main.cpp
//! \author S. Mushkaev
//! \brief  Example of universl-board-connect usage
//!
//! \endif
//!
//------------------------------------------------------------------------


#include "hal/hal.h"
	
int main()
{
	//int* sharedBuffer=halMalloc32(2048);// Allocates shared memory (in 32-bit words) 
	int* sharedBuffer = new int[2048];// Allocates shared memory (in 32-bit words) 
	//halConnect(sharedBuffer,2048);		// Connect and share buffer to host
	if (sharedBuffer==0) return -1;
	unsigned* src = (unsigned*)sharedBuffer;
	unsigned* dst = (unsigned*)sharedBuffer + 1024;
	unsigned  sync,size,incr;

	HalAccess *host_access = halGetAccess(NULL, NULL, NULL);

	sync = halSync(host_access, 0x6407, NULL);			// Handshake 
	size = halSync(host_access, (unsigned)src, NULL);	// Gets array size, sends input buffer address
	incr = halSync(host_access, (unsigned)dst, NULL);	// Gets increment (123), sends output buffer address
	// Host >>>>>> Shared memory 		// Here host writes data to shared memory
	sync = halSync(host_access, 0x4321, NULL);			// Gets ready status (0x600DB00F)
	for (int i = 0; i<size; i++)		// Calcualates output array 
		dst[i] = src[i] + incr;
	halSync(host_access, 0x600DBEEF, NULL);			// Sends signal that array is modified and ready for reading
	// Host <<<<<< Shared memory 		// Here host reads data from shared memory
	

	//int* sharedBuffer2=halMalloc32(2048);// Allocates second shared memory (in 32-bit words) 
	int* sharedBuffer2= new int [2048];// Allocates second shared memory (in 32-bit words) 
	for (int i = 0; i<2048; i++)		// Calcualates output array 
		sharedBuffer2[i] = 1234;
	halSync(host_access, (unsigned)sharedBuffer2, NULL);			// Sends signal that array is modified and ready for reading
	// Host <<<<<< Shared memory 		// Here host reads data from shared memory
	

	//halFree(sharedBuffer);					// dealocates sharedBuffer
	return 0x600D+(halGetProcessorNo()<<28);
}
