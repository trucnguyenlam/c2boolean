/********************************************************
*                                                       *
*     Copyright (C) Microsoft. All rights reserved.     *
*                                                       *
********************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

///////////////////////////////////////////////////////////////////////////////////
#define MEMORY_SIZE 40
// int MEMORY_ADDRESS[MEMORY_SIZE];
int MEMORY_VALUE[MEMORY_SIZE];
int num_malloc = 1;

///////////////////////////////////////////////////////////////////////////////////


// void __VERIFIER_atomic_init_MEMORY(void)
// {
    // MEMORY_ADDRESS[0] = 1;
    // MEMORY_ADDRESS[1] = 2;
    // MEMORY_ADDRESS[2] = 3;
    // MEMORY_ADDRESS[3] = 4;
    // MEMORY_ADDRESS[4] = 5;
    // MEMORY_ADDRESS[5] = 6;
    // MEMORY_ADDRESS[6] = 7;
    // MEMORY_ADDRESS[7] = 8;
    // MEMORY_ADDRESS[8] = 9;
    // MEMORY_ADDRESS[9] = 10;
    // MEMORY_ADDRESS[10] = 11;
    // MEMORY_ADDRESS[11] = 12;
    // MEMORY_ADDRESS[12] = 13;
    // MEMORY_ADDRESS[13] = 14;
    // MEMORY_ADDRESS[14] = 15;
    // MEMORY_ADDRESS[15] = 16;
    // MEMORY_ADDRESS[16] = 17;
    // MEMORY_ADDRESS[17] = 18;
    // MEMORY_ADDRESS[18] = 19;
    // MEMORY_ADDRESS[19] = 20;
    // num_malloc = 1;
// }


int __VERIFIER_atomic_malloc(int size)
{
    int tmp = num_malloc;
    num_malloc += size;
    return tmp;
}

void __VERIFIER_atomic_free(int address)
{
    // TODO
}


// typedef struct Obj
// {
//     int field;
// } Obj;

// void Init_ObjType(Obj *r)     // r also is tracked
void Init_ObjType(int r)     // r also is tracked
{
    // r->field = 0;
    MEMORY_VALUE[r] = 0;
}

// void Operation(Obj *r)
void Operation(int r)
{
    // r->field++;
    MEMORY_VALUE[r] += 1;
}

// void Check(Obj *r)
void Check(int r)
{
    // assert(r->field == 1);
    assert(MEMORY_VALUE[r] == 1);
}

//
// A WorkStealQueue is a wait-free, lock-free structure associated with a single
// thread that can Push and Pop elements. Other threads can do Take operations
// on the other end of the WorkStealQueue with little contention.
// </summary>
//
typedef struct WorkStealQueue
{
    // A 'WorkStealQueue' always runs its code in a single OS thread. We call this the
    // 'bound' thread. Only the code in the Take operation can be executed by
    // other 'foreign' threads that try to steal work.
    //
    // The queue is implemented as an array. The head and tail index this
    // array. To avoid copying elements, the head and tail index the array modulo
    // the size of the array. By making this a power of two, we can use a cheap
    // bit-and operation to take the modulus. The "mask" is always equal to the
    // size of the task array minus one (where the size is a power of two).
    //
    // The head and tail are volatile as they can be updated from different OS threads.
    // The "head" is only updated by foreign threads as they Take (steal) a task from
    // this queue. By putting a lock in Take, there is at most one foreign thread
    // changing head at a time. The tail is only updated by the bound thread.
    //
    // invariants:
    //   tasks.length is a power of 2
    //   mask == tasks.length-1
    //   head is only written to by foreign threads
    //   tail is only written to by the bound thread
    //   At most one foreign thread can do a Take
    //   All methods except Take are executed from a single bound thread
    //   tail points to the first unused location
    //
    pthread_mutex_t cs;

    long MaxSize;
    long InitialSize; // must be a power of 2

    long head;  // only updated by Take
    long tail;  // only updated by Push and Pop

    // Obj**  elems;         // the array of tasks
    int elems;         // the array of tasks
    long mask;           // the mask for taking modulus

} WorkStealQueue;


long __VERIFIER_atomic_exchange(long *obj, long v)
{
    long t = *obj;
    *obj = v;
    return t;
}

_Bool __VERIFIER_atomic_compare_exchange_strong(long* obj, long* expected, long desired)
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

long readV(long *v) {
    long expected = 0;
    __VERIFIER_atomic_compare_exchange_strong(v, &expected, 0);
    return expected;
}

void writeV(long *v, long w) {
    __VERIFIER_atomic_exchange(v, w);
}


void Init_WorkStealQueue(WorkStealQueue* q, long size)
{
    q->MaxSize = 1024 * 1024;
    q->InitialSize = 1024;
    pthread_mutex_init(&q->cs, NULL);
    writeV(&q->head, 0);
    q->mask = size - 1;
    writeV(&q->tail, 0);
    // q->elems = malloc(size * sizeof(Obj*));   // q->elems  needs to be track   // fix
    q->elems = __VERIFIER_atomic_malloc(size * 1);
}

void Destroy_WorkStealQueue(WorkStealQueue *q)
{
    // free(q->elems);
    __VERIFIER_atomic_free(q->elems);
}

// Push/Pop and Steal can be executed interleaved. In particular:
// 1) A take and pop should be careful when there is just one element
//    in the queue. This is done by first incrementing the head/decrementing the tail
//    and than checking if it interleaved (head > tail).
// 2) A push and take can interleave in the sense that a push can overwrite the
//    value that is just taken. To account for this, we check conservatively in
//    the push to assume that the size is one less than it actually is.
//
// See the CILK "THE" protocol for more information:
//   "The implementation of the CILK-5 multi-threaded language"
//   Matteo Frigo, Charles Leiserson, and Keith Randall.
//

// _Bool Steal(WorkStealQueue *q, Obj **result)   // result is track
_Bool Steal(WorkStealQueue *q, int * result)   // result is track
{
    _Bool found;
    pthread_mutex_lock(&q->cs);

    // ensure that at most one (foreign) thread writes to head
    // increment the head. Save in local h for efficiency
    //
    long h = readV(&q->head);
    writeV(&q->head, h + 1);

    // insert a memory fence here if memory is not sequentially consistent
    //
    if (h < readV(&q->tail)) {
        // == (h+1 <= tail) == (head <= tail)
        //
        // BUG: writeV(&q->head, h + 1);
        // *result = q->elems[h & q->mask];
        *result = MEMORY_VALUE[q->elems + (h & q->mask)];
        found = 1;
    }
    else {
        // failure: either empty or single element interleaving with pop
        //
        writeV(&q->head, h);              // restore the head
        found = 0;
    }
    pthread_mutex_unlock(&q->cs);
    return found;
}

// _Bool SyncPop(WorkStealQueue* q, Obj **result)   // result is tracked
_Bool SyncPop(WorkStealQueue* q, int * result)   // result is tracked
{
    _Bool found;

    pthread_mutex_lock(&q->cs);

    // ensure that no Steal interleaves with this pop
    //
    long t = readV(&q->tail) - 1;
    writeV(&q->tail, t);
    if (readV(&q->head) <= t)
    {
        // == (head <= tail)
        //
        // *result = q->elems[t & q->mask];
        *result = MEMORY_VALUE[q->elems + (t & q->mask)];
        found = 1;
    }
    else
    {
        writeV(&q->tail, t + 1);       // restore tail
        found = 0;
    }
    if (readV(&q->head) > t)
    {
        // queue is empty: reset head and tail
        //
        writeV(&q->head, 0);
        writeV(&q->tail, 0);
        found = 0;
    }
    pthread_mutex_unlock(&q->cs);
    return found;
}

// _Bool Pop(WorkStealQueue *q, Obj **result)   // track results
_Bool Pop(WorkStealQueue *q, int * result)   // track results
{
    // decrement the tail. Use local t for efficiency.
    //
    long t = readV(&q->tail) - 1;
    writeV(&q->tail, t);

    // insert a memory fence here if memory is not sequentially consistent
    //
    if (readV(&q->head) <= t)
    {
        // BUG:  writeV(&q->tail, t);

        // == (head <= tail)
        //
        // *result = q->elems[t & q->mask];    // result need to be tracked
        *result = MEMORY_VALUE[q->elems + (t & q->mask)];    // result need to be tracked
        return 1;
    }
    else
    {
        // failure: either empty or single element interleaving with take
        //
        writeV(&q->tail, t + 1);             // restore the tail
        return SyncPop(q, result);   // do a single-threaded pop
    }
}

// void SyncPush(WorkStealQueue *q, Obj* elem)     // elem is tracked
void SyncPush(WorkStealQueue *q, int elem)     // elem is tracked
{
    pthread_mutex_lock(&q->cs);
    // ensure that no Steal interleaves here
    // cache head, and calculate number of tasks
    //
    long h = readV(&q->head);
    long count = readV(&q->tail) - h;

    // normalize indices
    //
    h = h & q->mask;           // normalize head
    writeV(&q->head, h);
    writeV(&q->tail, h + count);

    // check if we need to enlarge the tasks
    //
    if (count >= q->mask)
    {
        // == (count >= size-1)
        //
        long newsize = (q->mask == 0 ? q->InitialSize : 2 * (q->mask + 1));

        assert(newsize < q->MaxSize);

        // Obj ** newtasks = malloc(newsize * sizeof(Obj*));    // newtasks is track
        int newtasks = __VERIFIER_atomic_malloc(newsize * 1);    // newtasks is track
        long i;
        for (i = 0; i < count; i++)
        {
            // newtasks[i] = q->elems[(h + i) & q->mask];
            MEMORY_VALUE[newtasks + i] = MEMORY_VALUE[q->elems + ((h + i) & q->mask)];
        }
        // free(q->elems);       // is track
        __VERIFIER_atomic_free(q->elems);       // is track
        q->elems = newtasks;
        q->mask = newsize - 1;
        writeV(&q->head, 0);
        writeV(&q->tail, count);
    }

    assert(count < q->mask);

    // push the element
    //
    long t = readV(&q->tail);
    // q->elems[t & q->mask] = elem;    // they are all tracked
    MEMORY_VALUE[q->elems + (t & q->mask)] = elem;    // they are all tracked
    writeV(&q->tail, t + 1);
    pthread_mutex_unlock(&q->cs);
}


// void Push(WorkStealQueue *q, Obj* elem)       // Obj is track
void Push(WorkStealQueue *q, int elem)       // Obj is track
{
    long t = readV(&q->tail);
    // Careful here since we might interleave with Steal.
    // This is no problem since we just conservatively check if there is
    // enough space left (t < head + size). However, Steal might just have
    // incremented head and we could potentially overwrite the old head
    // entry, so we always leave at least one extra 'buffer' element and
    // check (tail < head + size - 1). This also plays nicely with our
    // initial mask of 0, where size is 2^0 == 1, but the tasks array is
    // still null.
    //
    // Correct: if (t < readV(&q->head) + mask && t < MaxSize)
#define BUG3
#ifdef BUG3
    if (t < readV(&q->head) + q->mask + 1 && t < q->MaxSize)
#else
    if (t < readV(&q->head) + q->mask   // == t < head + size - 1
            && t < q->MaxSize)
#endif
    {
        // q->elems[t & q->mask] = elem;       // they are all tracked
        MEMORY_VALUE[q->elems + (t & q->mask)] = elem;       // they are all tracked
        writeV(&q->tail, t + 1);       // only increment once we have initialized the task entry.
    }
    else
    {
        // failure: we need to resize or re-index
        //
        SyncPush(q, elem);      // second parameter needs to be tracked
    }
}

#define INITQSIZE 4 // must be power of 2

// #define nItems 8
// #define nStealers 4
// #define nStealAttempts 2

void *Stealer(void *param)
{
    WorkStealQueue *q = (WorkStealQueue*) param;

    int i;
    // Obj *r;
    int r;   // r is a position
    // for (i = 0; i < nStealAttempts; i++)
    {
        if (Steal(q, &r))
        {
            Operation(r);
        }
    }
    {
        if (Steal(q, &r))
        {
            Operation(r);
        }
    }
    return 0;
}

int main(void)
{
    // int i;
    // pthread_t handles[nStealers];    // We will not touch it
    pthread_t handles[4];    // We will not touch it
    // Obj *items = malloc(nItems*sizeof(Obj));    // something creating by malloc must be tracked
    int items = __VERIFIER_atomic_malloc(8 * 1);  // items is a pointer

    // for (i = 0; i < nItems; i++)
    // {
    // Init_ObjType(&items[i]);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 0);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 1);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 2);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 3);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 4);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 5);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 6);    // related to tracked variable also need to be tracked
    Init_ObjType(items + 7);    // related to tracked variable also need to be tracked
    // }

    WorkStealQueue q;

    Init_WorkStealQueue(&q, INITQSIZE);

    // for (i = 0; i < nStealers; i++)
    // {
    pthread_create(&handles[0], NULL, Stealer, &q);  // No
    pthread_create(&handles[1], NULL, Stealer, &q);  // No
    pthread_create(&handles[2], NULL, Stealer, &q);  // No
    pthread_create(&handles[3], NULL, Stealer, &q);  // No
    // }

    // for (i = 0; i < nItems / 2; i++)
    {
        // Push(&q, &items[0]);
        Push(&q, items);
        // Push(&q, &items[1]);
        Push(&q, items + 1);
        // Obj *r; // r is track
        int r; // r is track
        if (Pop(&q, &r))    // r is tracked
        {
            Operation(r);
        }
    }
    {
        // Push(&q, &items[2]);
        Push(&q, items + 2);
        // Push(&q, &items[3]);
        Push(&q, items + 3);
        // Obj *r; // r is track
        int r; // r is track
        if (Pop(&q, &r))    // r is tracked
        {
            Operation(r);
        }
    }

    {
        // Push(&q, &items[2]);
        Push(&q, items + 4);
        // Push(&q, &items[3]);
        Push(&q, items + 5);
        // Obj *r; // r is track
        int r; // r is track
        if (Pop(&q, &r))    // r is tracked
        {
            Operation(r);
        }
    }

    {
        // Push(&q, &items[2]);
        Push(&q, items + 6);
        // Push(&q, &items[3]);
        Push(&q, items + 7);
        // Obj *r; // r is track
        int r; // r is track
        if (Pop(&q, &r))    // r is tracked
        {
            Operation(r);
        }
    }

    // for (i = 0; i < nItems / 2; i++)
    {
        // Obj *r; // r is tracked
        int r; // r is track
        if (Pop(&q, &r))
        {
            Operation(r);
        }
    }
    {
        // Obj *r; // r is tracked
        int r; // r is track
        if (Pop(&q, &r))
        {
            Operation(r);
        }
    }
    {
        // Obj *r; // r is tracked
        int r; // r is track
        if (Pop(&q, &r))
        {
            Operation(r);
        }
    }
    {
        // Obj *r; // r is tracked
        int r; // r is track
        if (Pop(&q, &r))
        {
            Operation(r);
        }
    }

    // for (i = 0; i < nStealers; i++)
    // {
    pthread_join(handles[0], NULL);
    pthread_join(handles[1], NULL);
    pthread_join(handles[2], NULL);
    pthread_join(handles[3], NULL);
    // }

    // for (i = 0; i < nItems; i++)
    // {
    // Check(&items[0]);
    Check(items + 0);
    // Check(&items[1]);
    Check(items + 1);
    // Check(&items[2]);
    Check(items + 2);
    // Check(&items[3]);
    Check(items + 3);
    Check(items + 4);
    Check(items + 5);
    Check(items + 6);
    Check(items + 7);
    // }

    __VERIFIER_atomic_free(items);
    return 0;
}

