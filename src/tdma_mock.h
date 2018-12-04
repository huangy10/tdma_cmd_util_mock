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


#define u32 unsigned long 
#define u16 unsigned short
#define u8 unsigned char

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