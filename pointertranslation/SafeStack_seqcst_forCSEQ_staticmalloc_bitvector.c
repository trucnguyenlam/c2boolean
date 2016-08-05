#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define NUM_THREADS 3

typedef struct SafeStackItem
{
    volatile __CPROVER_bitvector[8] Value;
    __CPROVER_bitvector[8] Next;
} SafeStackItem;

typedef struct SafeStack
{
    SafeStackItem* array;
    __CPROVER_bitvector[8] head;
    __CPROVER_bitvector[8] count;
} SafeStack;

pthread_t threads[NUM_THREADS];

/// Global
SafeStack stack;

void __VERIFIER_atomic_store(__CPROVER_bitvector[8] *obj, __CPROVER_bitvector[8] v)
{
    *obj = v;
}

__CPROVER_bitvector[8] __VERIFIER_atomic_load(__CPROVER_bitvector[8] *obj)
{
    return *obj;
}

__CPROVER_bitvector[8] __VERIFIER_atomic_exchange(__CPROVER_bitvector[8] *obj, __CPROVER_bitvector[8] v)
{
    __CPROVER_bitvector[8] t = *obj;
    *obj = v;
    return t;
}

_Bool __VERIFIER_atomic_compare_and_exchange(__CPROVER_bitvector[8] *obj, __CPROVER_bitvector[8] *expected, __CPROVER_bitvector[8] desired)
{
    if(*obj == *expected)
    {
        *obj = desired;
        return 1;
    }
    else
    {
        *expected = *obj;
        return 0;
    }
}

__CPROVER_bitvector[8] __VERIFIER_atomic_fetch_add(__CPROVER_bitvector[8] * obj, __CPROVER_bitvector[8] v)
{
    __CPROVER_bitvector[8] old = *obj;
    *obj = old + v;
    return old;
}

__CPROVER_bitvector[8] __VERIFIER_atomic_fetch_sub(__CPROVER_bitvector[8] * obj, __CPROVER_bitvector[8] v)
{
    __CPROVER_bitvector[8] old = *obj;
    *obj = old - v;
    return old;
}

void Init(__CPROVER_bitvector[8] pushCount)
{
    __CPROVER_bitvector[8] i;
    stack.array = malloc(3 * sizeof(SafeStackItem));
    __VERIFIER_atomic_store(&stack.count, pushCount);
    __VERIFIER_atomic_store(&stack.head, 0);
    for (i = 0; i < pushCount - 1; i++)
    {
        __VERIFIER_atomic_store(&stack.array[i].Next, i + 1);
    }
    __VERIFIER_atomic_store(&stack.array[pushCount - 1].Next, -1);
}

void Destroy(void)
{
    free(stack.array);
}

__CPROVER_bitvector[8] Pop(void)
{
    while (__VERIFIER_atomic_load(&stack.count) > 1)
    {
        __CPROVER_bitvector[8] head1 = __VERIFIER_atomic_load(&stack.head);
        __CPROVER_bitvector[8] next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);

        if (next1 >= 0)
        {
            __CPROVER_bitvector[8] head2 = head1;
            if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))
            {
                __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                return head1;
            }
            else
            {
                __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
            }
        }
    }

    return -1;
}

void Push(__CPROVER_bitvector[8] index)
{
    __CPROVER_bitvector[8] head1 = __VERIFIER_atomic_load(&stack.head);
    do
    {
        __VERIFIER_atomic_store(&stack.array[index].Next, head1);

    } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
    __VERIFIER_atomic_fetch_add(&stack.count, 1);
}


void* thread(void* arg)
{
    size_t i;
    __CPROVER_bitvector[8] idx = (__CPROVER_bitvector[8])(size_t)arg;
    for (i = 0; i < 2; i++)
    {
        __CPROVER_bitvector[8] elem;
        for (;;)
        {
            elem = Pop();
            if (elem >= 0)
                break;
        }

        stack.array[elem].Value = idx;
        assert(stack.array[elem].Value == idx);

        Push(elem);
    }
    return NULL;
}

int main(void)
{
    __CPROVER_bitvector[8] i;
    Init(NUM_THREADS);
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, thread, (void *)i);
    }

    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

