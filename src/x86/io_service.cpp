#include "hal/hal.h"
#include "hal-core.h"
#include <iostream>
#include "logger.h"

#ifdef HAL_IO_HOST
#include "io_host.h"


typedef struct IO_Service IO_Service;

struct HalIO{
    IO_Service *io;
    HalIO(HalAccess *access,  const char *program_file, FILE *file);
    ~HalIO();
};

extern "C"{

    HalIO::HalIO(HalAccess *access, const char *program_file, FILE *file){
        Log(LOG_DEBUG1).get() << __FUNCTION__;
        void *ops = NULL;        
        if(dynamic_cast<IHalAccessIO *>(access->access_interface)){
            ops = dynamic_cast<IHalAccessIO *>(access->access_interface)->getOpsForIO();
        }
        PL_Access *a = (PL_Access *)access->access_interface->native();
        io = IO_ServiceStart(program_file, a, file, ops);
        if(io == 0){
            std::cout << "Failed IO Service start" << std::endl;  
        }
    }

    HalIO::~HalIO(){
        if(io){
            IO_ServiceStop(&io);
        }

    }


    HalIO *halStartIO(HalAccess *access, const char *program_file, FILE *file){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        HalIO *result = new HalIO(access, program_file, file);
        if(result->io == 0){
            delete result;
            result = NULL;
        }
        return result;
        
    }

    int halStopIO(HalIO *hal_io){
        Log(LOG_DEBUG).get() << __FUNCTION__;
        delete hal_io;
        return 0;
    }
}
#endif //HAL_IO_HOST