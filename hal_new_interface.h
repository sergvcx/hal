#include <stddef.h>

struct HalBoard;        //for nm and x86 different realizations
struct HalAccess;       //for nm and x86 different realizations
struct HalCore;

typedef size_t hintptr;

enum HAL_ERROR{
    HAL_OK,
    HAL_ERROR,
    HAL_BAD_ARGUMENT,
    HAL_WRONG_MEM,
    HAL_NOT_IMPLEMENTED
};

enum HalCoreAttribute{
    HAL_CORE_NUMBER,
    HAL_CLUSTER_NUMBER,
    HAL_NONE
};

/**
 * @brief open board
 * 
 * @param id 
 * @return HalBoard* 
 * @details example "mc12101:0", "mb7707:0x1A2B3C4D5E6F", "mc7601:192.168.1.2", "x86:0"(нужно ли?)
 */
HalBoard *halOpenBoard(const char *id);   

/**
 * @brief Close board
 * 
 * @param board 
 */
void halCloseBoard(HalBoard *board);

/**
 * @brief Get last error
 * 
 * @return int 
 */
int halGetLastError();

/**
 * @brief 
 * 
 * @param board 
 * @param attrib_list array atributes in format: HalCoreAttribute0, value0, HalCoreAttribute1, value1 ... HalCoreAttributeN, valueN, HAL_NONE
 * @return HalAccess* 
 * @details 
 */
HalAccess *halCreateAccess(HalBoard *board, int *attrib_list);      
void halDestroyAccess(HalAccess *access);

void halLoadProgram(HalAccess *, const char *filename); // work only on host

int halSync(HalCore *board, int value);
hintptr halSyncAddr(HalCore *board, hintptr value);

void halReadMemBlock (HalAccess *access, const void* dstHostAddr, hintptr srcBoardAddr, size_t size32);
void halWriteMemBlock(HalAccess *access, const void* srcHostAddr, hintptr dstBoardAddr, size_t size32);

int halGetResult(HalAccess *access);
void halSetTimeout(int msec);



