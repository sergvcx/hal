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

#include <stdio.h>
#include "hal_target.h"
#include "sleep.h"
#include "hal.h"
#include "led.h"

//DISABLE_PRINTF();
int main()
{
	halLedOn(0);
	halSleep(500);
	halLedOff(0);
	
	printf("Hello %d from NMC[0]\n",0);	
	for(int i=0; i<10; i++){
		printf("Hello %d from NMC[0]\n",i);	
		halSleep(500);
	}
	
	//halLedOff(1);
	halSleep(500);
	//halSleep(10000);
	return 0x600D;
}
