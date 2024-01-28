#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinit");
MODULE_DESCRIPTION("Kernel Module to Map Virtual to Physical Address");

static int target_pid = -1;
static unsigned long target_address = 0;

module_param(target_pid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(target_pid, "Target process ID");
module_param(target_address, ulong, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(target_address, "Target virtual address");

static int __init lkm3_init(void) {
    struct pid *pid_struct;
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    unsigned long pfn, phys_addr;

    pr_info("LKM3: Module loaded\n");

    if (target_pid <= 0 || target_address == 0) {
        pr_err("LKM3: Invalid input parameters\n");
        return -EINVAL;
    }

    // Get the task_struct of the target process ID
    pid_struct = find_get_pid(target_pid);
    task = pid_task(pid_struct, PIDTYPE_PID);

    if (!task) {
        pr_err("LKM3: Target process with PID %d not found\n", target_pid);
        return -ESRCH; // No such process error
    }

    mm = get_task_mm(task);

    if (!mm) {
        pr_err("LKM3: Unable to get memory information for PID %d\n", target_pid);
        return -ENOMEM;
    }

    // Iterate through the process's memory mappings
    down_read(&mm->mmap_lock);

    vma=find_vma(mm,target_address);

    if(vma!=NULL)
    {
        pfn = page_to_pfn(virt_to_page((void *)target_address));
        if(pfn_valid(pfn))
        {
        phys_addr = (pfn << PAGE_SHIFT) + (target_address & ~PAGE_MASK);
        printk(KERN_INFO "PID:%d, Virtual address: %lu, Physical address: %llx\n",target_pid,target_address,phys_addr);
        } else{
        printk(KERN_INFO"Virtual address %lu is not mapped in the address space of PID %d\n",target_address,target_pid);
        }
    }
    up_read(&mm->mmap_lock);

    mmput(mm);

    return 0;
}

static void __exit lkm3_exit(void) {
    pr_info("LKM3: Module unloaded\n");
}

module_init(lkm3_init);
module_exit(lkm3_exit);
