#include "hal/hal-options.h"
#include <stdarg.h>
#include "hal-core.h"
#include "string.h"




extern "C" {
    

    HalBoardOptions *halCreateBoardOptions(){
        HalBoardOptions *options = new HalBoardOptions();
        options->board_type = HAL_NO_BOARD;
        return options;
    }
    HalAccessOptions *halCreateAccessOptions(){
        return new HalAccessOptions();
    }

    void halDestroyBoardOptions(HalBoardOptions *options){
        delete options;
    }
    void halDestroyAccessOptions(HalAccessOptions *options){
        delete options;
    }

    HalBoardOptions *halSetBoardOption(HalBoardOptions *options, int optcode, ...){
        va_list ap;
        va_start(ap, optcode);
        switch (optcode)
        {
        case HAL_BOARD_TYPE:
            options->board_type = va_arg(ap, int);
            break;
        case HAL_BOARD_NUMBER:
            options->board_no = va_arg(ap, int);
            break;
        case HAL_BOARD_MAC_ADDR:{
            const char *addr = va_arg(ap, const char*);
            memcpy(options->host_mac_addr, addr, 6);
            break;
        }
        case HAL_SERVER_IP:{
            const char *server_ip = va_arg(ap, const char*);
            strcpy(options->server_ip, server_ip);
            break;
        }
        case HAL_SERVER_PORT:
            options->server_port = va_arg(ap, int);
            break;
        case HAL_SERVER_ENABLED:
            options->server_enabled = va_arg(ap, int);
            break;
        default:
            break;
        }
        va_end(ap);
        return options;
    }

    HalAccessOptions *halSetAccessOption(HalAccessOptions *builder, int option, ...){
        va_list ap;
        va_start(ap, option);
        switch (option)
        {
        case HAL_CORE:
            builder->core = va_arg(ap, int);
            break;
        case HAL_CLUSTER:
            builder->cluster = va_arg(ap, int);
            break;        
        default:
            break;
        }
        va_end(ap);
        return builder;
    }

   
}