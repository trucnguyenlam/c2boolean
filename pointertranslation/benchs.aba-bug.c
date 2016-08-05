#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define CASDEF(t,ty) \
    char __VERIFIER_atomic ## t ## _cas(ty *p, ty cmp, ty new) { \
        if (*p == cmp) { \
            *p = new; \
            return 1; \
        } \
        else \
            return 0; \
    }
#define CAS(t,x,y,z) __VERIFIER_atomic ## t ## _cas(x,y,z)

#define LOCATION_ARRAY_SIZE 8
#define COLLISION_ARRAY_SIZE 1

// enum stack_op { POP = 0, PUSH };
char POP  = 0;
char PUSH = 1;

typedef struct Cell Cell;
struct Cell {
    Cell        *pnext;
    char         pdata;
};

typedef struct ThreadInfo ThreadInfo;
struct ThreadInfo {
    unsigned char  id;
    char          op;
    Cell          cell;
};

typedef struct Simple_Stack Simple_Stack;
struct Simple_Stack {
    Cell *ptop;
};

Simple_Stack S;
ThreadInfo *location[LOCATION_ARRAY_SIZE];
char collision[COLLISION_ARRAY_SIZE];

char unique_id = 0;

void StackOp(ThreadInfo *p);
char TryPerformStackOp(ThreadInfo *p);

CASDEF(char, char)
CASDEF(ti, ThreadInfo*)
CASDEF(c, Cell*)

#define MEMORYSIZE 4

ThreadInfo tipointers[MEMORYSIZE];
char allocated[MEMORYSIZE];

ThreadInfo* __VERIFIER_atomic_malloc_ThreadInfo() {
    char i = nondet_int();
    __VERIFIER_assume(0 <= i && i < MEMORYSIZE);
    __VERIFIER_assume(!allocated[i]);
    allocated[i] = 1;
    return &tipointers[i];
}


void __VERIFIER_atomic_free_ThreadInfo(ThreadInfo* n) {
//     char i;
    char i = nondet_int();
    __VERIFIER_assume(0 <= i && i < MEMORYSIZE);
    __VERIFIER_assume(&tipointers[i] == n);
    allocated[i] = 0; // BUG Free the memory to witness the ABA bug
}

void LesOP(ThreadInfo *p) {
    char mypid = p->id;

    location[mypid] = p;
    char him = collision[0];

    __VERIFIER_assume (CAS(char, &collision[0], him, mypid));

    if (him > 0) {
        ThreadInfo* q = location[him];

        if (q != NULL && q->id == him && q->op != p->op) {
            if (CAS(ti, &location[mypid], p, NULL)) {

                if (__VERIFIER_atomic_TryCollision(p, q, him) == 1) {
                    return;
                } else {
                    if (TryPerformStackOp(p) == 1)
                        return;
                }
            }
            else {
                __VERIFIER_atomic_FinishCollision(p);
                return;
            }
        }
    }

    if (!CAS(ti, &location[mypid], p, NULL)) {
        __VERIFIER_atomic_FinishCollision(p);
        return;
    }

    __VERIFIER_assume(0); // you can only return in the first loop iteration
}

char TryPerformStackOp(ThreadInfo* p) {
    Cell *phead, *pnext;

    if (p->op == PUSH) {
        phead = S.ptop;
        p->cell.pnext = phead;

        if (CAS(c, &S.ptop, phead, &p->cell))
        {
            return 1;
        } else
        {
            return 0;
        }
    }
    else {

        phead = S.ptop;
        if (phead == NULL) {
            p->cell.pdata = 2;
            return 1;
        }
        pnext = phead->pnext;


        if (CAS(c, &S.ptop, phead, pnext)) {
            p->cell.pdata = phead->pdata;
            ThreadInfo *oldti;
            __VERIFIER_assume(&oldti->cell == phead);
            __VERIFIER_atomic_free_ThreadInfo(oldti);
            return 1;
        }
        else {
            p->cell.pdata = 2;
            return 0;
        }
    }
}


void __VERIFIER_atomic_FinishCollision(ThreadInfo *p) {
    char mypid = p->id;

    if (p->op == POP) {
        p->cell.pdata = location[mypid]->cell.pdata;
        location[mypid] = NULL;
    }
}

char __VERIFIER_atomic_TryCollision(ThreadInfo *p, ThreadInfo *q, char him) {
    char mypid = p->id;

    if (p->op == PUSH) {
        if (CAS(ti, &location[him], q, p))
            return 1;
        else
            return 0;
    }
    else {
        if (CAS(ti, &location[him], q, NULL)) {
            p->cell.pdata = q->cell.pdata;

            location[mypid] = NULL;
            return 1;
        }
        else {
            return 0;
        }
    }
    return 0;
}

void Init() {
    S.ptop = NULL;
}

void Push(char x) {
    ThreadInfo *ti = __VERIFIER_atomic_malloc_ThreadInfo();
    __VERIFIER_assume(ti);

    ti->id = ++unique_id;
    ti->op = PUSH;
    ti->cell.pdata = x;

    if (TryPerformStackOp(ti) == 0)
    {
        LesOP(ti);
    }

//     assert(!(PushDone[0] == 0 && PushOpen[0] == 1 && PopDone[0] == 0 && PushDone[1] == 0 && PushOpen[1] == 0 && PopDone[1] == 0));
}

char Pop() {
    ThreadInfo *ti = __VERIFIER_atomic_malloc_ThreadInfo();
    __VERIFIER_assume(ti);

    ti->id = ++unique_id;
    ti->op = POP;

    if (TryPerformStackOp(ti) == 0)
    {
        LesOP(ti);
    }

    char v = ti->cell.pdata;
    __VERIFIER_atomic_free_ThreadInfo(ti);
    return v;
}


#define PushArg0Max 2

char PushOpen[PushArg0Max];
char PushDone[PushArg0Max];


#define PopRetMax 3
char PopOpen;
char PopDone[PopRetMax];


void checkInvariant()
{
    assert (
        PopDone[0]  <= PushDone[0]  + PushOpen[0] &&
        PopDone[1]  <= PushDone[1]  + PushOpen[1]
    );
}

void __VERIFIER_atomic_atomicIncr_Push(char localPush1) {
    PushOpen[localPush1]++;
}

void __VERIFIER_atomic_atomicDecrIncr_Push(char localPush1) {
    PushOpen[localPush1]--;
    PushDone[localPush1]++;
    checkInvariant();
}

void __VERIFIER_atomic_atomicIncr_Pop() {
    PopOpen++;
}

void __VERIFIER_atomic_atomicDecrIncr_Pop(char localPop_ret) {
    PopOpen--;
    PopDone[localPop_ret]++;
    checkInvariant();
}

void* instrPush0(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPush1(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPush2(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return NULL;
}

void* instrPop3(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop4(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop5(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}

void* instrPop6(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return NULL;
}


int main(void) {
    Init();
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7;

    pthread_create(&tid1, NULL, &instrPush0, NULL);
    pthread_create(&tid2, NULL, &instrPush1, NULL);
    pthread_create(&tid3, NULL, &instrPush2, NULL);
    pthread_create(&tid4, NULL, &instrPop3, NULL);
    pthread_create(&tid5, NULL, &instrPop4, NULL);
    pthread_create(&tid6, NULL, &instrPop5, NULL);
    pthread_create(&tid7, NULL, &instrPop6, NULL);
}
