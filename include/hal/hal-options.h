#ifndef __HAL_BUILDER_H_INCLUDED
#define __HAL_BUILDER_H_INCLUDED

typedef struct HalBoard HalBoard;        //for nm and x86 different realizations
typedef struct HalAccess HalAccess;       //for nm and x86 different realizations


enum HAL_BOARD_OPTIONS{
    HAL_BOARD_NUMBER,
    HAL_BOARD_TYPE,
    HAL_BOARD_MAC_ADDR,
    HAL_SERVER_IP,
    HAL_SERVER_PORT,
    HAL_SERVER_ENABLED
};

enum HAL_ACCESS_OPTIONS{
    HAL_CORE,
    HAL_CLUSTER
};
typedef struct HalBoardOptions HalBoardOptions;
typedef struct HalAccessOptions HalAccessOptions;


#ifdef __cplusplus
extern "C"{
#endif

    

    HalBoardOptions *halSetBoardOption(HalBoardOptions *builder, int option, ...);
    HalAccessOptions *halSetAccessOption(HalAccessOptions *builder, int option, ...);

    HalBoardOptions *halCreateBoardOptions();
    HalAccessOptions *halCreateAccessOptions();
    void halDestroyBoardOptions(HalBoardOptions *builder);
    void halDestroyAccessOptions(HalAccessOptions *builder);

#ifdef __cplusplus
}
#endif

#endif //__HAL_BUILDER_H_INCLUDED