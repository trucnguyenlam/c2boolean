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
//     SafeStackItem* array;
//     int head;
//     int count;
// } SafeStack;

pthread_t threads[NUM_THREADS];

/// Global
// SafeStack stack;     // will be track

// int MEMORY_VALUE[9];

unsigned int stack = 0;     // 0, 1, 2
//stack + 0  :=  stack.array
//stack + 1  :=  stack.head
//stack + 2  :=  stack.count



// // void atomic_store(int *obj, int v)
// void atomic_store(unsigned int obj, int v)
// {
//     // obj does not pointing to any new location, so does not need to be track
//     // v is a value
//     // *obj = v;
//     MEMORY_VALUE[obj] = v;
// }

// // int atomic_load(int *obj)
// int atomic_load(unsigned int obj)
// {
//     // obj does not pointing to any new location, so does not need to be track
//     // return *obj;
//     return MEMORY_VALUE[obj];
// }

// // int atomic_exchange(int *obj, int v)
// int atomic_exchange(unsigned int obj, int v)
// {
//     // obj does not pointing to any new location, so does not need to be track
//     // int t = *obj;
//     int t = MEMORY_VALUE[obj];
//     // *obj = v;
//     MEMORY_VALUE[obj] = v;
//     return t;
// }

// // _Bool atomic_compare_and_exchange(int *obj, int *expected, int desired)
// _Bool atomic_compare_and_exchange(unsigned int obj, int *expected, int desired)
// {
//     // obj does not pointing to any new location, so does not need to be track
//     // expected does not pointing to any new location, so does not need to be track
//     // if(*obj == *expected)
//     if (MEMORY_VALUE[obj] == *expected)
//     {
//         // *obj = desired;
//         MEMORY_VALUE[obj] = desired;
//         return 1;
//     }
//     else
//     {
//         *expected = MEMORY_VALUE[obj];
//         return 0;
//     }
// }

// // int atomic_fetch_add(int * obj, int v)
// int atomic_fetch_add(unsigned int obj, int v)
// {
//     // obj does not pointing to any new location, so does not need to be track
//     // int old = *obj;
//     int old = MEMORY_VALUE[obj];
//     // *obj = old + v;
//     MEMORY_VALUE[obj] = old + v;
//     return old;
// }

// // int atomic_fetch_sub(int * obj, int v)
// int atomic_fetch_sub(unsigned int obj, int v)
// {
//     // obj does not pointing to any new location, so does not need to be track
//     // int old = *obj;
//     int old = MEMORY_VALUE[obj];
//     // *obj = old - v;
//     MEMORY_VALUE[obj] = old - v;
//     return old;
// }

void Init(int pushCount)
{
    int i;        // is is only used as scalar
    // stack.array = malloc(3 * sizeof(SafeStackItem));     // stack.array = base position of array of 6 in memory
    MEMORY_VALUE[stack + 0] = 3;   // stack.array = base position of array of 6 in memory
    // it is only used to read the address
    // atomic_store(&stack.count, pushCount);    // the address of stack.count has been used
    MEMORY_VALUE[stack + 2] = pushCount;  // the address of stack.count has been used
    // atomic_store(&stack.head, 0);  // the address of stack.head has been used
    MEMORY_VALUE[stack + 1] = 0; // the address of stack.head has been used
    for (i = 0; i < pushCount - 1; i++)
    {
        // atomic_store(&stack.array[i].Next, i + 1);
        MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * i + 1] = i + 1;
    }
    // atomic_store(&stack.array[pushCount - 1].Next, -1);
    MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (pushCount - 1) + 1] = -1;
}

void Destroy(void)
{
    // free(stack.array);
}

int Pop(void)
{
    // while (atomic_load(&stack.count) > 1)
    while (MEMORY_VALUE[stack + 2] > 1)
    {
        // int head1 = atomic_load(&stack.head);
        int head1 = MEMORY_VALUE[stack + 1];
        // int next1 = atomic_exchange(&stack.array[head1].Next, -1);
        __VERIFIER_atomic_begin();
        int __retval__atomic_exchange_0;
        {
            int obj = MEMORY_VALUE[stack + 0] + 2 * (head1) + 1;
            int t = MEMORY_VALUE[obj];
            MEMORY_VALUE[obj] = -1;
            __retval__atomic_exchange_0 = t;
        }
        int next1 = __retval__atomic_exchange_0;
        __VERIFIER_atomic_end();

        if (next1 >= 0)
        {
            int head2 = head1;    // head20 is used as an address and there is a write to that address
            // if (atomic_compare_and_exchange(&stack.head, &head2, next1))
            __VERIFIER_atomic_begin();
            _Bool __local_if_cond_0;
            _Bool __retval__atomic_compare_and_exchange_0;
            {
                unsigned int __csLOCALPARAM_atomic_compare_and_exchange_obj;
                __csLOCALPARAM_atomic_compare_and_exchange_obj = stack + 1;
                if (MEMORY_VALUE[__csLOCALPARAM_atomic_compare_and_exchange_obj] == head2)
                {
                    MEMORY_VALUE[__csLOCALPARAM_atomic_compare_and_exchange_obj] = next1;
                    __retval__atomic_compare_and_exchange_0 = 1; goto __exit__atomic_compare_and_exchange_0;
                }
                else
                {
                    head2 = MEMORY_VALUE[__csLOCALPARAM_atomic_compare_and_exchange_obj];
                    __retval__atomic_compare_and_exchange_0 = 0; goto __exit__atomic_compare_and_exchange_0;
                }

__exit__atomic_compare_and_exchange_0: ;
            }
            __local_if_cond_0 = __retval__atomic_compare_and_exchange_0;
            __VERIFIER_atomic_end();

            if (__local_if_cond_0)
            {
                // atomic_fetch_sub(&stack.count, 1);
                __VERIFIER_atomic_begin();
                int __retval__atomic_fetch_sub_0;
                {
                    unsigned int __csLOCALPARAM_atomic_fetch_sub_obj;
                    __csLOCALPARAM_atomic_fetch_sub_obj = stack + 2;
                    int __csLOCALPARAM_atomic_fetch_sub_v; __csLOCALPARAM_atomic_fetch_sub_v = 1;
                    int __csLOCAL_atomic_fetch_sub_old;
                    __csLOCAL_atomic_fetch_sub_old = MEMORY_VALUE[__csLOCALPARAM_atomic_fetch_sub_obj];
                    MEMORY_VALUE[__csLOCALPARAM_atomic_fetch_sub_obj] = __csLOCAL_atomic_fetch_sub_old - __csLOCALPARAM_atomic_fetch_sub_v;
                    __retval__atomic_fetch_sub_0 = __csLOCAL_atomic_fetch_sub_old; goto __exit__atomic_fetch_sub_0;
__exit__atomic_fetch_sub_0: ;
                }
                __VERIFIER_atomic_end();
                return head1;
            }
            else
            {
                // atomic_exchange(&stack.array[head1].Next, next1);
                __VERIFIER_atomic_begin();
                static int __retval__atomic_exchange_1;
                {
                    static unsigned int __csLOCALPARAM_atomic_exchange_obj;
                    __csLOCALPARAM_atomic_exchange_obj = (MEMORY_VALUE[stack + 0] + (2 * head1)) + 1;
                    static int __csLOCALPARAM_atomic_exchange_v;
                    __csLOCALPARAM_atomic_exchange_v = -1;
                    static int __csLOCAL_atomic_exchange_t;
                    __csLOCAL_atomic_exchange_t = MEMORY_VALUE[__csLOCALPARAM_atomic_exchange_obj];
                    MEMORY_VALUE[__csLOCALPARAM_atomic_exchange_obj] = __csLOCALPARAM_atomic_exchange_v;
                    __retval__atomic_exchange_1 = __csLOCAL_atomic_exchange_t; goto __exit__atomic_exchange_1;
__exit__atomic_exchange_1: ;
                }
                __VERIFIER_atomic_end();
            }
        }
    }

    return -1;
}

void Push(int index)
{
    // int head10 = atomic_load(&stack.head);
    int head1 = MEMORY_VALUE[stack + 1];

    _Bool __local_while_cond_0;
    {
        // atomic_store(&stack.array[index].Next, head1);
        MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (index) + 1] = head1;

        __VERIFIER_atomic_begin();
        _Bool __retval__atomic_compare_and_exchange_0;
        {
            unsigned int __csLOCALPARAM_atomic_compare_and_exchange_obj;
            __csLOCALPARAM_atomic_compare_and_exchange_obj = stack + 1;
            if (MEMORY_VALUE[__csLOCALPARAM_atomic_compare_and_exchange_obj] == head1)
            {
                MEMORY_VALUE[__csLOCALPARAM_atomic_compare_and_exchange_obj] = index;
                __retval__atomic_compare_and_exchange_0 = 1; goto __exit__atomic_compare_and_exchange_0;
            }
            else
            {
                head1 = MEMORY_VALUE[__csLOCALPARAM_atomic_compare_and_exchange_obj];
                __retval__atomic_compare_and_exchange_0 = 0; goto __exit__atomic_compare_and_exchange_0;
            }

__exit__atomic_compare_and_exchange_0: ;
        }
        __local_while_cond_0 = __retval__atomic_compare_and_exchange_0;
        __VERIFIER_atomic_end();
    } while (!__local_while_cond_0);
    // atomic_fetch_add(&stack.count, 1);

    __VERIFIER_atomic_begin();
    int __retval__atomic_fetch_add_1;
    {
        unsigned int __csLOCALPARAM_atomic_fetch_add_obj;
        __csLOCALPARAM_atomic_fetch_add_obj = stack + 2;
        int __csLOCALPARAM_atomic_fetch_add_v;
        __csLOCALPARAM_atomic_fetch_add_v = 1;
        int __csLOCAL_atomic_fetch_add_old;
        __csLOCAL_atomic_fetch_add_old = MEMORY_VALUE[__csLOCALPARAM_atomic_fetch_add_obj];
        MEMORY_VALUE[__csLOCALPARAM_atomic_fetch_add_obj] = __csLOCAL_atomic_fetch_add_old + __csLOCALPARAM_atomic_fetch_add_v;
        __retval__atomic_fetch_add_1 = __csLOCAL_atomic_fetch_add_old; goto __exit__atomic_fetch_add_1;
    __exit__atomic_fetch_add_1: ;
    }
    __VERIFIER_atomic_end();
}



void* thread0(void* arg)
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
        MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (elem) + 0] = idx;
        // assert(stack.array[elem].Value == idx);
        assert(MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (elem) + 0] == idx);

        Push(elem);
    }
    return NULL;
}
void* thread1(void* arg)
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
        MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (elem) + 0] = idx;
        // assert(stack.array[elem].Value == idx);
        assert(MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (elem) + 0] == idx);

        Push(elem);
    }
    return NULL;
}
void* thread2(void* arg)
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
        MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (elem) + 0] = idx;
        // assert(stack.array[elem].Value == idx);
        assert(MEMORY_VALUE[MEMORY_VALUE[stack + 0] + 2 * (elem) + 0] == idx);

        Push(elem);
    }
    return NULL;
}

int main(void)
{
    // int i;     /* i is not tracked since it has not been referred to as global share*/
    Init(NUM_THREADS);
    pthread_create(&threads[0], NULL, thread0, 0);
    pthread_create(&threads[1], NULL, thread1, 1);
    pthread_create(&threads[2], NULL, thread2, 2);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    return 0;
}

