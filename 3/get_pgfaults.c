#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/version.h>

 
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0) 
#define HAVE_PROC_OPS 
#endif 

#define procfs_name "get_pgfaults"

static struct proc_dir_entry *proc_file; 
static unsigned long *cntr_buf;

static ssize_t procfile_read(struct file *file_pointer, char __user *buffer,size_t buffer_length, loff_t *offset)  {
  ssize_t len;
  char s[100];

  if (*offset > 0) {
     len = 0;
  } else {
     all_vm_events(cntr_buf);
     len=sprintf(s, "%lu\n", cntr_buf[PGFAULT]);
     copy_to_user(buffer,s,len);
    *offset=1;
  }
  return len;
}


static const struct proc_ops proc_file_fops = { 
    .proc_read = procfile_read, 
}; 

static int __init get_pgfaults_init(void) 
{ 
    cntr_buf = kmalloc(NR_VM_ZONE_STAT_ITEMS * sizeof(unsigned long),GFP_KERNEL );

    if (cntr_buf == NULL) {
            printk("kmalloc failed.\n");
        }

    proc_file = proc_create(procfs_name, 0644, NULL, &proc_file_fops); 
    if (NULL == proc_file) { 
        proc_remove(proc_file); 
        pr_alert("Error:Could not initialize /proc/%s\n", procfs_name); 
        return -ENOMEM; 
    } 
 
    pr_info("/proc/%s created\n", procfs_name); 
    return 0; 
}


static void __exit get_pgfaults_exit(void) 
{ 
    proc_remove(proc_file); 
    kfree(cntr_buf);
    pr_info("/proc/%s removed\n", procfs_name); 
} 

module_init(get_pgfaults_init); 
module_exit(get_pgfaults_exit); 
 
MODULE_LICENSE("GPL");