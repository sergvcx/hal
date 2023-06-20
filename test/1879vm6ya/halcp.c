#include "hal/halcp.h"
#include "stdio.h"
#define assert(cond) if(!(cond)){ printf("%s:%d: Assertion \'%s\' failed\n", __FILE__, __LINE__, #cond); exit(1); }

void test_halcpOpenShm_whenCorrectBuffer_shouldResultGreaterZero(){
    //data_
    int buffer[1024];
    int fd = halcpOpenShm(buffer, 1024);

    assert(fd > 0);

    halcpClose(fd);
}

int main(int argc, char argv[]){
    test_halcpOpenShm_whenCorrectBuffer_shouldResultGreaterZero();
    return 0;
}