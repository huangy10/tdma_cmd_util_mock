#include <errno.h>
#include "tdma_mock.h"


static void send_undefined(struct tdma_nl_mock *mock) {
    ssize_t size;
    int err;
    printf("send msg\n");
    size = sendmsg(mock->sock_fd, &mock->msg, 0);
    printf("send size %d!\n", size);
    printf("start to recv\n");
    size = recvmsg(mock->sock_fd, &mock->msg, 0);
    err = *((int *)mock->recv_payload);
    printf("error code: %d\n", err);
}

int main() {
    struct tdma_nl_mock *mock = create_tdma_mock(255);
    if (!mock) {
        printf("Fail to create mock\n");
        return -1;
    }
    send_undefined(mock);
    free_tdma_mock(mock);
    return 0;
}
