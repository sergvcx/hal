#include "hal.h"
#include "section-hal.h"
//#include "stdio.h"
#include "nmlink.h"
#include "led.h"

#define LINK_PORT 2
#define SIZE 16

INSECTION(".data_imu7") int data[16384];

INSECTION(".text_shared0") int main()
{
	halLinkInit();
	for (int i = 0; i < 16384; i++) {
		data[i] = i;
	}
	int counter = 0;
	for (int i = 0; i < 100; i++) {		
		halLed4Lo(counter++ % 16);
		halLinkStart(data, SIZE, LINK_PORT, LINK_OUTPUT);
		while (halLinkIsCompleted(LINK_PORT, LINK_OUTPUT) == 0);
		halSleep(10);
	}
	return 0x600D600D;
} 

