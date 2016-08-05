# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 1 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/elimstack.aba-bug.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/elimstack.aba-bug.c"
# 1 "./preprocessor/ccpp-0.1/fake_include/stdlib.h" 1
# 1 "./preprocessor/ccpp-0.1/fake_include/_fake_defines.h" 1
# 2 "./preprocessor/ccpp-0.1/fake_include/stdlib.h" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/_fake_typedefs.h" 1



typedef int _____STARTSTRIPPINGFROMHERE_____;

typedef int __cs_t;
typedef int __cs_attr_t;
typedef int __cs_mutex_t;
typedef int __cs_mutexattr_t;
typedef int __cs_cond_t;
typedef int __cs_condattr_t;
typedef int __cs_key_t;
typedef int __cs_once_t;
typedef int __cs_rwlock_t;
typedef int __cs_rwlockattr_t;
typedef int __cs_spinlock_t;
typedef int __cs_barrier_t;
typedef int __cs_barrierattr_t;

typedef int size_t;
typedef int __builtin_va_list;
typedef int __gnuc_va_list;
typedef int __int8_t;
typedef int __uint8_t;
typedef int __int16_t;
typedef int __uint16_t;
typedef int __int_least16_t;
typedef int __uint_least16_t;
typedef int __int32_t;
typedef int __uint32_t;
typedef int __int64_t;
typedef int __uint64_t;
typedef int __int_least32_t;
typedef int __uint_least32_t;
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
typedef int _off_t;
typedef int __dev_t;
typedef int __uid_t;
typedef int __gid_t;
typedef int _off64_t;
typedef int _fpos_t;
typedef int _ssize_t;
typedef int wint_t;
typedef int _mbstate_t;
typedef int _flock_t;
typedef int _iconv_t;
typedef int __ULong;
typedef int __FILE;
typedef int ptrdiff_t;
typedef int wchar_t;
typedef int __off_t;
typedef int __pid_t;
typedef int __loff_t;
typedef int u_char;
typedef int u_short;
typedef int u_int;
typedef int u_long;
typedef int ushort;
typedef int uint;
typedef int clock_t;
typedef int time_t;
typedef int daddr_t;
typedef int caddr_t;
typedef int ino_t;
typedef int off_t;
typedef int dev_t;
typedef int uid_t;
typedef int gid_t;
typedef int pid_t;
typedef int key_t;
typedef int ssize_t;
typedef int mode_t;
typedef int nlink_t;
typedef int fd_mask;
typedef int _types_fd_set;
typedef int clockid_t;
typedef int timer_t;
typedef int useconds_t;
typedef int suseconds_t;
typedef int FILE;
typedef int fpos_t;
typedef int cookie_read_function_t;
typedef int cookie_write_function_t;
typedef int cookie_seek_function_t;
typedef int cookie_close_function_t;
typedef int cookie_io_functions_t;
typedef int div_t;
typedef int ldiv_t;
typedef int lldiv_t;
typedef int sigset_t;
typedef int __sigset_t;
typedef int _sig_func_ptr;
typedef int sig_atomic_t;
typedef int __tzrule_type;
typedef int __tzinfo_type;
typedef int mbstate_t;
typedef int sem_t;
typedef int pthread_t;
typedef int pthread_attr_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int jmp_buf;
typedef int rlim_t;
typedef int sigjmp_buf;
typedef int stack_t;
typedef int siginfo_t;
typedef int z_stream;


typedef int int8_t;
typedef int uint8_t;
typedef int int16_t;
typedef int uint16_t;
typedef int int32_t;
typedef int uint32_t;
typedef int int64_t;
typedef int uint64_t;


typedef int int_least8_t;
typedef int uint_least8_t;
typedef int int_least16_t;
typedef int uint_least16_t;
typedef int int_least32_t;
typedef int uint_least32_t;
typedef int int_least64_t;
typedef int uint_least64_t;


typedef int int_fast8_t;
typedef int uint_fast8_t;
typedef int int_fast16_t;
typedef int uint_fast16_t;
typedef int int_fast32_t;
typedef int uint_fast32_t;
typedef int int_fast64_t;
typedef int uint_fast64_t;


typedef int intptr_t;
typedef int uintptr_t;


typedef int intmax_t;
typedef int uintmax_t;


typedef _Bool bool;

typedef int va_list;

typedef int fd_set;

typedef int _____STOPSTRIPPINGFROMHERE_____;
# 2 "./preprocessor/ccpp-0.1/fake_include/stdlib.h" 2
# 2 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/elimstack.aba-bug.c" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/stdio.h" 1
# 3 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/elimstack.aba-bug.c" 2
# 1 "./preprocessor/ccpp-0.1/fake_include/pthread.h" 1
# 4 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/elimstack.aba-bug.c" 2
# 21 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/elimstack.aba-bug.c"
char POP = 0;
char PUSH = 1;

typedef struct Cell Cell;
struct Cell {
    Cell *pnext;
    char pdata;
};

typedef struct ThreadInfo ThreadInfo;
struct ThreadInfo {
    unsigned char id;
    char op;
    Cell cell;
};

typedef struct Simple_Stack Simple_Stack;
struct Simple_Stack {
    Cell *ptop;
};

Simple_Stack S;
ThreadInfo *location[8];
char collision[1];

char unique_id = 0;

void StackOp(ThreadInfo *p);
char TryPerformStackOp(ThreadInfo *p);

char __VERIFIER_atomicchar_cas(char *p, char cmp, char new) { if (*p == cmp) { *p = new; return 1; } else return 0; }
char __VERIFIER_atomicti_cas(ThreadInfo* *p, ThreadInfo* cmp, ThreadInfo* new) { if (*p == cmp) { *p = new; return 1; } else return 0; }
char __VERIFIER_atomicc_cas(Cell* *p, Cell* cmp, Cell* new) { if (*p == cmp) { *p = new; return 1; } else return 0; }



ThreadInfo tipointers[4];
char allocated[4];

ThreadInfo* __VERIFIER_atomic_malloc_ThreadInfo() {
    char i = nondet_int();
    __VERIFIER_assume(0 <= i && i < 4);
    __VERIFIER_assume(!allocated[i]);
    allocated[i] = 1;
    return &tipointers[i];
}


void __VERIFIER_atomic_free_ThreadInfo(ThreadInfo* n) {

    char i = nondet_int();
    __VERIFIER_assume(0 <= i && i < 4);
    __VERIFIER_assume(&tipointers[i] == n);
    allocated[i] = 0;
}

void LesOP(ThreadInfo *p) {
    char mypid = p->id;

    location[mypid] = p;
    char him = collision[0];

    __VERIFIER_assume (__VERIFIER_atomicchar_cas(&collision[0],him,mypid));

    if (him > 0) {
        ThreadInfo* q = location[him];

        if (q != 0 && q->id == him && q->op != p->op) {
            if (__VERIFIER_atomicti_cas(&location[mypid],p,0)) {

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

    if (!__VERIFIER_atomicti_cas(&location[mypid],p,0)) {
        __VERIFIER_atomic_FinishCollision(p);
        return;
    }

    __VERIFIER_assume(0);
}

char TryPerformStackOp(ThreadInfo* p) {
    Cell *phead, *pnext;

    if (p->op == PUSH) {
        phead = S.ptop;
        p->cell.pnext = phead;

        if (__VERIFIER_atomicc_cas(&S.ptop,phead,&p->cell))
        {
            return 1;
        } else
        {
            return 0;
        }
    }
    else {

        phead = S.ptop;
        if (phead == 0) {
            p->cell.pdata = 2;
            return 1;
        }
        pnext = phead->pnext;


        if (__VERIFIER_atomicc_cas(&S.ptop,phead,pnext)) {
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
        location[mypid] = 0;
    }
}

char __VERIFIER_atomic_TryCollision(ThreadInfo *p, ThreadInfo *q, char him) {
    char mypid = p->id;

    if (p->op == PUSH) {
        if (__VERIFIER_atomicti_cas(&location[him],q,p))
            return 1;
        else
            return 0;
    }
    else {
        if (__VERIFIER_atomicti_cas(&location[him],q,0)) {
            p->cell.pdata = q->cell.pdata;

            location[mypid] = 0;
            return 1;
        }
        else {
            return 0;
        }
    }
    return 0;
}

void Init() {
    S.ptop = 0;
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




char PushOpen[2];
char PushDone[2];



char PopOpen;
char PopDone[3];


void checkInvariant()
{
    assert (
        PopDone[0] <= PushDone[0] + PushOpen[0] &&
        PopDone[1] <= PushDone[1] + PushOpen[1]
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
    return 0;
}

void* instrPush1(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return 0;
}

void* instrPush2(void* unused) {
    __VERIFIER_atomic_atomicIncr_Push(1);
    Push(1);
    __VERIFIER_atomic_atomicDecrIncr_Push(1);
    return 0;
}

void* instrPop3(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}

void* instrPop4(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}

void* instrPop5(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}

void* instrPop6(void* unused) {
    __VERIFIER_atomic_atomicIncr_Pop();
    char localPop_ret = Pop();
    __VERIFIER_atomic_atomicDecrIncr_Pop(localPop_ret);
    return 0;
}


int main(void) {
    Init();
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6, tid7;

    pthread_create(&tid1, 0, &instrPush0, 0);
    pthread_create(&tid2, 0, &instrPush1, 0);
    pthread_create(&tid3, 0, &instrPush2, 0);
    pthread_create(&tid4, 0, &instrPop3, 0);
    pthread_create(&tid5, 0, &instrPop4, 0);
    pthread_create(&tid6, 0, &instrPop5, 0);
    pthread_create(&tid7, 0, &instrPop6, 0);
}
