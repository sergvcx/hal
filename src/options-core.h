#ifndef __HAL_OPTIONS_CORE_H_INCLUDED__
#define ____HAL_OPTIONS_CORE_H_INCLUDED__
struct HalBoardOptions{
    int board_no;
    int board_type;
    unsigned char host_mac_addr[7];
    char server_ip[16];
    int server_port;
    int server_enabled;
};

struct HalAccessOptions{
    int core;
    int cluster;
};

#endif //__HAL_OPTIONS_CORE_H_INCLUDED__