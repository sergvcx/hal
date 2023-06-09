#include "hal-core.h"
#include "library.h"



LibraryHandle open_library(const char* name){
    char win_name[50];
    sprintf(win_name, "%s.dll", name);
    printf("%s\n", win_name);
    return LoadLibrary(win_name);
}

LibraryFuncAddr library_get_addr(LibraryHandle handle, const char* func_name){
    return GetProcAddress(handle, func_name);
}
void close_library(LibraryHandle handle){
    FreeLibrary(handle);
}




