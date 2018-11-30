#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#define NETLINK_TDMA    31

struct sock *nl_sk = NULL;

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