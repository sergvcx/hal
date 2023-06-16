// Loader for MC121.01
// Settings

#include "mc12101load.h"

#define DEVICE_NAME "mc12101"
#define DEVICE_NAME_STR "MC121.01"

#ifndef NM_RPC

#define RUN_VERSION_MAJOR 6
#define RUN_VERSION_MINOR 1

#define COPYRIGHT_STR "(C) 2022 RC Module Inc."

#else

#define RUN_VERSION_MAJOR 1
#define RUN_VERSION_MINOR 0

#define COPYRIGHT_STR "(C) 2022 RC Module Inc."

#endif

#define NM_IO_ServiceX

const unsigned int CORES_COUNT = 2;

const PL_Addr Dump_memory_begin_default = 0x00018000; // (Must be multiple of 2)
const PL_Addr Dump_memory_size_default = 0x1000; // (Must be multiple of 2)

#ifdef _WIN32
const DWORD MAX_TIMEOUT = 1000000000; // (mS)
#else 
const unsigned long MAX_TIMEOUT = 1000000000; // (mS)
#endif 

extern "C" {
#ifdef NM_RPC
void PL_ConnectToHost(char *, int);
#endif
int PL_LoadProgramFileArgs(PL_Access *access, const char *filename, const char *mainArgs);
int PL_LoadProgramFileArgs_Int(PL_Access *access, const char *filename, const char *mainArgs);
} // extern "C"

void Display_version(int argc, char **argv);
void Parse_command_line(int argc, char **argv);
int Dump_shared_memory(const char *ext);
int Receive_binary_dump_from_board();
int Send_binary_file_to_board();
int Get_sect_addr(const char *Sect_name, PL_Addr *Sect_addr);
