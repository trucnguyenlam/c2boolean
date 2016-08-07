#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define NUM_THREADS 3
/*
typedef struct SafeStackItem
{
    volatile int Value;
    int Next;
} SafeStackItem;

typedef struct SafeStack
{
    SafeStackItem* array;
    int head;
    int count;
} SafeStack;


/// Global
SafeStack stack;
*/
int stack_array_Value[NUM_THREADS];
int stack_array_Next[NUM_THREADS];
/*int stack_array_Value_cs_arr_0;
int stack_array_Value_cs_arr_1;
int stack_array_Value_cs_arr_2;
int stack_array_Next_cs_arr_0;
int stack_array_Next_cs_arr_1;
int stack_array_Next_cs_arr_2;*/
int stack_head;
int stack_count;
/*
void __VERIFIER_atomic_store(int *obj, int v)
{
    *obj = v;
}

int __VERIFIER_atomic_load(int *obj)
{
	int __VERIFIER_atomic_load_tmp=*obj;
	return __VERIFIER_atomic_load_tmp;
}

int __VERIFIER_atomic_exchange(int *obj, int v)
{
    int t = *obj;
    *obj = v;
	return t;
}
*/
int __VERIFIER_atomic_exchange_stack_array_Next(int index, int v)
{
    int t = stack_array_Next[index];
    stack_array_Next[index] = v;
	return t;
}

/*
_Bool __VERIFIER_atomic_compare_and_exchange(int *obj, int *expected, int desired)
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
*/
_Bool __VERIFIER_atomic_compare_and_exchange_stack_head(int *expected, int desired)
{
    if(stack_head == *expected)
    {
        stack_head = desired;
		return 1;
    }
    else
    {
        *expected = stack_head;
		return 0;
    }
}

/*
int __VERIFIER_atomic_fetch_add(int * obj, int v)
{
    int old = *obj;
    *obj = *obj + v;

	return old;
}
*/
int __VERIFIER_atomic_fetch_add_stack_count(int v)
{
    int old = stack_count;
    stack_count = stack_count + v;

	return old;
}
/*
int __VERIFIER_atomic_fetch_sub(int * obj, int v)
{
    int old = *obj;
    *obj = *obj - v;

    return old;
}
*/
int __VERIFIER_atomic_fetch_sub_stack_count(int v)
{
    int old = stack_count;
    stack_count = stack_count - v;

    return old;
}

void Init(int pushCount)
{
    int i;
    //stack.array = malloc(pushCount * sizeof(SafeStackItem));
    stack_count = pushCount;
    stack_head = 0;
    for (i = 0; i < pushCount - 1; i++)
    {
        stack_array_Next[i] = i + 1;
    }
    stack_array_Next[pushCount - 1] = -1;
}

int Pop(void)
{

    while (stack_count > 1)
    {
        int head1 = stack_head;
        int next1 = __VERIFIER_atomic_exchange_stack_array_Next(head1, -1);

        if (next1 >= 0)
        {
            int head2 = head1;
            if (__VERIFIER_atomic_compare_and_exchange_stack_head(&head2, next1))
            {
                __VERIFIER_atomic_fetch_sub_stack_count(1);
                return head1;
            }
            else
            {
                __VERIFIER_atomic_exchange_stack_array_Next(head1, next1);
            }
        }
    }

    return -1;
}

void Push(int index)
{
	_Bool tmpCompare;
    int head1 = stack_head;
    do
    {
        stack_array_Next[index] = head1;
		tmpCompare = __VERIFIER_atomic_compare_and_exchange_stack_head(&head1, index);
    } while (!(tmpCompare));
    __VERIFIER_atomic_fetch_add_stack_count(1);

}


void* thread(void* arg)
{
    pthread_t i;
    int idx = (int)(pthread_t)arg;
    for (i = 0; i < 2; i++)
    {
        int elem;
        for (;;)
        {
            elem = Pop();
            if (elem >= 0)
                break;
        }

        stack_array_Value[elem] = idx;
        assert(stack_array_Value[elem] == idx);

        Push(elem);
    }
    return NULL;
}

int main(void)
{
	pthread_t threads[NUM_THREADS];
    int i;
    Init(NUM_THREADS);
    /*for (i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, thread, (void*) i);
    }*/
	i=0;
    pthread_create(&threads[i], NULL, thread, (void*) i);
	i=1;
    pthread_create(&threads[i], NULL, thread, (void*) i);
	i=2;
    pthread_create(&threads[i], NULL, thread, (void*) i);

/*
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
*/
    return 0;
}

