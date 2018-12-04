#include "tdma_mock.h"

int main() {
    struct tdma_nl_mock *mock = create_tdma_mock(255);
    if (!mock) {
        printf("Fail to create mock\n");
        return -1;
    }
    sendmsg_tdma_mock(mock);
    return 0;
}
