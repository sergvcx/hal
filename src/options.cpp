#include "hal/hal-options.h"
#include <stdarg.h>
#include "hal-core.h"

extern "C" {
    HalBoard *halGetBoardOpt(HalBoardOptions *board_options){
        HalBoard *board = NULL;
        if(board_options->board_type == MC12101){
            board = HalBoard::createBoard_MC12101(board_options->board_no);
        }
        if(board) board->open();
        return board;
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