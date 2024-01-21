#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
static unsigned int helloCounter = 1;
module_param(helloCounter, uint, 0644);
MODULE_PARM_DESC(helloCounter, "Param desc");
MODULE_AUTHOR("Denys Kutsepal");
MODULE_DESCRIPTION("Custom module description");
MODULE_LICENSE("Dual BSD/GPL");
struct ls_object {
  struct list_head list;
  ktime_t timestamp;
};
static LIST_HEAD(ls_head);
static int __init hello_init(void)
{
  if (helloCounter > 10) {
    pr_err("Parameter should be <= 10\n");
    return -EINVAL;
  } else if (helloCounter == 0 || (helloCounter >= 5 && helloCounter <= 10))
    pr_warn("Parameter is 0 or >5 <10\n");

  int i;

  for (i = 0; i < helloCounter; i++) {
    struct ls_object *ls_elem = kmalloc(sizeof(*ls_elem), GFP_KERNEL);

    ls_elem->timestamp = ktime_get();
    list_add(&ls_elem->list, &ls_head);
    printk(KERN_EMERG "Hello, world!\n");
  }

  return 0;
}
static void __exit hello_exit(void)
{
  struct ls_object *ls_elem, *temp;

  list_for_each_entry_safe(ls_elem, temp, &ls_head, list) {
    pr_info("Time stamp: %lld\n", ls_elem->timestamp);
    list_del(&ls_elem->list);
    kfree(ls_elem);
  }
}
module_init(hello_init);
module_exit(hello_exit);
