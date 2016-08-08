# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 1 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call_Mem_ver2.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call_Mem_ver2.c"
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
# 2 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call_Mem_ver2.c" 2

extern int __VERIFIER_nondet_int(void);
extern unsigned int __VERIFIER_nondet_uint(void);






unsigned int TRACK_ADDRESS[31];
int TRACK_VALUE[31];

_Bool TRACK_VALID[31];
unsigned int TRACK_POS[31];


unsigned int num_malloc;



void init_MEM(void)
{
    unsigned int TRACK_ADDRESS_tmp[13];
    TRACK_ADDRESS[0] = 0;
    TRACK_ADDRESS[1] = 1;
    TRACK_ADDRESS[2] = 2;
    TRACK_ADDRESS[3] = 3;
    TRACK_ADDRESS[4] = 4;
    TRACK_ADDRESS[5] = 5;
    TRACK_ADDRESS[6] = 6;
    TRACK_ADDRESS[7] = 7;
    TRACK_ADDRESS[8] = 8;
    TRACK_ADDRESS[9] = 9;
    TRACK_ADDRESS[10] = 10;
    TRACK_ADDRESS[11] = 11;
    TRACK_ADDRESS[12] = 12;
    TRACK_ADDRESS[13] = 13;
    TRACK_ADDRESS[14] = 14;
    TRACK_ADDRESS[15] = 15;
    TRACK_ADDRESS[16] = 16;
    num_malloc = 17;
    TRACK_ADDRESS[17] = TRACK_ADDRESS_tmp[0];
    __CPROVER_assume((TRACK_ADDRESS[17] <= 1000) & (TRACK_ADDRESS[17] > TRACK_ADDRESS[16]));
    TRACK_ADDRESS[18] = TRACK_ADDRESS_tmp[1];
    __CPROVER_assume((TRACK_ADDRESS[18] <= 1000) & (TRACK_ADDRESS[18] > TRACK_ADDRESS[17]));
    TRACK_ADDRESS[19] = TRACK_ADDRESS_tmp[2];
    __CPROVER_assume((TRACK_ADDRESS[19] <= 1000) & (TRACK_ADDRESS[19] > TRACK_ADDRESS[18]));
    TRACK_ADDRESS[20] = TRACK_ADDRESS_tmp[3];
    __CPROVER_assume((TRACK_ADDRESS[20] <= 1000) & (TRACK_ADDRESS[20] > TRACK_ADDRESS[19]));
    TRACK_ADDRESS[21] = TRACK_ADDRESS_tmp[4];
    __CPROVER_assume((TRACK_ADDRESS[21] <= 1000) & (TRACK_ADDRESS[21] > TRACK_ADDRESS[20]));
    TRACK_ADDRESS[22] = TRACK_ADDRESS_tmp[5];
    __CPROVER_assume((TRACK_ADDRESS[22] <= 1000) & (TRACK_ADDRESS[22] > TRACK_ADDRESS[21]));
    TRACK_ADDRESS[23] = TRACK_ADDRESS_tmp[6];
    __CPROVER_assume((TRACK_ADDRESS[23] <= 1000) & (TRACK_ADDRESS[23] > TRACK_ADDRESS[22]));
    TRACK_ADDRESS[24] = TRACK_ADDRESS_tmp[7];
    __CPROVER_assume((TRACK_ADDRESS[24] <= 1000) & (TRACK_ADDRESS[24] > TRACK_ADDRESS[23]));
    TRACK_ADDRESS[25] = TRACK_ADDRESS_tmp[8];
    __CPROVER_assume((TRACK_ADDRESS[25] <= 1000) & (TRACK_ADDRESS[25] > TRACK_ADDRESS[24]));
    TRACK_ADDRESS[26] = TRACK_ADDRESS_tmp[9];
    __CPROVER_assume((TRACK_ADDRESS[26] <= 1000) & (TRACK_ADDRESS[26] > TRACK_ADDRESS[25]));
    TRACK_ADDRESS[27] = TRACK_ADDRESS_tmp[10];
    __CPROVER_assume((TRACK_ADDRESS[27] <= 1000) & (TRACK_ADDRESS[27] > TRACK_ADDRESS[26]));
    TRACK_ADDRESS[28] = TRACK_ADDRESS_tmp[11];
    __CPROVER_assume((TRACK_ADDRESS[28] <= 1000) & (TRACK_ADDRESS[28] > TRACK_ADDRESS[27]));
    TRACK_ADDRESS[29] = TRACK_ADDRESS_tmp[12];
    __CPROVER_assume((TRACK_ADDRESS[29] <= 1000) & (TRACK_ADDRESS[29] > TRACK_ADDRESS[28]));
    TRACK_ADDRESS[30] = 1000 + 1;

}


unsigned int my_malloc(size_t size)
{
    unsigned int tmp = num_malloc;

    num_malloc += size + 1;

    return tmp + 1;
}


void my_free(unsigned int p)
{

}


int READ(int codevar)
{
    return TRACK_VALUE[codevar];
}

void WRITE(int codevar, int value)
{
    TRACK_VALUE[codevar] = value;
}

unsigned int pos;

int READ_NORMAL(unsigned int address)
{

    pos = __VERIFIER_nondet_uint();
    __CPROVER_assume((pos >= 0) & (pos < 31 - 1) );

    __CPROVER_assume(TRACK_ADDRESS[pos] == address);
    return TRACK_VALUE[pos];
}

int READ_POS(unsigned int position)
{
    if (TRACK_VALID[position])
    {
        pos = TRACK_POS[position];
        return TRACK_VALUE[pos];
    }
    return READ_NORMAL(TRACK_VALUE[position]);
}

void WRITE_NORMAL(unsigned int address, int value)
{
    pos = __VERIFIER_nondet_uint();
    __CPROVER_assume((pos >= 0) && (pos < (31 - 1)));
    __CPROVER_assume((address >= TRACK_ADDRESS[pos]) & (address < TRACK_ADDRESS[pos + 1]));
    if (TRACK_ADDRESS[pos] == address)
    {
        TRACK_VALUE[pos] = value;
    }
    else
    {
        __CPROVER_assume(0);
    }
}

void WRITE_POS(unsigned int position, int value)
{
    if (TRACK_VALID[position])
    {
        pos = TRACK_POS[position];
        TRACK_VALUE[pos] = value;
    }
    else
    {
        WRITE_NORMAL(TRACK_VALUE[position], value);
    }
}
# 171 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call_Mem_ver2.c"
static void merge_single_node(unsigned int dst, unsigned int data)
{
    TRACK_VALUE[12] = dst;
    TRACK_VALUE[13] = data;




    TRACK_VALUE[14] = READ_NORMAL(TRACK_VALUE[13]);
    TRACK_VALID[14] = 1;
    TRACK_POS[14] = pos;


    WRITE_POS(pos, READ_POS(14));


    WRITE_POS(14, 0);



    WRITE_NORMAL(READ_NORMAL(TRACK_VALUE[12]), TRACK_VALUE[14]);


    WRITE_NORMAL(TRACK_VALUE[12], TRACK_VALUE[14]);
}


static void merge_pair(unsigned int dst, unsigned int sub1, unsigned int sub2)
{
    TRACK_VALUE[9] = dst;
    TRACK_VALUE[10] = sub1;
    TRACK_VALUE[11] = sub2;



    while ((TRACK_VALUE[10] || TRACK_VALUE[11])) {

        if (!TRACK_VALUE[11] || (TRACK_VALUE[10] && READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1)))
        {

            merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
        }
        else
        {

            merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
        }
    }
}


static unsigned int seq_sort_core(unsigned int data)
{
    TRACK_VALUE[6] = data;



    TRACK_VALUE[7] = 0;


    while (TRACK_VALUE[6]) {


        TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
        TRACK_VALID[8] = 1;
        TRACK_VALID[8] = pos;


        if (!TRACK_VALUE[8]) {


            WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
            TRACK_VALID[pos] = 1;
            TRACK_POS[pos] = 7;

            TRACK_VALUE[7] = TRACK_VALUE[6];
            TRACK_VALID[7] = 1;
            TRACK_POS[7] = 6;
            break;
        }



        merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));


        WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
        TRACK_VALID[pos] = 1;
        TRACK_POS[pos] = 7;


        TRACK_VALUE[7] = TRACK_VALUE[6];
        TRACK_VALID[7] = 1;
        TRACK_POS[7] = 6;



        TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
        TRACK_VALID[6] = 1;
        TRACK_POS[6] = pos;


        my_free(TRACK_VALUE[8]);
    }


    return TRACK_VALUE[7];
}


static void inspect_before(unsigned int shape)
{

    TRACK_VALUE[5] = shape;


    assert(TRACK_VALUE[5]);


    while (READ_NORMAL(TRACK_VALUE[5] + 1))
    {
        assert(TRACK_VALUE[5]);

        assert(READ_NORMAL(TRACK_VALUE[5] + 1));

        assert(READ_POS(5));

        assert(READ_NORMAL(READ_POS(5)) == 0);
        TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
        TRACK_VALID[5] = 1;
        TRACK_POS[5] = pos;
    }


    assert(TRACK_VALUE[5]);

    assert(READ_NORMAL(TRACK_VALUE[5] + 1) == 0);

    assert(READ_POS(5));

    assert(READ_NORMAL(READ_POS(5)) == 0);
}


static void inspect_after(unsigned int shape)
{
    TRACK_VALUE[15] = shape;


    assert(TRACK_VALUE[15]);


    assert(READ_NORMAL(TRACK_VALUE[15] + 1) == 0);


    assert(READ_POS(15) != 0);




    TRACK_VALUE[16] = READ_NORMAL(TRACK_VALUE[15]);
    TRACK_VALID[16] = 1;
    TRACK_POS[16] = 1;

    while(READ_POS(16))
    {

        assert(!READ_NORMAL(TRACK_VALUE[16]));
        TRACK_VALUE[16] = READ_POS(16);
        TRACK_VALID[16] = 1;
        TRACK_POS[16] = pos;
    }
}



int main()
{
    init_MEM();



    TRACK_VALUE[0] = 0;
    TRACK_VALID[0] = 0;

    while (__VERIFIER_nondet_int()) {


        TRACK_VALUE[1] = my_malloc(1 * 2);
# 369 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call_Mem_ver2.c"
        WRITE_POS(1, TRACK_VALUE[1]);
        TRACK_VALID[pos] = 1;
        TRACK_POS[pos] = 1;


        WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());




        TRACK_VALUE[2] = my_malloc(1 * 2);
# 389 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call_Mem_ver2.c"
        WRITE_POS(2, TRACK_VALUE[1]);
        TRACK_VALID[pos] = 1;
        TRACK_POS[pos] = 1;


        WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
        TRACK_VALID[pos] = 1;
        TRACK_POS[pos] = 0;


        TRACK_VALUE[0] = TRACK_VALUE[2];
        TRACK_VALID[0] = 1;
        TRACK_POS[0] = 2;
    }
# 412 "/home/trucnguyenlam/Development/PLDI2016/memory_model/merge_sort_false-unreach-call_Mem_ver2.c"
    inspect_before(TRACK_VALUE[0]);



    while (READ_NORMAL(TRACK_VALUE[0] + 1))
    {

        TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
        TRACK_VALID[0] = 1;
        TRACK_POS[0] = 7;
    }


    inspect_after(TRACK_VALUE[0]);



    TRACK_VALUE[3] = READ_POS(0);

    my_free(TRACK_VALUE[0]);


    while (TRACK_VALUE[3]) {


        TRACK_VALUE[4] = READ_POS(3);
        TRACK_VALID[4] = 1;
        TRACK_POS[4] = 3;

        my_free(TRACK_VALUE[4]);

        TRACK_VALUE[3] = TRACK_VALUE[4];
        TRACK_VALID[3] = 1;
        TRACK_POS[3] = 1;
    }


    return 0;
}
