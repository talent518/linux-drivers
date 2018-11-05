#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static int __init ApiModuleAddress_init(void);
static void __exit ApiModuleAddress_exit(void);

// 此处定义一个自己添加的内核函数，函数参数必须写入void
int a_module(void) {
	return 0;
}

int __init ApiModuleAddress_init(void) {
	printk("ApiModuleAddress module init ...\n");

	struct module *ret; // 用于接收测试函数返回值
	unsigned long addr = (unsigned long) a_module; // 得到内核符号a_module的地址

	/* 调用 __module_address() 函数之前，必须禁止中断，以防止模块在执行操作期间被释放 */
	preempt_disable(); // 禁止抢占
	ret = __module_address(addr);
	preempt_enable(); // 允许抢占

	/* 如果返回不为空，则输出该模块的信息 */
	if(ret != NULL) {
		printk("ret->name: %s\n", ret->name);
		printk("ret->state: %d\n", ret->state);
	#if LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,36)
		printk("ret->core_layout.size: %d\n", ret->core_layout.size);
	#else
		printk("ret->core_size: %d\n", ret->core_size);
	#endif
		printk("ret->name: %s\n", ret->name);
	} else {
		printk(KERN_ERR "__module_address return NULL\n");
	}

	printk("ApiModuleAddress module init ok\n");

	return 0;
}

void __exit ApiModuleAddress_exit(void) {
	printk("ApiModuleAddress module exit ok\n");
}

module_init(ApiModuleAddress_init);
module_exit(ApiModuleAddress_exit);
