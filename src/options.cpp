#include "hal/hal-options.h"
#include <stdarg.h>
#include "hal-core.h"
#include "string.h"

extern "C" {
    HalBoard *halGetBoardOpt(HalBoardOptions *board_options){
        HalBoard *board = NULL;
        switch (board_options->board_type)
        {
        case MC12101:
            board = createBoard_MC12101(board_options->board_no);
            break;
        case MB7707:
            board = createBoard_MB7707(board_options->host_mac_addr);
            break;
        default:
            break;
        }
        if(!board) return NULL;
        if(board->is_initialized){
            board->open();
            return board;
        } else {
            delete board;
            return NULL;
        }
    }

    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options){
        return board->getAccess(access_options);
    }

    HalBoardOptions *halCreateBoardOptions(){
        return new HalBoardOptions();
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

    HalBoardOptions *halSetBoardOption(HalBoardOptions *builder, int option, ...){
        va_list ap;
        va_start(ap, option);
        switch (option)
        {
        case HAL_BOARD_TYPE:
            builder->board_type = va_arg(ap, int);
            break;
        case HAL_BOARD_NUMBER:
            builder->board_no = va_arg(ap, int);
            break;
        case HAL_BOARD_MAC_ADDR:{
            const char *addr = va_arg(ap, const char*);
            memcpy(builder->host_mac_addr, addr, 6);
            break;
        }
        default:
            break;
        }
        va_end(ap);
        return builder;
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
            builder->core = va_arg(ap, int);
            break;        
        default:
            break;
        }
        va_end(ap);
        return builder;
    }

   
}