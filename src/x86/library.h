#ifndef __LIBRARY_H_INCLUDED
#define __LIBRARY_H_INCLUDED

#ifdef __MINGW32__
#   include <windows.h> 
typedef  void* LibraryHandle;
typedef long long int (*LibraryFuncAddr)();
#elif _WIN32
#   include <windows.h> 
#   define LibraryHandle HINSTANCE 
#   define LibraryFuncAddr void*
#else
#   include "dlfcn.h"
#   define LibraryHandle void * 
#   define LibraryFuncAddr void*
#endif //_WIN32

LibraryHandle open_library(const char* name);

LibraryFuncAddr library_get_addr(LibraryHandle handle, const char* func_name);

// template <typename T>
// T library_get_addr_template(LibraryHandle handle, const char* func_name){
//     return (T)library_get_addr(LibraryHandle handle, const char* func_name);
// }

void close_library(LibraryHandle);

#endif //__LIBRARY_H_INCLUDED