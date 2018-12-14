/**
 * 这个文件会产生一个模拟的tdma route ls命令发往内核
 */
#include "tdma_mock.h"

int main() {
    struct tdma_nl_mock *mock = create_tdma_mock(1000);
    struct tdma_nl_cmd_route cmd;
    ssize_t size;

    int node_num;
    struct nlmsghdr *nlh;
    struct nlattr *ack_attr;
    struct tdma_nl_route_info *info;

    int i, j;

    if (!mock) {
        printf("fail to create mock\n");
        return -1;
    }
    memset(&cmd, 0, sizeof(struct tdma_nl_cmd_route));
    mock->attr->nla_type = (TDMA_NL_CMD_ROUTE << 8) | TDMA_NL_CMD_ROUTE_LS;
    memcpy_tdma_mock(mock, &cmd, sizeof(cmd), 0);
    printf("nlatter len: %u\n", mock->attr->nla_len);

    printf("sendmsg\n");
    size = sendmsg(mock->sock_fd, &mock->msg, 0);
    printf("send size %d!\n", size);
    printf("start to recv\n");
    size = recvmsg(mock->sock_fd, &mock->msg, 0);

    printf("recv size %d!\n", size);
    info = (struct tdma_nl_route_info *)mock->recv_payload;
    
    printf("err code: %d\n", info->err);
    printf("node num: %d\n", info->node_num);
    for (i = 0; i < info->node_num; i += 1) {
        for (j = 0; j < info->node_num; j += 1) {
            printf("%u, ", info->links[i][j].score);
        }
        printf("\n");
    }

    nlh = (struct nlmsghdr*)(mock->recv_payload + sizeof(struct tdma_nl_route_info));
    ack_attr = (struct nlattr *)NLMSG_DATA(nlh);
    printf("ack cmd: 0x%x\n", ack_attr->nla_type);
    
    return 0;
}

