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


pthread_t threads[NUM_THREADS];


/// Global, split stack structure
// int array_Value[NUM_THREADS];
// int array_Next[NUM_THREADS];
// int head;
// int count;

// Following the above order
// int stack = 0;   // dummy value
int MEMORY_VALUE[8];
unsigned int array_Value = 0; //[0-2]
unsigned int array_Next = 3; //[3-5]
unsigned int head = 6; //[6]
unsigned int count = 7; //[7]


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
    // array_Value = malloc(3 * sizeof(int));
    // array_Next = malloc(3 * sizeof(int));
    // __VERIFIER_atomic_store(&stack.count, pushCount);
    __VERIFIER_atomic_store(count, pushCount);
    // __VERIFIER_atomic_store(&head, 0);
    __VERIFIER_atomic_store(head, 0);
    for (i = 0; i < pushCount - 1; i++)
    {
        // __VERIFIER_atomic_store(&array_Next[i], i + 1);
        __VERIFIER_atomic_store(array_Next + i, i + 1);
    }
    // __VERIFIER_atomic_store(&array_Next[pushCount - 1], -1);
    __VERIFIER_atomic_store(array_Next + pushCount - 1, -1);
}

void Destroy(void)
{
    // free(stack.array);
}

int Pop(void)
{
    // while (__VERIFIER_atomic_load(&count) > 1)
    while (__VERIFIER_atomic_load(count) > 1)
    {
        // int head1 = __VERIFIER_atomic_load(&head);
        int head1 = __VERIFIER_atomic_load(head);
        // int next1 = __VERIFIER_atomic_exchange(&array_Next[head1], -1);
        int next1 = __VERIFIER_atomic_exchange(array_Next + head1, -1);

        if (next1 >= 0)
        {
            int head2 = head1;
            // if (__VERIFIER_atomic_compare_and_exchange(&head, &head2, next1))
            if (__VERIFIER_atomic_compare_and_exchange(head, &head2, next1))
            {
                // __VERIFIER_atomic_fetch_sub(&count, 1);
                __VERIFIER_atomic_fetch_sub(count, 1);
                return head1;
            }
            else
            {
                // __VERIFIER_atomic_exchange(&array_Next[head1], next1);
                __VERIFIER_atomic_exchange(array_Next + head1, next1);
            }
        }
    }

    return -1;
}

void Push(int index)
{
    // int head1 = __VERIFIER_atomic_load(&head);
    int head1 = __VERIFIER_atomic_load(head);
    do
    {
        // __VERIFIER_atomic_store(&array_Next[index], head1);
        __VERIFIER_atomic_store(array_Next + index, head1);

    // } while (!(__VERIFIER_atomic_compare_and_exchange(&head, &head1, index)));
    } while (!(__VERIFIER_atomic_compare_and_exchange(head, &head1, index)));
    // __VERIFIER_atomic_fetch_add(&count, 1);
    __VERIFIER_atomic_fetch_add(count, 1);
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

        // array_Value[elem] = idx;
        MEMORY_VALUE[array_Value + elem] = idx;
        // assert(array_Value[elem] == idx);
        assert(MEMORY_VALUE[array_Value + elem] == idx);

        Push(elem);
    }
    return NULL;
}

int main(void)
{
    int i;
    Init(NUM_THREADS);
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, thread, (void *)i);
    }

    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

