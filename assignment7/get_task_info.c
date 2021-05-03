#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <linux/sched.h>

asmlinkage long sys_get_task_info(void)
{
    struct task_struct* task_struct = get_current();
    printk("PID: %d\n", task_struct->pid);
    printk("TGID: %d\n", task_struct->tgid);
    printk("PPID: %d\n", task_struct->real_parent->pid);
    printk("STATE: %ld\n", task_struct->state);
    printk("PRIORITY: %d\n", task_struct->prio);
    printk("STATIC_PRIO: %d\n", task_struct->static_prio);
    printk("NORMAL_PRIO: %d\n", task_struct->normal_prio);
    printk("RT_PRIORITY: %d\n", task_struct->rt_priority);
    printk("POLICY: %d\n", task_struct->policy);
    printk("Session ID: %d\n", task_struct->sessionid);
    return 0;
}
