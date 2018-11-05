#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static int __init ApiModuleTextAddress_init(void);
static void __exit ApiModuleTextAddress_exit(void);

int func_a(void) {
	return 0;
}

static int var_b = 4;

int __init ApiModuleTextAddress_init(void) {
	unsigned long addr = (unsigned long) func_a; // addr 为函数 func_a 的入口地址
	struct module *ret; // 用于接收测试函数返回值

	printk("ApiModuleTextAddress module init ...\n");

	/* 调用 __module_address() 函数之前，必须禁止中断，以防止模块在执行操作期间被释放 */
	preempt_disable(); // 禁止抢占
	ret = __module_text_address(addr);
	preempt_enable(); // 允许抢占

	/* 如果查找成功，则输出该模块的信息 */
	printk("it's about func_a:\n");
	if(ret != NULL) {
		printk("ret->name: %s\n", ret->name); // 输出模块名
		printk("ret->state: %d\n", ret->state); // 输出模块状态

		// 输出模块core段所占空间大小
	#if LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,36)
		printk("ret->core_layout.size: %d\n", ret->core_layout.size);
	#else
		printk("ret->core_size: %d\n", ret->core_size);
	#endif
		printk("refs of %s is %d \n", ret->name, module_refcount(ret));
	} else {
		printk(KERN_ERR "func_a is not in text area\n");
	}

	// =================================================================================
	printk(" \n");

	addr = (unsigned long)&var_b; // add 为静态全局变量 var_b 的地址
	preempt_disable(); // 禁止抢占
	ret = __module_text_address(addr);
	preempt_enable(); // 允许抢占

	/* 如果查找成功，则输出该模块的信息 */
	printk("it's about var_b:\n");
	if(ret != NULL) {
		printk("ret->name: %s\n", ret->name); // 输出模块名
		printk("ret->state: %d\n", ret->state); // 输出模块状态

		// 输出模块core段所占空间大小
	#if LINUX_VERSION_CODE>=KERNEL_VERSION(2,6,36)
		printk("ret->core_layout.size: %d\n", ret->core_layout.size);
	#else
		printk("ret->core_size: %d\n", ret->core_size);
	#endif
		printk("refs of %s is %d \n", ret->name, module_refcount(ret));
	} else {
		printk(KERN_ERR "var_b is not in text area\n");
	}

	printk("ApiModuleTextAddress module init ok\n");

	return 0;
}

void __exit ApiModuleTextAddress_exit(void) {
	printk("ApiModuleTextAddress module exit ok\n");
}

module_init(ApiModuleTextAddress_init);
module_exit(ApiModuleTextAddress_exit);
