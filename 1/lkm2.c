#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/pid.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinit");
MODULE_DESCRIPTION("Kernel Module to Print Child Processes' PID");

static int pid = 1; // Default to process with PID 1 (init process)

module_param(pid, int, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP);
MODULE_PARM_DESC(pid, "process ID (default: 1)");

static void print_child_processes(struct task_struct *task) {
    struct task_struct *child;
    struct list_head *list;

    // Iterate over the list of child processes
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        
        // Print PID and process state of each child process
        pr_info("Child PID: %d\n", child->pid);

        // Recursively call for child's children
        print_child_processes(child);
    }
}

static int __init lkm2_init(void) {
    struct pid *pid_struct;
    struct task_struct *task;

    pr_info("LKM2: Module loaded\n");

    // Get the task_struct of the process ID
    pid_struct = find_get_pid(pid);
    task = pid_task(pid_struct, PIDTYPE_PID);

    if (task) {
        pr_info("Process PID: %d\n", task->pid);
        print_child_processes(task);
    } else {
        pr_err("LKM2:process with PID %d not found\n",pid);
        return -ESRCH; // No such process error
    }

    return 0;
}

static void __exit lkm2_exit(void) {
    pr_info("LKM2: Module unloaded\n");
}

module_init(lkm2_init);
module_exit(lkm2_exit);
