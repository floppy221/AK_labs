#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"
MODULE_AUTHOR("Denys Kutsepal");
MODULE_DESCRIPTION("Custom module description");
MODULE_LICENSE("Dual BSD/GPL");
static LIST_HEAD(ls_head);
void print_hello(unsigned int helloCounter)
{
  int i;

  for (i = 0; i < helloCounter; i++) {
    struct ls_object *ls_elem = kmalloc(sizeof(*ls_elem), GFP_KERNEL);
    if (i == 1)
      ls_elem = 0;
    ls_elem->timestamp_start = ktime_get();
    pr_info("Hello, world!\n");
    ls_elem->timestamp_end = ktime_get();
    list_add(&ls_elem->list, &ls_head);
  }
}
EXPORT_SYMBOL(print_hello);
static void __exit hello1_exit(void)
{
  struct ls_object *ls_elem, *temp;
  list_for_each_entry_safe(ls_elem, temp, &ls_head, list) {
    pr_info("Time start: %lld\n", ls_elem->timestamp_start);
    pr_info("Time spent: %lld\n", ktime_to_ns(ktime_sub(ls_elem->timestamp_end, ls_elem->timestamp_start)));
    list_del(&ls_elem->list);
    kfree(ls_elem);
  }
}
module_exit(hello1_exit);
