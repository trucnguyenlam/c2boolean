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



// void __VERIFIER_atomic_store(int *obj, int v)
void __VERIFIER_atomic_store(unsigned int obj, int v)
{
    // obj does not pointing to any new location, so does not need to be track
    // v is a value
    // *obj = v;
    MEMORY_VALUE[obj] = v;
}

// int __VERIFIER_atomic_load(int *obj)
int __VERIFIER_atomic_load(unsigned int obj)
{
    // obj does not pointing to any new location, so does not need to be track
    // return *obj;
    return MEMORY_VALUE[obj];
}

// int __VERIFIER_atomic_exchange(int *obj, int v)
int __VERIFIER_atomic_exchange(unsigned int obj, int v)
{
    // obj does not pointing to any new location, so does not need to be track
    // int t = *obj;
    int t = MEMORY_VALUE[obj];
    // *obj = v;
    MEMORY_VALUE[obj] = v;
    return t;
}

// _Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
_Bool __VERIFIER_atomic_compare_and_exchange(unsigned int obj, int *expected, int desired)
{
    // obj does not pointing to any new location, so does not need to be track
    // expected does not pointing to any new location, so does not need to be track
    // if(*obj == *expected)
    if(MEMORY_VALUE[obj] == *expected)
    {
        // *obj = desired;
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
int __VERIFIER_atomic_fetch_add(unsigned int obj, int v)
{
    // obj does not pointing to any new location, so does not need to be track
    // int old = *obj;
    int old = MEMORY_VALUE[obj];
    // *obj = old + v;
    MEMORY_VALUE[obj] = old + v;
    return old;
}

// int __VERIFIER_atomic_fetch_sub(int * obj, int v)
int __VERIFIER_atomic_fetch_sub(unsigned int obj, int v)
{
    // obj does not pointing to any new location, so does not need to be track
    // int old = *obj;
    int old = MEMORY_VALUE[obj];
    // *obj = old - v;
    MEMORY_VALUE[obj] = old - v;
    return old;
}

void Init(int pushCount)
{
    int i;        // is is only used as scalar
    // stack.array = malloc(3 * sizeof(SafeStackItem));     // stack.array = base position of array of 6 in memory
    MEMORY_VALUE[stack+0] = 3;     // stack.array = base position of array of 6 in memory
                                                         // it is only used to read the address
    // __VERIFIER_atomic_store(&stack.count, pushCount);    // the address of stack.count has been used
    __VERIFIER_atomic_store(stack+2, pushCount);    // the address of stack.count has been used
    // __VERIFIER_atomic_store(&stack.head, 0);  // the address of stack.head has been used
    __VERIFIER_atomic_store(stack+1, 0);  // the address of stack.head has been used
    for (i = 0; i < pushCount - 1; i++)
    {
        // __VERIFIER_atomic_store(&stack.array[i].Next, i + 1);
        __VERIFIER_atomic_store(MEMORY_VALUE[stack+0] + 2*i + 1, i + 1);   // can be reduced further
    }
    // __VERIFIER_atomic_store(&stack.array[pushCount - 1].Next, -1);
    __VERIFIER_atomic_store(MEMORY_VALUE[stack+0] + 2*(pushCount - 1) + 1, -1);
}

void Destroy(void)
{
    // free(stack.array);
}

int Pop0(void)
{
    // while (__VERIFIER_atomic_load(&stack.count) > 1)
    while (__VERIFIER_atomic_load(stack+2) > 1)
    {
        // int head1 = __VERIFIER_atomic_load(&stack.head);
        int head1 = __VERIFIER_atomic_load(stack+1);
        // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
        int next1 = __VERIFIER_atomic_exchange(MEMORY_VALUE[stack+0] + 2*(head1) + 1, -1);

        if (next1 >= 0)
        {
            int head20 = head1;    // head20 is used as an address and there is a write to that address
            // if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head20, next1))
            if (__VERIFIER_atomic_compare_and_exchange(stack+1, &head20, next1))
            {
                // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                __VERIFIER_atomic_fetch_sub(stack+2, 1);
                return head1;
            }
            else
            {
                // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                __VERIFIER_atomic_exchange(MEMORY_VALUE[stack+0] + 2*(head1) + 1, next1);
            }
        }
    }

    return -1;
}
int Pop1(void)
{
    // while (__VERIFIER_atomic_load(&stack.count) > 1)
    while (__VERIFIER_atomic_load(stack+2) > 1)
    {
        // int head1 = __VERIFIER_atomic_load(&stack.head);
        int head1 = __VERIFIER_atomic_load(stack+1);
        // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
        int next1 = __VERIFIER_atomic_exchange(MEMORY_VALUE[stack+0] + 2*(head1) + 1, -1);

        if (next1 >= 0)
        {
            int head21 = head1;    // head21 is used as an address and there is a write to that address
            // if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head21, next1))
            if (__VERIFIER_atomic_compare_and_exchange(stack+1,  &head21, next1))
            {
                // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                __VERIFIER_atomic_fetch_sub(stack+2, 1);
                return head1;
            }
            else
            {
                // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                __VERIFIER_atomic_exchange(MEMORY_VALUE[stack+0] + 2*(head1) + 1, next1);
            }
        }
    }

    return -1;
}
int Pop2(void)
{
    // while (__VERIFIER_atomic_load(&stack.count) > 1)
    while (__VERIFIER_atomic_load(stack+2) > 1)
    {
        // int head1 = __VERIFIER_atomic_load(&stack.head);
        int head1 = __VERIFIER_atomic_load(stack+1);
        // int next1 = __VERIFIER_atomic_exchange(&stack.array[head1].Next, -1);
        int next1 = __VERIFIER_atomic_exchange(MEMORY_VALUE[stack+0] + 2*(head1) + 1, -1);

        if (next1 >= 0)
        {
            int head22 = head1;    // head22 is used as an address and there is a write to that address
            // if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head22, next1))
            if (__VERIFIER_atomic_compare_and_exchange(stack+1,  &head22, next1))
            {
                // __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                __VERIFIER_atomic_fetch_sub(stack+2, 1);
                return head1;
            }
            else
            {
                // __VERIFIER_atomic_exchange(&stack.array[head1].Next, next1);
                __VERIFIER_atomic_exchange(MEMORY_VALUE[stack+0] + 2*(head1) + 1, next1);
            }
        }
    }

    return -1;
}

void Push0(int index)
{
    // int head10 = __VERIFIER_atomic_load(&stack.head);   // head10 is used as an address and there is a write to that address
    int head10 = __VERIFIER_atomic_load(stack+1);   // head10 is used as an address and there is a write to that address
    do
    {
        // __VERIFIER_atomic_store(&stack.array[index].Next, head10);
        __VERIFIER_atomic_store(MEMORY_VALUE[stack+0] + 2*(index) + 1, head10);

    // } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head10, index)));
    } while (!(__VERIFIER_atomic_compare_and_exchange(stack+1, &head10, index)));
    // __VERIFIER_atomic_fetch_add(&stack.count, 1);
    __VERIFIER_atomic_fetch_add(stack+2, 1);
}
void Push1(int index)
{
    // int head11 = __VERIFIER_atomic_load(&stack.head);   // head11 is used as an address and there is a write to that address
    int head11 = __VERIFIER_atomic_load(stack+1);   // head11 is used as an address and there is a write to that address
    do
    {
        // __VERIFIER_atomic_store(&stack.array[index].Next, head11);
        __VERIFIER_atomic_store(MEMORY_VALUE[stack+0] + 2*(index) + 1, head11);

    // } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head11, index)));
    } while (!(__VERIFIER_atomic_compare_and_exchange(stack+1, &head11, index)));
    // __VERIFIER_atomic_fetch_add(&stack.count, 1);
    __VERIFIER_atomic_fetch_add(stack+2, 1);
}
void Push2(int index)
{
    // int head12 = __VERIFIER_atomic_load(&stack.head);   // head12 is used as an address and there is a write to that address
    int head12 = __VERIFIER_atomic_load(stack+1);   // head12 is used as an address and there is a write to that address
    do
    {
        // __VERIFIER_atomic_store(&stack.array[index].Next, head12);
        __VERIFIER_atomic_store(MEMORY_VALUE[stack+0] + 2*(index) + 1, head12);

    // } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head12, index)));
    } while (!(__VERIFIER_atomic_compare_and_exchange(stack+1, &head12, index)));
    // __VERIFIER_atomic_fetch_add(&stack.count, 1);
    __VERIFIER_atomic_fetch_add(stack+2, 1);
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
            elem = Pop0();
            if (elem >= 0)
                break;
        }

        // stack.array[elem].Value = idx;
        MEMORY_VALUE[MEMORY_VALUE[stack+0] + 2*(elem) + 0] = idx;
        // assert(stack.array[elem].Value == idx);
        assert(MEMORY_VALUE[MEMORY_VALUE[stack+0] + 2*(elem) + 0] == idx);

        Push0(elem);
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
            elem = Pop1();
            if (elem >= 0)
                break;
        }

        // stack.array[elem].Value = idx;
        MEMORY_VALUE[MEMORY_VALUE[stack+0] + 2*(elem) + 0] = idx;
        // assert(stack.array[elem].Value == idx);
        assert(MEMORY_VALUE[MEMORY_VALUE[stack+0] + 2*(elem) + 0] == idx);

        Push1(elem);
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
            elem = Pop2();
            if (elem >= 0)
                break;
        }

        // stack.array[elem].Value = idx;
        MEMORY_VALUE[MEMORY_VALUE[stack+0] + 2*(elem) + 0] = idx;
        // assert(stack.array[elem].Value == idx);
        assert(MEMORY_VALUE[MEMORY_VALUE[stack+0] + 2*(elem) + 0] == idx);

        Push2(elem);
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

