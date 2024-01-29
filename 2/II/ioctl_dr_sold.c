#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/ioctl.h>

typedef struct
{
    pid_t self_pid; 
    pid_t parent_pid;
} driver_arg_t;

#define CHNG_PARENT _IOW('q', 1, driver_arg_t *)

#define FIRST_MINOR 0
#define MINOR_CNT 1

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

static void change_parent(pid_t sold_pid, pid_t central_station_pid)
{
    struct task_struct *task_central_station, *task_sold;
    struct list_head *head_central_station, *head_sold, *tem_h;

    task_sold = pid_task(find_get_pid(sold_pid), PIDTYPE_PID);
    task_central_station = pid_task(find_get_pid(central_station_pid), PIDTYPE_PID);

    if (task_central_station == NULL || task_sold == NULL)
    {
        printk(KERN_INFO "Invalid pid");
        return;
    }

    head_sold = &task_sold->parent->children;

    list_for_each(tem_h, head_sold)
    {
        if (list_entry(tem_h, struct task_struct, sibling)->pid == task_sold->pid)
        {
            printk(KERN_INFO "Found soldier's pid in parent's children list\n");
            list_del(tem_h);
            printk(KERN_INFO "Deleted soldier's pid from parent's children list\n");
            break;
        }
    }

    task_sold->parent = task_central_station;
    task_sold->real_parent = task_central_station;
    printk(KERN_INFO "soldier's parent Changed\n");

    head_central_station = &task_central_station->children;
    list_add(&task_sold->sibling, head_central_station);

    printk(KERN_INFO "%d %d\n", sold_pid, central_station_pid);
    return;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    driver_arg_t q;

    switch (cmd)
    {
    case CHNG_PARENT:
        if (copy_from_user(&q, (driver_arg_t *)arg, sizeof(driver_arg_t)))
        {
            return -EACCES;
        }
        change_parent(q.self_pid, q.parent_pid);
        break;
    default:
        return -EINVAL;
    }

    return 0;
}

static struct file_operations driver_fops =
    {
        .owner = THIS_MODULE,
        .open = my_open,
        .release = my_close,
        .unlocked_ioctl = my_ioctl};

static int __init ioctl_driver_init(void)
{
    int ret;
    struct device *dev_ret;

    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "ioctl_dr_sold")) < 0)
    {
        return ret;
    }

    cdev_init(&c_dev, &driver_fops);

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
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "ioctl_dr_sold")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }

    return 0;
}

static void __exit ioctl_driver_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}

module_init(ioctl_driver_init);
module_exit(ioctl_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("23M0775");
MODULE_DESCRIPTION("ioctl_dr_sold driver to change parent of a soldier");