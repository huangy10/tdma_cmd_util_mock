//
// Created by 黄延 on 2017/11/30.
//
// Copy Right Reserved.
//
//

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


#define u64 unsigned long long
#define u32 unsigned long 
#define u16 unsigned short
#define u8 unsigned char
#define s32 long
#define s16 short
#define ETH_ALEN 6

#define TDMA_NODE_COUNT 5

#define NETLINK_TDMA    31

#define TDMA_NL_CMD_TYPE(x) (x >> 8)
#define TDMA_NL_CMD_SUBTYPE(x) (x & 0xff)

// types
#define TDMA_NL_CMD_ROUTE   0x01
#define TDMA_NL_CMD_ICMP    0x02
#define TDMA_NL_CMD_STATS   0x03
#define TDMA_NL_CMD_HLOG    0x04

// subtypesNETLINK_TDMA
#define TDMA_NL_CMD_ROUTE_ADD   0x01
#define TDMA_NL_CMD_ROUTE_DEL   0x02
#define TDMA_NL_CMD_ROUTE_LS    0x03
#define TDMA_NL_CMD_ROUTE_RESET 0x04

struct tdma_nl_cmd_route {
    u16 flags;  // bool型的flag放在这里
    u8  src_node; // 源节点
    u8  dst_node; // 目标节点

    u16 weights;
    u16 delay;
};
// 定义route命令的flag的mask
#define TDMA_NL_CMD_ROUTE_MASK_VERBOSE  0x01    // --verbose
#define TDMA_NL_CMD_ROUTE_MASK_SYMM     0x02    // --symm -s (对称链接)
#define TDMA_NL_CMD_ROUTE_MASK_WATCH    0x04    // --watch

#define TDMA_NL_MAX_PAYLOAD 1000
/**
 * 
 */
struct tdma_nl_mock {
    struct sockaddr_nl src_addr;
    struct sockaddr_nl dst_addr;

    struct iovec iov;
    struct msghdr msg;

    int sock_fd;
    struct nlmsghdr *nlh;
    struct nlattr *attr;
    
    void *buf;
    void *recv_payload;
};

struct tdma_nl_mock* create_tdma_mock(int max_payload_size);
void free_tdma_mock(struct tdma_nl_mock* mock);
void memcpy_tdma_mock(struct tdma_nl_mock *mock, void* data, int length, int offset);

void sendmsg_tdma_mock(struct tdma_nl_mock *mock);

struct tdma_node_info {
    u8      id;
    u16     nnode;                      // abbr for next-hop-node
    u8      addr[ETH_ALEN];             // mac address
    bool    is_known;
    u8      ttl;                        // ttl 到0表路径断裂。
    u8      credit;                     // 节点信息可信度，传递跳数过多的节点不可信
    u8      pi_mac[ETH_ALEN];           // mac address of binded raspberry pi
    s32     latitude;                   // 到赤道距离,m,百位以下表2位小数，正表北
    s32     longitude;                  // 到本初子午线距离,m,百位以下表2位小数，正表东
    s16     speed_EW;                   // 东西方向速度，正表东
    s16     speed_NS;                   // 南北方向速度，正表北
    u64     gps_info_time;              // gps写入位置时的网络时间
};

struct  tdma_link_info {
    u32     score;      // 链接的打分
    u32     distance;   // 链接的距离, 如果缺少距离数据，可以设置为0
    bool    locked;     // 锁定score，禁止自动更新
};

struct tdma_nl_route_info {
    int err;
    int node_num;
    struct tdma_link_info links[TDMA_NODE_COUNT][TDMA_NODE_COUNT];
    struct tdma_node_info nodes[TDMA_NODE_COUNT];
};