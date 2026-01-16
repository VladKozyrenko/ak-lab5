// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>

#include "hello1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("IO-33 Kozyrenko Vladyslav");
MODULE_DESCRIPTION("hello2: calls print_hello() from hello1");

static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "How many times to call print_hello() (default=1)");

static int __init hello2_init(void)
{
	int ret;

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello2: count=%u (warning case), continuing\n", count);

	if (count > 10) {
		pr_err("hello2: count=%u is too large, refusing to load\n", count);
		return -EINVAL;
	}

	ret = print_hello(count);
	if (ret) {
		pr_err("hello2: print_hello() failed: %d\n", ret);
		return ret;
	}

	pr_info("hello2: called print_hello() %u times\n", count);
	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

