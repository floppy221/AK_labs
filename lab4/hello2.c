#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"
static unsigned int helloCounter = 1;
module_param(helloCounter, uint, 0644);
MODULE_PARM_DESC(helloCounter, "Param desc");
MODULE_AUTHOR("Denys Kutsepal");
MODULE_DESCRIPTION("Custom module description");
MODULE_LICENSE("Dual BSD/GPL");
static int __init hello2_init(void)
{
  if (helloCounter > 10) {
    pr_err("Parameter should be <= 10\n");
    return -EINVAL;
  } else if (helloCounter == 0 || (helloCounter >= 5 && helloCounter <= 10))
    pr_warn("Parameter is 0 or >5 <10\n");
  print_hello(helloCounter);
  return 0;
}
static void __exit hello2_exit(void)
{
  pr_info("Hello2 module uninstalled\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
