/*  Generated 0.0.0.0.0.0 (-t3 -r4 -u3 -bcbmc) 2015-12-02 17:13:22  */
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
unsigned __CPROVER_bitvector[8] pc[THREADS+1];
unsigned __CPROVER_bitvector[9] pc_cs;
unsigned int thread_index;
unsigned __CPROVER_bitvector[8] thread_lines[] = {6, 139, 139, 139};

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
int __cs_sched_yield(unsigned __CPROVER_bitvector[9] val) {pc_cs = val + 1; return 0;}    // Version 1
//int __cs_sched_yield(unsigned __CPROVER_bitvector[9] val) {return 0;}     // Version 2


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

unsigned __CPROVER_bitvector[8] guess_pc() {unsigned __CPROVER_bitvector[8] i; return i;}

/* here */


                                                       unsigned int TRACK_ADDRESS[6];
                                                       int TRACK_VALUE[6];
                                                       unsigned int num_malloc;
                                                       extern unsigned int nondet_uint(void);
                                                       void __CS_atomic_init_MEMORY(void)
                                                       {
                                                         TRACK_ADDRESS[0] = 1;
                                                         TRACK_ADDRESS[1] = 2;
                                                         TRACK_ADDRESS[2] = 3;
                                                         TRACK_ADDRESS[3] = 4;
                                                         TRACK_ADDRESS[4] = 5;
                                                         TRACK_ADDRESS[5] = 6;
                                                         num_malloc = 1;
                                                       }

                                                       unsigned int __CS_atomic_malloc(size_t __csLOCALPARAM___CS_atomic_malloc_size)
                                                       {
                                                         static unsigned int __csLOCAL___CS_atomic_malloc_tmp;
                                                         __csLOCAL___CS_atomic_malloc_tmp = num_malloc;
                                                         num_malloc += __csLOCALPARAM___CS_atomic_malloc_size;
                                                         return __csLOCAL___CS_atomic_malloc_tmp;
                                                       }

                                                       void __CS_atomic_free(unsigned int __csLOCALPARAM___CS_atomic_free_address)
                                                       {
                                                       }

                                                       int __CS_atomic_READ(unsigned int __csLOCALPARAM___CS_atomic_READ_address)
                                                       {
                                                         return TRACK_VALUE[__csLOCALPARAM___CS_atomic_READ_address - 1];
                                                       }

                                                       void __CS_atomic_WRITE(unsigned int __csLOCALPARAM___CS_atomic_WRITE_address, int __csLOCALPARAM___CS_atomic_WRITE_value)
                                                       {
                                                         TRACK_VALUE[__csLOCALPARAM___CS_atomic_WRITE_address - 1] = __csLOCALPARAM___CS_atomic_WRITE_value;
                                                       }

typedef
                                                       struct SafeStack
                                                       {
                                                       		unsigned int array;
                                                       		int head;
                                                       		int count;
                                                       } SafeStack;
                                                       __cs_t threads[3];
                                                       SafeStack stack;
                                                       void __CS_atomic_store(int *__csLOCALPARAM___CS_atomic_store_obj, int __csLOCALPARAM___CS_atomic_store_v)
                                                       {
                                                         *__csLOCALPARAM___CS_atomic_store_obj = __csLOCALPARAM___CS_atomic_store_v;
                                                       }

                                                       void __CS_atomic_store_ADDR(unsigned int __csLOCALPARAM___CS_atomic_store_ADDR_obj, int __csLOCALPARAM___CS_atomic_store_ADDR_v)
                                                       {
                                                         __CS_atomic_WRITE(__csLOCALPARAM___CS_atomic_store_ADDR_obj, __csLOCALPARAM___CS_atomic_store_ADDR_v);
                                                       }

                                                       int __CS_atomic_load(int *__csLOCALPARAM___CS_atomic_load_obj)
                                                       {
                                                         return *__csLOCALPARAM___CS_atomic_load_obj;
                                                       }

                                                       int __CS_atomic_load_ADDR(unsigned int __csLOCALPARAM___CS_atomic_load_ADDR_obj)
                                                       {
                                                         return __CS_atomic_READ(__csLOCALPARAM___CS_atomic_load_ADDR_obj);
                                                       }

                                                       int __CS_atomic_exchange(int *__csLOCALPARAM___CS_atomic_exchange_obj, int __csLOCALPARAM___CS_atomic_exchange_v)
                                                       {
                                                         static int __csLOCAL___CS_atomic_exchange_t;
                                                         __csLOCAL___CS_atomic_exchange_t = *__csLOCALPARAM___CS_atomic_exchange_obj;
                                                         *__csLOCALPARAM___CS_atomic_exchange_obj = __csLOCALPARAM___CS_atomic_exchange_v;
                                                         return __csLOCAL___CS_atomic_exchange_t;
                                                       }

                                                       int __CS_atomic_exchange_ADDR(unsigned int __csLOCALPARAM___CS_atomic_exchange_ADDR_obj, int __csLOCALPARAM___CS_atomic_exchange_ADDR_v)
                                                       {
                                                         static int __csLOCAL___CS_atomic_exchange_ADDR_t;
                                                         __csLOCAL___CS_atomic_exchange_ADDR_t = TRACK_VALUE[__csLOCALPARAM___CS_atomic_exchange_ADDR_obj - 1];
                                                         TRACK_VALUE[__csLOCALPARAM___CS_atomic_exchange_ADDR_obj - 1] = __csLOCALPARAM___CS_atomic_exchange_ADDR_v;
                                                         return __csLOCAL___CS_atomic_exchange_ADDR_t;
                                                       }

                                                       _Bool __CS_atomic_compare_and_exchange(int *__csLOCALPARAM___CS_atomic_compare_and_exchange_obj, int *__csLOCALPARAM___CS_atomic_compare_and_exchange_expected, int __csLOCALPARAM___CS_atomic_compare_and_exchange_desired)
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

                                                       int __CS_atomic_fetch_add(int *__csLOCALPARAM___CS_atomic_fetch_add_obj, int __csLOCALPARAM___CS_atomic_fetch_add_v)
                                                       {
                                                         static int __csLOCAL___CS_atomic_fetch_add_old;
                                                         __csLOCAL___CS_atomic_fetch_add_old = *__csLOCALPARAM___CS_atomic_fetch_add_obj;
                                                         *__csLOCALPARAM___CS_atomic_fetch_add_obj = __csLOCAL___CS_atomic_fetch_add_old + __csLOCALPARAM___CS_atomic_fetch_add_v;
                                                         return __csLOCAL___CS_atomic_fetch_add_old;
                                                       }

                                                       int __CS_atomic_fetch_sub(int *__csLOCALPARAM___CS_atomic_fetch_sub_obj, int __csLOCALPARAM___CS_atomic_fetch_sub_v)
                                                       {
                                                         static int __csLOCAL___CS_atomic_fetch_sub_old;
                                                         __csLOCAL___CS_atomic_fetch_sub_old = *__csLOCALPARAM___CS_atomic_fetch_sub_obj;
                                                         *__csLOCALPARAM___CS_atomic_fetch_sub_obj = __csLOCAL___CS_atomic_fetch_sub_old - __csLOCALPARAM___CS_atomic_fetch_sub_v;
                                                         return __csLOCAL___CS_atomic_fetch_sub_old;
                                                       }

                                                       int __cs_function_Init_inlined = 1;
                                                       int __cs_function_Destroy_inlined = 1;
                                                       int __cs_function_Pop_inlined = 1;
                                                       int __cs_function_Push_inlined = 1;
                                                       void *thread_0(void *__csLOCALPARAM_thread_arg)
                                                       {
                                                       static size_t __csLOCAL_thread_i;
                                                       static int __csLOCAL_thread_idx;
IF(1,0,tthread_0_1)                                    __csLOCAL_thread_idx = (int) ((size_t) __csLOCALPARAM_thread_arg);
                                                       __csLOCAL_thread_i = 0;
                                                       	{
                                                       static int __csLOCAL_thread_elem;
                                                       		{
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_1: IF(1,1,tthread_0_2)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_2: IF(1,2,tthread_0_3)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_3: IF(1,3,tthread_0_4)                       __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_4: IF(1,4,tthread_0_5)                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_5: IF(1,5,tthread_0_6)                       __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,6)
tthread_0_6: IF(1,6,tthread_0_7)                       __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,7);
                                                       					}
                                                       					GUARD(1,7);
tthread_0_7: IF(1,7,tthread_0_8)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_8: IF(1,8,tthread_0_9)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_9: IF(1,9,tthread_0_10)                      __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_10: IF(1,10,tthread_0_11)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_11: IF(1,11,tthread_0_12)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,12)
tthread_0_12: IF(1,12,tthread_0_13)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,13);
                                                       					}
                                                       					GUARD(1,13);
tthread_0_13: IF(1,13,tthread_0_14)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_14: IF(1,14,tthread_0_15)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_15: IF(1,15,tthread_0_16)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_16: IF(1,16,tthread_0_17)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_17: IF(1,17,tthread_0_18)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,18)
tthread_0_18: IF(1,18,tthread_0_19)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,19);
                                                       					}
                                                       					GUARD(1,19);
tthread_0_19: IF(1,19,tthread_0_20)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_20: IF(1,20,tthread_0_21)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_21: IF(1,21,tthread_0_22)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_22: IF(1,22,tthread_0_23)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_23: IF(1,23,tthread_0_24)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_24: IF(1,24,tthread_0_25)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,25)
tthread_0_25: IF(1,25,tthread_0_26)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,26);
                                                       					}
                                                       					GUARD(1,26);
tthread_0_26: IF(1,26,tthread_0_27)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_27: IF(1,27,tthread_0_28)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_28: IF(1,28,tthread_0_29)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_29: IF(1,29,tthread_0_30)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_30: IF(1,30,tthread_0_31)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,31)
tthread_0_31: IF(1,31,tthread_0_32)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,32);
                                                       					}
                                                       					GUARD(1,32);
tthread_0_32: IF(1,32,tthread_0_33)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_33: IF(1,33,tthread_0_34)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_34: IF(1,34,tthread_0_35)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_35: IF(1,35,tthread_0_36)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_36: IF(1,36,tthread_0_37)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,37)
tthread_0_37: IF(1,37,tthread_0_38)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,38);
                                                       					}
                                                       					GUARD(1,38);
tthread_0_38: IF(1,38,tthread_0_39)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_39: IF(1,39,tthread_0_40)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_40: IF(1,40,tthread_0_41)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_41: IF(1,41,tthread_0_42)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_42: IF(1,42,tthread_0_43)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_43: IF(1,43,tthread_0_44)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,44)
tthread_0_44: IF(1,44,tthread_0_45)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,45);
                                                       					}
                                                       					GUARD(1,45);
tthread_0_45: IF(1,45,tthread_0_46)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_46: IF(1,46,tthread_0_47)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_47: IF(1,47,tthread_0_48)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_48: IF(1,48,tthread_0_49)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_49: IF(1,49,tthread_0_50)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,50)
tthread_0_50: IF(1,50,tthread_0_51)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,51);
                                                       					}
                                                       					GUARD(1,51);
tthread_0_51: IF(1,51,tthread_0_52)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_52: IF(1,52,tthread_0_53)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_53: IF(1,53,tthread_0_54)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_54: IF(1,54,tthread_0_55)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_55: IF(1,55,tthread_0_56)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,56)
tthread_0_56: IF(1,56,tthread_0_57)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,57);
                                                       					}
                                                       					GUARD(1,57);
tthread_0_57: IF(1,57,tthread_0_58)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
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
tthread_0_58: IF(1,58,tthread_0_59)                    __CS_atomic_WRITE(stack.array + (2 * __csLOCAL_thread_elem), __csLOCAL_thread_idx);
tthread_0_59: IF(1,59,tthread_0_60)                    assert(__CS_atomic_READ(stack.array + (2 * __csLOCAL_thread_elem)) == __csLOCAL_thread_idx);
                                                       		{
                                                       static int __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static int __csLOCAL_Push_head1;
tthread_0_60: IF(1,60,tthread_0_61)                    __csLOCAL_Push_head1 = __CS_atomic_load(&stack.head);
                                                       ;
                                                       static int __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_0_61: IF(1,61,tthread_0_62)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_6: GUARD(1,62)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_0_62: IF(1,62,tthread_0_63)                    __csLOCAL_Push___while_cond_1 =
                                                       !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_0_63: IF(1,63,tthread_0_64)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_0_64: IF(1,64,tthread_0_65)                    __csLOCAL_Push___while_cond_1 =
                                                       !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_0_65: IF(1,65,tthread_0_66)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_0_66: IF(1,66,tthread_0_67)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_0_67: IF(1,67,tthread_0_68)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_0_68: IF(1,68,tthread_0_69)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_7: GUARD(1,69)
                                                       ;
tthread_0_69: IF(1,69,tthread_0_70)                    __CS_atomic_fetch_add(&stack.count, 1);
                                                       			__exit__Push_1_0: GUARD(1,70)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       	{
                                                       static int __csLOCAL_thread_elem;
                                                       		{
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_70: IF(1,70,tthread_0_71)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_71: IF(1,71,tthread_0_72)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_72: IF(1,72,tthread_0_73)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_73: IF(1,73,tthread_0_74)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_74: IF(1,74,tthread_0_75)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,75)
tthread_0_75: IF(1,75,tthread_0_76)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,76);
                                                       					}
                                                       					GUARD(1,76);
tthread_0_76: IF(1,76,tthread_0_77)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_77: IF(1,77,tthread_0_78)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_78: IF(1,78,tthread_0_79)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_79: IF(1,79,tthread_0_80)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_80: IF(1,80,tthread_0_81)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,81)
tthread_0_81: IF(1,81,tthread_0_82)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,82);
                                                       					}
                                                       					GUARD(1,82);
tthread_0_82: IF(1,82,tthread_0_83)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_83: IF(1,83,tthread_0_84)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_84: IF(1,84,tthread_0_85)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_85: IF(1,85,tthread_0_86)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_86: IF(1,86,tthread_0_87)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,87)
tthread_0_87: IF(1,87,tthread_0_88)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,88);
                                                       					}
                                                       					GUARD(1,88);
tthread_0_88: IF(1,88,tthread_0_89)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_9: GUARD(1,89)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_3;
                                                       				__exit__Pop_1_3: GUARD(1,89)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_89: IF(1,89,tthread_0_90)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_90: IF(1,90,tthread_0_91)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_91: IF(1,91,tthread_0_92)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_92: IF(1,92,tthread_0_93)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_93: IF(1,93,tthread_0_94)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,94)
tthread_0_94: IF(1,94,tthread_0_95)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,95);
                                                       					}
                                                       					GUARD(1,95);
tthread_0_95: IF(1,95,tthread_0_96)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_96: IF(1,96,tthread_0_97)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_97: IF(1,97,tthread_0_98)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_98: IF(1,98,tthread_0_99)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_99: IF(1,99,tthread_0_100)                   __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,100)
tthread_0_100: IF(1,100,tthread_0_101)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,101);
                                                       					}
                                                       					GUARD(1,101);
tthread_0_101: IF(1,101,tthread_0_102)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_102: IF(1,102,tthread_0_103)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_103: IF(1,103,tthread_0_104)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_104: IF(1,104,tthread_0_105)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_105: IF(1,105,tthread_0_106)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,106)
tthread_0_106: IF(1,106,tthread_0_107)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,107);
                                                       					}
                                                       					GUARD(1,107);
tthread_0_107: IF(1,107,tthread_0_108)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_10: GUARD(1,108)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_4;
                                                       				__exit__Pop_1_4: GUARD(1,108)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_0_108: IF(1,108,tthread_0_109)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_109: IF(1,109,tthread_0_110)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_110: IF(1,110,tthread_0_111)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_111: IF(1,111,tthread_0_112)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_112: IF(1,112,tthread_0_113)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,113)
tthread_0_113: IF(1,113,tthread_0_114)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,114);
                                                       					}
                                                       					GUARD(1,114);
tthread_0_114: IF(1,114,tthread_0_115)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_115: IF(1,115,tthread_0_116)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_116: IF(1,116,tthread_0_117)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_117: IF(1,117,tthread_0_118)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_118: IF(1,118,tthread_0_119)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,119)
tthread_0_119: IF(1,119,tthread_0_120)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,120);
                                                       					}
                                                       					GUARD(1,120);
tthread_0_120: IF(1,120,tthread_0_121)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_0_121: IF(1,121,tthread_0_122)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_0_122: IF(1,122,tthread_0_123)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_0_123: IF(1,123,tthread_0_124)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_0_124: IF(1,124,tthread_0_125)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(1,125)
tthread_0_125: IF(1,125,tthread_0_126)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(1,126);
                                                       					}
                                                       					GUARD(1,126);
tthread_0_126: IF(1,126,tthread_0_127)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_11: GUARD(1,127)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_5;
                                                       				__exit__Pop_1_5: GUARD(1,127)
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
                                                       		__exit_loop_8: GUARD(1,127)
                                                       ;
tthread_0_127: IF(1,127,tthread_0_128)                 __CS_atomic_WRITE(stack.array + (2 * __csLOCAL_thread_elem), __csLOCAL_thread_idx);
tthread_0_128: IF(1,128,tthread_0_129)                 assert(__CS_atomic_READ(stack.array + (2 * __csLOCAL_thread_elem)) == __csLOCAL_thread_idx);
                                                       		{
                                                       static int __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static int __csLOCAL_Push_head1;
tthread_0_129: IF(1,129,tthread_0_130)                 __csLOCAL_Push_head1 = __CS_atomic_load(&stack.head);
                                                       ;
                                                       static int __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_0_130: IF(1,130,tthread_0_131)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_12: GUARD(1,131)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_0_131: IF(1,131,tthread_0_132)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_0_132: IF(1,132,tthread_0_133)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_0_133: IF(1,133,tthread_0_134)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_0_134: IF(1,134,tthread_0_135)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_0_135: IF(1,135,tthread_0_136)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_0_136: IF(1,136,tthread_0_137)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_0_137: IF(1,137,tthread_0_138)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_13: GUARD(1,138)
                                                       ;
tthread_0_138: IF(1,138,tthread_0_139)                 __CS_atomic_fetch_add(&stack.count, 1);
                                                       			__exit__Push_1_1: GUARD(1,139)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       assume(!(__csLOCAL_thread_i < 2));
                                                       	__exit_loop_1: GUARD(1,139)
                                                       ;
                                                       goto __exit_thread;
                                                       	__exit_thread: GUARD(1,139)
                                                       ;
tthread_0_139:                                         STOP_NONVOID(139);
                                                       }



                                                       void *thread_1(void *__csLOCALPARAM_thread_arg)
                                                       {
                                                       static size_t __csLOCAL_thread_i;
                                                       static int __csLOCAL_thread_idx;
IF(2,0,tthread_1_1)                                    __csLOCAL_thread_idx = (int) ((size_t) __csLOCALPARAM_thread_arg);
                                                       __csLOCAL_thread_i = 0;
                                                       	{
                                                       static int __csLOCAL_thread_elem;
                                                       		{
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_1: IF(2,1,tthread_1_2)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_2: IF(2,2,tthread_1_3)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_3: IF(2,3,tthread_1_4)                       __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_4: IF(2,4,tthread_1_5)                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_5: IF(2,5,tthread_1_6)                       __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,6)
tthread_1_6: IF(2,6,tthread_1_7)                       __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,7);
                                                       					}
                                                       					GUARD(2,7);
tthread_1_7: IF(2,7,tthread_1_8)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_8: IF(2,8,tthread_1_9)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_9: IF(2,9,tthread_1_10)                      __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_10: IF(2,10,tthread_1_11)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_11: IF(2,11,tthread_1_12)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,12)
tthread_1_12: IF(2,12,tthread_1_13)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,13);
                                                       					}
                                                       					GUARD(2,13);
tthread_1_13: IF(2,13,tthread_1_14)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_14: IF(2,14,tthread_1_15)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_15: IF(2,15,tthread_1_16)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_16: IF(2,16,tthread_1_17)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_17: IF(2,17,tthread_1_18)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,18)
tthread_1_18: IF(2,18,tthread_1_19)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,19);
                                                       					}
                                                       					GUARD(2,19);
tthread_1_19: IF(2,19,tthread_1_20)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_20: IF(2,20,tthread_1_21)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_21: IF(2,21,tthread_1_22)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_22: IF(2,22,tthread_1_23)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_23: IF(2,23,tthread_1_24)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_24: IF(2,24,tthread_1_25)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,25)
tthread_1_25: IF(2,25,tthread_1_26)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,26);
                                                       					}
                                                       					GUARD(2,26);
tthread_1_26: IF(2,26,tthread_1_27)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_27: IF(2,27,tthread_1_28)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_28: IF(2,28,tthread_1_29)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_29: IF(2,29,tthread_1_30)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_30: IF(2,30,tthread_1_31)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,31)
tthread_1_31: IF(2,31,tthread_1_32)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,32);
                                                       					}
                                                       					GUARD(2,32);
tthread_1_32: IF(2,32,tthread_1_33)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_33: IF(2,33,tthread_1_34)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_34: IF(2,34,tthread_1_35)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_35: IF(2,35,tthread_1_36)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_36: IF(2,36,tthread_1_37)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,37)
tthread_1_37: IF(2,37,tthread_1_38)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,38);
                                                       					}
                                                       					GUARD(2,38);
tthread_1_38: IF(2,38,tthread_1_39)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_39: IF(2,39,tthread_1_40)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_40: IF(2,40,tthread_1_41)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_41: IF(2,41,tthread_1_42)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_42: IF(2,42,tthread_1_43)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_43: IF(2,43,tthread_1_44)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,44)
tthread_1_44: IF(2,44,tthread_1_45)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,45);
                                                       					}
                                                       					GUARD(2,45);
tthread_1_45: IF(2,45,tthread_1_46)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_46: IF(2,46,tthread_1_47)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_47: IF(2,47,tthread_1_48)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_48: IF(2,48,tthread_1_49)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_49: IF(2,49,tthread_1_50)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,50)
tthread_1_50: IF(2,50,tthread_1_51)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,51);
                                                       					}
                                                       					GUARD(2,51);
tthread_1_51: IF(2,51,tthread_1_52)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_52: IF(2,52,tthread_1_53)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_53: IF(2,53,tthread_1_54)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_54: IF(2,54,tthread_1_55)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_55: IF(2,55,tthread_1_56)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,56)
tthread_1_56: IF(2,56,tthread_1_57)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,57);
                                                       					}
                                                       					GUARD(2,57);
tthread_1_57: IF(2,57,tthread_1_58)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
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
tthread_1_58: IF(2,58,tthread_1_59)                    __CS_atomic_WRITE(stack.array + (2 * __csLOCAL_thread_elem), __csLOCAL_thread_idx);
tthread_1_59: IF(2,59,tthread_1_60)                    assert(__CS_atomic_READ(stack.array + (2 * __csLOCAL_thread_elem)) == __csLOCAL_thread_idx);
                                                       		{
                                                       static int __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static int __csLOCAL_Push_head1;
tthread_1_60: IF(2,60,tthread_1_61)                    __csLOCAL_Push_head1 = __CS_atomic_load(&stack.head);
                                                       ;
                                                       static int __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_1_61: IF(2,61,tthread_1_62)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_6: GUARD(2,62)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_1_62: IF(2,62,tthread_1_63)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_1_63: IF(2,63,tthread_1_64)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_1_64: IF(2,64,tthread_1_65)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_1_65: IF(2,65,tthread_1_66)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_1_66: IF(2,66,tthread_1_67)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_1_67: IF(2,67,tthread_1_68)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_1_68: IF(2,68,tthread_1_69)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_7: GUARD(2,69)
                                                       ;
tthread_1_69: IF(2,69,tthread_1_70)                    __CS_atomic_fetch_add(&stack.count, 1);
                                                       			__exit__Push_1_0: GUARD(2,70)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       	{
                                                       static int __csLOCAL_thread_elem;
                                                       		{
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_70: IF(2,70,tthread_1_71)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_71: IF(2,71,tthread_1_72)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_72: IF(2,72,tthread_1_73)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_73: IF(2,73,tthread_1_74)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_74: IF(2,74,tthread_1_75)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,75)
tthread_1_75: IF(2,75,tthread_1_76)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,76);
                                                       					}
                                                       					GUARD(2,76);
tthread_1_76: IF(2,76,tthread_1_77)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_77: IF(2,77,tthread_1_78)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_78: IF(2,78,tthread_1_79)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_79: IF(2,79,tthread_1_80)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_80: IF(2,80,tthread_1_81)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,81)
tthread_1_81: IF(2,81,tthread_1_82)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,82);
                                                       					}
                                                       					GUARD(2,82);
tthread_1_82: IF(2,82,tthread_1_83)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_83: IF(2,83,tthread_1_84)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_84: IF(2,84,tthread_1_85)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_85: IF(2,85,tthread_1_86)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_86: IF(2,86,tthread_1_87)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,87)
tthread_1_87: IF(2,87,tthread_1_88)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,88);
                                                       					}
                                                       					GUARD(2,88);
tthread_1_88: IF(2,88,tthread_1_89)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_9: GUARD(2,89)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_3;
                                                       				__exit__Pop_1_3: GUARD(2,89)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_89: IF(2,89,tthread_1_90)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_90: IF(2,90,tthread_1_91)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_91: IF(2,91,tthread_1_92)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_92: IF(2,92,tthread_1_93)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_93: IF(2,93,tthread_1_94)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,94)
tthread_1_94: IF(2,94,tthread_1_95)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,95);
                                                       					}
                                                       					GUARD(2,95);
tthread_1_95: IF(2,95,tthread_1_96)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_96: IF(2,96,tthread_1_97)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_97: IF(2,97,tthread_1_98)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_98: IF(2,98,tthread_1_99)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_99: IF(2,99,tthread_1_100)                   __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,100)
tthread_1_100: IF(2,100,tthread_1_101)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,101);
                                                       					}
                                                       					GUARD(2,101);
tthread_1_101: IF(2,101,tthread_1_102)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_102: IF(2,102,tthread_1_103)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_103: IF(2,103,tthread_1_104)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_104: IF(2,104,tthread_1_105)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_105: IF(2,105,tthread_1_106)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,106)
tthread_1_106: IF(2,106,tthread_1_107)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,107);
                                                       					}
                                                       					GUARD(2,107);
tthread_1_107: IF(2,107,tthread_1_108)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_10: GUARD(2,108)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_4;
                                                       				__exit__Pop_1_4: GUARD(2,108)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_1_108: IF(2,108,tthread_1_109)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_109: IF(2,109,tthread_1_110)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_110: IF(2,110,tthread_1_111)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_111: IF(2,111,tthread_1_112)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_112: IF(2,112,tthread_1_113)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,113)
tthread_1_113: IF(2,113,tthread_1_114)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,114);
                                                       					}
                                                       					GUARD(2,114);
tthread_1_114: IF(2,114,tthread_1_115)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_115: IF(2,115,tthread_1_116)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_116: IF(2,116,tthread_1_117)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_117: IF(2,117,tthread_1_118)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_118: IF(2,118,tthread_1_119)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,119)
tthread_1_119: IF(2,119,tthread_1_120)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,120);
                                                       					}
                                                       					GUARD(2,120);
tthread_1_120: IF(2,120,tthread_1_121)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_1_121: IF(2,121,tthread_1_122)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_1_122: IF(2,122,tthread_1_123)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_1_123: IF(2,123,tthread_1_124)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_1_124: IF(2,124,tthread_1_125)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(2,125)
tthread_1_125: IF(2,125,tthread_1_126)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(2,126);
                                                       					}
                                                       					GUARD(2,126);
tthread_1_126: IF(2,126,tthread_1_127)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_11: GUARD(2,127)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_5;
                                                       				__exit__Pop_1_5: GUARD(2,127)
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
                                                       		__exit_loop_8: GUARD(2,127)
                                                       ;
tthread_1_127: IF(2,127,tthread_1_128)                 __CS_atomic_WRITE(stack.array + (2 * __csLOCAL_thread_elem), __csLOCAL_thread_idx);
tthread_1_128: IF(2,128,tthread_1_129)                 assert(__CS_atomic_READ(stack.array + (2 * __csLOCAL_thread_elem)) == __csLOCAL_thread_idx);
                                                       		{
                                                       static int __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static int __csLOCAL_Push_head1;
tthread_1_129: IF(2,129,tthread_1_130)                 __csLOCAL_Push_head1 = __CS_atomic_load(&stack.head);
                                                       ;
                                                       static int __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_1_130: IF(2,130,tthread_1_131)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_12: GUARD(2,131)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_1_131: IF(2,131,tthread_1_132)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_1_132: IF(2,132,tthread_1_133)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_1_133: IF(2,133,tthread_1_134)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_1_134: IF(2,134,tthread_1_135)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_1_135: IF(2,135,tthread_1_136)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_1_136: IF(2,136,tthread_1_137)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_1_137: IF(2,137,tthread_1_138)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_13: GUARD(2,138)
                                                       ;
tthread_1_138: IF(2,138,tthread_1_139)                 __CS_atomic_fetch_add(&stack.count, 1);
                                                       			__exit__Push_1_1: GUARD(2,139)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       assume(!(__csLOCAL_thread_i < 2));
                                                       	__exit_loop_1: GUARD(2,139)
                                                       ;
                                                       goto __exit_thread;
                                                       	__exit_thread: GUARD(2,139)
                                                       ;
tthread_1_139:                                         STOP_NONVOID(139);
                                                       }



                                                       void *thread_2(void *__csLOCALPARAM_thread_arg)
                                                       {
                                                       static size_t __csLOCAL_thread_i;
                                                       static int __csLOCAL_thread_idx;
IF(3,0,tthread_2_1)                                    __csLOCAL_thread_idx = (int) ((size_t) __csLOCALPARAM_thread_arg);
                                                       __csLOCAL_thread_i = 0;
                                                       	{
                                                       static int __csLOCAL_thread_elem;
                                                       		{
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_1: IF(3,1,tthread_2_2)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_2: IF(3,2,tthread_2_3)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_3: IF(3,3,tthread_2_4)                       __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_4: IF(3,4,tthread_2_5)                       __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_5: IF(3,5,tthread_2_6)                       __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,6)
tthread_2_6: IF(3,6,tthread_2_7)                       __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,7);
                                                       					}
                                                       					GUARD(3,7);
tthread_2_7: IF(3,7,tthread_2_8)                       __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_8: IF(3,8,tthread_2_9)                       __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_9: IF(3,9,tthread_2_10)                      __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_10: IF(3,10,tthread_2_11)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_11: IF(3,11,tthread_2_12)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,12)
tthread_2_12: IF(3,12,tthread_2_13)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,13);
                                                       					}
                                                       					GUARD(3,13);
tthread_2_13: IF(3,13,tthread_2_14)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_3;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_14: IF(3,14,tthread_2_15)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_15: IF(3,15,tthread_2_16)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_16: IF(3,16,tthread_2_17)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_17: IF(3,17,tthread_2_18)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_0;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,18)
tthread_2_18: IF(3,18,tthread_2_19)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,19);
                                                       					}
                                                       					GUARD(3,19);
tthread_2_19: IF(3,19,tthread_2_20)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_3: GUARD(3,20)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_0;
                                                       				__exit__Pop_1_0: GUARD(3,20)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_20: IF(3,20,tthread_2_21)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_21: IF(3,21,tthread_2_22)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_22: IF(3,22,tthread_2_23)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_23: IF(3,23,tthread_2_24)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_24: IF(3,24,tthread_2_25)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,25)
tthread_2_25: IF(3,25,tthread_2_26)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,26);
                                                       					}
                                                       					GUARD(3,26);
tthread_2_26: IF(3,26,tthread_2_27)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_27: IF(3,27,tthread_2_28)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_28: IF(3,28,tthread_2_29)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_29: IF(3,29,tthread_2_30)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_30: IF(3,30,tthread_2_31)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,31)
tthread_2_31: IF(3,31,tthread_2_32)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,32);
                                                       					}
                                                       					GUARD(3,32);
tthread_2_32: IF(3,32,tthread_2_33)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_4;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_33: IF(3,33,tthread_2_34)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_34: IF(3,34,tthread_2_35)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_35: IF(3,35,tthread_2_36)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_36: IF(3,36,tthread_2_37)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_1;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,37)
tthread_2_37: IF(3,37,tthread_2_38)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,38);
                                                       					}
                                                       					GUARD(3,38);
tthread_2_38: IF(3,38,tthread_2_39)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_4: GUARD(3,39)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_1;
                                                       				__exit__Pop_1_1: GUARD(3,39)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_39: IF(3,39,tthread_2_40)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_40: IF(3,40,tthread_2_41)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_41: IF(3,41,tthread_2_42)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_42: IF(3,42,tthread_2_43)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_43: IF(3,43,tthread_2_44)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,44)
tthread_2_44: IF(3,44,tthread_2_45)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,45);
                                                       					}
                                                       					GUARD(3,45);
tthread_2_45: IF(3,45,tthread_2_46)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_46: IF(3,46,tthread_2_47)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_47: IF(3,47,tthread_2_48)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_48: IF(3,48,tthread_2_49)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_49: IF(3,49,tthread_2_50)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,50)
tthread_2_50: IF(3,50,tthread_2_51)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,51);
                                                       					}
                                                       					GUARD(3,51);
tthread_2_51: IF(3,51,tthread_2_52)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_5;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_52: IF(3,52,tthread_2_53)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_53: IF(3,53,tthread_2_54)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_54: IF(3,54,tthread_2_55)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_55: IF(3,55,tthread_2_56)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_2;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,56)
tthread_2_56: IF(3,56,tthread_2_57)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,57);
                                                       					}
                                                       					GUARD(3,57);
tthread_2_57: IF(3,57,tthread_2_58)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_5: GUARD(3,58)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_2;
                                                       				__exit__Pop_1_2: GUARD(3,58)
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
                                                       		__exit_loop_2: GUARD(3,58)
                                                       ;
tthread_2_58: IF(3,58,tthread_2_59)                    __CS_atomic_WRITE(stack.array + (2 * __csLOCAL_thread_elem), __csLOCAL_thread_idx);
tthread_2_59: IF(3,59,tthread_2_60)                    assert(__CS_atomic_READ(stack.array + (2 * __csLOCAL_thread_elem)) == __csLOCAL_thread_idx);
                                                       		{
                                                       static int __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static int __csLOCAL_Push_head1;
tthread_2_60: IF(3,60,tthread_2_61)                    __csLOCAL_Push_head1 = __CS_atomic_load(&stack.head);
                                                       ;
                                                       static int __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_2_61: IF(3,61,tthread_2_62)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_6: GUARD(3,62)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_2_62: IF(3,62,tthread_2_63)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_2_63: IF(3,63,tthread_2_64)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_2_64: IF(3,64,tthread_2_65)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_2_65: IF(3,65,tthread_2_66)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_2_66: IF(3,66,tthread_2_67)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_7;
                                                       			}
                                                       			;
                                                       			{
tthread_2_67: IF(3,67,tthread_2_68)                    __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_2_68: IF(3,68,tthread_2_69)                    __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_7: GUARD(3,69)
                                                       ;
tthread_2_69: IF(3,69,tthread_2_70)                    __CS_atomic_fetch_add(&stack.count, 1);
                                                       			__exit__Push_1_0: GUARD(3,70)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       	{
                                                       static int __csLOCAL_thread_elem;
                                                       		{
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_70: IF(3,70,tthread_2_71)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_71: IF(3,71,tthread_2_72)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_72: IF(3,72,tthread_2_73)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_73: IF(3,73,tthread_2_74)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_74: IF(3,74,tthread_2_75)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,75)
tthread_2_75: IF(3,75,tthread_2_76)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,76);
                                                       					}
                                                       					GUARD(3,76);
tthread_2_76: IF(3,76,tthread_2_77)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_77: IF(3,77,tthread_2_78)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_78: IF(3,78,tthread_2_79)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_79: IF(3,79,tthread_2_80)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_80: IF(3,80,tthread_2_81)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,81)
tthread_2_81: IF(3,81,tthread_2_82)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,82);
                                                       					}
                                                       					GUARD(3,82);
tthread_2_82: IF(3,82,tthread_2_83)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_9;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_83: IF(3,83,tthread_2_84)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_84: IF(3,84,tthread_2_85)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_85: IF(3,85,tthread_2_86)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_86: IF(3,86,tthread_2_87)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_3;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,87)
tthread_2_87: IF(3,87,tthread_2_88)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,88);
                                                       					}
                                                       					GUARD(3,88);
tthread_2_88: IF(3,88,tthread_2_89)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_9: GUARD(3,89)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_3;
                                                       				__exit__Pop_1_3: GUARD(3,89)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_89: IF(3,89,tthread_2_90)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_90: IF(3,90,tthread_2_91)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_91: IF(3,91,tthread_2_92)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_92: IF(3,92,tthread_2_93)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_93: IF(3,93,tthread_2_94)                    __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,94)
tthread_2_94: IF(3,94,tthread_2_95)                    __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,95);
                                                       					}
                                                       					GUARD(3,95);
tthread_2_95: IF(3,95,tthread_2_96)                    __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_96: IF(3,96,tthread_2_97)                    __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_97: IF(3,97,tthread_2_98)                    __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_98: IF(3,98,tthread_2_99)                    __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_99: IF(3,99,tthread_2_100)                   __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,100)
tthread_2_100: IF(3,100,tthread_2_101)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,101);
                                                       					}
                                                       					GUARD(3,101);
tthread_2_101: IF(3,101,tthread_2_102)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_10;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_102: IF(3,102,tthread_2_103)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_103: IF(3,103,tthread_2_104)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_104: IF(3,104,tthread_2_105)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_105: IF(3,105,tthread_2_106)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_4;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,106)
tthread_2_106: IF(3,106,tthread_2_107)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,107);
                                                       					}
                                                       					GUARD(3,107);
tthread_2_107: IF(3,107,tthread_2_108)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_10: GUARD(3,108)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_4;
                                                       				__exit__Pop_1_4: GUARD(3,108)
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
                                                       static int __retval__Pop_1;
                                                       			{
                                                       static _Bool __csLOCAL_Pop___while_cond_0;
tthread_2_108: IF(3,108,tthread_2_109)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_109: IF(3,109,tthread_2_110)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_110: IF(3,110,tthread_2_111)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_111: IF(3,111,tthread_2_112)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_112: IF(3,112,tthread_2_113)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,113)
tthread_2_113: IF(3,113,tthread_2_114)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,114);
                                                       					}
                                                       					GUARD(3,114);
tthread_2_114: IF(3,114,tthread_2_115)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_115: IF(3,115,tthread_2_116)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_116: IF(3,116,tthread_2_117)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_117: IF(3,117,tthread_2_118)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_118: IF(3,118,tthread_2_119)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,119)
tthread_2_119: IF(3,119,tthread_2_120)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,120);
                                                       					}
                                                       					GUARD(3,120);
tthread_2_120: IF(3,120,tthread_2_121)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       if (!__csLOCAL_Pop___while_cond_0)
                                                       				{
                                                       goto __exit_loop_11;
                                                       				}
                                                       				;
                                                       				{
                                                       static int __csLOCAL_Pop_head1;
tthread_2_121: IF(3,121,tthread_2_122)                 __csLOCAL_Pop_head1 = __CS_atomic_load(&stack.head);
                                                       static int __csLOCAL_Pop_next1;
tthread_2_122: IF(3,122,tthread_2_123)                 __csLOCAL_Pop_next1 = __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, -1);
                                                       if (__csLOCAL_Pop_next1 >= 0)
                                                       					{
                                                       static int __csLOCAL_Pop_head2;
                                                       __csLOCAL_Pop_head2 = __csLOCAL_Pop_head1;
                                                       static _Bool __csLOCAL_Pop___if_cond_0;
tthread_2_123: IF(3,123,tthread_2_124)                 __csLOCAL_Pop___if_cond_0 = __CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Pop_head2, __csLOCAL_Pop_next1);
                                                       if (__csLOCAL_Pop___if_cond_0)
                                                       						{
tthread_2_124: IF(3,124,tthread_2_125)                 __CS_atomic_fetch_sub(&stack.count, 1);
                                                       __retval__Pop_1 = __csLOCAL_Pop_head1;
                                                       goto __exit__Pop_1_5;
                                                       						}
                                                       						else
                                                       						{ GUARD(3,125)
tthread_2_125: IF(3,125,tthread_2_126)                 __CS_atomic_exchange_ADDR((stack.array + (2 * __csLOCAL_Pop_head1)) + 1, __csLOCAL_Pop_next1);
                                                       						}
                                                       						GUARD(3,126);
                                                       					}
                                                       					GUARD(3,126);
tthread_2_126: IF(3,126,tthread_2_127)                 __csLOCAL_Pop___while_cond_0 = __CS_atomic_load(&stack.count) > 1;
                                                       				};
                                                       assume(!__csLOCAL_Pop___while_cond_0);
                                                       				__exit_loop_11: GUARD(3,127)
                                                       ;
                                                       __retval__Pop_1 = -1;
                                                       goto __exit__Pop_1_5;
                                                       				__exit__Pop_1_5: GUARD(3,127)
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
                                                       		__exit_loop_8: GUARD(3,127)
                                                       ;
tthread_2_127: IF(3,127,tthread_2_128)                 __CS_atomic_WRITE(stack.array + (2 * __csLOCAL_thread_elem), __csLOCAL_thread_idx);
tthread_2_128: IF(3,128,tthread_2_129)                 assert(__CS_atomic_READ(stack.array + (2 * __csLOCAL_thread_elem)) == __csLOCAL_thread_idx);
                                                       		{
                                                       static int __csLOCALPARAM_Push_index;
                                                       __csLOCALPARAM_Push_index = __csLOCAL_thread_elem;
                                                       static int __csLOCAL_Push_head1;
tthread_2_129: IF(3,129,tthread_2_130)                 __csLOCAL_Push_head1 = __CS_atomic_load(&stack.head);
                                                       ;
                                                       static int __csLOCAL_Push___cs_dowhile_onetime_1;
                                                       __csLOCAL_Push___cs_dowhile_onetime_1 = 0;
                                                       			{
tthread_2_130: IF(3,130,tthread_2_131)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
                                                       			};
                                                       __csLOCAL_Push___cs_dowhile_onetime_1++;
                                                       assume(!(__csLOCAL_Push___cs_dowhile_onetime_1 < 1));
                                                       			__exit_loop_12: GUARD(3,131)
                                                       ;
                                                       static _Bool __csLOCAL_Push___while_cond_1;
tthread_2_131: IF(3,131,tthread_2_132)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_2_132: IF(3,132,tthread_2_133)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_2_133: IF(3,133,tthread_2_134)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_2_134: IF(3,134,tthread_2_135)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_2_135: IF(3,135,tthread_2_136)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       if (!__csLOCAL_Push___while_cond_1)
                                                       			{
                                                       goto __exit_loop_13;
                                                       			}
                                                       			;
                                                       			{
tthread_2_136: IF(3,136,tthread_2_137)                 __CS_atomic_store_ADDR((stack.array + (2 * __csLOCALPARAM_Push_index)) + 1, __csLOCAL_Push_head1);
tthread_2_137: IF(3,137,tthread_2_138)                 __csLOCAL_Push___while_cond_1 = !__CS_atomic_compare_and_exchange(&stack.head, &__csLOCAL_Push_head1, __csLOCALPARAM_Push_index);
                                                       			};
                                                       assume(!__csLOCAL_Push___while_cond_1);
                                                       			__exit_loop_13: GUARD(3,138)
                                                       ;
tthread_2_138: IF(3,138,tthread_2_139)                 __CS_atomic_fetch_add(&stack.count, 1);
                                                       			__exit__Push_1_1: GUARD(3,139)
                                                       ;
                                                       		};
                                                       0;
                                                       	};
                                                       __csLOCAL_thread_i++;
                                                       assume(!(__csLOCAL_thread_i < 2));
                                                       	__exit_loop_1: GUARD(3,139)
                                                       ;
                                                       goto __exit_thread;
                                                       	__exit_thread: GUARD(3,139)
                                                       ;
tthread_2_139:                                         STOP_NONVOID(139);
                                                       }



                                                       int main_thread(void)
                                                       {
IF(0,0,tmain_1)                                        static int __csLOCAL_main_i;
                                                       	{
                                                       static int __csLOCALPARAM_Init_pushCount;
                                                       __csLOCALPARAM_Init_pushCount = 3;
                                                       static int __csLOCAL_Init_i;
                                                       stack.array = __CS_atomic_malloc(__csLOCALPARAM_Init_pushCount * 2);
                                                       __CS_atomic_store(&stack.count, __csLOCALPARAM_Init_pushCount);
                                                       __CS_atomic_store(&stack.head, 0);
                                                       __csLOCAL_Init_i = 0;
                                                       if (!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)))
                                                       		{
                                                       goto __exit_loop_14;
                                                       		}
                                                       		;
                                                       		{
                                                       __CS_atomic_store_ADDR((stack.array + (2 * __csLOCAL_Init_i)) + 1, __csLOCAL_Init_i + 1);
                                                       		};
                                                       __csLOCAL_Init_i++;
                                                       if (!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)))
                                                       		{
                                                       goto __exit_loop_14;
                                                       		}
                                                       		;
                                                       		{
                                                       __CS_atomic_store_ADDR((stack.array + (2 * __csLOCAL_Init_i)) + 1, __csLOCAL_Init_i + 1);
                                                       		};
                                                       __csLOCAL_Init_i++;
                                                       if (!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)))
                                                       		{
                                                       goto __exit_loop_14;
                                                       		}
                                                       		;
                                                       		{
                                                       __CS_atomic_store_ADDR((stack.array + (2 * __csLOCAL_Init_i)) + 1, __csLOCAL_Init_i + 1);
                                                       		};
                                                       __csLOCAL_Init_i++;
                                                       assume(!(__csLOCAL_Init_i < (__csLOCALPARAM_Init_pushCount - 1)));
                                                       		__exit_loop_14: GUARD(0,1)
                                                       ;
                                                       __CS_atomic_store_ADDR((stack.array + (2 * (__csLOCALPARAM_Init_pushCount - 1))) + 1, -1);
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



                                                       int main(void) {
                                                        __CS_atomic_init_MEMORY();
                                                                 // round 0
                                                                 thread_index = 0;
                                                                 pc_cs = pc[0] + guess_pc();
                                                                 assume((pc_cs > 0) && (pc_cs <= 6));
                                                                 main_thread();
                                                                 pc[0] = pc_cs;

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 139));
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 139));
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= 0) && (pc_cs <= 139));
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 // round 1
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume((pc_cs >= pc[0]) && (pc_cs <= 6));
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 139));
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 139));
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 139));
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 // round 2
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume((pc_cs >= pc[0]) && (pc_cs <= 6));
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 139));
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 139));
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 139));
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 // round 3
                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) { /** main **/
                                                                     pc_cs = pc[0] + guess_pc();
                                                                     assume((pc_cs >= pc[0]) && (pc_cs <= 6));
                                                                     main_thread();
                                                                     pc[0] = pc_cs;
                                                                 }

                                                                 thread_index = 1;
                                                                 if (active_thread[1] == 1) { /** thread_0 **/
                                                                    pc_cs = pc[1] + guess_pc();
                                                                    assume((pc_cs >= pc[1]) && (pc_cs <= 139));
                                                                    thread_0(threadargs[1]);
                                                                    pc[1] = pc_cs;
                                                                 }

                                                                 thread_index = 2;
                                                                 if (active_thread[2] == 1) { /** thread_1 **/
                                                                    pc_cs = pc[2] + guess_pc();
                                                                    assume((pc_cs >= pc[2]) && (pc_cs <= 139));
                                                                    thread_1(threadargs[2]);
                                                                    pc[2] = pc_cs;
                                                                 }

                                                                 thread_index = 3;
                                                                 if (active_thread[3] == 1) { /** thread_2 **/
                                                                    pc_cs = pc[3] + guess_pc();
                                                                    assume((pc_cs >= pc[3]) && (pc_cs <= 139));
                                                                    thread_2(threadargs[3]);
                                                                    pc[3] = pc_cs;
                                                                 }

                                                                 thread_index = 0;
                                                                 if (active_thread[0] == 1) {
                                                                    pc_cs = pc[0] + guess_pc();
                                                                    assume((pc_cs >= pc[0]) && (pc_cs <= 6));
                                                                    main_thread();
                                                                 }

                                                          return 0;
                                                       }



