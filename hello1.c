// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/moduleparam.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-33 Kozyrenko Vladyslav");
MODULE_DESCRIPTION("hello1: exports print_hello(), stores ktime events");

struct hello_event {
	struct list_head list;
	ktime_t t_start;
	ktime_t t_end;
};

static LIST_HEAD(hello_event_list);

int print_hello(unsigned int count)
{
	unsigned int i;

	for (i = 0; i < count; i++) {
		struct hello_event *ev;

		ev = kmalloc(sizeof(*ev), GFP_KERNEL);
		if (!ev)
			return -ENOMEM;

		ev->t_start = ktime_get();
		pr_info("hello1: Hello World call#%u\n", i + 1);
		ev->t_end = ktime_get();

		list_add_tail(&ev->list, &hello_event_list);
	}

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_event *ev, *tmp;

	list_for_each_entry_safe(ev, tmp, &hello_event_list, list) {
		s64 dt_ns = ktime_to_ns(ktime_sub(ev->t_end, ev->t_start));

		pr_info("hello1: print took %lld ns\n", dt_ns);
		list_del(&ev->list);
		kfree(ev);
	}

	pr_info("hello1: unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

