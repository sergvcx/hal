//typedef int(*cnt_func)();
#include "dma5103.h"
extern "C"
{ 
	void SetCallBack( int vectoraddr, int ProcAddress);
	//int dmainit( int dmahigh,  int *dstptr,  int *srcptr, int n );
	// int halDmaStart( int *q );
	//int dmac0();

	extern int ofdmainit;
	int *packet[4];
	extern int packetcnt;
	void BellFromDmaPacket()// ���������� DMA ���������� (������� 58h,60h)
	{   if (packetcnt>1)    
	    {	packetcnt--;
		packet[0]++; packet[1]++; packet[2]++;
		if (*packet[0])
		dmainit(0,(int*)*packet[2],(int*)*packet[1], *packet[0]);//OK
	    } else packetcnt=0;		
	}

	int halInitPacketDMA(void** src,  void** dst,  int* size32, int N)
	{
		if(packetcnt==-1)
		{  SetCallBack(0x58, (int)BellFromDmaPacket);
		   SetCallBack(0x60, (int)BellFromDmaPacket);
		   packetcnt=N;
		}
		packet[1]=(int*)src;
		packet[2]=(int*)dst;
		packet[0]=(int*)size32;
		statusFunc=dmainit(0,(int*)*packet[2],(int*)*packet[1], *packet[0]);//OK
		return 0;
	}

};