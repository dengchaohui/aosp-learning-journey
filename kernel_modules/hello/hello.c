#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

// 模块加载时执行
static int __init hello_init(void) {
    printk(KERN_INFO "Hello World! My first kernel module.\n");
    return 0;
}

// 模块卸载时执行
static void __exit hello_exit(void) {
    printk(KERN_INFO "Goodbye kernel module!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Hello World kernel module");
