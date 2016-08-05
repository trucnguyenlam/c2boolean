#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define NUM_THREADS 3

// typedef struct SafeStackItem
// {
//     volatile int Value;
//     int Next;
// } SafeStackItem;

// typedef struct SafeStack
// {
//     SafeStackItem array;
//     int head;
//     int count;
// } SafeStack;

pthread_t threads[NUM_THREADS];

/// Global
// SafeStack stack;
unsigned int stack;

// int STACK_VALUE[3];
// int MALLOC_VALUE[6];

// void __VERIFIER_atomic_store(int *obj, int v)
// {
//     *obj = v;
// }
void __VERIFIER_atomic_store(unsigned int obj, int v)
{
    STACK_VALUE[obj] = v;
}
void __VERIFIER_atomic_store_malloc(unsigned int obj, int v)
{
    MALLOC_VALUE[obj] = v;
}

// int __VERIFIER_atomic_load(int *obj)
// {
//     return *obj;
// }
int __VERIFIER_atomic_load(unsigned int obj)
{
    return STACK_VALUE[obj];
}

// int __VERIFIER_atomic_exchange(int *obj, int v)
// {
//     int t = *obj;
//     *obj = v;
//     return t;
// }
int __VERIFIER_atomic_exchange(unsigned int obj, int v)
{
    int t = STACK_VALUE[obj];
    STACK_VALUE[obj] = v;
    return t;
}

int __VERIFIER_atomic_exchange_malloc(unsigned int obj, int v)
{
    int t = MALLOC_VALUE[obj];
    MALLOC_VALUE[obj] = v;
    return t;
}

// _Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
// {
//     if(*obj == *expected)
//     {
//         *obj = desired;
//         return 1;
//     }
//     else
//     {
//         *expected = *obj;
//         return 0;
//     }
// }
_Bool __VERIFIER_atomic_compare_and_exchange(unsigned int obj, int *expected, int desired)
{
    if(STACK_VALUE[obj] == *expected)
    {
        STACK_VALUE[obj] = desired;
        return 1;
    }
    else
    {
        *expected = STACK_VALUE[obj];
        return 0;
    }
}

// int __VERIFIER_atomic_fetch_add(int * obj, int v)
// {
//     int old = *obj;
//     *obj = old + v;
//     return old;
// }
int __VERIFIER_atomic_fetch_add(unsigned int obj, int v)
{
    int old = STACK_VALUE[obj];
    STACK_VALUE[obj] = old + v;
    return old;
}

// int __VERIFIER_atomic_fetch_sub(int * obj, int v)
// {
//     int old = *obj;
//     *obj = old - v;
//     return old;
// }
int __VERIFIER_atomic_fetch_sub(unsigned int obj, int v)
{
    int old = STACK_VALUE[obj];
    STACK_VALUE[obj] = old - v;
    return old;
}
void Init(int pushCount)
{
    int i;
    // stack.array = malloc(3 * sizeof(SafeStackItem));
    STACK_VALUE[stack + 0] = 0;
    // __VERIFIER_atomic_store(&stack.count, pushCount);
    __VERIFIER_atomic_store(stack + 2, pushCount);
    // __VERIFIER_atomic_store(&stack.head, 0);
    __VERIFIER_atomic_store(stack + 1, 0);
    for (i = 0; i < pushCount - 1; i++)
    {
        // __VERIFIER_atomic_store(&stack.array[i].Next, i + 1);
        __VERIFIER_atomic_store_malloc(STACK_VALUE[stack + 0] + 2*i + 1, i + 1);
    }
    // __VERIFIER_atomic_store(&stack.array[pushCount - 1].Next, -1);
    __VERIFIER_atomic_store_malloc(STACK_VALUE[stack + 0] + 2*(pushCount - 1) + 1, -1);
}

void Destroy(void)
{
    // free(stack.array);
}

int Pop(void)
{
    // while (__VERIFIER_atomic_load(&stack.count) > 1)
    while (__VERIFIER_atomic_load(stack + 2) > 1)
    {
        // int head1 = __VERIFIER_atomic_load(&stack.head);
        int head1 = __VERIFIER_atomic_load(stack + 1);
        // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
        int next1 = __VERIFIER_atomic_exchange_malloc(STACK_VALUE[stack + 0] + 2*head1 + 1, -1);

        if (next1 >= 0)
        {
            int head2 = head1;
            // if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))
            if (__VERIFIER_atomic_compare_and_exchange(stack + 1, &head2, next1))
            {
                // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                __VERIFIER_atomic_fetch_sub(stack + 2, 1);
                return head1;
            }
            else
            {
                // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                __VERIFIER_atomic_exchange_malloc(STACK_VALUE[stack + 0] + 2*(head1) + 1, next1);
            }
        }
    }

    return -1;
}

void Push(int index)
{
    // int head1 = __VERIFIER_atomic_load(&stack.head);
    int head1 = __VERIFIER_atomic_load(stack + 1);
    do
    {
        // __VERIFIER_atomic_store(&stack.array[index].Next, head1);
        __VERIFIER_atomic_store_malloc(STACK_VALUE[stack + 0] + 2*(index) + 1, head1);

    // } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
    } while (!(__VERIFIER_atomic_compare_and_exchange(stack + 1, &head1, index)));
    // __VERIFIER_atomic_fetch_add(&stack.count, 1);
    __VERIFIER_atomic_fetch_add(stack + 2, 1);
}


void* thread(void* arg)
{
    size_t i;
    int idx = (int)(size_t)arg;
    for (i = 0; i < 2; i++)
    {
        int elem;
        for (;;)
        {
            elem = Pop();
            if (elem >= 0)
                break;
        }

        // stack.array[elem].Value = idx;
        MALLOC_VALUE[STACK_VALUE[stack + 0] + 2*(elem)] = idx;
        assert(MALLOC_VALUE[STACK_VALUE[stack + 0] + 2*(elem)] == idx);

        Push(elem);
    }
    return NULL;
}

int main(void)
{
    // int i;
    Init(NUM_THREADS);
    // for (i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[0], NULL, thread, 0);
        pthread_create(&threads[1], NULL, thread, 1);
        pthread_create(&threads[2], NULL, thread, 2);
    // }

    // for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
    // }

    return 0;
}

