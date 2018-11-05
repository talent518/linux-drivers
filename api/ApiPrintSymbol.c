#include <linux/module.h>
#include <linux/init.h>
#include <linux/kallsyms.h>

MODULE_LICENSE("GPL");

static int __init ApiPrintSymbol_init(void);
static void __init ApiPrintSymbol_exit(void);

// 符号 a_symbol
int a_symbol(void) { // 此处void必须添加
	return 1;
}
EXPORT_SYMBOL(a_symbol);

int __init ApiPrintSymbol_init(void) {
	char *fmt; // 格式化字符串
	unsigned long address; // 表示符号地址
	char *name; // 模块名字
	struct module *fmodule = NULL; // 指向一个模块的指针

	printk("ApiPrintSymbol module init ...\n");

	address = (unsigned long) __builtin_return_address(0); // 当前函数的返回地址
	fmt = "it's the first part,\n %s";
	__print_symbol(fmt, address);
	printk("\n\n");

	name = "usbhid"; // 可以通过命令lsmod查看当前内核模块，然后选择一个
	fmodule = find_module(name); // 查找模块名为"usbhid"的模块

	if(fmodule != NULL) {
		printk("fmodule->name: %s\n", fmodule->name);
		/* 将模块的内存起始地址赋值给address */
		fmt = "it's the second part, \n %s";
		__print_symbol(fmt, address);
	}
	printk("\n\n");

	/* 将当前模块中符号 a_symbol 的地址加四偏移量 5 赋值给 address */
	address = (unsigned long) a_symbol + 5;
	fmt = "it's the third part,\n %s";
	__print_symbol(fmt, address);
	printk("\n\n");

	printk("ApiPrintSymbol module init ok\n");

	return 0;
}

void __init ApiPrintSymbol_exit(void) {
	printk("ApiPrintSymbol module exit ok\n");
}

module_init(ApiPrintSymbol_init);
module_exit(ApiPrintSymbol_exit);