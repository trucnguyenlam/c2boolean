#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

/////////////////////////////////////////////////////////////////

// Dedicated only for dynamic memory
#define TRACK_SIZE 6
unsigned int TRACK_ADDRESS[TRACK_SIZE];
int TRACK_VALUE[TRACK_SIZE];
unsigned int num_malloc;

extern unsigned int nondet_uint(void);


void __VERIFIER_atomic_init_MEMORY(void)
{
    TRACK_ADDRESS[0] = 1;
    TRACK_ADDRESS[1] = 2;
    TRACK_ADDRESS[2] = 3;
    TRACK_ADDRESS[3] = 4;
    TRACK_ADDRESS[4] = 5;
    TRACK_ADDRESS[5] = 6;
    num_malloc = 1;
}

unsigned int __VERIFIER_atomic_malloc(size_t size)
{
    unsigned int tmp = num_malloc;
    num_malloc += size;
    return tmp;
}


void __VERIFIER_atomic_free(unsigned int address)
{
    // TODO
}


int __VERIFIER_atomic_READ(unsigned int address)
{
    return TRACK_VALUE[address - 1];
}

// int __VERIFIER_atomic_READ_STATIC(unsigned int position)
// {
//     return TRACK_VALUE[position];
// }


void __VERIFIER_atomic_WRITE(unsigned int address, int value)
{
    TRACK_VALUE[address - 1] = value;
}

// void __VERIFIER_atomic_WRITE_STATIC(unsigned int position, int value)
// {
//     TRACK_VALUE[position] = value;
// }



/////////////////////////////////////////////////////////////////


#define NUM_THREADS 3

// TODO: lazy-cseq should be able to handle it
// typedef struct SafeStackItem
// {
//     volatile int Value;
//     int Next;
// } SafeStackItem;

typedef struct SafeStack
{
    // SafeStackItem* array;
    unsigned int array;  // now it will be an address
    int head;
    int count;
} SafeStack;

pthread_t threads[NUM_THREADS];

/// Global
SafeStack stack;


void __VERIFIER_atomic_store(int *obj, int v)
{
    *obj = v;
}

void __VERIFIER_atomic_store_ADDR(unsigned int obj, int v)
{
    __VERIFIER_atomic_WRITE(obj, v);
}

int __VERIFIER_atomic_load(int *obj)
{
    return *obj;
}

int __VERIFIER_atomic_load_ADDR(unsigned int obj)
{
    return __VERIFIER_atomic_READ(obj);
}

int __VERIFIER_atomic_exchange(int *obj, int v)
{
    int t = *obj;
    *obj = v;
    return t;
}

int __VERIFIER_atomic_exchange_ADDR(unsigned int obj, int v)
{
    int t = TRACK_VALUE[obj - 1];
    TRACK_VALUE[obj - 1] = v;
    return t;
}

_Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
{
    if (*obj == *expected)
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

int __VERIFIER_atomic_fetch_add(int * obj, int v)
{
    int old = *obj;
    *obj = old + v;
    return old;
}

int __VERIFIER_atomic_fetch_sub(int * obj, int v)
{
    int old = *obj;
    *obj = old - v;
    return old;
}

void Init(int pushCount)
{
    int i;
    stack.array = __VERIFIER_atomic_malloc(pushCount * 2);
    __VERIFIER_atomic_store(&stack.count, pushCount);
    __VERIFIER_atomic_store(&stack.head, 0);
    for (i = 0; i < pushCount - 1; i++)
    {
        __VERIFIER_atomic_store_ADDR(stack.array + 2 * i + 1, i + 1);
    }
    __VERIFIER_atomic_store_ADDR(stack.array + 2 * (pushCount - 1) + 1, -1);
}

void Destroy(void)
{
    __VERIFIER_atomic_free(stack.array);
}

int Pop(void)
{
    while (__VERIFIER_atomic_load(&stack.count) > 1)
    {
        int head1 = __VERIFIER_atomic_load(&stack.head);
        int next1 = __VERIFIER_atomic_exchange_ADDR(stack.array + 2 * (head1) + 1, -1);

        if (next1 >= 0)
        {
            int head2 = head1;
            if (__VERIFIER_atomic_compare_and_exchange(&stack.head, &head2, next1))
            {
                __VERIFIER_atomic_fetch_sub(&stack.count, 1);
                return head1;
            }
            else
            {
                __VERIFIER_atomic_exchange_ADDR(stack.array + 2 * (head1) + 1, next1);
            }
        }
    }

    return -1;
}

void Push(int index)
{
    int head1 = __VERIFIER_atomic_load(&stack.head);
    do
    {
        __VERIFIER_atomic_store_ADDR(stack.array + 2 * index + 1, head1);

    } while (!(__VERIFIER_atomic_compare_and_exchange(&stack.head, &head1, index)));
    __VERIFIER_atomic_fetch_add(&stack.count, 1);
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

        __VERIFIER_atomic_WRITE(stack.array + 2 * elem, idx);
        assert(__VERIFIER_atomic_READ(stack.array + 2 * elem) == idx);

        Push(elem);
    }
    return NULL;
}

int main(void)
{
    int i;
    Init(NUM_THREADS);
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, thread, (void*) i);
    }

    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

