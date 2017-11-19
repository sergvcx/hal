#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "ubc-defs.h"
#include "ubc_target.h"
#define TRACE(str) printf("%s", str)

#define SHARED_MEM_SIZE 1024


//TCHAR sSyncName[]     =TEXT("Global\\SyncFileMappingObject");
//TCHAR sSharedMemName[]=TEXT("Global\\SharedFileMappingObject");

HANDLE hMapFile;
HANDLE hMapFileSharedMem;

// 
SyncBuf  *pSyncBuf=0;


//const int MAX_SHARED
static int* pSharedMem;//[MAX_SHARED];
static int  sharedDiff[MAX_COUNT_PROCESSORS];

int             bufferRegistryCount=0;

MirrorRegistry mirrorRegistry[MAX_COUNT_PROCESSORS];

extern int procNo=0;
extern "C"{

int ubcSync(int val,int processor=0){
	printf ("HOST[%d]:Sync[%d] (%8xh)...",processor,pSyncBuf[processor].counter0,val);
	pSyncBuf[processor].sync0=val;
	pSyncBuf[processor].counter0++;
	while (pSyncBuf[processor].counter0!=pSyncBuf[processor].counter1);
	printf("=%xh\n",pSyncBuf[processor].sync1);
	return pSyncBuf[processor].sync1;
}

//static int* sharedBuffer[MAX_COUNT_PROCESSORS]={0,0,0,0,0,0,0,0};
static char* absFile[MAX_COUNT_PROCESSORS]={0,0,0,0,0,0,0,0};

int ubcOpen(char* absfile,...){
	procNo=-1;
	
	//unsigned sharedSize32;

	va_list args;
	va_start(args, absfile);
	absFile[0] = absfile;
	absFile[1] = va_arg(args, char*);
	va_end(args);
	
	//-------------- create sync-memory buffer ---------------
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		SYNC_BUF_SIZE,           // maximum object size (low-order DWORD)
		TEXT(SYNC_BUFFER_MAPPING_NAME));              // name of mapping object

	if (hMapFile == NULL){
		_tprintf(TEXT("Could not create file sync mapping object (%d).\n"),GetLastError());
		return 1;
	}

	pSyncBuf = (SyncBuf*) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		SYNC_BUF_SIZE);

	if (pSyncBuf == NULL){
		_tprintf(TEXT("Could not map view of sync file (%d).\n"),GetLastError());
		CloseHandle(hMapFile);
		return 1;
	}
	
	//pSyncBuf=(SyncBuf*)ubcMalloc32(sizeof(SyncBuf)*2*4,SYNCRO_BUFFER);
	if (pSyncBuf==0){
		printf("ERROR:Shared memory allocation error on host \n");
		return 1;
	}
	SyncBuf SB={-1,-1,-1,-1};
	CopyMemory((PVOID)&pSyncBuf[0], &SB, sizeof(SB));
	CopyMemory((PVOID)&pSyncBuf[1], &SB, sizeof(SB));

	/*
	//--------- handshake  ---------------------------
	int ok;
	unsigned sharedSize32;
	for(int processor=0; absFile[processor]; processor++){
		ok=ubcSync(0x8086,processor);
		
	
		if (processor==0){
			sharedSize32 = ubcSync(0,processor);
			if (sharedBuffer==0){
				printf("Shared memory allocation error on target \n");
				return 1;
			}

			hMapFileSharedMem = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,	// read/write access
				FALSE,					// do not inherit the name
				sSharedMemName);		// name of mapping object

			if (hMapFileSharedMem == NULL){
				_tprintf(TEXT("Could-not open file mapping shared object (%d).\n"),GetLastError());
				continue;//return -1;
			}
		
			if (hMapFileSharedMem == NULL){
				_tprintf(TEXT("Could not open file mapping shared object (%d).\n"),GetLastError());
				return -1;
			}

			pSharedMem  = (int*) MapViewOfFile(
				hMapFileSharedMem,		// handle to map object
				FILE_MAP_ALL_ACCESS,	// read/write permission
				0,
				0,
				sharedSize32*4);

			if (pSharedMem == NULL){
				_tprintf(TEXT("Could not map view of shared file (%d).\n"),GetLastError());
				CloseHandle(hMapFileSharedMem);
				return -1;
			}
			sharedBuffer[processor] = (int*)ubcSync(0, processor);	
			printf("HOST:Shared: %x %x\n",sharedBuffer[processor] ,sharedSize32*4);
			sharedDiff[processor] = pSharedMem - sharedBuffer[processor];
		}
		else {
			ok = ubcSync(sharedSize32, processor);	
			sharedBuffer[processor] = (int*)ubcSync(0, processor);	
			printf("HOST:Shared: %x %x\n",sharedBuffer[processor] ,sharedSize32*4);
			sharedDiff[processor] = pSharedMem - sharedBuffer[processor];
		}
		
	}
	*/
	TRACE ("Connected!\n");
	return 0;
}


int updateMirrorRegistry(int processorNo){
	
	// ���� ��������� ������ �� �����������, ��������
	if (mirrorRegistry[processorNo].pBufferRegistry==0){
		HANDLE hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			createName(TEXT(CONST_REGISTRY_MAPPING_NAME),processorNo,processorNo));               // name of mapping object

		if (hMapFile == NULL){
			_tprintf(TEXT("ERROR:Could not open file mapping object (%d).\n"),GetLastError());
			return 0;
		}

		void* buffer= MapViewOfFile(
			hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			sizeof(BufferRegistry));

		if (buffer== NULL){
			_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());
			CloseHandle(hMapFile);
			return 0;
		}
		mirrorRegistry[processorNo].pBufferRegistry=(BufferRegistry*) buffer;
		mirrorRegistry[processorNo].hBufferRegistry=hMapFile;
	}

	// ��������� �������
	for (int i=mirrorRegistry[processorNo].count; i<mirrorRegistry[processorNo].pBufferRegistry->count; i++ ){
		HANDLE hMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			createName(TEXT(CONST_BUFFER_MAPPING_NAME),processorNo,i));               // name of mapping object

		if (hMapFile == NULL){
			_tprintf(TEXT("ERROR:Could not open file mapping object (%d).\n"),GetLastError());
			return 0;
		}

		void* buffer= MapViewOfFile(
			hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS,  // read/write permission
			0,
			0,
			mirrorRegistry[processorNo].pBufferRegistry->buffer[i].size32*4);

		if (buffer== NULL){
			_tprintf(TEXT("Could not map view of file (%d).\n"),GetLastError());
			CloseHandle(hMapFile);
			return 0;
		}
		MirrorBuffer& mirrorBuffer=mirrorRegistry[processorNo].buffer[i];
		MappedBuffer& mappedBuffer=mirrorRegistry[processorNo].pBufferRegistry->buffer[i];

		mirrorBuffer.address=mappedBuffer.address;
		mirrorBuffer.size32 =mappedBuffer.size32;
		mirrorBuffer.diff   =(int*)buffer-mappedBuffer.address; 
		mirrorBuffer.handle =hMapFile;
		mirrorRegistry[processorNo].count++;
	}



	return 1;
}
MirrorBuffer* findBuffer(unsigned boardAddr,int processorNo){

	if (updateMirrorRegistry(processorNo)==0  )
		return 0;
	
	for( int i=0; i<mirrorRegistry[processorNo].count ;i++){
		MirrorBuffer& mirrorBuffer=mirrorRegistry[processorNo].buffer[i];
		if ((int*)boardAddr>=mirrorBuffer.address)
			if ((int*)boardAddr<mirrorBuffer.address+mirrorBuffer.size32)
				return &mirrorBuffer;

	}
	return 0;
}

int ubcReadMemBlock (int* dstHostAddr, unsigned srcBoardAddr, unsigned size32, unsigned processor=0){
	MirrorBuffer* pMirrorBuffer=findBuffer(srcBoardAddr,processor);
	if (pMirrorBuffer){
		CopyMemory((PVOID)dstHostAddr, (char*)((int*)pMirrorBuffer->address + pMirrorBuffer->diff), size32*4);
		return 1;
	}
	return 0;
	//CopyMemory((PVOID)dstHostAddr, (char*)((int*)srcBoardAddr + sharedDiff[processor]), size32*4);
	
}

int ubcWriteMemBlock(unsigned long* srcHostAddr, unsigned dstBoardAddr, unsigned size32, unsigned processor=0){
	MirrorBuffer* pMirrorBuffer=findBuffer(dstBoardAddr,processor);
	if (pMirrorBuffer){
		CopyMemory((char*)((unsigned*)dstBoardAddr + pMirrorBuffer->diff), (PVOID)srcHostAddr,  size32 * 4);
		return 1;
	}
	return 0;
	

	//CopyMemory((char*)((int*)dstBoardAddr + sharedDiff[processor]), (PVOID)srcHostAddr,  size32 * 4);
	
}


int ubcClose(){
	for(int processor=1; absFile[processor]; processor++){  
		ubcSync(0x600DBA10+processor,processor);
	}
	ubcSync((int)0x600DBA100);
	if (pSyncBuf)
		UnmapViewOfFile(pSyncBuf);
	if (pSharedMem)
		UnmapViewOfFile(pSharedMem);

	if (hMapFile)
		CloseHandle(hMapFile);
	if (hMapFileSharedMem)
		CloseHandle(hMapFileSharedMem);
	

	return 1;
}


int ubcGetResult(unsigned long* returnCode, int processor=0){
	return 1;
	
}

};