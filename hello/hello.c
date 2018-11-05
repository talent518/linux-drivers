//hello.c

//auther:heat nan

//programe:char driver-hello world

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>

MODULE_LICENSE("Dual BSD/GPL");   //内核2.6以上版本建议我们加上这个license

struct timer_list mytimer;
int __timezone = 0;

void mytimer_function(unsigned long arg) {
	struct timex txc;
	struct rtc_time tm;

	mod_timer(&mytimer, jiffies + HZ);

	do_gettimeofday(&(txc.time));

#ifdef TIMEZONE
	if(__timezone) {
		txc.time.tv_sec += __timezone;
	} else {
#endif
		txc.time.tv_sec -= sys_tz.tz_minuteswest * 60; // 把UTC时间调整为本地时间
#ifdef TIMEZONE
	}
#endif
	
	rtc_time_to_tm(txc.time.tv_sec, &tm);

	printk("%04d-%02d-%02d %02d:%02d:%02d: mytimer function (%lu)...\n", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, arg);
}

static int __init hello_init(void) {
	printk(KERN_ALERT "init hello driver module\n");

	init_timer(&mytimer);

	mytimer.expires = jiffies + HZ; // 设定超时时间，HZ代表1秒
	mytimer.data = 250; // 传递给定时器超时函数的值
	mytimer.function = mytimer_function; // 设置定时器超时函数
	
	add_timer(&mytimer);

#ifdef TIMEZONE
{
	char sign='+';
	int hour=0, minute=0;
	if(sscanf(TIMEZONE, "%c%d:%d", &sign, &hour, &minute)!=3) {
		printk(KERN_ERR "sscanf input items error for TIMEZONE micro\n");
	}
	__timezone = hour*3600+minute*60;
	if(sign == '-') {
		__timezone = - __timezone;
	}
	printk("timezone = "TIMEZONE" = %d, sign = %c, hour = %d, minute = %d\n", __timezone, sign, hour, minute);
}
#endif


	return 0;
}

static void __exit hello_exit(void) {
	del_timer(&mytimer);

	printk(KERN_ALERT "exit hello driver module\n");
}

module_init(hello_init);
module_exit(hello_exit);
