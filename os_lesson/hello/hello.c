/**
 * @file 
 * @brief �ں�ģ����ص�helloʾ��
 * @note insmod ģ����������ģ�飬rmmod ģ������ж��ģ��
 * @version 1.0
 */
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/**
 * @brief ģ�����
 *
 * @return �ɹ�Ϊ0
 */
static int __init hello_init(void)
{
    printk(KERN_ALERT  "hello Ruijie!\n");

    return 0;
}

/**
 * @brief ģ�����
 *
 * @return �ɹ�Ϊ0
 */
static void __exit hello_exit(void)
{
    printk(KERN_ALERT  "goodbye Ruijie!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("example@ruijie.com.cn");


