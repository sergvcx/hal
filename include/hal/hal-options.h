#ifndef __HAL_BUILDER_H_INCLUDED
#define __HAL_BUILDER_H_INCLUDED

typedef struct HalBoard HalBoard;        //for nm and x86 different realizations
typedef struct HalAccess HalAccess;       //for nm and x86 different realizations

enum HalBoardType{
    NO_BOARD,
    MC12101,
    MB7707,
    MC7601,
    MC5103,
    VIRTUAL
};


enum HAL_BOARD_OPTIONS{
    HAL_BOARD_NUMBER,
    HAL_BOARD_TYPE,
    HAL_IP,
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

    HalBoard *halGetBoardOpt(HalBoardOptions *board_options);
    HalAccess *halGetAccessOpt(HalBoard *board, HalAccessOptions *access_options);

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