#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinit");
MODULE_DESCRIPTION("List Processes Kernel Module");

static int __init lkm1_init(void) {
    struct task_struct *task;
    
    printk(KERN_INFO "Listing all processes in running or runnable state:\n");

    // Iterate through each process
    for_each_process(task) {
        if (task->__state == TASK_RUNNING) {
            printk(KERN_INFO "PID: %d\n", task->pid);
        }
    }

    return 0;
}

static void __exit lkm1_exit(void) {
    printk(KERN_INFO "Module Unloaded\n");
}

module_init(lkm1_init);
module_exit(lkm1_exit);
