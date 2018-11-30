#include "tdma_mock.h"

// struct msghdr msg;

int main() {
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;
    struct iovec iov;
    int sock_fd;
    struct msghdr* msg;
    int payload_size;
    struct nlattr tlv;
    struct tdma_nl_cmd_route cmd;

    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TDMA);
    if (sock_fd < 0) {
        printf("Fail to create sock");
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;
    dest_addr.nl_groups = 0;

    // payload_size = sizeof(struct nlattr) + sizeof(struct tdma_nl_cmd_route);
    payload_size = 1000;
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(payload_size));

    nlh->nlmsg_len = NLMSG_SPACE(payload_size);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    tlv.nla_len = sizeof(struct tdma_nl_cmd_route);
    tlv.nla_type = TDMA_NL_CMD_ROUTE << 8 & TDMA_NL_CMD_ROUTE_LS;
    memset(&cmd, 0, sizeof(cmd));
    memcpy(NLMSG_DATA(nlh), &tlv, sizeof(tlv));
    memcpy(NLMSG_DATA(nlh) + sizeof(tlv), &cmd, sizeof(cmd));

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg = malloc(sizeof(struct msghdr));
    msg->msg_name = (void *) &dest_addr;
    msg->msg_namelen = sizeof(dest_addr);
    msg->msg_iov = &iov;
    msg->msg_iovlen = 1;

    printf("Sending message to kernel: message size: %lu\n", iov.iov_len);
    printf("my pid: %d\n", getpid());
    sendmsg(sock_fd, msg, 0);

    printf("wait\n");
    recvmsg(sock_fd, msg, 0);
    printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
    close(sock_fd);
    free(nlh);
    free(msg);
}
