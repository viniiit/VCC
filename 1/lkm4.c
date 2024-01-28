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

module_param(target_pid, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(target_pid, "Target process ID");

static int __init lkm4_init(void) {
    struct pid *pid_struct;
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct vma_iterator __vmi;
    unsigned long phys_addr,pfn;
    

    pr_info("LKM4: Module loaded\n");

    if (target_pid <= 0) {
        pr_err("LKM4: Invalid input parameters\n");
        return -EINVAL;
    }

    pid_struct = find_get_pid(target_pid);
    task = pid_task(pid_struct, PIDTYPE_PID);    

    if (!task) {
        pr_err("LKM4: Target process with PID %d not found\n", target_pid);
        return -ESRCH; // No such process error
    }

    mm = get_task_mm(task);

    if (!mm) {
        pr_err("LKM4: Unable to get memory information for PID %d\n", target_pid);
        return -ENOMEM;
    }

    int pages=mm->total_vm;

    unsigned long total_vms = pages * (1UL << PAGE_SHIFT);


    printk(KERN_INFO "Total virtual memory space of PID %d : %lu\n",target_pid, total_vms);

    int anon = get_mm_counter(mm, MM_ANONPAGES);
    int file = get_mm_counter(mm, MM_FILEPAGES);
    int shmem = get_mm_counter(mm, MM_SHMEMPAGES);
    int total_pages= anon+file+shmem;
    printk(KERN_INFO "Physmem: Physical memory allocated to PID %d: %lu KB\n", target_pid, total_pages* (1UL << PAGE_SHIFT));
  

// unsigned long start, end;
// int phys_pages=0;

//         for_each_vma(__vmi,vma) {
//             start = vma->vm_start;
//             end = vma->vm_end;
//         printk(KERN_INFO"before while");
//             // Count the physical pages within the range
//             while (start!=end) {
//                 pfn = page_to_pfn(virt_to_page((void *)start));
//                     if(pfn_valid(pfn))
//                     {
//                     phy_page++;
//                     } 
//                 start+=PAGE_SIZE;
//             }
//         printk(KERN_INFO"after while");
//         }

//         printk(KERN_INFO "Physmem: Physical memory allocated to PID %d: %lu KB\n", target_pid, phy_page * (1UL << PAGE_SHIFT));
    return 0;
}

static void __exit lkm4_exit(void) {
    pr_info("LKM4: Module unloaded\n");
}

module_init(lkm4_init);
module_exit(lkm4_exit);
