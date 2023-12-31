#ifndef HAL_RINGBUFFERT_INCLUDED
#define HAL_RINGBUFFERT_INCLUDED

#include "stdlib.h"
//#include "dma.h"
#include "hal/hal.h"
#include <typeinfo>
#include "hal/utils.h"
#include "hal/dma.h"
#include "hal/sleep.h"

#include "stdio.h"
#include "nmassert.h"
//#include "memory.h"


#define RING_BUFFER_DECLARED	0xDEC1A8ED
#define RING_BUFFER_ALLOCATED	0xA10CA7ED
#define RING_BUFFER_MEM_ERROR	0xDEADF00D
#define RING_BUFFER_ERROR		0xBEDABEDA
#define RING_BUFFER_OK			0x0
#define RING_BUFFER_INCORRECT_COUNT -1
#define RING_BUFFER_SLEEP 100



#ifdef __cplusplus
extern "C" {
#endif

//typedef  void* (*tmemcopy)(void *dst,  void const *src, unsigned int size);
typedef  void* (*tmemcopy32)(const void *src, void *dst, unsigned int size);

#ifdef __cplusplus
		};
#endif


//typedef  void *(*t_bytecpy)(void *to, int toIndex, void const *from, int fromIndex, size_t size) ;


template <class T, int SIZE_> struct HalRingBufferData{
	volatile unsigned 	sizeofInt;			///< sizeof на данной платформе (1-nm , 4- x86,arm)
	volatile unsigned 	bufferId;			///
	volatile unsigned 	size;
	volatile unsigned 	size1;
	volatile unsigned 	head;				///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	volatile unsigned	tail;				///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	volatile unsigned   auxtail[4]; 		///<  additional tails
	volatile unsigned   service[6]; 		///<  service words for alignment address of data at 16-word boundary (neceassary for dma)
	
	//bool 		headLocked;
	//bool		tailLocked;
	//long long	size;			/// long long because to do 64-bit align 
	//#ifdef __NM__
	//T*	 		data[SIZE];			///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	T*	 		data;			///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	//#else 
	//T*	 		data;				///<  контейнер данных кольцевого буфера размера SIZE. SIZE - должен быть степенью двойки
	//#endif
	volatile unsigned  final[2];
	
	HalRingBufferData(){
		//size=SIZE;
		//#ifndef __NM__
		//data = (T*)halMalloc32(SIZE*sizeof32(T));
		//#else 
		sizeofInt=sizeof(int);
		bufferId = 0xDEADBEEF;
		head=0;
		tail=0;
		size=0;//SIZE;
		size1=-1;//SIZE-1;
		//#endif
	}
	~HalRingBufferData(){
		//printf("destrctor!\n");
		//#ifndef __NM__
		//halFree(data);
		//data=0;
		//#else 
		head=0xdeadb00f;
		tail=0xdeadb00f;
		//#endif
	}
	void init(int Size){
		NMASSERT((Size&(Size - 1)) == 0);
		sizeofInt = sizeof(int);
		bufferId = 0x600DB00F;
		for (int i=0; i<6; i++)
			service[i]=0xBABADEDA;
		final[0]=0xBABADEDA;
		final[1]=0xBABADEDA;
		size=Size;//Size;
		size1=size-1;
		head=0;
		tail=0;
	}
	inline bool isEmpty() {
		return head == tail;
	}
	
	inline bool isFull() {
		return head == tail + size;
	}
	
	inline T* ptrHead(){
		return data + (head & (size1));
		
	}
	
	
	inline T* ptrTail(){
		return data + (tail & (size1));
		
	}
	inline T* ptr(unsigned int idx){
		return data + (idx & (size1));
		
	}

};
//	#define HEAD (*pHead)
//	#define TAIL (*pTail)
//
//#ifdef __NM__
//	#define WRITE_HEAD(value)  	(HEAD=value);
//	#define READ_HEAD()			(HEAD)
//	#define WRITE_TAIL(value)  	(TAIL=value);
//	#define READ_TAIL()         (TAIL)
//	#define RING_ADDR32(src,disp) (((int*)src) +  (disp))
//	#define RING_ADDR_T(src,disp) ((src) +  (disp))
//#endif
//int * halMalloc323(int size);
// works! #define  halMalloc32(size) malloc (size)
void*  memCopy(const void* src, void* dst, unsigned int size32);
//extern int statusDMA;
template <class T, int SIZE> struct HalRingBufferConnector{
// Важен порядок полей, не менять!
	unsigned 	sizeofBufferInt;///< значение sizeof(int) на стороне контейнера кольцевого буффера
	unsigned 	_bufferId;		///< buffer id
	unsigned 	size;
	unsigned 	size1;
	unsigned* 	pHead;			///< адрес head в RingBufferData|
	unsigned*	pTail;			///< адрес tail в RingBufferData|
	T*	 		data;
	HalRingBufferData<T, SIZE> *container;
	bool		internalContainer;
public:
	unsigned 	polltime;		///<  время опроса кольцевого буфера если пуст или заполнен в мс
	bool		headExternalControl;
	bool  		tailExternalControl;
	unsigned    sizeof32Item;		///<  размер в int-ах
	tmemcopy32 	memcopyPush;	///<  указатель на функцию копирования типа halCopy_32s
	tmemcopy32	memcopyPop;		///<  указатель на функцию копирования типа halCopy_32s


	int check(){
		if (pHead==0 || pTail==0 || ((int)data&1)|| ((sizeofBufferInt !=4) &&  (sizeofBufferInt !=1)) && (size&&(size-1)==0)  ){
			//printf("Ring buffer %s error\n", typeid(T).name());
			printf("Ring buffer error\n");
			printf("size=0x%x\n", size);
			printf("pHead=0x%x\n",pHead); 
			printf("pTail=0x%x\n",pTail);
			printf("data=0x%x\n",data);
			printf("sizeofBufferInt=%x\n",sizeofBufferInt);
			return -1;
		} 
		return 0;
	}
	HalRingBufferConnector(){
		sizeofBufferInt = 0;
		_bufferId = 0;
		pHead=0;
		pTail=0;
		data =0;
		
		memcopyPush=0;//halCopyRISC;
		memcopyPop =0;//halCopyRISC;
		polltime=10;
		headExternalControl=false;
	  	tailExternalControl=false;
		internalContainer = false;
		sizeof32Item = sizeof32(T);
		
	}
	//void* create(tmemcopy32 _memcopyPush= halCopyRISC, tmemcopy32 _memcopyPop= halCopyRISC){
	void* create(tmemcopy32 _memcopyPush, tmemcopy32 _memcopyPop){
		int size=sizeof(HalRingBufferData<T, SIZE>) / sizeof(int);
		container = (HalRingBufferData<T,SIZE>*) halMalloc32(size);
		
		//HalRingBufferConnector(HalRingBufferData<T, SIZE>* ringBufferData, tmemcopy32 _memcopyPush , tmemcopy32 _memcopyPop ) {
		if (container){
			container->init();
			connect(container, _memcopyPush, _memcopyPop);
			internalContainer = true;
		}
		return container;
		//printf(" [%x] [%x]\n",memcopyPop, _memcopyPop);
	}
	
	//HalRingBufferConnector(HalRingBufferData<T,SIZE>* ringBufferData, tmemcopy32 _memcopyPush= halCopyRISC, tmemcopy32 _memcopyPop= halCopyRISC){
	HalRingBufferConnector(HalRingBufferData<T, SIZE>* ringBufferData, tmemcopy32 _memcopyPush , tmemcopy32 _memcopyPop ) {
		init(ringBufferData, _memcopyPush, _memcopyPop);
		internalContainer = false;
		//printf(" [%x] [%x]\n",memcopyPop, _memcopyPop);
	}
	~HalRingBufferConnector(){
		if (internalContainer)
			if (container){
				free(container);
				container =0;
			}
	}
	//int connect(void* ringBufferData, tmemcopy32 _memcopyPush = halCopyRISC, tmemcopy32 _memcopyPop = halCopyRISC) {
	int connect(void* ringBufferData, tmemcopy32 _memcopyPush , tmemcopy32 _memcopyPop ) {
		return init((HalRingBufferData<T, SIZE>*)ringBufferData, _memcopyPush, _memcopyPop);
	}
	//int init(HalRingBufferData<T,SIZE>* ringBufferData, tmemcopy32 _memcopyPush= halCopyRISC, tmemcopy32 _memcopyPop= halCopyRISC){
	int init(HalRingBufferData<T,SIZE>* ringBufferData, tmemcopy32 _memcopyPush, tmemcopy32 _memcopyPop){
		//container=0;
		memcopyPush=_memcopyPush;
		memcopyPop =_memcopyPop;
		#ifdef __NM__
			sizeofBufferInt = sizeof(int);
			_bufferId=ringBufferData->bufferId;
			size=ringBufferData->size;
			size1=ringBufferData->size1;
		#else
			memcopyPop(ringBufferData,&sizeofBufferInt,4);	// читаем заничение sizeof(int) на стороне ringbuffer
															// bufferid
															// size
															// size1 (size-1)
			//memcopyPop((unsigned (ringBufferData) + 4 * sizeofBufferInt),&size,2); // читаем size и size1
		#endif
		pHead=(unsigned*)	((unsigned)ringBufferData + 4 * sizeofBufferInt);
		pTail=(unsigned*)	((unsigned)ringBufferData + 5 * sizeofBufferInt);
		
		
		
		//data =(T*)			((unsigned)ringBufferData + 16* sizeofBufferInt) ;
		memcopyPop(			(void*)((unsigned)ringBufferData + 16* sizeofBufferInt) ,&data,1);
		
		polltime=10;
		headExternalControl=false;
	  	tailExternalControl=false;
		sizeof32Item = sizeof32(T);
		return check();
	}
	
#ifdef __NM__
	inline void     setHead(int value)		{ *pHead = value;}
	inline unsigned getHead()				{ return *pHead; }
	inline void     setTail(int value)		{ *pTail = value;}
	inline unsigned	getTail()				{ return *pTail; }
	inline T*       ptrItem(unsigned disp)	{ return (T*)(   data + (disp&(size1))); }
	inline T*       ptrHead()				{ return (T*)(data + ((*pHead)&(size1))); }
	inline T*       ptrTail()				{ return (T*)(data + ((*pTail)&(size1))); }
	//inline T*       ptrExtern(const T* base, unsigned disp) { return (T*)(base + disp); }
#else
	inline void     setHead(int value)		{ memcopyPush(&value, pHead, 1); }
	inline void     setTail(int value)		{ memcopyPush(&value, pTail, 1); }
	inline unsigned getHead()				{ unsigned res; 	memcopyPop(pHead, &res, 1); return res; }
	inline unsigned	getTail()				{ unsigned res; 	memcopyPop(pTail, &res, 1); return res; }
	
	inline   T*     ptrItem(unsigned indx)	{ return (T*)((int)data + (     indx&(size1))*sizeofBufferInt*sizeof32(T)); }
	inline   T*     ptrHead()				{ return (T*)((int)data + (getHead()&(size1))*sizeofBufferInt*sizeof32(T)); }
	inline   T*     ptrTail()				{ return (T*)((int)data + (getTail()&(size1))*sizeofBufferInt*sizeof32(T)); }
	//inline   T*     ptrExtern(const T* base, unsigned disp) { return (T*)(((int)base) +   disp*sizeofBufferInt*sizeof32(T)); }
#endif
	
	inline void     incHead() { setHead(getHead() + 1); }
	inline void     incTail() { setTail(getTail() + 1); }

	inline int popAvail(){
		return getHead() - getTail();
	}
	
	inline int pushAvail(){
		return size-(getHead() - getTail());
	}
	inline bool isEmpty() {
		check();
		return getHead() == getTail();
	}

	inline bool isFull() {
		check();
		return getHead() == getTail() + size;
	}

	void 
	#ifdef __NM__
	__attribute__((optimize("O0")))
	#endif //__NM__
	push   (const T* src, 	size_t count){
		check();
		volatile size_t fixHead= getHead();
		volatile size_t fixTail;
		do {
			fixTail = getTail();
			if (fixTail + size - fixHead >= count)
				break;
			halSleep(polltime);
		}  while (1);
		
		
		volatile size_t posHead = fixHead & (size1);			
		volatile size_t posTail = fixTail & (size1);
		T* addrHead    = ptrItem(posHead);
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead || fixHead==fixTail){
			volatile size_t countToEnd = size - posHead;
			if (count <= countToEnd){
				memcopyPush(src,addrHead,count*sizeof32(T));
			}
		// [*******<Head>......<Tail>*****]
			else {
				int firstCount = countToEnd;
				int secondCount = count-countToEnd;
				memcopyPush(src,addrHead, firstCount*sizeof32(T));
				memcopyPush(src+firstCount, data, secondCount*sizeof32(T));
			}
		}
		else {
			memcopyPush(src,addrHead, count*sizeof32(T));
		}
		if (!headExternalControl)
			setHead(fixHead + count);
	}


	void 
#ifdef __NM__
	__attribute__((optimize("O0")))
#endif //__NM__
	pop(T* dst, size_t count){
		check();
		//printf("----------\n");
		//dst[0]=111;
		volatile size_t fixHead;
		volatile size_t fixTail =getTail();
		
		
		do {
			
			fixHead=getHead();
			//printf("head=%d, tail=%d\n", fixHead, fixTail) ;
			if (fixHead-fixTail >= count)
				break;
			//halSleep(polltime);
		} while(1);
		
		volatile size_t posHead = fixHead & (size1);			
		volatile size_t posTail = fixTail & (size1);
		T* addrTail    = ptrItem(posTail);
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead){
			memcopyPop( addrTail, dst, count*sizeof32(T));
			//printf("-----1----- %x %x %x (%x) (%x)\n",addrTail, dst, count*sizeof32(T), memcopyPop, halCopyRISC);
		}
		// [*******<Head>......<Tail>*****]
		else {
			volatile size_t countToEnd = size - posTail;
			if (count <= countToEnd){
				memcopyPop(addrTail, dst, count*sizeof32(T));
				//printf("-----2-----\n");
			}
			else {
				int firstCount = countToEnd;
				int secondCount= count-countToEnd;
				memcopyPop( addrTail, dst, firstCount*sizeof32(T));
				memcopyPop( data, dst+firstCount,secondCount*sizeof32(T));
				//printf("-----3-----\n");
			}
		}
		if (!tailExternalControl)
			setTail(fixTail + count);
	}
};

/*
template <class T, int SIZE> struct HalRingBufferDataDMA{
//	size_t 		maxCount;		///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
//	size_t 		maxCountMinus1;	///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
//	size_t 		size;			///<  размер кольцевого буфера входных данных (в элементах; гарантируется что это степень двойки)
	unsigned 	head;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код MASTER уже записал в	буфер входных данных [заполняется MASTER]
	unsigned	tail;			///<  сколько элементов ОТ НАЧАЛА ПОТОКА код SLAVE  уже прочитал (обработал) 			 [заполняется SLAVE]
	size_t		pendingCount;
	T	 		data[SIZE];			///<  физический адрес кольцевого буфера входных данных 
	int 		polltime;
	FuncSingleCopy singleCopy;
	FuncDoubleCopy doubleCopy;
	FuncSetCallbackDMA setCallback;
	
	int 	init   (T* buffer, 	size_t count, FuncSingleCopy singleMemCopy, FuncDoubleCopy doubleMemCopy,  FuncSetCallbackDMA setCallback);
	
	inline int isBusy(){
		if (setCallback==0)
			return false;
		return pendingCount;
	}

	inline bool isEmpty() {
		return head == tail;
	}

	inline bool isFull() {
		return head == tail + SIZE;
	}

	inline T* ptrHead(){
		return data + head & (size1);
	}
	
	inline T* ptrTail(){
		return data + tail & (size1);
	}

	void 	push   (const T* src, 	size_t count){
		while (tail+SIZE-head < count)
			halSleep(polltime);
		
		//if (ringBuffer->setCallback){
		//	while (ringBuffer->pendingCount)
		//		halSleep(RING_BUFFER_SLEEP);
		//	ringBuffer->pendingCount=count;
		//	ringBuffer->setCallback(halRingBufferPushCallback);
		//	activeRingBuffer=ringBuffer;
		//}

		
		size_t tail    = this->tail;
		size_t posHead = head & (size1);			
		size_t posTail = tail & (size1);
		T* addrHead    = data+posHead;
		
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead || head==tail){
			size_t countToEnd = SIZE - posHead;
			if (count <= countToEnd){
				singleCopy(src, addrHead ,count*sizeof(T));
			}
		// [*******<Head>......<Tail>*****]
			else {
				int firstCount = countToEnd;
				int secondCount = count-countToEnd;
				doubleCopy(src, src+firstCount, addrHead, data, firstCount*sizeof(T), secondCount*sizeof(T));
			}
		}
		else {
			singleCopy(src, addrHead, count);
		}

		if (setCallback==0)
			head += count;
	}


	void pop(T* dst, size_t count){
		while (head-tail < count)
			halSleep(polltime);
	
		//if (setCallback){
		//	while (pendingCount)
		//		halSleep(RING_BUFFER_SLEEP);
		//	pendingCount=count;
		//	setCallback(halRingBufferPopCallback);
		//	activeRingBuffer=ringBuffer;
		//}

		size_t head = this->head;
		size_t posHead = head & (size1);			
		size_t posTail = tail & (size1);
		T* addrTail    = data + posTail;
	
		// [.......<Tail>******<Head>.....]
		if (posTail<posHead){
			singleCopy(addrTail, dst, count*sizeof(T));
		}
		// [*******<Head>......<Tail>*****]
		else {
			size_t countToEnd = SIZE - posTail;
			if (count <= countToEnd){
				singleCopy(addrTail, dst, count*sizeof(T));
			}
			else {
				int firstCount = countToEnd;
				int secondCount= count-countToEnd;
				doubleCopy(addrTail, data, dst, dst+firstCount, firstCount*sizeof(T), secondCount*sizeof(T));
			}
		}
		if (setCallback==0)
			tail += count;
	}
};


*/

#endif //HAL_RINGBUFFERT_INCLUDED
