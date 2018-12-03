#include "tdma_mock.h"

int main() {
    struct tdma_nl_mock *mock = create_tdma_mock(255);
    if (!mock) {
        printf("Fail to create mock\n");
        return -1;
    }
    printf("send msg\n");
    sendmsg(mock->sock_fd, &mock->msg, 0);
    printf("start to recv\n");
    recvmsg(mock->sock_fd, &mock->msg, 0);
    printf("Received message payload: %s\n", (char *)mock->recv_payload);
    free_tdma_mock(mock);
    return 0;
}
