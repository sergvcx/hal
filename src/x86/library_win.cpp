#include "hal-core.h"
#include "library.h"


#ifdef _WIN32
LibraryHandle open_library(const char* name){
    char win_name[50];
    sprintf(win_name, "%s.dll", name);
    return LoadLibrary(win_name);
}

LibraryFuncAddr library_get_addr(LibraryHandle handle, const char* func_name){
    return GetProcAddress(handle, func_name);
}
void close_library(LibraryHandle handle){
    FreeLibrary(handle);
}
#endif

#ifdef __linux__
LibraryHandle open_library(const char* name){
    char lin_name[50];
    sprintf(lin_name, "lib%s.so", name);
    return dlopen(lin_name, RTLD_LAZY);    
}

LibraryFuncAddr library_get_addr(LibraryHandle handle, const char* func_name){
    return dlsym(handle, func_name);;
}
void close_library(LibraryHandle handle){
    dlclose(handle);
}
#endif




