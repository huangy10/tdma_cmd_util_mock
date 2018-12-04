/**
 * 这个文件会产生一个模拟的tdma route ls命令发往内核
 */
#include "tdma_mock.h"

int main() {
    struct tdma_nl_mock *mock = create_tdma_mock(255);
    struct tdma_nl_cmd_route cmd;
    if (!mock) {
        printf("fail to create mock\n");
        return -1;
    }
    memset(&cmd, 0, sizeof(struct tdma_nl_cmd_route));
    mock->attr->nla_type = (TDMA_NL_CMD_ROUTE << 8) | TDMA_NL_CMD_ROUTE_LS;
    memcpy_tdma_mock(mock, &cmd, sizeof(cmd), 0);
    printf("nlatter len: %u\n", mock->attr->nla_len);

    sendmsg_tdma_mock(mock);
    return 0;
}

