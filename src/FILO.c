#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "coroutine_int.h"

void myrq_init(struct my_rq *rq)
{
    // setup the ring buffer
    rq->top = 0;
    rq->mask = RINGBUFFER_SIZE - 1;
}

static inline unsigned int __ringbuffer_unused(struct my_rq *rq)
{
    return (rq->top == rq->mask);
}

// enqueue at top
int myrq_enqueue(struct my_rq *rq, struct task_struct *task)
{
    if (__ringbuffer_unused(rq))
        return -EAGAIN;

    rq->r[rq->top] = task;
    rq->top++;

    return 0;
}

// dequeue at top
struct task_struct *myrq_dequeue(struct my_rq *rq)
{
    struct task_struct *rev;

    if (rq->top == 0)
        return NULL;
    rq->top--;
    rev = rq->r[rq->top];
    return rev;
}
