/**
 * @file 
 * @brief 内核模块加载的hello示例
 * @note insmod 模块名：加载模块，rmmod 模块名：卸载模块
 * @version 1.0
 */
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/**
 * @brief 模块入口
 *
 * @return 成功为0
 */
static int __init hello_init(void)
{
    printk(KERN_ALERT  "hello Ruijie!\n");

    return 0;
}

/**
 * @brief 模块出口
 *
 * @return 成功为0
 */
static void __exit hello_exit(void)
{
    printk(KERN_ALERT  "goodbye Ruijie!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("example@ruijie.com.cn");


