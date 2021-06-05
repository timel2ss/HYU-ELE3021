#include "sched.h"

static void put_prev_task_myprio(struct rq *rq, struct task_struct *p);
static int select_task_rq_myprio(struct task_struct *p, int cpu, int sd_flag, int flags);
static void set_curr_task_myprio(struct rq *rq);
static void task_tick_myprio(struct rq *rq,struct task_struct *p, int oldprio);
static void switched_to_myprio(struct rq *rq, struct task_struct *p);
void init_myprio_rq(struct myprio_rq *myprio_rq);
static void update_curr_myprio(struct rq *rq);
static void enqueue_task_myprio(struct rq *rq, struct task_struct *p, int flags);
static void dequeue_task_myprio(struct rq *rq, struct task_struct *p, int flags);
static void check_preempt_curr_myprio(struct rq *rq, struct task_struct *p,int flags);
struct task_struct *pick_next_task_myprio(struct rq *rq, struct task_struct *prev);
static void prio_changed_myprio(struct rq *rq, struct task_struct *p, int oldprio);

const struct sched_class myprio_sched_class={
        .next=&fair_sched_class,
        .enqueue_task=&enqueue_task_myprio,
        .dequeue_task=dequeue_task_myprio,
        .check_preempt_curr=check_preempt_curr_myprio,
        .pick_next_task=pick_next_task_myprio,
        .put_prev_task=put_prev_task_myprio,
#ifdef CONFIG_SMP
        .select_task_rq=select_task_rq_myprio,
#endif
        .set_curr_task=set_curr_task_myprio,
        .task_tick=task_tick_myprio,
        .prio_changed=prio_changed_myprio,
        .switched_to=switched_to_myprio,
        .update_curr=update_curr_myprio,
};


void init_myprio_rq (struct myprio_rq *myprio_rq)
{
        printk(KERN_INFO "***[MYPRIO] Myprio class is online \n");
        myprio_rq->nr_running = 0;
        INIT_LIST_HEAD(&myprio_rq->queue);
}

static void update_curr_myprio(struct rq *rq){
        struct task_struct* curr_p = rq->curr;
        struct sched_myprio_entity* curr_entity = &curr_p->myprio;
        struct myprio_rq* myprio_rq = &rq->myprio;

        /*
        If highest priority task is not curr, change its position in rq to curr's position
        Set old curr's wait time, Set old curr's priority to originnal priority
        Ask rescheduling
        */
        unsigned int highest_prio = 0;
        struct sched_myprio_entity* highest_prio_entity = NULL;
        struct task_struct* highest_prio_task = NULL;

        // find the highest priority task in the myprio rq
        struct sched_myprio_entity* iter = NULL;
        list_for_each_entry(iter, myprio_rq->queue, run_list) {
                if(highest_prio < iter->priority) {
                        highest_prio = iter->priority;
                        hightest_prio_entity = iter;
                }
        }
        highest_prio_task = container_of(highest_prio_entity, struct task_struct, myprio);

        if(curr_entity->priority < highest_prio_entity->priority) {
                printk(KERN_INFO "***[MYPRIO] update_curr_myprio         curr pid=%d curr priority=%d next pid=%d next priority=%d \n", curr_p->pid, curr_entity->priority, highest_prio_task->pid, highest_prio_entity->priority);
                list_del(&highest_prio_entity->run_list);
                list_add(&highest_prio_entity->run_list, myprio_rq->run_list);
                resched_curr(rq);
        }

}

static void enqueue_task_myprio(struct rq *rq, struct task_struct *p, int flags) {
        struct myprio_rq* myprio_rq = &rq->myprio;
        struct sched_myprio_entity* myprio_entity = &p->myprio;

        // priority inject
        // myprio_entity->priority = p->mypriority
        list_add_tail(&myprio_entity->run_list, &myprio_rq->queue);
        myprio_rq->nr_running++;

        printk(KERN_INFO "***[MYPRIO] enqueue: success cpu=%d,nr_running=%d,p->state=%ld,p->pid=%d \n", cpu_of(rq), myprio_rq->nr_running, p->state, p->pid);
}
static void dequeue_task_myprio(struct rq *rq, struct task_struct *p, int flags)
{
        struct myprio_rq* myprio_rq = &rq->myprio;
        struct sched_myprio_entity* myprio_entity = &p->myprio;


        if(myprio_rq->nr_running > 0) {
                printk(KERN_INFO "***[MYPRIO] dequeue: start \n");
                list_del_init(&myprio_entity->run_list);
                myprio_rq->nr_running--;
                printk(KERN_INFO "***[MYPRIO] dequeue: the dequeued task is curr, set TIF_NEED_RESCHED flag cpu=%d,p->state=%ld,p->pid=%d,curr->pid=%d \n", cpu_of(rq), p->state, p->pid, rq->curr->pid);
                printk(KERN_INFO "***[MYPRIO] dequeue: end \n");
        }
}
void check_preempt_curr_myprio(struct rq *rq, struct task_struct *p, int flags) {
        printk("***[MYPRIO] check_preempt_curr_myprio\n");
}
struct task_struct *pick_next_task_myprio(struct rq *rq, struct task_struct *prev)
{
        struct task_struct* next_p = NULL;
        struct myprio_rq* myprio_rq = &rq->myprio;
        struct list_head* queue = &myprio_rq->queue;
        struct sched_myprio_entity* next_entity = NULL;

        if(rq->myprio.nr_running == 0) {
                return NULL;
        }

        next_entity = list_entry(queue->next, struct sched_myprio_entity, run_list);
        next_p = container_of(next_entity, struct task_struct, myprio);

        printk(KERN_INFO "***[MYPRIO] pick_next_task: cpu=%d,prev->pid=%d, next_p->pid=%d,nr_running=%d \n", cpu_of(rq), prev->pid, next_p->pid, myprio_rq->nr_running);

        return next_p;
}
void put_prev_task_myprio(struct rq *rq, struct task_struct *p) {
        printk(KERN_INFO "\t***[MYPRIO] put_prev_task: do_nothing, p->pid=%d\n",p->pid);
}
int select_task_rq_myprio(struct task_struct *p, int cpu, int sd_flag, int flags){return task_cpu(p);}
void set_curr_task_myprio(struct rq *rq){
        printk(KERN_INFO"***[MYPRIO] set_curr_task_myprio\n");
}
void task_tick_myprio(struct rq *rq, struct task_struct *p, int queued) {
        update_curr_myprio(rq);
}
void prio_changed_myprio(struct rq *rq, struct task_struct *p, int oldprio) { }
/*This routine is called when a task migrates between classes*/
void switched_to_myprio(struct rq *rq, struct task_struct *p)
{
        resched_curr(rq);
}