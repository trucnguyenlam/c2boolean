# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 1 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call.c"
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
# 2 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call.c" 2

extern int __VERIFIER_nondet_int(void);

struct node {
    struct node *next;
    int value;
};

struct list {
    struct node *slist;
    struct list *next;
};

static void merge_single_node(struct node ***dst,
    struct node **data)
{

    struct node *node = *data;
    *data = node->next;
    node->next = 0;


    **dst = node;
    *dst = &node->next;
}

static void merge_pair(struct node **dst,
    struct node *sub1,
    struct node *sub2)
{

    while (sub1 || sub2) {
        if (!sub2 || (sub1 && sub1->value < sub2->value))
            merge_single_node(&dst, &sub1);
        else
            merge_single_node(&dst, &sub2);
    }
}

static struct list* seq_sort_core(struct list *data)
{
    struct list *dst = 0;

    while (data) {
        struct list *next = data->next;
        if (!next) {

            data->next = dst;
            dst = data;
            break;
        }


        merge_pair(&data->slist, data->slist, next->slist);
        data->next = dst;
        dst = data;


        data = next->next;
        free(next);
    }

    return dst;
}

static void inspect_before(struct list *shape)
{

    assert(shape);

    for (; shape->next; shape = shape->next) {
        assert(shape);
        assert(shape->next);
        assert(shape->slist);
        assert(shape->slist->next == 0);
    }


    assert(shape);
    assert(shape->next == 0);
    assert(shape->slist);
    assert(shape->slist->next == 0);
}

static void inspect_after(struct list *shape)
{

    assert(shape);
    assert(shape->next == 0);
    assert(shape->slist != 0);


    struct node *pos;
    for (pos = shape->slist; pos->next; pos = pos->next);
    assert(!pos->next);
}



int main()
{

    struct list *data = 0;
    while (__VERIFIER_nondet_int()) {
        struct node *node = malloc(1*sizeof(struct node));



        node->next = node;
        node->value = __VERIFIER_nondet_int();

        struct list *item = malloc(1*sizeof(struct list));



        item->slist = node;
        item->next = data;
        data = item;
    }

    if (!data)
        return 0;

    inspect_before(data);




    while (data->next)
    {
        data = seq_sort_core(data);
    }
    inspect_after(data);

    struct node *node = data->slist;
    free(data);

    while (node) {
        struct node *snext = node->next;
        free(node);
        node = snext;
    }

    return 0;
}
