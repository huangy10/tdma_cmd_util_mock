/**
 * 这个文件用来测试tdma route del命令
 */

#include "tdma_mock.h"

int main() {
    struct tdma_nl_mock *mock = create_tdma_mock(1000);
    struct tdma_nl_cmd_route cmd;
    int err;
    ssize_t size;

    if (!mock) {
        printf("fail to create mock\n");
        return -1;
    }
    memset(&cmd, 0, sizeof(struct tdma_nl_cmd_route));
    cmd.flags = TDMA_NL_CMD_ROUTE_MASK_SYMM;
    cmd.src_node = 1;
    cmd.dst_node = 3;
    mock->attr->nla_type = (TDMA_NL_CMD_ROUTE << 8) | TDMA_NL_CMD_ROUTE_DEL;
    memcpy_tdma_mock(mock, &cmd, sizeof(cmd), 0);
    printf("sending msg\n");
    size = sendmsg(mock->sock_fd, &mock->msg, 0);
    printf("send size %d!\n", size);
    printf("start to recv\n");
    size = recvmsg(mock->sock_fd, &mock->msg, 0);

    err = *((int*)(mock->recv_payload));
    printf("ERR: %d\n", err);
}