#include "led.h"
#include "sleep.h"
#include "stdio.h"
#include "malloc.h"
#include "hal.h"

DISABLE_PRINTF();

void blink(int led, int count, int ms){
	for(int i=0; i<count; i++){
		halLedOn(led);
		halSleep(ms);
		halLedOff(led);
		halSleep(ms);
	}
}
int main()
{ 
	
	printf("Starting led test...\n");
	printf("switch off all leds\n");
	halLed(0x0);
	halSleep(1000);
	for(int led=0; led<halLedCount(); led++){
		halLedOn(led);
		halSleep(100);
	}
	//return 8;
	
	for(int led=0; led<halLedCount(); led++){
		halLedOff(led);
		halSleep(100);
	}
	halSleep(1000);
	for(int led=0; led<halLedCount(); led++){
		halLedBlink(led,20,1000);
	}
	
	halLedSOS(0,4);
	return 0x600D;
	
	
} 