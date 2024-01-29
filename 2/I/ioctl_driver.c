#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <asm/io.h>
#include <asm/processor.h>

#define GET_PHY_ADDR _IO('q',1)
#define CHNG_VAL _IO('q',2)

 
#define FIRST_MINOR 0
#define MINOR_CNT 1

typedef struct
{
    unsigned long val,pid, virt_addr,phys_addr;
}arg_t;

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    arg_t a;
 
    switch (cmd)
    {
        case GET_PHY_ADDR:
            if (copy_from_user(&a, (arg_t *)arg, sizeof(arg_t)))
            {
                return -EACCES;
            }

            a.phys_addr = virt_to_phys(a.virt_addr);
            if (copy_to_user((void __user *)arg, &a, sizeof(arg_t)))
                return -EFAULT;
            break;

            // if (copy_from_user(&a, (arg_t *)arg, sizeof(arg_t)))
            // {
            //     return -EACCES;
            // }
            // struct pid *pid_struct;
            // struct task_struct *task;
            // struct mm_struct *mm;
            // struct vm_area_struct *vma;
            // unsigned long pfn;


            // if (a.pid <= 0 || a.virt_addr == 0) {
            //     pr_err("Invalid input parameters\n");
            //     return -EINVAL;
            // }

            // // Get the task_struct of the target process ID
            // pid_struct = find_get_pid(a.pid);
            // task = pid_task(pid_struct, PIDTYPE_PID);

            // if (!task) {
            //     pr_err("Target process with PID %d not found\n", a.pid);
            //     return -ESRCH; // No such process error
            // }

            // mm = get_task_mm(task);

            // if (!mm) {
            //     pr_err("Unable to get memory information for PID %d\n", a.pid);
            //     return -ENOMEM;
            // }

            // // Iterate through the process's memory mappings
            // down_read(&mm->mmap_lock);

            // vma=find_vma(mm,a.virt_addr);

            // if(vma!=NULL)
            // {
            //     pfn = page_to_pfn(virt_to_page((void *)a.virt_addr));
            //     if(pfn_valid(pfn))
            //     {
            //     a.phys_addr = (pfn << PAGE_SHIFT) + (a.virt_addr & ~PAGE_MASK);
            //     printk(KERN_INFO "PID:%d, Virtual address: %lu, Physical address: %llx\n",a.pid,a.virt_addr,a.phys_addr);
            //     } else{
            //     printk(KERN_INFO"Virtual address %lu is not mapped in the address space of PID %d\n",a.virt_addr,a.pid);
            //     }
            // }
            // up_read(&mm->mmap_lock);

            // mmput(mm);

            // if (copy_to_user((arg_t *)arg, &a, sizeof(arg_t)))
            // {
            //     return -EACCES;
            // }
            // break;

        case CHNG_VAL:
            if (copy_from_user(&a, (arg_t *)arg, sizeof(arg_t)))
            {
                return -EACCES;
            }
            // if (!phys_addr_valid(a.phys_addr))
            // return -EINVAL;

            unsigned long virt= phys_to_virt(a.phys_addr);
            *(int*)virt=a.val;
            
            break;
        default:
            return -EINVAL;
    }
 
    return 0;
}

static struct file_operations query_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = my_ioctl

};

static int __init ioctl_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "ioctl_driver")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &query_fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "ioctl_driver")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
 
    return 0;
}
static void __exit ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}
 
module_init(ioctl_init);
module_exit(ioctl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinit");
MODULE_DESCRIPTION("ioctl() Char Driver");