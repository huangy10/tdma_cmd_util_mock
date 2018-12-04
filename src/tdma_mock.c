#include "tdma_mock.h"

struct tdma_nl_mock* create_tdma_mock(int max_payload_size) {
    struct tdma_nl_mock *mock;
    struct nlattr tlv;
    
    mock = (struct tdma_nl_mock*)malloc(sizeof(struct tdma_nl_mock));
    mock->sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TDMA);
    if (mock->sock_fd < 0) {
        printf("Fail to create sock!\n");
        goto mock_fail;
    }
    memset(&mock->src_addr, 0, sizeof(struct sockaddr_nl));
    mock->src_addr.nl_family = AF_NETLINK;
    mock->src_addr.nl_pid = getpid();
    bind(mock->sock_fd, (struct sockaddr*)&mock->src_addr, sizeof(mock->src_addr));

    memset(&mock->dst_addr, 0, sizeof(struct sockaddr_nl));
    mock->dst_addr.nl_family = AF_NETLINK;
    mock->dst_addr.nl_pid = 0;
    mock->dst_addr.nl_groups = 0;

    mock->nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(max_payload_size));
    mock->nlh->nlmsg_len = NLMSG_SPACE(max_payload_size);
    mock->nlh->nlmsg_pid = getpid();
    mock->nlh->nlmsg_flags = 0;

    memset(&tlv, 0, sizeof(struct nlattr));
    mock->attr = NLMSG_DATA(mock->nlh);
    mock->recv_payload = mock->attr;
    memcpy(mock->attr, &tlv, sizeof(struct nlattr));
    mock->buf = mock->attr + sizeof(struct nlattr);

    mock->iov.iov_base = (void *)mock->nlh;
    mock->iov.iov_len = mock->nlh->nlmsg_len;
    mock->msg.msg_name = (void *)&mock->dst_addr;
    mock->msg.msg_iov = &mock->iov;
    mock->msg.msg_iovlen = 1;

    return mock;
mock_fail:
    free(mock);
    return NULL;
}

void free_tdma_mock(struct tdma_nl_mock *mock) {
    close(mock->sock_fd);
    free(mock);
}

void memcpy_tdma_mock(
    struct tdma_nl_mock *mock, 
    void* data, 
    int length, 
    int offset)
{
    memcpy(mock->buf + offset, data, length);
    mock->attr->nla_len = offset + length;
}

void sendmsg_tdma_mock(struct tdma_nl_mock *mock) {
    printf("send msg\n");
    sendmsg(mock->sock_fd, &mock->msg, 0);
    printf("start to recv\n");
    recvmsg(mock->sock_fd, &mock->msg, 0);
    printf("Received message payload: %s\n", (char *)mock->recv_payload);
    free_tdma_mock(mock);
}