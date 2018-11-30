#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

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

struct sock *nl_sk = NULL;

static void parse_cmd(struct nlmsghdr *nlh) {
    struct nlattr *attr;
    attr = (struct nlattr*)nlmsg_data(nlh);

    switch (TDMA_NL_CMD_TYPE(attr->nla_type)) {
        case TDMA_NL_CMD_ROUTE:
            if (TDMA_NL_CMD_SUBTYPE(attr->nla_type) == TDMA_NL_CMD_ROUTE_LS) {
                printk(KERN_INFO "tdma route ls\n");
                return;
            }
            break;
        default:
            break;
    }
    printk(KERN_ALERT "Undefined command: %u\n", attr->nla_type);
    printk(KERN_ALERT "TLV lenght: %u\n", attr->nla_len);
}

static void simple_msg_input(struct sk_buff *skb) {
    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;

    int msg_size;
    char *msg = "tdma route ls received";
    int res;

    printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

    msg_size = strlen(msg);

    nlh = (struct nlmsghdr *)skb->data;
    pid = nlh->nlmsg_pid;
    printk(KERN_INFO "Receive msg from %d\n", pid);
    parse_cmd(nlh);

    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Fail to allocate new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;
    strncpy(nlmsg_data(nlh), msg, msg_size);
    res = nlmsg_unicast(nl_sk, skb_out, pid);

    if (res < 0) {
        printk(KERN_INFO "Error when sending response\n");
    }
}

static int __init hello_init(void) {
    struct netlink_kernel_cfg cfg = {
        .input = simple_msg_input,
    };
    printk(KERN_INFO "tdma mock init\n");
    nl_sk = netlink_kernel_create(&init_net, NETLINK_TDMA, &cfg);
    if (!nl_sk) {
        printk(KERN_ALERT "ERROR Create sock\n");
        return -10;
    }
    printk(KERN_INFO "tdma mock init success\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "tdma mock exit\n");
    netlink_kernel_release(nl_sk);
    nl_sk = NULL;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");