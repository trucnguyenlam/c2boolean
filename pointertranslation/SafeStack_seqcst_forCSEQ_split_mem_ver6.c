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
//     unsigned int array_Value;
//     unsigned int array_Next;
//     int head;
//     int count;
// } SafeStack;

pthread_t threads[NUM_THREADS];

/// Global
// SafeStack stack;
unsigned int stack;
// stack + 0 array_value
// stack + 1 array_next
// stack + 2 head
// stack + 3 count
// stack + 4 - 6 value
// stack + 7 - 9 next

// int MEMORY_VALUE[10];

// void __VERIFIER_atomic_store(int *obj, int v)
// {
//     *obj = v;
// }
void __VERIFIER_atomic_store(unsigned int obj, int v)
{
    MEMORY_VALUE[obj] = v;
}

// int __VERIFIER_atomic_load(int *obj)
// {
//     return *obj;
// }
int __VERIFIER_atomic_load(unsigned int obj)
{
    return MEMORY_VALUE[obj];
}

// int __VERIFIER_atomic_exchange(int *obj, int v)
// {
//     int t = *obj;
//     *obj = v;
//     return t;
// }
int __VERIFIER_atomic_exchange(unsigned int obj, int v)
{
    int t = MEMORY_VALUE[obj];
    MEMORY_VALUE[obj] = v;
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
    if(MEMORY_VALUE[obj] == *expected)
    {
        MEMORY_VALUE[obj] = desired;
        return 1;
    }
    else
    {
        *expected = MEMORY_VALUE[obj];
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
    int old = MEMORY_VALUE[obj];
    MEMORY_VALUE[obj] = old + v;
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
    int old = MEMORY_VALUE[obj];
    MEMORY_VALUE[obj] = old - v;
    return old;
}

void Init(int pushCount)
{
    int i;
    // stack.array = malloc(3 * sizeof(SafeStackItem));
    MEMORY_VALUE[stack + 0] = 4;
    MEMORY_VALUE[stack + 1] = 7;
    // __VERIFIER_atomic_store(&stack.count, pushCount);
    __VERIFIER_atomic_store(stack + 3, pushCount);
    // __VERIFIER_atomic_store(&stack.head, 0);
    __VERIFIER_atomic_store(stack + 2, 0);
    for (i = 0; i < pushCount - 1; i++)
    {
        // __VERIFIER_atomic_store(&stack.array[i].Next, i + 1);
        __VERIFIER_atomic_store(MEMORY_VALUE[stack + 1] + i, i + 1);
    }
    // __VERIFIER_atomic_store(&stack.array[pushCount - 1].Next, -1);
    __VERIFIER_atomic_store(MEMORY_VALUE[stack + 1] + (pushCount - 1), -1);
}

void Destroy(void)
{
    // free(stack.array);
}

int Pop(void)
{
    // while (__VERIFIER_atomic_load(&stack.count) > 1)
    while (__VERIFIER_atomic_load(stack + 3) > 1)
    {
        // int head1 = __VERIFIER_atomic_load(&stack.head);
        int head1 = __VERIFIER_atomic_load(stack + 2);
        // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
        int next1 = __VERIFIER_atomic_exchange(MEMORY_VALUE[stack + 1] + head1, -1);

        if (next1 >= 0)
        {
            int head2 = head1;
            // if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))
            if (__VERIFIER_atomic_compare_and_exchange(stack + 2, &head2, next1))
            {
                // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                __VERIFIER_atomic_fetch_sub(stack + 3, 1);
                return head1;
            }
            else
            {
                // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                __VERIFIER_atomic_exchange(MEMORY_VALUE[stack + 1] + head1, next1);
            }
        }
    }

    return -1;
}

void Push(int index)
{
    // int head1 = __VERIFIER_atomic_load(&stack.head);
    int head1 = __VERIFIER_atomic_load(stack + 2);
    do
    {
        // __VERIFIER_atomic_store(&stack.array[index].Next, head1);
        __VERIFIER_atomic_store(MEMORY_VALUE[stack + 1] + index, head1);

    // } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
    } while (!(__VERIFIER_atomic_compare_and_exchange(stack + 2, &head1, index)));
    // __VERIFIER_atomic_fetch_add(&stack.count, 1);
    __VERIFIER_atomic_fetch_add(stack + 3, 1);
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
        MEMORY_VALUE[MEMORY_VALUE[stack + 0] + elem] = idx;
        // assert(stack.array[elem].Value == idx);
        assert(MEMORY_VALUE[MEMORY_VALUE[stack + 0] + elem] == idx);

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

