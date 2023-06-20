#include "hal-core.h"
#include "library.h"



LibraryHandle open_library(const char* name){
    return LoadLibrary(name);;
}

LibraryFuncAddr library_get_addr(LibraryHandle handle, const char* func_name){
    return GetProcAddress(handle, func_name);
}
void close_library(LibraryHandle handle){
    FreeLibrary(handle);
}




