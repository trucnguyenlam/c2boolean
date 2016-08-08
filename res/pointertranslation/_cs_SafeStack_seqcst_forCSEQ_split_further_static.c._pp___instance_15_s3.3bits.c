/*  Generated {"main": [[1, 6]], "thread_1": [[1, 34], [35, 68]], "thread_0": [[1, 34], [35, 68]], "thread_2": [[35, 68], [69, 102]]} (-t3 -r4 -u3 -bcbmc) 2015-12-09 17:32:00  */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define THREADS 3
#define ROUNDS 4

#define STOP_VOID(A) return;
#define STOP_NONVOID(A) return 0;

#define IF(T,A,B)  if ((pc[T] > A) | (A >= pc_cs)) goto B;
#define GUARD(T,B)  assume( pc_cs >= B );

#ifndef NULL
#define NULL 0
#endif

#define assume(x) __CPROVER_assume(x)
#define __VERIFIER_assume(x) __CPROVER_assume(x)

#define assert(x) assert(x)
#define __VERIFIER_assert(x) assert(x)

#define __VERIFIER_nondet_int() nondet_int()
int nondet_int();
#define __VERIFIER_nondet_uint() nondet_uint()
unsigned int nondet_uint();
#define __VERIFIER_nondet_bool() nondet_bool()
_Bool nondet_bool();
#define __VERIFIER_nondet_char() nondet_char()
char nondet_char();
#define __VERIFIER_nondet_uchar() nondet_uchar()
unsigned char nondet_uchar();

unsigned __CPROVER_bitvector[1] active_thread[THREADS+1]={1};
unsigned __CPROVER_bitvector[7] pc[THREADS+1];
unsigned __CPROVER_bitvector[8] pc_cs;
unsigned int thread_index;
unsigned __CPROVER_bitvector[7] thread_lines[] = {6, 69, 69, 69};

/* pthread API */
typedef int __cs_t;
typedef int __cs_attr_t;
typedef int __cs_mutex_t;
typedef int __cs_mutexattr_t;
typedef int __cs_cond_t;
typedef int __cs_condattr_t;
typedef int __cs_spinlock_t;
typedef int __cs_key_t;
typedef int __cs_once_t;
typedef int __cs_rwlock_t;
typedef int __cs_rwlockattr_t;
typedef int __cs_barrier_t;
typedef int __cs_barrierattr_t;

/* define PTHREAD CONSTANTS */
#define PTHREAD_MUTEX_INITIALIZER -1
#define PTHREAD_COND_INITIALIZER -1
#define PTHREAD_RWLOCK_INITIALIZER 0
#define PTHREAD_BARRIER_SERIAL_THREAD 0
#define PTHREAD_CANCEL_ASYNCHRONOUS 0
#define PTHREAD_CANCEL_ENABLE 0
#define PTHREAD_CANCEL_DEFERRED 0
#define PTHREAD_CANCEL_DISABLE 0
#define PTHREAD_CANCELED 0
#define PTHREAD_CREATE_DETACHED 0
#define PTHREAD_CREATE_JOINABLE 0
#define PTHREAD_EXPLICIT_SCHED 0
#define PTHREAD_INHERIT_SCHED 0
#define PTHREAD_MUTEX_DEFAULT 0
#define PTHREAD_MUTEX_ERRORCHECK 0
#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_RECURSIVE 0
#define PTHREAD_MUTEX_ROBUST 0
#define PTHREAD_MUTEX_STALLED 0
#define PTHREAD_ONCE_INIT 0
#define PTHREAD_PRIO_INHERIT 0
#define PTHREAD_PRIO_NONE 0
#define PTHREAD_PRIO_PROTECT 0
#define PTHREAD_PROCESS_SHARED 0
#define PTHREAD_PROCESS_PRIVATE 0
#define PTHREAD_SCOPE_PROCESS 0
#define PTHREAD_SCOPE_SYSTEM 0

void *threadargs[THREADS+1];

int __cs_create(__cs_t *id, void *attr, void *(*t)(void*), void *arg, int threadID) { if (threadID > THREADS) return 0;  *id = threadID; active_thread[threadID] = 1; threadargs[threadID] = arg; return 0; }
int __cs_join(__cs_t id, void **value_ptr) { assume(pc[id] == thread_lines[id]); return 0; }

void __cs_exit(void *value_ptr) { return; }   // only for parsing
__cs_t __cs_self() {  return thread_index;  }
int __cs_cancel(__cs_t id) {  return 0;  }

int __cs_mutex_init (__cs_mutex_t *m, int val) { *m = -1; return 0; }
int __cs_mutex_destroy(__cs_mutex_t *m) { return 0; }
int __cs_mutex_lock(__cs_mutex_t *m) { assume(*m == -1); *m = thread_index; return 0; }
int __cs_mutex_unlock(__cs_mutex_t *m) { assume(*m == thread_index); *m = -1; return 0; }

int __cs_cond_init(__cs_cond_t *cond, void *attr) { *cond = -1; return 0; }
int __cs_cond_destroy(__cs_cond_t *cond) { return 0; }
/* Already handle in lock optimizer module */
// int __cs_cond_wait(__cs_cond_t *cond, __cs_mutex_t *mutex) { __cs_mutex_unlock(mutex); assume(*cond != -1); __cs_mutex_lock(mutex); return 0;}
// int __cs_cond_timedwait(__cs_cond_t *cond, __cs_mutex_t *mutex, struct timespec *abstime) { if(__VERIFIER_nondet_bool()) {__cs_mutex_unlock(mutex); assume(*cond != -1); __cs_mutex_lock(mutex); return 0;} else {return ETIMEDOUT;}}
int __cs_cond_signal(__cs_cond_t *cond) { *cond = 1; return 0; }
int __cs_cond_broadcast(__cs_cond_t *cond) { *cond = 2; return 0; }

int __cs_spin_init(__cs_spinlock_t *l, int val) {*l = -1; return 0;}
int __cs_spin_destroy(__cs_spinlock_t *l) {return 0;}
int __cs_spin_lock(__cs_spinlock_t *l) { assume(*l == -1); *l = thread_index; return 0; }
int __cs_spin_unlock(__cs_spinlock_t *l) { assume(*l == thread_index); *l = -1; return 0; }

// Dirty fix for those macro in this situation val = __cs_..MACRO
#define __cs_mutex_lock_MACRO(m) 0; assume(m == -1); m = thread_index;
#define __cs_mutex_unlock_MACRO(m) 0; assume(m == thread_index); m = -1;
#define __cs_cond_signal_MACRO(cond) 0; cond = 1;
#define __cs_cond_broadcast_MACRO(cond) 0; cond = 2;
#define __cs_spin_unlock_MACRO(l) 0; assume(l == thread_index); l = -1;
#define __cs_spin_lock_MACRO(l) 0; assume(l == -1); l = thread_index;

// For schedule yield function
int __cs_sched_yield(unsigned __CPROVER_bitvector[8] val) {pc_cs = val + 1; return 0;}    // Version 1
//int __cs_sched_yield(unsigned __CPROVER_bitvector[8] val) {return 0;}     // Version 2


void * __cs_safe_malloc(size_t size) { void * ptr = malloc(size); assume(ptr); return ptr; }
void * __cs_unsafe_malloc(size_t size) { if(__VERIFIER_nondet_int()) return 0; return malloc(size); }
#define __cs_safe_malloc_LOCAL __cs_safe_malloc
#define __cs_unsafe_malloc malloc

void __cs_init_scalar(void *var, size_t size) {
    if (size == sizeof(int))
        *(int *)var = __VERIFIER_nondet_int();
    else {
        char * ptr = (char *)var;
        // size_t j;
        // for (j=0; j<size; j++)
        //     ptr[j] = __cs_nondet_char();
    }
}
size_t nondet_size_t();

unsigned __CPROVER_bitvector[7] guess_pc() {unsigned __CPROVER_bitvector[7] i; return i;}

/* here */


                                                       __cs_t threads[3];
                                                       __CPROVER_bitvector[3] array_Value[3];
                                                       __CPROVER_bitvector[3] array_Next[3];
                                                       __CPROVER_bitvector[3] head;
                                                       __CPROVER_bitvector[3] count;
                                                       void __CS_atomic_store(__CPROVER_bitvector[3] *__csLOCALPARAM___CS_atomic_store_obj, __CPROVER_bitvector[3] __csLOCALPARAM___CS_atomic_store_v)
                                                       {
                                                         *__csLOCALPARAM___CS_atomic_store_obj = __csLOCALPARAM___CS_atomic_store_v;
                                                       }

                                                       __CPROVER_bitvector[3] __CS_atomic_load(__CPROVER_bitvector[3] *__csLOCALPARAM___CS_atomic_load_obj)
                                                       {
                                                         return *__csLOCALPARAM___CS_atomic_load_obj;
                                                       }

                                                       __CPROVER_bitvector[3] __CS_atomic_exchange(__CPROVER_bitvector[3] *__csLOCALPARAM___CS_atomic_exchange_obj, __CPROVER_bitvector[3] __csLOCALPARAM___CS_atomic_exchange_v)
                                                       {
                                                         static __CPROVER_bitvector[3] __csLOCAL___CS_atomic_exchange_t;
                                                         __csLOCAL___CS_atomic_exchange_t = *__csLOCALPARAM___CS_atomic_exchange_obj;
                                                         *__csLOCALPARAM___CS_atomic_exchange_obj = __csLOCALPARAM___CS_atomic_exchange_v;
                                                         return __csLOCAL___CS_atomic_exchange_t;
                                                       }

                                                       _Bool __CS_atomic_compare_and_exchange(__CPROVER_bitvector[3] *__csLOCALPARAM___CS_atomic_compare_and_exchange_obj, __CPROVER_bitvector[3] *__csLOCALPARAM___CS_atomic_compare_and_exchange_expected, __CPROVER_bitvector[3] __csLOCALPARAM___CS_atomic_compare_and_exchange_desired)
                                                       {
                                                         if ((*__csLOCALPARAM___CS_atomic_compare_and_exchange_obj) == (*__csLOCALPARAM___CS_atomic_compare_and_exchange_expected))
                                                         {
                                                           *__csLOCALPARAM___CS_atomic_compare_and_exchange_obj = __csLOCALPARAM___CS_atomic_compare_and_exchange_desired;
                                                           return 1;
                                                         }
                                                         else
                                                         {
                                                           *__csLOCALPARAM___CS_atomic_compare_and_exchange_expected = *__csLOCALPARAM___CS_atomic_compare_and_exchange_obj;
                                                           return 0;
                                                         }

                                                       }

                                                       __CPROVER_bitvector[3] __CS_atomic_fetch_add(__CPROVER_bitvector[3] *__csLOCALPARAM___CS_atomic_fetch_add_obj, __CPROVER_bitvector[3] __csLOCALPARAM___CS_atomic_fetch_add_v)
                                                       {
                                                         static __CPROVER_bitvector[3] __csLOCAL___CS_atomic_fetch_add_old;
                                                         __csLOCAL___CS_atomic_fetch_add_old = *__csLOCALPARAM___CS_atomic_fetch_add_obj;
                                                         *__csLOCALPARAM___CS_atomic_fetch_add_obj = __csLOCAL___CS_atomic_fetch_add_old + __csLOCALPARAM___CS_atomic_fetch_add_v;
                                                         return __csLOCAL___CS_atomic_fetch_add_old;
                                                       }

                                                       __CPROVER_bitvector[3] __CS_atomic_fetch_sub(__CPROVER_bitvector[3] *__csLOCALPARAM___CS_atomic_fetch_sub_obj, __CPROVER_bitvector[3] __csLOCALPARAM___CS_atomic_fetch_sub_v)
                                                       {
                                                         static __CPROVER_bitvector[3] __csLOCAL___CS_atomic_fetch_sub_old;
                                                         __csLOCAL___CS_atomic_fetch_sub_old = *__csLOCALPARAM___CS_atomic_fetch_sub_obj;
                                                         *__csLOCALPARAM___CS_atomic_fetch_sub_obj = __csLOCAL___CS_atomic_fetch_sub_old - __csLOCALPARAM___CS_atomic_fetch_sub_v;
                                                         return __csLOCAL___CS_atomic_fetch_sub_old;
                                                       }

                                                       __CPROVER_bitvector[3] __cs_function_Init_inlined = 1;
                                                       __CPROVER_bitvector[3] __cs_function_Destroy_inlined = 1;
                                                       __CPROVER_bitvector[3] __cs_function_Pop_inlined = 1;
                                                       __CPROVER_bitvector[3] __cs_function_Push_inlined = 1;
                                                       void *thread_0(void *__csLOCALPARAM_thread_arg)
                                                       {
                                                       static size_t __csLOCAL_thread_i;
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_idx;
IF(1,0,tthread_0_1)                                    __csLOCAL_thread_idx = (__CPROVER_bitvector[3]) ((size_t) __csLOCALPARAM_thread_arg);
                                                       __csLOCAL_thread_i = 0;
                                                       	{
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_elem;
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_1: IF(1,1,tthread_0_2)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_2: IF(1,2,tthread_0_3)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_3: IF(1,3,tthread_0_4)                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_4: IF(1,4,tthread_0_5)                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_5: IF(1,5,tthread_0_6)                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,6)
tthread_0_6: IF(1,6,tthread_0_7)                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,7);
                                                       					}
                                                       					GUARD(1,7);
tthread_0_7: IF(1,7,tthread_0_8)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_8: IF(1,8,tthread_0_9)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_9: IF(1,9,tthread_0_10)                      __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_10: IF(1,10,tthread_0_11)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_11: IF(1,11,tthread_0_12)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,12)
tthread_0_12: IF(1,12,tthread_0_13)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,13);
                                                       					}
                                                       					GUARD(1,13);
tthread_0_13: IF(1,13,tthread_0_14)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_14: IF(1,14,tthread_0_15)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_15: IF(1,15,tthread_0_16)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_16: IF(1,16,tthread_0_17)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_17: IF(1,17,tthread_0_18)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,18)
tthread_0_18: IF(1,18,tthread_0_19)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,19);
                                                       					}
                                                       					GUARD(1,19);
tthread_0_19: IF(1,19,tthread_0_20)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_3: GUARD(1,20)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_0;
                                                       				__exit__Pop_1_0: GUARD(1,20)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_20: IF(1,20,tthread_0_21)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_21: IF(1,21,tthread_0_22)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_22: IF(1,22,tthread_0_23)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_23: IF(1,23,tthread_0_24)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_24: IF(1,24,tthread_0_25)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,25)
tthread_0_25: IF(1,25,tthread_0_26)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,26);
                                                       					}
                                                       					GUARD(1,26);
tthread_0_26: IF(1,26,tthread_0_27)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_27: IF(1,27,tthread_0_28)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_28: IF(1,28,tthread_0_29)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_29: IF(1,29,tthread_0_30)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_30: IF(1,30,tthread_0_31)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,31)
tthread_0_31: IF(1,31,tthread_0_32)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,32);
                                                       					}
                                                       					GUARD(1,32);
tthread_0_32: IF(1,32,tthread_0_33)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_33: IF(1,33,tthread_0_34)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_34: IF(1,34,tthread_0_35)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_35: IF(1,35,tthread_0_36)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_36: IF(1,36,tthread_0_37)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,37)
tthread_0_37: IF(1,37,tthread_0_38)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,38);
                                                       					}
                                                       					GUARD(1,38);
tthread_0_38: IF(1,38,tthread_0_39)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_4: GUARD(1,39)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_1;
                                                       				__exit__Pop_1_1: GUARD(1,39)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_39: IF(1,39,tthread_0_40)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_40: IF(1,40,tthread_0_41)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_41: IF(1,41,tthread_0_42)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_42: IF(1,42,tthread_0_43)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_43: IF(1,43,tthread_0_44)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,44)
tthread_0_44: IF(1,44,tthread_0_45)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,45);
                                                       					}
                                                       					GUARD(1,45);
tthread_0_45: IF(1,45,tthread_0_46)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_46: IF(1,46,tthread_0_47)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_47: IF(1,47,tthread_0_48)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_48: IF(1,48,tthread_0_49)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_49: IF(1,49,tthread_0_50)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,50)
tthread_0_50: IF(1,50,tthread_0_51)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,51);
                                                       					}
                                                       					GUARD(1,51);
tthread_0_51: IF(1,51,tthread_0_52)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_0_52: IF(1,52,tthread_0_53)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_0_53: IF(1,53,tthread_0_54)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_54: IF(1,54,tthread_0_55)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_55: IF(1,55,tthread_0_56)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,56)
tthread_0_56: IF(1,56,tthread_0_57)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,57);
                                                       					}
                                                       					GUARD(1,57);
tthread_0_57: IF(1,57,tthread_0_58)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_5: GUARD(1,58)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_2;
                                                       				__exit__Pop_1_2: GUARD(1,58)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       assume(!1);
                                                       		__exit_loop_2: GUARD(1,58)
                                                       ;
tthread_0_58: IF(1,58,tthread_0_59)                    array_Value[__csLOCAL_thread_elem] = __csLOCAL_thread_idx;
tthread_0_59: IF(1,59,tthread_0_60)                    assert(array_Value[__csLOCAL_thread_elem] == __csLOCAL_thread_idx);
                                                       		{
                                                       static __CPROVER_bitvector[3] __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push_head1;
tthread_0_60: IF(1,60,tthread_0_61)                    __csLOCAL_Push_head1 = __CS_atomic_load(&head);
                                                       ;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_0_61: IF(1,61,tthread_0_62)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_6: GUARD(1,62)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_0_62: IF(1,62,tthread_0_63)                    __csLOCAL_Push___while_cond_1 =
                                                       !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_0_63: IF(1,63,tthread_0_64)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_0_64: IF(1,64,tthread_0_65)                    __csLOCAL_Push___while_cond_1 =
                                                       !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_0_65: IF(1,65,tthread_0_66)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_0_66: IF(1,66,tthread_0_67)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_0_67: IF(1,67,tthread_0_68)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_0_68: IF(1,68,tthread_0_69)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_7: GUARD(1,69)
                                                       ;
                                                       __CS_atomic_fetch_add(&count, 1);
                                                       			__exit__Push_1_0: GUARD(1,69)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       	{
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_elem;
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_9: GUARD(1,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_3;
                                                       				__exit__Pop_1_3: GUARD(1,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_10: GUARD(1,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_4;
                                                       				__exit__Pop_1_4: GUARD(1,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_11: GUARD(1,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_5;
                                                       				__exit__Pop_1_5: GUARD(1,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       assume(!1);
                                                       		__exit_loop_8: GUARD(1,69)
                                                       ;
                                                       array_Value[__csLOCAL_thread_elem] = __csLOCAL_thread_idx;
                                                       assert(array_Value[__csLOCAL_thread_elem] == __csLOCAL_thread_idx);
                                                       		{
                                                       static __CPROVER_bitvector[3] __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push_head1;
                                                       __csLOCAL_Push_head1 = __CS_atomic_load(&head);
                                                       ;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_12: GUARD(1,69)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_13: GUARD(1,69)
                                                       ;
                                                       __CS_atomic_fetch_add(&count, 1);
                                                       			__exit__Push_1_1: GUARD(1,69)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       assume(!(__csLOCAL_thread_i < 2));
                                                       	__exit_loop_1: GUARD(1,69)
                                                       ;
                                                       goto __exit_thread;
                                                       	__exit_thread: GUARD(1,69)
                                                       ;
tthread_0_69:                                          STOP_NONVOID(69);
                                                       }



                                                       void *thread_1(void *__csLOCALPARAM_thread_arg)
                                                       {
                                                       static size_t __csLOCAL_thread_i;
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_idx;
IF(2,0,tthread_1_1)                                    __csLOCAL_thread_idx = (__CPROVER_bitvector[3]) ((size_t) __csLOCALPARAM_thread_arg);
                                                       __csLOCAL_thread_i = 0;
                                                       	{
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_elem;
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_1: IF(2,1,tthread_1_2)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_2: IF(2,2,tthread_1_3)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_3: IF(2,3,tthread_1_4)                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_4: IF(2,4,tthread_1_5)                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_5: IF(2,5,tthread_1_6)                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,6)
tthread_1_6: IF(2,6,tthread_1_7)                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,7);
                                                       					}
                                                       					GUARD(2,7);
tthread_1_7: IF(2,7,tthread_1_8)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_8: IF(2,8,tthread_1_9)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_9: IF(2,9,tthread_1_10)                      __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_10: IF(2,10,tthread_1_11)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_11: IF(2,11,tthread_1_12)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,12)
tthread_1_12: IF(2,12,tthread_1_13)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,13);
                                                       					}
                                                       					GUARD(2,13);
tthread_1_13: IF(2,13,tthread_1_14)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_14: IF(2,14,tthread_1_15)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_15: IF(2,15,tthread_1_16)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_16: IF(2,16,tthread_1_17)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_17: IF(2,17,tthread_1_18)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,18)
tthread_1_18: IF(2,18,tthread_1_19)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,19);
                                                       					}
                                                       					GUARD(2,19);
tthread_1_19: IF(2,19,tthread_1_20)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_3: GUARD(2,20)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_0;
                                                       				__exit__Pop_1_0: GUARD(2,20)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_20: IF(2,20,tthread_1_21)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_21: IF(2,21,tthread_1_22)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_22: IF(2,22,tthread_1_23)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_23: IF(2,23,tthread_1_24)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_24: IF(2,24,tthread_1_25)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,25)
tthread_1_25: IF(2,25,tthread_1_26)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,26);
                                                       					}
                                                       					GUARD(2,26);
tthread_1_26: IF(2,26,tthread_1_27)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_27: IF(2,27,tthread_1_28)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_28: IF(2,28,tthread_1_29)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_29: IF(2,29,tthread_1_30)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_30: IF(2,30,tthread_1_31)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,31)
tthread_1_31: IF(2,31,tthread_1_32)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,32);
                                                       					}
                                                       					GUARD(2,32);
tthread_1_32: IF(2,32,tthread_1_33)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_33: IF(2,33,tthread_1_34)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_34: IF(2,34,tthread_1_35)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_35: IF(2,35,tthread_1_36)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_36: IF(2,36,tthread_1_37)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,37)
tthread_1_37: IF(2,37,tthread_1_38)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,38);
                                                       					}
                                                       					GUARD(2,38);
tthread_1_38: IF(2,38,tthread_1_39)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_4: GUARD(2,39)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_1;
                                                       				__exit__Pop_1_1: GUARD(2,39)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_39: IF(2,39,tthread_1_40)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_40: IF(2,40,tthread_1_41)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_41: IF(2,41,tthread_1_42)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_42: IF(2,42,tthread_1_43)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_43: IF(2,43,tthread_1_44)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,44)
tthread_1_44: IF(2,44,tthread_1_45)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,45);
                                                       					}
                                                       					GUARD(2,45);
tthread_1_45: IF(2,45,tthread_1_46)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_46: IF(2,46,tthread_1_47)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_47: IF(2,47,tthread_1_48)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_48: IF(2,48,tthread_1_49)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_49: IF(2,49,tthread_1_50)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,50)
tthread_1_50: IF(2,50,tthread_1_51)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,51);
                                                       					}
                                                       					GUARD(2,51);
tthread_1_51: IF(2,51,tthread_1_52)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_1_52: IF(2,52,tthread_1_53)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_1_53: IF(2,53,tthread_1_54)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_54: IF(2,54,tthread_1_55)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_55: IF(2,55,tthread_1_56)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,56)
tthread_1_56: IF(2,56,tthread_1_57)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,57);
                                                       					}
                                                       					GUARD(2,57);
tthread_1_57: IF(2,57,tthread_1_58)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_5: GUARD(2,58)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_2;
                                                       				__exit__Pop_1_2: GUARD(2,58)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       assume(!1);
                                                       		__exit_loop_2: GUARD(2,58)
                                                       ;
tthread_1_58: IF(2,58,tthread_1_59)                    array_Value[__csLOCAL_thread_elem] = __csLOCAL_thread_idx;
tthread_1_59: IF(2,59,tthread_1_60)                    assert(array_Value[__csLOCAL_thread_elem] == __csLOCAL_thread_idx);
                                                       		{
                                                       static __CPROVER_bitvector[3] __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push_head1;
tthread_1_60: IF(2,60,tthread_1_61)                    __csLOCAL_Push_head1 = __CS_atomic_load(&head);
                                                       ;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_1_61: IF(2,61,tthread_1_62)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_6: GUARD(2,62)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_1_62: IF(2,62,tthread_1_63)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_1_63: IF(2,63,tthread_1_64)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_1_64: IF(2,64,tthread_1_65)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_1_65: IF(2,65,tthread_1_66)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_1_66: IF(2,66,tthread_1_67)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_1_67: IF(2,67,tthread_1_68)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_1_68: IF(2,68,tthread_1_69)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_7: GUARD(2,69)
                                                       ;
                                                       __CS_atomic_fetch_add(&count, 1);
                                                       			__exit__Push_1_0: GUARD(2,69)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       	{
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_elem;
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_9: GUARD(2,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_3;
                                                       				__exit__Pop_1_3: GUARD(2,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_10: GUARD(2,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_4;
                                                       				__exit__Pop_1_4: GUARD(2,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_11: GUARD(2,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_5;
                                                       				__exit__Pop_1_5: GUARD(2,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       assume(!1);
                                                       		__exit_loop_8: GUARD(2,69)
                                                       ;
                                                       array_Value[__csLOCAL_thread_elem] = __csLOCAL_thread_idx;
                                                       assert(array_Value[__csLOCAL_thread_elem] == __csLOCAL_thread_idx);
                                                       		{
                                                       static __CPROVER_bitvector[3] __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push_head1;
                                                       __csLOCAL_Push_head1 = __CS_atomic_load(&head);
                                                       ;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_12: GUARD(2,69)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_13: GUARD(2,69)
                                                       ;
                                                       __CS_atomic_fetch_add(&count, 1);
                                                       			__exit__Push_1_1: GUARD(2,69)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       assume(!(__csLOCAL_thread_i < 2));
                                                       	__exit_loop_1: GUARD(2,69)
                                                       ;
                                                       goto __exit_thread;
                                                       	__exit_thread: GUARD(2,69)
                                                       ;
tthread_1_69:                                          STOP_NONVOID(69);
                                                       }



                                                       void *thread_2(void *__csLOCALPARAM_thread_arg)
                                                       {
                                                       static size_t __csLOCAL_thread_i;
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_idx;
IF(3,0,tthread_2_1)                                    __csLOCAL_thread_idx = (__CPROVER_bitvector[3]) ((size_t) __csLOCALPARAM_thread_arg);
                                                       __csLOCAL_thread_i = 0;
                                                       	{
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_elem;
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_3: GUARD(3,1)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_0;
                                                       				__exit__Pop_1_0: GUARD(3,1)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_1: IF(3,1,tthread_2_2)                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_2: IF(3,2,tthread_2_3)                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,3)
tthread_2_3: IF(3,3,tthread_2_4)                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,4);
                                                       					}
                                                       					GUARD(3,4);
tthread_2_4: IF(3,4,tthread_2_5)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_4: GUARD(3,5)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_1;
                                                       				__exit__Pop_1_1: GUARD(3,5)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_5: IF(3,5,tthread_2_6)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_6: IF(3,6,tthread_2_7)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_7: IF(3,7,tthread_2_8)                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_8: IF(3,8,tthread_2_9)                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_9: IF(3,9,tthread_2_10)                      __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,10)
tthread_2_10: IF(3,10,tthread_2_11)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,11);
                                                       					}
                                                       					GUARD(3,11);
tthread_2_11: IF(3,11,tthread_2_12)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_12: IF(3,12,tthread_2_13)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_13: IF(3,13,tthread_2_14)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_14: IF(3,14,tthread_2_15)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_15: IF(3,15,tthread_2_16)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,16)
tthread_2_16: IF(3,16,tthread_2_17)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,17);
                                                       					}
                                                       					GUARD(3,17);
tthread_2_17: IF(3,17,tthread_2_18)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_18: IF(3,18,tthread_2_19)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_19: IF(3,19,tthread_2_20)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_20: IF(3,20,tthread_2_21)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_21: IF(3,21,tthread_2_22)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,22)
tthread_2_22: IF(3,22,tthread_2_23)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,23);
                                                       					}
                                                       					GUARD(3,23);
tthread_2_23: IF(3,23,tthread_2_24)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_5: GUARD(3,24)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_2;
                                                       				__exit__Pop_1_2: GUARD(3,24)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_2;
                                                       			}
                                                       			;
                                                       		};
                                                       assume(!1);
                                                       		__exit_loop_2: GUARD(3,24)
                                                       ;
tthread_2_24: IF(3,24,tthread_2_25)                    array_Value[__csLOCAL_thread_elem] = __csLOCAL_thread_idx;
tthread_2_25: IF(3,25,tthread_2_26)                    assert(array_Value[__csLOCAL_thread_elem] == __csLOCAL_thread_idx);
                                                       		{
                                                       static __CPROVER_bitvector[3] __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push_head1;
tthread_2_26: IF(3,26,tthread_2_27)                    __csLOCAL_Push_head1 = __CS_atomic_load(&head);
                                                       ;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_2_27: IF(3,27,tthread_2_28)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_6: GUARD(3,28)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_2_28: IF(3,28,tthread_2_29)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_2_29: IF(3,29,tthread_2_30)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_2_30: IF(3,30,tthread_2_31)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_2_31: IF(3,31,tthread_2_32)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_2_32: IF(3,32,tthread_2_33)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_2_33: IF(3,33,tthread_2_34)                    __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
tthread_2_34: IF(3,34,tthread_2_35)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_7: GUARD(3,35)
                                                       ;
tthread_2_35: IF(3,35,tthread_2_36)                    __CS_atomic_fetch_add(&count, 1);
                                                       			__exit__Push_1_0: GUARD(3,36)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       	{
                                                       static __CPROVER_bitvector[3] __csLOCAL_thread_elem;
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_36: IF(3,36,tthread_2_37)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_37: IF(3,37,tthread_2_38)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_38: IF(3,38,tthread_2_39)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_39: IF(3,39,tthread_2_40)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_40: IF(3,40,tthread_2_41)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,41)
tthread_2_41: IF(3,41,tthread_2_42)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,42);
                                                       					}
                                                       					GUARD(3,42);
tthread_2_42: IF(3,42,tthread_2_43)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_43: IF(3,43,tthread_2_44)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_44: IF(3,44,tthread_2_45)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_45: IF(3,45,tthread_2_46)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_46: IF(3,46,tthread_2_47)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,47)
tthread_2_47: IF(3,47,tthread_2_48)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,48);
                                                       					}
                                                       					GUARD(3,48);
tthread_2_48: IF(3,48,tthread_2_49)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_49: IF(3,49,tthread_2_50)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_50: IF(3,50,tthread_2_51)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_51: IF(3,51,tthread_2_52)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_52: IF(3,52,tthread_2_53)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,53)
tthread_2_53: IF(3,53,tthread_2_54)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,54);
                                                       					}
                                                       					GUARD(3,54);
tthread_2_54: IF(3,54,tthread_2_55)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_9: GUARD(3,55)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_3;
                                                       				__exit__Pop_1_3: GUARD(3,55)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_55: IF(3,55,tthread_2_56)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_56: IF(3,56,tthread_2_57)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_57: IF(3,57,tthread_2_58)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_58: IF(3,58,tthread_2_59)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_59: IF(3,59,tthread_2_60)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,60)
tthread_2_60: IF(3,60,tthread_2_61)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,61);
                                                       					}
                                                       					GUARD(3,61);
tthread_2_61: IF(3,61,tthread_2_62)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_62: IF(3,62,tthread_2_63)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
tthread_2_63: IF(3,63,tthread_2_64)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_64: IF(3,64,tthread_2_65)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_65: IF(3,65,tthread_2_66)                    __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,66)
tthread_2_66: IF(3,66,tthread_2_67)                    __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,67);
                                                       					}
                                                       					GUARD(3,67);
tthread_2_67: IF(3,67,tthread_2_68)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
tthread_2_68: IF(3,68,tthread_2_69)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_10: GUARD(3,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_4;
                                                       				__exit__Pop_1_4: GUARD(3,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       		{
                                                       static __CPROVER_bitvector[3] __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head1;
                                                       __csLOCAL_Pop_head1 = __CS_atomic_load(&head);
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_next1;
                                                       __csLOCAL_Pop_next1 = __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static __CPROVER_bitvector[3] __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
                                                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
                                                       __CS_atomic_fetch_sub(&count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{
                                                       __CS_atomic_exchange(&array_Next[__csLOCAL_Pop_head1], __csLOCAL_Pop_next1);
                                                       						}
                                                       						;
                                                       					}
                                                       					;
                                                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_11: GUARD(3,69)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_5;
                                                       				__exit__Pop_1_5: GUARD(3,69)
                                                       ;
                                                       			};
                                                       __csLOCAL_thread_elem = __retval__Pop_1;
                                                       if (__csLOCAL_thread_elem >= 0)
                                                       			{
                                                       goto __exit_loop_8;
                                                       			}
                                                       			;
                                                       		};
                                                       assume(!1);
                                                       		__exit_loop_8: GUARD(3,69)
                                                       ;
                                                       array_Value[__csLOCAL_thread_elem] = __csLOCAL_thread_idx;
                                                       assert(array_Value[__csLOCAL_thread_elem] == __csLOCAL_thread_idx);
                                                       		{
                                                       static __CPROVER_bitvector[3] __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push_head1;
                                                       __csLOCAL_Push_head1 = __CS_atomic_load(&head);
                                                       ;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_12: GUARD(3,69)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Push_index], __csLOCAL_Push_head1);
                                                       __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_13: GUARD(3,69)
                                                       ;
                                                       __CS_atomic_fetch_add(&count, 1);
                                                       			__exit__Push_1_1: GUARD(3,69)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       assume(!(__csLOCAL_thread_i < 2));
                                                       	__exit_loop_1: GUARD(3,69)
                                                       ;
                                                       goto __exit_thread;
                                                       	__exit_thread: GUARD(3,69)
                                                       ;
tthread_2_69:                                          STOP_NONVOID(69);
                                                       }



                                                       __CPROVER_bitvector[3] main_thread(void)
                                                       {
IF(0,0,tmain_1)                                        static __CPROVER_bitvector[3] __csLOCAL_main_i;
                                                       	{
                                                       static __CPROVER_bitvector[3] __csLOCALPARAM_Init_pushCount;
                                                       __csLOCALPARAM_Init_pushCount = 3;
                                                       static __CPROVER_bitvector[3] __csLOCAL_Init_i;
                                                       __CS_atomic_store(&count, __csLOCALPARAM_Init_pushCount);
                                                       __CS_atomic_store(&head, 0);
                                                       __csLOCAL_Init_i = 0;
                                                       if (!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)))
                                                       		{
                                                       goto __exit_loop_14;
                                                       		}
                                                       		;
                                                       		{
                                                       __CS_atomic_store(&array_Next[__csLOCAL_Init_i], __csLOCAL_Init_i + 1);
                                                       		};
                                                       __csLOCAL_Init_i++;
                                                       if (!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)))
                                                       		{
                                                       goto __exit_loop_14;
                                                       		}
                                                       		;
                                                       		{
                                                       __CS_atomic_store(&array_Next[__csLOCAL_Init_i], __csLOCAL_Init_i + 1);
                                                       		};
                                                       __csLOCAL_Init_i++;
                                                       if (!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)))
                                                       		{
                                                       goto __exit_loop_14;
                                                       		}
                                                       		;
                                                       		{
                                                       __CS_atomic_store(&array_Next[__csLOCAL_Init_i], __csLOCAL_Init_i + 1);
                                                       		};
                                                       __csLOCAL_Init_i++;
                                                       assume(!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)));
                                                       		__exit_loop_14: GUARD(0,1)
                                                       ;
                                                       __CS_atomic_store(&array_Next[__csLOCALPARAM_Init_pushCount - 1], -1);
                                                       		__exit__Init_1: GUARD(0,1)
                                                       ;
                                                       	};
                                                       0;
                                                       __csLOCAL_main_i = 0;
                                                       if (!(__csLOCAL_main_i < 3))
                                                       	{
                                                       goto __exit_loop_15;
                                                       	}
                                                       	;
                                                       	{
                                                       __cs_create(&threads[__csLOCAL_main_i], 0, thread_0, (void *) __csLOCAL_main_i, 1);
                                                       	};
                                                       ++__csLOCAL_main_i;
                                                       if (!(__csLOCAL_main_i < 3))
                                                       	{
                                                       goto __exit_loop_15;
                                                       	}
                                                       	;
                                                       	{
tmain_1: IF(0,1,tmain_2)                               __cs_create(&threads[__csLOCAL_main_i], 0, thread_1, (void *) __csLOCAL_main_i, 2);
                                                       	};
                                                       ++__csLOCAL_main_i;
                                                       if (!(__csLOCAL_main_i < 3))
                                                       	{
                                                       goto __exit_loop_15;
                                                       	}
                                                       	;
                                                       	{
tmain_2: IF(0,2,tmain_3)                               __cs_create(&threads[__csLOCAL_main_i], 0, thread_2, (void *) __csLOCAL_main_i, 3);
                                                       	};
                                                       ++__csLOCAL_main_i;
                                                       assume(!(__csLOCAL_main_i < 3));
                                                       	__exit_loop_15: GUARD(0,3)
                                                       ;
                                                       __csLOCAL_main_i = 0;
                                                       if (!(__csLOCAL_main_i < 3))
                                                       	{
                                                       goto __exit_loop_16;
                                                       	}
                                                       	;
                                                       	{
tmain_3: IF(0,3,tmain_4)                               __cs_join(threads[__csLOCAL_main_i], 0);
                                                       	};
                                                       ++__csLOCAL_main_i;
                                                       if (!(__csLOCAL_main_i < 3))
                                                       	{
                                                       goto __exit_loop_16;
                                                       	}
                                                       	;
                                                       	{
tmain_4: IF(0,4,tmain_5)                               __cs_join(threads[__csLOCAL_main_i], 0);
                                                       	};
                                                       ++__csLOCAL_main_i;
                                                       if (!(__csLOCAL_main_i < 3))
                                                       	{
                                                       goto __exit_loop_16;
                                                       	}
                                                       	;
                                                       	{
tmain_5: IF(0,5,tmain_6)                               __cs_join(threads[__csLOCAL_main_i], 0);
                                                       	};
                                                       ++__csLOCAL_main_i;
                                                       assume(!(__csLOCAL_main_i < 3));
                                                       	__exit_loop_16: GUARD(0,6)
                                                       ;
                                                       goto __exit_main;
                                                       	__exit_main: GUARD(0,6)
                                                       ;
tmain_6:                                               STOP_NONVOID(6);
                                                       }



                                                       __CPROVER_bitvector[3] main(void) {
                                                                 // round 0
                                                                 thread_index = 0;
                                                                 pc_cs = pc[0] + guess_pc();
                                                                 assume(pc_cs > 0 && pc_cs <= 6);
                                                                 main_thread();
                                                                 pc[0] = pc_cs;

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume(pc_cs >= 0 && pc_cs <= 69);
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume(pc_cs >= 0 && pc_cs <= 69);
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume(pc_cs >= 0 && pc_cs <= 69);
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 // round 1
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume(pc_cs >= pc[0] && pc_cs <= 6);
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume(pc_cs >= pc[1] && pc_cs <= 69);
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume(pc_cs >= pc[2] && pc_cs <= 69);
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume(pc_cs >= pc[3] && pc_cs <= 69);
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 // round 2
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume(pc_cs >= pc[0] && pc_cs <= 6);
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume(pc_cs >= pc[1] && pc_cs <= 69);
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume(pc_cs >= pc[2] && pc_cs <= 69);
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume(pc_cs >= pc[3] && pc_cs <= 69);
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 // round 3
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume(pc_cs >= pc[0] && pc_cs <= 6);
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume(pc_cs >= pc[1] && pc_cs <= 69);
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume(pc_cs >= pc[2] && pc_cs <= 69);
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume(pc_cs >= pc[3] && pc_cs <= 69);
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) {
                                                                    pc_cs = pc[0] + guess_pc();
                                                                    assume(pc_cs >= pc[0] && pc_cs <= 6);
                                                                    main_thread();
                                                                 }

                                                          return 0;
                                                       }


