#ifndef __HAL_UTILS_INCLUDED__
#define __HAL_UTILS_INCLUDED__
#include "hal/nmtype.h"
#include "malloc.h"

#ifdef __NM__
//	typedef void nm8s ;
//	typedef void nm16s;
#define DISABLE_PRINTF() extern "C" {	int printf ( const char * format, ... ){	return 0;	}};
#else 
//	
//	typedef char nm8s;
//	typedef short nm16s;
#define DISABLE_PRINTF() 
#endif

#ifdef	__cplusplus
extern "C"
{
#endif

void nmc_malloc_set_heap (int h_num);
int  nmc_malloc_get_heap (void) ;

#ifdef	__cplusplus
}
#endif

#ifdef __NM__
    //void* halMalloc32(size_t size32);
	inline int* halMalloc32(int sharedSize32) {return (int*)malloc(sharedSize32); }
	inline void halFree(void* shared) { free(shared);}
#else 
	extern "C"{
	int* halMalloc32(int sharedSize32) ;//{return (int*)malloc(sharedSize32 * 4); }
	void halFree(void* );
	};
#endif
	
#ifdef __cplusplus
	extern "C" {
#endif

	void halEnterCriticalSectionCore(int coreID);
	void halExitCriticalSectionCore(int coreID);
	void halEnterCriticalSection();
	void halExitCriticalSection();
	
	void*  halCopyRISC  (const void* src,  void* dst,  unsigned size32);
	void*  halCopyDMA   (const void* src,  void* dst,  unsigned size32);
	void*  halCopyInt   (const void* src,  void* dst,  unsigned size32);
	void*  halCopyFlt   (const void* src,  void* dst,  unsigned size32);
	
	// отображение  адреса в глобальное адресное пространство
	//void* halMapAddr(const void* srcAddr);	
	void* halMapAddrTo  (const void* ownAddress, int toProccessor);
	void* halMapAddrFrom(const void* extAddress, int fromProcessor);

	void halSetActiveHeap(int heapNo);

	

#ifdef __cplusplus
		};
#endif

	/**
    *
    *     \defgroup hal HAL функции 
    */

int			halGet_8s (const nm8s*  pVec, int nIndex);
int			halGet_16s(const nm16s* pVec, int nIndex);

#endif //__HAL_UTILS_INCLUDED__