//line[]: a_cs_arr_0
//item: a_cs_arr_0
#include <stdlib.h>
#include <pthread.h>

#define LOCK_SIZE 0
#define vLOCK_SIZE 0

//#define NUM_WRITES <insert-NUM_WRITES-here>

#define MEM_SIZE 51
#define MEM_SIZE_VARS 5
#define NumVars 10
#define MaxNumThreads 3
#define NumAddresses 5
#define NumMalloc 5

#define  intV unsigned __CPROVER_bitvector[17]
//#define  intV int
#define __CS_type unsigned char
#define  __cs_t unsigned __CPROVER_bitvector[17]


#define __CS_typeM unsigned char
#define __CS_typeV unsigned char
#define __CS_typeT unsigned char

/*
#define __CS_typeM unsigned __CPROVER_bitvector[<insert-type-typeM-here>]
#define __CS_typeV unsigned __CPROVER_bitvector[<insert-type-typeV-here>]
#define __CS_typeT unsigned __CPROVER_bitvector[<insert-type-typeT-here>]
*/

#define __CS_pthread_t unsigned char
#define __CS_pthread_mutex_t unsigned char
#define __CS_pthread_cond_t unsigned char

//start global variables

#define i 0
#define j 1
#define p1 2
#define p 3
#define a_cs_arr_0 4

// Supposely for pointer

#define _cs_pointer_0 5
#define _cs_pointer_1 6
#define _cs_pointer_2 7
#define _cs_pointer_3 8
#define _cs_pointer_4 9

//for each variable we represent a MU composed by
//for variable i
#define i_SIZE 5
//for variable j
#define j_SIZE 5
//for variable p1
#define p1_SIZE 5
//for variable p
#define p_SIZE 5
//for variable a_cs_arr_0
#define a_cs_arr_0_SIZE 5
//for variable _cs_pointer_0
#define _cs_pointer_0_SIZE 5
//for variable _cs_pointer_1
#define _cs_pointer_1_SIZE 5
//for variable _cs_pointer_2
#define _cs_pointer_2_SIZE 5
//for variable _cs_pointer_3
#define _cs_pointer_3_SIZE 5
//for variable _cs_pointer_4
#define _cs_pointer_4_SIZE 5
//end global variables


// shared memory




//for each variable "i" we represent a MU composed by
intV __value[NumVars] [MEM_SIZE_VARS]; //value written at variable i

__CS_typeM __ticket [NumVars] [MEM_SIZE_VARS + 1]; //Unique ticket assigned to the write opereation

__CS_typeT __thread_id [NumVars] [MEM_SIZE_VARS]; //Which thread writes variable i

__CS_typeT __visible [NumVars] [MEM_SIZE_VARS]; //it is not in a locked memory position

__CS_typeM __thw[NumVars][MaxNumThreads][MEM_SIZE_VARS]; //next write of each thread for i

__CS_typeM __last_used_pos_of[NumVars]; //last used pos of i

__CS_typeM __memory_notvalid_of[NumVars]; //last valid write pos of i


//Gildo: tolgo isBool
//<insert-bool-declaration-here>
//__CS_typeT thread[MEM_SIZE];
//__CS_typeM thw[MaxNumThreads][MEM_SIZE];

__CS_typeM ticket_pos; //greatest ticket number encountered during the simulation
_Bool __used_ticket[MEM_SIZE + NumVars + 3]; //used by guessing in order to guarantee that each ticket is unique


//<insert-defMax-here>

__CS_typeT n_treads_createad = 1;

_Bool __CS_error = 0;                                              //cseq: set whenever an error is found and checked after thread-wrapping
intV __CS_ret = 0;
_Bool terminated[MaxNumThreads];//={0,0,0};     //viene settato ad 1 solo se il thread viene terminato
intV GuessedNumThreads;
__CS_typeM ticket_pos_terminated[MaxNumThreads] = {0, 0, 0};    //last ticket_pos



//cseq: handling of the status of the threads
__CS_typeT __cs_thread_index;                                           //cseq: currently running thread ranging in [1..max+1], 1 being main()
__CS_typeT number_threads;

__CPROVER_bitvector[1] nondet_0() {__CPROVER_bitvector[1] pippo; return pippo;};
__CPROVER_bitvector[1] nondet_1() {__CPROVER_bitvector[1] pippo; return pippo;};
_Bool nondet_bool() {_Bool pippo; return pippo;};

intV Read_atomic(__CS_typeV codeVar);
intV Read_var_atomic();
void Write_var_atomic();

//<insert-read-write-functions-here>
intV Read(__CS_typeV codeVar) {
    __CS_typeM jump;

    if ((__CS_ret) || ( jump >= __memory_notvalid_of[codeVar] )) {__CS_ret = 1; return;}

    __CS_typeM ticket_jump = __ticket[codeVar][jump];
    __CPROVER_assume( (jump < __thw[codeVar][__cs_thread_index][__last_used_pos_of[codeVar]] )
                      && (__ticket[codeVar][jump + 1] > ticket_pos)

                    );
    /*&& ( !(ticket_jump > ticket_pos) || (__visible[codeVar][jump]) )*/

    ticket_pos = (ticket_jump > ticket_pos) ? ticket_jump : ticket_pos;
    /**/

    return (__value[codeVar][jump]);

}
void Write(__CS_typeV codeVar, intV val) {
    if (__CS_ret) return;

    __CS_typeM last_pos = __thw[codeVar][__cs_thread_index][__last_used_pos_of[codeVar]];

    if (last_pos >= __memory_notvalid_of[codeVar] ) {__CS_ret = 1; return;}
    __last_used_pos_of[codeVar] = last_pos;


    __CPROVER_assume (
        (__ticket[codeVar][last_pos] > ticket_pos)
        &&  (__value[codeVar][last_pos] == val)
    ) ;
    ticket_pos = __ticket[codeVar][last_pos];
    /**/

}
intV nondet_int();

/*
intV Read_atomic(__CS_typeV codeVar){
    //return (__value[codeVar][jump]);
    return (__value[codeVar][__last_used_pos_of[codeVar]]);
}
*/
_Bool nondet_bool();





intV __pthread_join( __CS_pthread_t t1, __CS_type __p) {
//  return;
    __CS_typeV v;
    __CPROVER_assume(v < NumVars);
    if (nondet_int()) {__CS_ret = 1; return;}
    Read(v);

    //Read_var_lock();

    __CPROVER_assume((terminated[t1]) && (ticket_pos >= ticket_pos_terminated[t1]));

}



intV __pthread_create_thread1(__CS_pthread_t *thread_id, void *attr, void *arg);
intV __pthread_create_thread2(__CS_pthread_t *thread_id, void *attr, void *arg);
intV __pthread_create_thread3(__CS_pthread_t *thread_id, void *attr, void *arg);


///???????
intV address[NumVars];//ordinati strett. crescente;


//void Write_ptr(__CS_typeV codeVar, intV offset, intV val){
void Write_ptr(intV addr, intV val) {
    //intV addr = Read(codeVar)+offset;
    __CS_typeV pos;
    __CPROVER_assume(pos < NumVars);
    //__CPROVER_assume(address[pos]==addr);
    //Write(pos, val);
    __CPROVER_assume((address[pos] <= addr) && (addr < address[pos + 1]));
    if (address[pos] == addr ) Write(pos, val);


}

//intV Read_ptr(__CS_typeV codeVar, intV offset){
intV Read_ptr(intV addr) {
    //intV addr = Read(codeVar)+offset;
    __CS_typeV pos;
    __CPROVER_assume(pos < NumVars);
    __CPROVER_assume(address[pos] == addr);
    return Read(pos);
}
intV mallocP[NumMalloc + 1]; //in mallocP[NumMalloc]=bigNumber
intV mallocPallocated[NumMalloc];


intV MyMalloc(intV size) {
    intV pos;
    __CPROVER_assume(pos < NumMalloc);
    __CPROVER_assume(!mallocPallocated[pos]
                     &&  (mallocP[pos] + size < mallocP[pos + 1])
                    );
    mallocPallocated[pos] = 1;
    return (mallocP[pos]);
}

void MyFree(__CS_typeV codeVar) {
    intV addr = Read(codeVar);
    __CS_typeV pos;
    __CPROVER_assume(pos < NumMalloc);
    __CPROVER_assume(mallocPallocated[pos]
                     &&  (mallocP[pos] == addr)
                    );
    mallocPallocated[pos] = 0;
    Write(codeVar, 0);
}

void *t1_0(void *__cs_param_t1_arg)
{
    Write_ptr(Read(p1), Read_ptr((address[j])));
    Write_ptr(address[a_cs_arr_0] + 0, (Read_ptr(Read(p1))) + 1);
    Write(i,  Read_ptr(address[a_cs_arr_0] + 0));
    if (nondet_int()    ) {__CS_ret = 1; return 0;} __CPROVER_assume(Read(i) == 4);

    Write(p,  address[a_cs_arr_0]);
    Write(i,  (Read_ptr(Read(p))) + 1);
    goto __exit_t1;
    Write(p1,  (address[a_cs_arr_0] + 0));
    Write_ptr(address[a_cs_arr_0] + 0, 3);
__exit_t1:
    ;
//__pthread_exit(NULL);;
}
void *t2_0(void *__cs_param_t2_arg)
{
    Write(p1,  address[i]);
__exit_t2:
    ;
//__pthread_exit(NULL);;
}

int __pthread_create_t1_0(__CS_pthread_t *thread_id, void *attr, void *arg) {
    int __ret = 0;
    __CS_typeT my_thread_index;
    __CS_typeM my_ticket_pos;

    __CS_typeM my__last_used_pos_of[NumVars];
    my__last_used_pos_of[i] = __last_used_pos_of[i];
    my__last_used_pos_of[j] = __last_used_pos_of[j];
    my__last_used_pos_of[p1] = __last_used_pos_of[p1];
    my__last_used_pos_of[p] = __last_used_pos_of[p];
    my__last_used_pos_of[a_cs_arr_0] = __last_used_pos_of[a_cs_arr_0];
    my__last_used_pos_of[_cs_pointer_0] = __last_used_pos_of[_cs_pointer_0];
    my__last_used_pos_of[_cs_pointer_1] = __last_used_pos_of[_cs_pointer_1];
    my__last_used_pos_of[_cs_pointer_2] = __last_used_pos_of[_cs_pointer_2];
    my__last_used_pos_of[_cs_pointer_3] = __last_used_pos_of[_cs_pointer_3];
    my__last_used_pos_of[_cs_pointer_4] = __last_used_pos_of[_cs_pointer_4];

    if (MaxNumThreads == ++number_threads) return 0;
    my_thread_index = __cs_thread_index;
    my_ticket_pos   = ticket_pos;
    __cs_thread_index = number_threads;
    n_treads_createad++;

    __CPROVER_assume(__thw[i][__cs_thread_index][0] > __last_used_pos_of[i]);
    __CPROVER_assume(__thw[j][__cs_thread_index][0] > __last_used_pos_of[j]);
    __CPROVER_assume(__thw[p1][__cs_thread_index][0] > __last_used_pos_of[p1]);
    __CPROVER_assume(__thw[p][__cs_thread_index][0] > __last_used_pos_of[p]);
    __CPROVER_assume(__thw[a_cs_arr_0][__cs_thread_index][0] > __last_used_pos_of[a_cs_arr_0]);
    __CPROVER_assume(__thw[_cs_pointer_0][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_0]);
    __CPROVER_assume(__thw[_cs_pointer_1][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_1]);
    __CPROVER_assume(__thw[_cs_pointer_2][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_2]);
    __CPROVER_assume(__thw[_cs_pointer_3][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_3]);
    __CPROVER_assume(__thw[_cs_pointer_4][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_4]);
    if (nondet_int()) { __CS_ret = 1; __ret = -1;}
    else { t1_0( arg); }
    *thread_id = __cs_thread_index;
    //__CPROVER_assume((thw[__cs_thread_index][thread_pos] >=memory_notvalid) );
    //__CPROVER_assume((thread_pos< memory_notvalid) && (thw[__cs_thread_index][thread_pos] >=memory_notvalid) );

    __CPROVER_assume(__thw[i][__cs_thread_index][__last_used_pos_of[i]] >= __memory_notvalid_of[i]) ;
    __CPROVER_assume(__thw[j][__cs_thread_index][__last_used_pos_of[j]] >= __memory_notvalid_of[j]) ;
    __CPROVER_assume(__thw[p1][__cs_thread_index][__last_used_pos_of[p1]] >= __memory_notvalid_of[p1]) ;
    __CPROVER_assume(__thw[p][__cs_thread_index][__last_used_pos_of[p]] >= __memory_notvalid_of[p]) ;
    __CPROVER_assume(__thw[a_cs_arr_0][__cs_thread_index][__last_used_pos_of[a_cs_arr_0]] >= __memory_notvalid_of[a_cs_arr_0]) ;
    __CPROVER_assume(__thw[_cs_pointer_0][__cs_thread_index][__last_used_pos_of[_cs_pointer_0]] >= __memory_notvalid_of[_cs_pointer_0]) ;
    __CPROVER_assume(__thw[_cs_pointer_1][__cs_thread_index][__last_used_pos_of[_cs_pointer_1]] >= __memory_notvalid_of[_cs_pointer_1]) ;
    __CPROVER_assume(__thw[_cs_pointer_2][__cs_thread_index][__last_used_pos_of[_cs_pointer_2]] >= __memory_notvalid_of[_cs_pointer_2]) ;
    __CPROVER_assume(__thw[_cs_pointer_3][__cs_thread_index][__last_used_pos_of[_cs_pointer_3]] >= __memory_notvalid_of[_cs_pointer_3]) ;
    __CPROVER_assume(__thw[_cs_pointer_4][__cs_thread_index][__last_used_pos_of[_cs_pointer_4]] >= __memory_notvalid_of[_cs_pointer_4]) ;
    //if (__CS_ret==1) terminated[__cs_thread_index]=1;
    terminated[__cs_thread_index] = !__CS_ret;
    __CPROVER_assume(ticket_pos_terminated[__cs_thread_index] == ticket_pos);
    //sistemare thread_pos_terminated[__cs_thread_index]=ticket_pos;
    //ticket_pos_terminated[__cs_thread_index]=ticket_pos;
    __CS_ret = 0;
    __cs_thread_index = my_thread_index;
    ticket_pos   = my_ticket_pos;

    __last_used_pos_of[i] = my__last_used_pos_of[i];
    __last_used_pos_of[j] = my__last_used_pos_of[j];
    __last_used_pos_of[p1] = my__last_used_pos_of[p1];
    __last_used_pos_of[p] = my__last_used_pos_of[p];
    __last_used_pos_of[a_cs_arr_0] = my__last_used_pos_of[a_cs_arr_0];
    __last_used_pos_of[_cs_pointer_0] = my__last_used_pos_of[_cs_pointer_0];
    __last_used_pos_of[_cs_pointer_1] = my__last_used_pos_of[_cs_pointer_1];
    __last_used_pos_of[_cs_pointer_2] = my__last_used_pos_of[_cs_pointer_2];
    __last_used_pos_of[_cs_pointer_3] = my__last_used_pos_of[_cs_pointer_3];
    __last_used_pos_of[_cs_pointer_4] = my__last_used_pos_of[_cs_pointer_4];
    return __ret;
}

int __pthread_create_t2_0(__CS_pthread_t *thread_id, void *attr, void *arg) {
    int __ret = 0;
    __CS_typeT my_thread_index;
    __CS_typeM my_ticket_pos;

    __CS_typeM my__last_used_pos_of[NumVars];
    my__last_used_pos_of[i] = __last_used_pos_of[i];
    my__last_used_pos_of[j] = __last_used_pos_of[j];
    my__last_used_pos_of[p1] = __last_used_pos_of[p1];
    my__last_used_pos_of[p] = __last_used_pos_of[p];
    my__last_used_pos_of[a_cs_arr_0] = __last_used_pos_of[a_cs_arr_0];
    my__last_used_pos_of[_cs_pointer_0] = __last_used_pos_of[_cs_pointer_0];
    my__last_used_pos_of[_cs_pointer_1] = __last_used_pos_of[_cs_pointer_1];
    my__last_used_pos_of[_cs_pointer_2] = __last_used_pos_of[_cs_pointer_2];
    my__last_used_pos_of[_cs_pointer_3] = __last_used_pos_of[_cs_pointer_3];
    my__last_used_pos_of[_cs_pointer_4] = __last_used_pos_of[_cs_pointer_4];

    if (MaxNumThreads == ++number_threads) return 0;
    my_thread_index = __cs_thread_index;
    my_ticket_pos   = ticket_pos;
    __cs_thread_index = number_threads;
    n_treads_createad++;

    __CPROVER_assume(__thw[i][__cs_thread_index][0] > __last_used_pos_of[i]);
    __CPROVER_assume(__thw[j][__cs_thread_index][0] > __last_used_pos_of[j]);
    __CPROVER_assume(__thw[p1][__cs_thread_index][0] > __last_used_pos_of[p1]);
    __CPROVER_assume(__thw[p][__cs_thread_index][0] > __last_used_pos_of[p]);
    __CPROVER_assume(__thw[a_cs_arr_0][__cs_thread_index][0] > __last_used_pos_of[a_cs_arr_0]);
    __CPROVER_assume(__thw[_cs_pointer_0][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_0]);
    __CPROVER_assume(__thw[_cs_pointer_1][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_1]);
    __CPROVER_assume(__thw[_cs_pointer_2][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_2]);
    __CPROVER_assume(__thw[_cs_pointer_3][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_3]);
    __CPROVER_assume(__thw[_cs_pointer_4][__cs_thread_index][0] > __last_used_pos_of[_cs_pointer_4]);
    if (nondet_int()) { __CS_ret = 1; __ret = -1;}
    else { t2_0( arg); }
    *thread_id = __cs_thread_index;
    //__CPROVER_assume((thw[__cs_thread_index][thread_pos] >=memory_notvalid) );
    //__CPROVER_assume((thread_pos< memory_notvalid) && (thw[__cs_thread_index][thread_pos] >=memory_notvalid) );

    __CPROVER_assume(__thw[i][__cs_thread_index][__last_used_pos_of[i]] >= __memory_notvalid_of[i]) ;
    __CPROVER_assume(__thw[j][__cs_thread_index][__last_used_pos_of[j]] >= __memory_notvalid_of[j]) ;
    __CPROVER_assume(__thw[p1][__cs_thread_index][__last_used_pos_of[p1]] >= __memory_notvalid_of[p1]) ;
    __CPROVER_assume(__thw[p][__cs_thread_index][__last_used_pos_of[p]] >= __memory_notvalid_of[p]) ;
    __CPROVER_assume(__thw[a_cs_arr_0][__cs_thread_index][__last_used_pos_of[a_cs_arr_0]] >= __memory_notvalid_of[a_cs_arr_0]) ;
    __CPROVER_assume(__thw[_cs_pointer_0][__cs_thread_index][__last_used_pos_of[_cs_pointer_0]] >= __memory_notvalid_of[_cs_pointer_0]) ;
    __CPROVER_assume(__thw[_cs_pointer_1][__cs_thread_index][__last_used_pos_of[_cs_pointer_1]] >= __memory_notvalid_of[_cs_pointer_1]) ;
    __CPROVER_assume(__thw[_cs_pointer_2][__cs_thread_index][__last_used_pos_of[_cs_pointer_2]] >= __memory_notvalid_of[_cs_pointer_2]) ;
    __CPROVER_assume(__thw[_cs_pointer_3][__cs_thread_index][__last_used_pos_of[_cs_pointer_3]] >= __memory_notvalid_of[_cs_pointer_3]) ;
    __CPROVER_assume(__thw[_cs_pointer_4][__cs_thread_index][__last_used_pos_of[_cs_pointer_4]] >= __memory_notvalid_of[_cs_pointer_4]) ;
    //if (__CS_ret==1) terminated[__cs_thread_index]=1;
    terminated[__cs_thread_index] = !__CS_ret;
    __CPROVER_assume(ticket_pos_terminated[__cs_thread_index] == ticket_pos);
    //sistemare thread_pos_terminated[__cs_thread_index]=ticket_pos;
    //ticket_pos_terminated[__cs_thread_index]=ticket_pos;
    __CS_ret = 0;
    __cs_thread_index = my_thread_index;
    ticket_pos   = my_ticket_pos;

    __last_used_pos_of[i] = my__last_used_pos_of[i];
    __last_used_pos_of[j] = my__last_used_pos_of[j];
    __last_used_pos_of[p1] = my__last_used_pos_of[p1];
    __last_used_pos_of[p] = my__last_used_pos_of[p];
    __last_used_pos_of[a_cs_arr_0] = my__last_used_pos_of[a_cs_arr_0];
    __last_used_pos_of[_cs_pointer_0] = my__last_used_pos_of[_cs_pointer_0];
    __last_used_pos_of[_cs_pointer_1] = my__last_used_pos_of[_cs_pointer_1];
    __last_used_pos_of[_cs_pointer_2] = my__last_used_pos_of[_cs_pointer_2];
    __last_used_pos_of[_cs_pointer_3] = my__last_used_pos_of[_cs_pointer_3];
    __last_used_pos_of[_cs_pointer_4] = my__last_used_pos_of[_cs_pointer_4];
    return __ret;
}

void *main_thread(int __cs_param_main_argc, char *__cs_param_main_argv[])
{
    __cs_t __cs_local_main_id1;
    __cs_t __cs_local_main_id2;
    __pthread_create_t1_0(&__cs_local_main_id1,  0,  0);;
    __pthread_create_t2_0(&__cs_local_main_id2,  0,  0);;
    _Bool __cs_local_main___cs_tmp_if_cond_0;
    __cs_local_main___cs_tmp_if_cond_0 = (((Read(i) != 1) && (Read(i) != 3)) && (Read(i) != 4)) && (Read(i) != 5);
    if (__cs_local_main___cs_tmp_if_cond_0)
    {
        if (!(0)) {
            if (__CS_ret) return 0;     __CS_error = 1;  return 0;
        };
    }
    goto __exit_main;
__exit_main:
    ;
//__pthread_exit(NULL);;
}


void guessMU(void) {


//INITIALIZATION OF SHARED VARIABLES
    __value[i][0] = 1;
    __value[j][0] = 3;
//END INITIALIZATION OF SHARED VARIABLES

    __CS_typeM tmp_ticket_pos_terminated[MaxNumThreads];
    ticket_pos_terminated[0] = tmp_ticket_pos_terminated[0];
    __CPROVER_assume((tmp_ticket_pos_terminated[0] >= 0 ) && (tmp_ticket_pos_terminated[0] < MEM_SIZE + NumVars ));
    ticket_pos_terminated[1] = tmp_ticket_pos_terminated[1];
    __CPROVER_assume((tmp_ticket_pos_terminated[1] >= 0 ) && (tmp_ticket_pos_terminated[1] < MEM_SIZE + NumVars ));
    ticket_pos_terminated[2] = tmp_ticket_pos_terminated[2];
    __CPROVER_assume((tmp_ticket_pos_terminated[2] >= 0 ) && (tmp_ticket_pos_terminated[2] < MEM_SIZE + NumVars ));

    //Pointers
    intV address_tmp[NumVars];
    address[0] = address_tmp[0];
    address[1] = address_tmp[1];
    __CPROVER_assume( address[1] > address[0] );
    address[2] = address_tmp[2];
    __CPROVER_assume( address[2] > address[1] );
    address[3] = address_tmp[3];
    __CPROVER_assume( address[3] > address[2] );
    address[4] = address_tmp[4];
    __CPROVER_assume( address[4] > address[3] );
    address[5] = address_tmp[5];
    __CPROVER_assume( address[5] > address[4] );
    address[6] = address_tmp[6];
    __CPROVER_assume( address[6] > address[5] );
    address[7] = address_tmp[7];
    __CPROVER_assume( address[7] > address[6] );
    address[8] = address_tmp[8];
    __CPROVER_assume( address[8] > address[7] );
    address[9] = address_tmp[9];
    __CPROVER_assume( address[9] > address[8] );

    //Malloc
    intV mallocP_tmp[NumMalloc + 1];
    mallocP[0] = mallocP_tmp[0];
    mallocP[1] = mallocP_tmp[1];
    __CPROVER_assume( mallocP[1] > mallocP[0] );
    mallocP[2] = mallocP_tmp[2];
    __CPROVER_assume( mallocP[2] > mallocP[1] );
    mallocP[3] = mallocP_tmp[3];
    __CPROVER_assume( mallocP[3] > mallocP[2] );
    mallocP[4] = mallocP_tmp[4];
    __CPROVER_assume( mallocP[4] > mallocP[3] );

    int values_tmp[MEM_SIZE];
    __CS_typeM ticket_tmp[MEM_SIZE];
    _Bool visible_tmp[MEM_SIZE];
    intV __value_tmp[MEM_SIZE];
    __CS_typeM  nondet_memory[NumVars];
    __CS_typeM thread_tmp[MEM_SIZE];
    //for each scalar variable we guess the MU

    //for variable i: size: 4
    __ticket[i][0] = 0;
    __memory_notvalid_of[i] = nondet_memory[0];
    __CPROVER_assume( (__memory_notvalid_of[i] > 0) && (__memory_notvalid_of[i] <= i_SIZE) );

    __thread_id[i][1] = thread_tmp[0];
    __visible[i][1] = visible_tmp[0];
    __CPROVER_assume((__thread_id[i][1] >= 0) && (__thread_id[i][1] < GuessedNumThreads));
    __value[i][1] = __value_tmp[0];
    __ticket[i][1] = ticket_tmp[0];
    __CPROVER_assume( (__ticket[i][1] < MEM_SIZE)
                      && (__ticket[i][1] > __ticket[i][0])
                      && (!__used_ticket[__ticket[i][1]]) );
    __used_ticket[__ticket[i][1]] = 1;

    __thread_id[i][2] = thread_tmp[1];
    __visible[i][2] = visible_tmp[1];
    __CPROVER_assume((__thread_id[i][2] >= 0) && (__thread_id[i][2] < GuessedNumThreads));
    __value[i][2] = __value_tmp[1];
    __ticket[i][2] = ticket_tmp[1];
    __CPROVER_assume( (__ticket[i][2] < MEM_SIZE)
                      && (__ticket[i][2] > __ticket[i][1])
                      && (!__used_ticket[__ticket[i][2]]) );
    __used_ticket[__ticket[i][2]] = 1;

    __thread_id[i][3] = thread_tmp[2];
    __visible[i][3] = visible_tmp[2];
    __CPROVER_assume((__thread_id[i][3] >= 0) && (__thread_id[i][3] < GuessedNumThreads));
    __value[i][3] = __value_tmp[2];
    __ticket[i][3] = ticket_tmp[2];
    __CPROVER_assume( (__ticket[i][3] < MEM_SIZE)
                      && (__ticket[i][3] > __ticket[i][2])
                      && (!__used_ticket[__ticket[i][3]]) );
    __used_ticket[__ticket[i][3]] = 1;

    __thread_id[i][4] = thread_tmp[3];
    __visible[i][4] = visible_tmp[3];
    __CPROVER_assume((__thread_id[i][4] >= 0) && (__thread_id[i][4] < GuessedNumThreads));
    __value[i][4] = __value_tmp[3];
    __ticket[i][4] = ticket_tmp[3];
    __CPROVER_assume( (__ticket[i][4] < MEM_SIZE)
                      && (__ticket[i][4] > __ticket[i][3])
                      && (!__used_ticket[__ticket[i][4]]) );
    __used_ticket[__ticket[i][4]] = 1;

    //__ticket[i][MEM_SIZE_VARS]=ticket_tmp[4];
    __ticket[i][i_SIZE] = MEM_SIZE;
//  __ticket[i][0] = 0;


    //for variable j: size: 4
    __ticket[j][0] = 0;
    __memory_notvalid_of[j] = nondet_memory[1];
    __CPROVER_assume( (__memory_notvalid_of[j] > 0) && (__memory_notvalid_of[j] <= j_SIZE) );

    __thread_id[j][1] = thread_tmp[4];
    __visible[j][1] = visible_tmp[4];
    __CPROVER_assume((__thread_id[j][1] >= 0) && (__thread_id[j][1] < GuessedNumThreads));
    __value[j][1] = __value_tmp[4];
    __ticket[j][1] = ticket_tmp[4];
    __CPROVER_assume( (__ticket[j][1] < MEM_SIZE)
                      && (__ticket[j][1] > __ticket[j][0])
                      && (!__used_ticket[__ticket[j][1]]) );
    __used_ticket[__ticket[j][1]] = 1;

    __thread_id[j][2] = thread_tmp[5];
    __visible[j][2] = visible_tmp[5];
    __CPROVER_assume((__thread_id[j][2] >= 0) && (__thread_id[j][2] < GuessedNumThreads));
    __value[j][2] = __value_tmp[5];
    __ticket[j][2] = ticket_tmp[5];
    __CPROVER_assume( (__ticket[j][2] < MEM_SIZE)
                      && (__ticket[j][2] > __ticket[j][1])
                      && (!__used_ticket[__ticket[j][2]]) );
    __used_ticket[__ticket[j][2]] = 1;

    __thread_id[j][3] = thread_tmp[6];
    __visible[j][3] = visible_tmp[6];
    __CPROVER_assume((__thread_id[j][3] >= 0) && (__thread_id[j][3] < GuessedNumThreads));
    __value[j][3] = __value_tmp[6];
    __ticket[j][3] = ticket_tmp[6];
    __CPROVER_assume( (__ticket[j][3] < MEM_SIZE)
                      && (__ticket[j][3] > __ticket[j][2])
                      && (!__used_ticket[__ticket[j][3]]) );
    __used_ticket[__ticket[j][3]] = 1;

    __thread_id[j][4] = thread_tmp[7];
    __visible[j][4] = visible_tmp[7];
    __CPROVER_assume((__thread_id[j][4] >= 0) && (__thread_id[j][4] < GuessedNumThreads));
    __value[j][4] = __value_tmp[7];
    __ticket[j][4] = ticket_tmp[7];
    __CPROVER_assume( (__ticket[j][4] < MEM_SIZE)
                      && (__ticket[j][4] > __ticket[j][3])
                      && (!__used_ticket[__ticket[j][4]]) );
    __used_ticket[__ticket[j][4]] = 1;

    //__ticket[j][MEM_SIZE_VARS]=ticket_tmp[8];
    __ticket[j][j_SIZE] = MEM_SIZE;
//  __ticket[j][0] = 0;


    //for variable p1: size: 4
    __ticket[p1][0] = 0;
    __memory_notvalid_of[p1] = nondet_memory[2];
    __CPROVER_assume( (__memory_notvalid_of[p1] > 0) && (__memory_notvalid_of[p1] <= p1_SIZE) );

    __thread_id[p1][1] = thread_tmp[8];
    __visible[p1][1] = visible_tmp[8];
    __CPROVER_assume((__thread_id[p1][1] >= 0) && (__thread_id[p1][1] < GuessedNumThreads));
    __value[p1][1] = __value_tmp[8];
    __ticket[p1][1] = ticket_tmp[8];
    __CPROVER_assume( (__ticket[p1][1] < MEM_SIZE)
                      && (__ticket[p1][1] > __ticket[p1][0])
                      && (!__used_ticket[__ticket[p1][1]]) );
    __used_ticket[__ticket[p1][1]] = 1;

    __thread_id[p1][2] = thread_tmp[9];
    __visible[p1][2] = visible_tmp[9];
    __CPROVER_assume((__thread_id[p1][2] >= 0) && (__thread_id[p1][2] < GuessedNumThreads));
    __value[p1][2] = __value_tmp[9];
    __ticket[p1][2] = ticket_tmp[9];
    __CPROVER_assume( (__ticket[p1][2] < MEM_SIZE)
                      && (__ticket[p1][2] > __ticket[p1][1])
                      && (!__used_ticket[__ticket[p1][2]]) );
    __used_ticket[__ticket[p1][2]] = 1;

    __thread_id[p1][3] = thread_tmp[10];
    __visible[p1][3] = visible_tmp[10];
    __CPROVER_assume((__thread_id[p1][3] >= 0) && (__thread_id[p1][3] < GuessedNumThreads));
    __value[p1][3] = __value_tmp[10];
    __ticket[p1][3] = ticket_tmp[10];
    __CPROVER_assume( (__ticket[p1][3] < MEM_SIZE)
                      && (__ticket[p1][3] > __ticket[p1][2])
                      && (!__used_ticket[__ticket[p1][3]]) );
    __used_ticket[__ticket[p1][3]] = 1;

    __thread_id[p1][4] = thread_tmp[11];
    __visible[p1][4] = visible_tmp[11];
    __CPROVER_assume((__thread_id[p1][4] >= 0) && (__thread_id[p1][4] < GuessedNumThreads));
    __value[p1][4] = __value_tmp[11];
    __ticket[p1][4] = ticket_tmp[11];
    __CPROVER_assume( (__ticket[p1][4] < MEM_SIZE)
                      && (__ticket[p1][4] > __ticket[p1][3])
                      && (!__used_ticket[__ticket[p1][4]]) );
    __used_ticket[__ticket[p1][4]] = 1;

    //__ticket[p1][MEM_SIZE_VARS]=ticket_tmp[12];
    __ticket[p1][p1_SIZE] = MEM_SIZE;
//  __ticket[p1][0] = 0;


    //for variable p: size: 4
    __ticket[p][0] = 0;
    __memory_notvalid_of[p] = nondet_memory[3];
    __CPROVER_assume( (__memory_notvalid_of[p] > 0) && (__memory_notvalid_of[p] <= p_SIZE) );

    __thread_id[p][1] = thread_tmp[12];
    __visible[p][1] = visible_tmp[12];
    __CPROVER_assume((__thread_id[p][1] >= 0) && (__thread_id[p][1] < GuessedNumThreads));
    __value[p][1] = __value_tmp[12];
    __ticket[p][1] = ticket_tmp[12];
    __CPROVER_assume( (__ticket[p][1] < MEM_SIZE)
                      && (__ticket[p][1] > __ticket[p][0])
                      && (!__used_ticket[__ticket[p][1]]) );
    __used_ticket[__ticket[p][1]] = 1;

    __thread_id[p][2] = thread_tmp[13];
    __visible[p][2] = visible_tmp[13];
    __CPROVER_assume((__thread_id[p][2] >= 0) && (__thread_id[p][2] < GuessedNumThreads));
    __value[p][2] = __value_tmp[13];
    __ticket[p][2] = ticket_tmp[13];
    __CPROVER_assume( (__ticket[p][2] < MEM_SIZE)
                      && (__ticket[p][2] > __ticket[p][1])
                      && (!__used_ticket[__ticket[p][2]]) );
    __used_ticket[__ticket[p][2]] = 1;

    __thread_id[p][3] = thread_tmp[14];
    __visible[p][3] = visible_tmp[14];
    __CPROVER_assume((__thread_id[p][3] >= 0) && (__thread_id[p][3] < GuessedNumThreads));
    __value[p][3] = __value_tmp[14];
    __ticket[p][3] = ticket_tmp[14];
    __CPROVER_assume( (__ticket[p][3] < MEM_SIZE)
                      && (__ticket[p][3] > __ticket[p][2])
                      && (!__used_ticket[__ticket[p][3]]) );
    __used_ticket[__ticket[p][3]] = 1;

    __thread_id[p][4] = thread_tmp[15];
    __visible[p][4] = visible_tmp[15];
    __CPROVER_assume((__thread_id[p][4] >= 0) && (__thread_id[p][4] < GuessedNumThreads));
    __value[p][4] = __value_tmp[15];
    __ticket[p][4] = ticket_tmp[15];
    __CPROVER_assume( (__ticket[p][4] < MEM_SIZE)
                      && (__ticket[p][4] > __ticket[p][3])
                      && (!__used_ticket[__ticket[p][4]]) );
    __used_ticket[__ticket[p][4]] = 1;

    //__ticket[p][MEM_SIZE_VARS]=ticket_tmp[16];
    __ticket[p][p_SIZE] = MEM_SIZE;
//  __ticket[p][0] = 0;


    //for variable a_cs_arr_0: size: 4
    __ticket[a_cs_arr_0][0] = 0;
    __CPROVER_assume( address[a_cs_arr_0] == MyMalloc(a_cs_arr_0) );
    __memory_notvalid_of[a_cs_arr_0] = nondet_memory[4];
    __CPROVER_assume( (__memory_notvalid_of[a_cs_arr_0] > 0) && (__memory_notvalid_of[a_cs_arr_0] <= a_cs_arr_0_SIZE) );

    __thread_id[a_cs_arr_0][1] = thread_tmp[16];
    __visible[a_cs_arr_0][1] = visible_tmp[16];
    __CPROVER_assume((__thread_id[a_cs_arr_0][1] >= 0) && (__thread_id[a_cs_arr_0][1] < GuessedNumThreads));
    __value[a_cs_arr_0][1] = __value_tmp[16];
    __ticket[a_cs_arr_0][1] = ticket_tmp[16];
    __CPROVER_assume( (__ticket[a_cs_arr_0][1] < MEM_SIZE)
                      && (__ticket[a_cs_arr_0][1] > __ticket[a_cs_arr_0][0])
                      && (!__used_ticket[__ticket[a_cs_arr_0][1]]) );
    __used_ticket[__ticket[a_cs_arr_0][1]] = 1;

    __thread_id[a_cs_arr_0][2] = thread_tmp[17];
    __visible[a_cs_arr_0][2] = visible_tmp[17];
    __CPROVER_assume((__thread_id[a_cs_arr_0][2] >= 0) && (__thread_id[a_cs_arr_0][2] < GuessedNumThreads));
    __value[a_cs_arr_0][2] = __value_tmp[17];
    __ticket[a_cs_arr_0][2] = ticket_tmp[17];
    __CPROVER_assume( (__ticket[a_cs_arr_0][2] < MEM_SIZE)
                      && (__ticket[a_cs_arr_0][2] > __ticket[a_cs_arr_0][1])
                      && (!__used_ticket[__ticket[a_cs_arr_0][2]]) );
    __used_ticket[__ticket[a_cs_arr_0][2]] = 1;

    __thread_id[a_cs_arr_0][3] = thread_tmp[18];
    __visible[a_cs_arr_0][3] = visible_tmp[18];
    __CPROVER_assume((__thread_id[a_cs_arr_0][3] >= 0) && (__thread_id[a_cs_arr_0][3] < GuessedNumThreads));
    __value[a_cs_arr_0][3] = __value_tmp[18];
    __ticket[a_cs_arr_0][3] = ticket_tmp[18];
    __CPROVER_assume( (__ticket[a_cs_arr_0][3] < MEM_SIZE)
                      && (__ticket[a_cs_arr_0][3] > __ticket[a_cs_arr_0][2])
                      && (!__used_ticket[__ticket[a_cs_arr_0][3]]) );
    __used_ticket[__ticket[a_cs_arr_0][3]] = 1;

    __thread_id[a_cs_arr_0][4] = thread_tmp[19];
    __visible[a_cs_arr_0][4] = visible_tmp[19];
    __CPROVER_assume((__thread_id[a_cs_arr_0][4] >= 0) && (__thread_id[a_cs_arr_0][4] < GuessedNumThreads));
    __value[a_cs_arr_0][4] = __value_tmp[19];
    __ticket[a_cs_arr_0][4] = ticket_tmp[19];
    __CPROVER_assume( (__ticket[a_cs_arr_0][4] < MEM_SIZE)
                      && (__ticket[a_cs_arr_0][4] > __ticket[a_cs_arr_0][3])
                      && (!__used_ticket[__ticket[a_cs_arr_0][4]]) );
    __used_ticket[__ticket[a_cs_arr_0][4]] = 1;

    //__ticket[a_cs_arr_0][MEM_SIZE_VARS]=ticket_tmp[20];
    __ticket[a_cs_arr_0][a_cs_arr_0_SIZE] = MEM_SIZE;
//  __ticket[a_cs_arr_0][0] = 0;


    //for variable _cs_pointer_0: size: 4
    __ticket[_cs_pointer_0][0] = 0;
    __memory_notvalid_of[_cs_pointer_0] = nondet_memory[5];
    __CPROVER_assume( (__memory_notvalid_of[_cs_pointer_0] > 0) && (__memory_notvalid_of[_cs_pointer_0] <= _cs_pointer_0_SIZE) );

    __thread_id[_cs_pointer_0][1] = thread_tmp[20];
    __visible[_cs_pointer_0][1] = visible_tmp[20];
    __CPROVER_assume((__thread_id[_cs_pointer_0][1] >= 0) && (__thread_id[_cs_pointer_0][1] < GuessedNumThreads));
    __value[_cs_pointer_0][1] = __value_tmp[20];
    __ticket[_cs_pointer_0][1] = ticket_tmp[20];
    __CPROVER_assume( (__ticket[_cs_pointer_0][1] < MEM_SIZE)
                      && (__ticket[_cs_pointer_0][1] > __ticket[_cs_pointer_0][0])
                      && (!__used_ticket[__ticket[_cs_pointer_0][1]]) );
    __used_ticket[__ticket[_cs_pointer_0][1]] = 1;

    __thread_id[_cs_pointer_0][2] = thread_tmp[21];
    __visible[_cs_pointer_0][2] = visible_tmp[21];
    __CPROVER_assume((__thread_id[_cs_pointer_0][2] >= 0) && (__thread_id[_cs_pointer_0][2] < GuessedNumThreads));
    __value[_cs_pointer_0][2] = __value_tmp[21];
    __ticket[_cs_pointer_0][2] = ticket_tmp[21];
    __CPROVER_assume( (__ticket[_cs_pointer_0][2] < MEM_SIZE)
                      && (__ticket[_cs_pointer_0][2] > __ticket[_cs_pointer_0][1])
                      && (!__used_ticket[__ticket[_cs_pointer_0][2]]) );
    __used_ticket[__ticket[_cs_pointer_0][2]] = 1;

    __thread_id[_cs_pointer_0][3] = thread_tmp[22];
    __visible[_cs_pointer_0][3] = visible_tmp[22];
    __CPROVER_assume((__thread_id[_cs_pointer_0][3] >= 0) && (__thread_id[_cs_pointer_0][3] < GuessedNumThreads));
    __value[_cs_pointer_0][3] = __value_tmp[22];
    __ticket[_cs_pointer_0][3] = ticket_tmp[22];
    __CPROVER_assume( (__ticket[_cs_pointer_0][3] < MEM_SIZE)
                      && (__ticket[_cs_pointer_0][3] > __ticket[_cs_pointer_0][2])
                      && (!__used_ticket[__ticket[_cs_pointer_0][3]]) );
    __used_ticket[__ticket[_cs_pointer_0][3]] = 1;

    __thread_id[_cs_pointer_0][4] = thread_tmp[23];
    __visible[_cs_pointer_0][4] = visible_tmp[23];
    __CPROVER_assume((__thread_id[_cs_pointer_0][4] >= 0) && (__thread_id[_cs_pointer_0][4] < GuessedNumThreads));
    __value[_cs_pointer_0][4] = __value_tmp[23];
    __ticket[_cs_pointer_0][4] = ticket_tmp[23];
    __CPROVER_assume( (__ticket[_cs_pointer_0][4] < MEM_SIZE)
                      && (__ticket[_cs_pointer_0][4] > __ticket[_cs_pointer_0][3])
                      && (!__used_ticket[__ticket[_cs_pointer_0][4]]) );
    __used_ticket[__ticket[_cs_pointer_0][4]] = 1;

    //__ticket[_cs_pointer_0][MEM_SIZE_VARS]=ticket_tmp[24];
    __ticket[_cs_pointer_0][_cs_pointer_0_SIZE] = MEM_SIZE;
//  __ticket[_cs_pointer_0][0] = 0;


    //for variable _cs_pointer_1: size: 4
    __ticket[_cs_pointer_1][0] = 0;
    __memory_notvalid_of[_cs_pointer_1] = nondet_memory[6];
    __CPROVER_assume( (__memory_notvalid_of[_cs_pointer_1] > 0) && (__memory_notvalid_of[_cs_pointer_1] <= _cs_pointer_1_SIZE) );

    __thread_id[_cs_pointer_1][1] = thread_tmp[24];
    __visible[_cs_pointer_1][1] = visible_tmp[24];
    __CPROVER_assume((__thread_id[_cs_pointer_1][1] >= 0) && (__thread_id[_cs_pointer_1][1] < GuessedNumThreads));
    __value[_cs_pointer_1][1] = __value_tmp[24];
    __ticket[_cs_pointer_1][1] = ticket_tmp[24];
    __CPROVER_assume( (__ticket[_cs_pointer_1][1] < MEM_SIZE)
                      && (__ticket[_cs_pointer_1][1] > __ticket[_cs_pointer_1][0])
                      && (!__used_ticket[__ticket[_cs_pointer_1][1]]) );
    __used_ticket[__ticket[_cs_pointer_1][1]] = 1;

    __thread_id[_cs_pointer_1][2] = thread_tmp[25];
    __visible[_cs_pointer_1][2] = visible_tmp[25];
    __CPROVER_assume((__thread_id[_cs_pointer_1][2] >= 0) && (__thread_id[_cs_pointer_1][2] < GuessedNumThreads));
    __value[_cs_pointer_1][2] = __value_tmp[25];
    __ticket[_cs_pointer_1][2] = ticket_tmp[25];
    __CPROVER_assume( (__ticket[_cs_pointer_1][2] < MEM_SIZE)
                      && (__ticket[_cs_pointer_1][2] > __ticket[_cs_pointer_1][1])
                      && (!__used_ticket[__ticket[_cs_pointer_1][2]]) );
    __used_ticket[__ticket[_cs_pointer_1][2]] = 1;

    __thread_id[_cs_pointer_1][3] = thread_tmp[26];
    __visible[_cs_pointer_1][3] = visible_tmp[26];
    __CPROVER_assume((__thread_id[_cs_pointer_1][3] >= 0) && (__thread_id[_cs_pointer_1][3] < GuessedNumThreads));
    __value[_cs_pointer_1][3] = __value_tmp[26];
    __ticket[_cs_pointer_1][3] = ticket_tmp[26];
    __CPROVER_assume( (__ticket[_cs_pointer_1][3] < MEM_SIZE)
                      && (__ticket[_cs_pointer_1][3] > __ticket[_cs_pointer_1][2])
                      && (!__used_ticket[__ticket[_cs_pointer_1][3]]) );
    __used_ticket[__ticket[_cs_pointer_1][3]] = 1;

    __thread_id[_cs_pointer_1][4] = thread_tmp[27];
    __visible[_cs_pointer_1][4] = visible_tmp[27];
    __CPROVER_assume((__thread_id[_cs_pointer_1][4] >= 0) && (__thread_id[_cs_pointer_1][4] < GuessedNumThreads));
    __value[_cs_pointer_1][4] = __value_tmp[27];
    __ticket[_cs_pointer_1][4] = ticket_tmp[27];
    __CPROVER_assume( (__ticket[_cs_pointer_1][4] < MEM_SIZE)
                      && (__ticket[_cs_pointer_1][4] > __ticket[_cs_pointer_1][3])
                      && (!__used_ticket[__ticket[_cs_pointer_1][4]]) );
    __used_ticket[__ticket[_cs_pointer_1][4]] = 1;

    //__ticket[_cs_pointer_1][MEM_SIZE_VARS]=ticket_tmp[28];
    __ticket[_cs_pointer_1][_cs_pointer_1_SIZE] = MEM_SIZE;
//  __ticket[_cs_pointer_1][0] = 0;


    //for variable _cs_pointer_2: size: 4
    __ticket[_cs_pointer_2][0] = 0;
    __memory_notvalid_of[_cs_pointer_2] = nondet_memory[7];
    __CPROVER_assume( (__memory_notvalid_of[_cs_pointer_2] > 0) && (__memory_notvalid_of[_cs_pointer_2] <= _cs_pointer_2_SIZE) );

    __thread_id[_cs_pointer_2][1] = thread_tmp[28];
    __visible[_cs_pointer_2][1] = visible_tmp[28];
    __CPROVER_assume((__thread_id[_cs_pointer_2][1] >= 0) && (__thread_id[_cs_pointer_2][1] < GuessedNumThreads));
    __value[_cs_pointer_2][1] = __value_tmp[28];
    __ticket[_cs_pointer_2][1] = ticket_tmp[28];
    __CPROVER_assume( (__ticket[_cs_pointer_2][1] < MEM_SIZE)
                      && (__ticket[_cs_pointer_2][1] > __ticket[_cs_pointer_2][0])
                      && (!__used_ticket[__ticket[_cs_pointer_2][1]]) );
    __used_ticket[__ticket[_cs_pointer_2][1]] = 1;

    __thread_id[_cs_pointer_2][2] = thread_tmp[29];
    __visible[_cs_pointer_2][2] = visible_tmp[29];
    __CPROVER_assume((__thread_id[_cs_pointer_2][2] >= 0) && (__thread_id[_cs_pointer_2][2] < GuessedNumThreads));
    __value[_cs_pointer_2][2] = __value_tmp[29];
    __ticket[_cs_pointer_2][2] = ticket_tmp[29];
    __CPROVER_assume( (__ticket[_cs_pointer_2][2] < MEM_SIZE)
                      && (__ticket[_cs_pointer_2][2] > __ticket[_cs_pointer_2][1])
                      && (!__used_ticket[__ticket[_cs_pointer_2][2]]) );
    __used_ticket[__ticket[_cs_pointer_2][2]] = 1;

    __thread_id[_cs_pointer_2][3] = thread_tmp[30];
    __visible[_cs_pointer_2][3] = visible_tmp[30];
    __CPROVER_assume((__thread_id[_cs_pointer_2][3] >= 0) && (__thread_id[_cs_pointer_2][3] < GuessedNumThreads));
    __value[_cs_pointer_2][3] = __value_tmp[30];
    __ticket[_cs_pointer_2][3] = ticket_tmp[30];
    __CPROVER_assume( (__ticket[_cs_pointer_2][3] < MEM_SIZE)
                      && (__ticket[_cs_pointer_2][3] > __ticket[_cs_pointer_2][2])
                      && (!__used_ticket[__ticket[_cs_pointer_2][3]]) );
    __used_ticket[__ticket[_cs_pointer_2][3]] = 1;

    __thread_id[_cs_pointer_2][4] = thread_tmp[31];
    __visible[_cs_pointer_2][4] = visible_tmp[31];
    __CPROVER_assume((__thread_id[_cs_pointer_2][4] >= 0) && (__thread_id[_cs_pointer_2][4] < GuessedNumThreads));
    __value[_cs_pointer_2][4] = __value_tmp[31];
    __ticket[_cs_pointer_2][4] = ticket_tmp[31];
    __CPROVER_assume( (__ticket[_cs_pointer_2][4] < MEM_SIZE)
                      && (__ticket[_cs_pointer_2][4] > __ticket[_cs_pointer_2][3])
                      && (!__used_ticket[__ticket[_cs_pointer_2][4]]) );
    __used_ticket[__ticket[_cs_pointer_2][4]] = 1;

    //__ticket[_cs_pointer_2][MEM_SIZE_VARS]=ticket_tmp[32];
    __ticket[_cs_pointer_2][_cs_pointer_2_SIZE] = MEM_SIZE;
//  __ticket[_cs_pointer_2][0] = 0;


    //for variable _cs_pointer_3: size: 4
    __ticket[_cs_pointer_3][0] = 0;
    __memory_notvalid_of[_cs_pointer_3] = nondet_memory[8];
    __CPROVER_assume( (__memory_notvalid_of[_cs_pointer_3] > 0) && (__memory_notvalid_of[_cs_pointer_3] <= _cs_pointer_3_SIZE) );

    __thread_id[_cs_pointer_3][1] = thread_tmp[32];
    __visible[_cs_pointer_3][1] = visible_tmp[32];
    __CPROVER_assume((__thread_id[_cs_pointer_3][1] >= 0) && (__thread_id[_cs_pointer_3][1] < GuessedNumThreads));
    __value[_cs_pointer_3][1] = __value_tmp[32];
    __ticket[_cs_pointer_3][1] = ticket_tmp[32];
    __CPROVER_assume( (__ticket[_cs_pointer_3][1] < MEM_SIZE)
                      && (__ticket[_cs_pointer_3][1] > __ticket[_cs_pointer_3][0])
                      && (!__used_ticket[__ticket[_cs_pointer_3][1]]) );
    __used_ticket[__ticket[_cs_pointer_3][1]] = 1;

    __thread_id[_cs_pointer_3][2] = thread_tmp[33];
    __visible[_cs_pointer_3][2] = visible_tmp[33];
    __CPROVER_assume((__thread_id[_cs_pointer_3][2] >= 0) && (__thread_id[_cs_pointer_3][2] < GuessedNumThreads));
    __value[_cs_pointer_3][2] = __value_tmp[33];
    __ticket[_cs_pointer_3][2] = ticket_tmp[33];
    __CPROVER_assume( (__ticket[_cs_pointer_3][2] < MEM_SIZE)
                      && (__ticket[_cs_pointer_3][2] > __ticket[_cs_pointer_3][1])
                      && (!__used_ticket[__ticket[_cs_pointer_3][2]]) );
    __used_ticket[__ticket[_cs_pointer_3][2]] = 1;

    __thread_id[_cs_pointer_3][3] = thread_tmp[34];
    __visible[_cs_pointer_3][3] = visible_tmp[34];
    __CPROVER_assume((__thread_id[_cs_pointer_3][3] >= 0) && (__thread_id[_cs_pointer_3][3] < GuessedNumThreads));
    __value[_cs_pointer_3][3] = __value_tmp[34];
    __ticket[_cs_pointer_3][3] = ticket_tmp[34];
    __CPROVER_assume( (__ticket[_cs_pointer_3][3] < MEM_SIZE)
                      && (__ticket[_cs_pointer_3][3] > __ticket[_cs_pointer_3][2])
                      && (!__used_ticket[__ticket[_cs_pointer_3][3]]) );
    __used_ticket[__ticket[_cs_pointer_3][3]] = 1;

    __thread_id[_cs_pointer_3][4] = thread_tmp[35];
    __visible[_cs_pointer_3][4] = visible_tmp[35];
    __CPROVER_assume((__thread_id[_cs_pointer_3][4] >= 0) && (__thread_id[_cs_pointer_3][4] < GuessedNumThreads));
    __value[_cs_pointer_3][4] = __value_tmp[35];
    __ticket[_cs_pointer_3][4] = ticket_tmp[35];
    __CPROVER_assume( (__ticket[_cs_pointer_3][4] < MEM_SIZE)
                      && (__ticket[_cs_pointer_3][4] > __ticket[_cs_pointer_3][3])
                      && (!__used_ticket[__ticket[_cs_pointer_3][4]]) );
    __used_ticket[__ticket[_cs_pointer_3][4]] = 1;

    //__ticket[_cs_pointer_3][MEM_SIZE_VARS]=ticket_tmp[36];
    __ticket[_cs_pointer_3][_cs_pointer_3_SIZE] = MEM_SIZE;
//  __ticket[_cs_pointer_3][0] = 0;


    //for variable _cs_pointer_4: size: 4
    __ticket[_cs_pointer_4][0] = 0;
    __memory_notvalid_of[_cs_pointer_4] = nondet_memory[9];
    __CPROVER_assume( (__memory_notvalid_of[_cs_pointer_4] > 0) && (__memory_notvalid_of[_cs_pointer_4] <= _cs_pointer_4_SIZE) );

    __thread_id[_cs_pointer_4][1] = thread_tmp[36];
    __visible[_cs_pointer_4][1] = visible_tmp[36];
    __CPROVER_assume((__thread_id[_cs_pointer_4][1] >= 0) && (__thread_id[_cs_pointer_4][1] < GuessedNumThreads));
    __value[_cs_pointer_4][1] = __value_tmp[36];
    __ticket[_cs_pointer_4][1] = ticket_tmp[36];
    __CPROVER_assume( (__ticket[_cs_pointer_4][1] < MEM_SIZE)
                      && (__ticket[_cs_pointer_4][1] > __ticket[_cs_pointer_4][0])
                      && (!__used_ticket[__ticket[_cs_pointer_4][1]]) );
    __used_ticket[__ticket[_cs_pointer_4][1]] = 1;

    __thread_id[_cs_pointer_4][2] = thread_tmp[37];
    __visible[_cs_pointer_4][2] = visible_tmp[37];
    __CPROVER_assume((__thread_id[_cs_pointer_4][2] >= 0) && (__thread_id[_cs_pointer_4][2] < GuessedNumThreads));
    __value[_cs_pointer_4][2] = __value_tmp[37];
    __ticket[_cs_pointer_4][2] = ticket_tmp[37];
    __CPROVER_assume( (__ticket[_cs_pointer_4][2] < MEM_SIZE)
                      && (__ticket[_cs_pointer_4][2] > __ticket[_cs_pointer_4][1])
                      && (!__used_ticket[__ticket[_cs_pointer_4][2]]) );
    __used_ticket[__ticket[_cs_pointer_4][2]] = 1;

    __thread_id[_cs_pointer_4][3] = thread_tmp[38];
    __visible[_cs_pointer_4][3] = visible_tmp[38];
    __CPROVER_assume((__thread_id[_cs_pointer_4][3] >= 0) && (__thread_id[_cs_pointer_4][3] < GuessedNumThreads));
    __value[_cs_pointer_4][3] = __value_tmp[38];
    __ticket[_cs_pointer_4][3] = ticket_tmp[38];
    __CPROVER_assume( (__ticket[_cs_pointer_4][3] < MEM_SIZE)
                      && (__ticket[_cs_pointer_4][3] > __ticket[_cs_pointer_4][2])
                      && (!__used_ticket[__ticket[_cs_pointer_4][3]]) );
    __used_ticket[__ticket[_cs_pointer_4][3]] = 1;

    __thread_id[_cs_pointer_4][4] = thread_tmp[39];
    __visible[_cs_pointer_4][4] = visible_tmp[39];
    __CPROVER_assume((__thread_id[_cs_pointer_4][4] >= 0) && (__thread_id[_cs_pointer_4][4] < GuessedNumThreads));
    __value[_cs_pointer_4][4] = __value_tmp[39];
    __ticket[_cs_pointer_4][4] = ticket_tmp[39];
    __CPROVER_assume( (__ticket[_cs_pointer_4][4] < MEM_SIZE)
                      && (__ticket[_cs_pointer_4][4] > __ticket[_cs_pointer_4][3])
                      && (!__used_ticket[__ticket[_cs_pointer_4][4]]) );
    __used_ticket[__ticket[_cs_pointer_4][4]] = 1;

    //__ticket[_cs_pointer_4][MEM_SIZE_VARS]=ticket_tmp[40];
    __ticket[_cs_pointer_4][_cs_pointer_4_SIZE] = MEM_SIZE;
//  __ticket[_cs_pointer_4][0] = 0;

    int ticket_index = 40;
    /*for each scalar variable, for each thread:
    at each position we calculate
    what is the next write of that thread of that varable.
    */
    //for variable i
    __thw[i][0][4] = MEM_SIZE;
    __thw[i][1][4] = MEM_SIZE;
    __thw[i][2][4] = MEM_SIZE;
    __thw[i][0][3] =    (__thread_id[i][4] == 0) ? 4 : __thw[i][0][4] ;
    __thw[i][1][3] =    (__thread_id[i][4] == 1) ? 4 : __thw[i][1][4] ;
    __thw[i][2][3] =    (__thread_id[i][4] == 2) ? 4 : __thw[i][2][4] ;

    __thw[i][0][2] =    (__thread_id[i][3] == 0) ? 3 : __thw[i][0][3] ;
    __thw[i][1][2] =    (__thread_id[i][3] == 1) ? 3 : __thw[i][1][3] ;
    __thw[i][2][2] =    (__thread_id[i][3] == 2) ? 3 : __thw[i][2][3] ;

    __thw[i][0][1] =    (__thread_id[i][2] == 0) ? 2 : __thw[i][0][2] ;
    __thw[i][1][1] =    (__thread_id[i][2] == 1) ? 2 : __thw[i][1][2] ;
    __thw[i][2][1] =    (__thread_id[i][2] == 2) ? 2 : __thw[i][2][2] ;

    __thw[i][0][0] =    (__thread_id[i][1] == 0) ? 1 : __thw[i][0][1] ;
    __thw[i][1][0] =    (__thread_id[i][1] == 1) ? 1 : __thw[i][1][1] ;
    __thw[i][2][0] =    (__thread_id[i][1] == 2) ? 1 : __thw[i][2][1] ;

    //for variable j
    __thw[j][0][4] = MEM_SIZE;
    __thw[j][1][4] = MEM_SIZE;
    __thw[j][2][4] = MEM_SIZE;
    __thw[j][0][3] =    (__thread_id[j][4] == 0) ? 4 : __thw[j][0][4] ;
    __thw[j][1][3] =    (__thread_id[j][4] == 1) ? 4 : __thw[j][1][4] ;
    __thw[j][2][3] =    (__thread_id[j][4] == 2) ? 4 : __thw[j][2][4] ;

    __thw[j][0][2] =    (__thread_id[j][3] == 0) ? 3 : __thw[j][0][3] ;
    __thw[j][1][2] =    (__thread_id[j][3] == 1) ? 3 : __thw[j][1][3] ;
    __thw[j][2][2] =    (__thread_id[j][3] == 2) ? 3 : __thw[j][2][3] ;

    __thw[j][0][1] =    (__thread_id[j][2] == 0) ? 2 : __thw[j][0][2] ;
    __thw[j][1][1] =    (__thread_id[j][2] == 1) ? 2 : __thw[j][1][2] ;
    __thw[j][2][1] =    (__thread_id[j][2] == 2) ? 2 : __thw[j][2][2] ;

    __thw[j][0][0] =    (__thread_id[j][1] == 0) ? 1 : __thw[j][0][1] ;
    __thw[j][1][0] =    (__thread_id[j][1] == 1) ? 1 : __thw[j][1][1] ;
    __thw[j][2][0] =    (__thread_id[j][1] == 2) ? 1 : __thw[j][2][1] ;

    //for variable p1
    __thw[p1][0][4] = MEM_SIZE;
    __thw[p1][1][4] = MEM_SIZE;
    __thw[p1][2][4] = MEM_SIZE;
    __thw[p1][0][3] =    (__thread_id[p1][4] == 0) ? 4 : __thw[p1][0][4] ;
    __thw[p1][1][3] =    (__thread_id[p1][4] == 1) ? 4 : __thw[p1][1][4] ;
    __thw[p1][2][3] =    (__thread_id[p1][4] == 2) ? 4 : __thw[p1][2][4] ;

    __thw[p1][0][2] =    (__thread_id[p1][3] == 0) ? 3 : __thw[p1][0][3] ;
    __thw[p1][1][2] =    (__thread_id[p1][3] == 1) ? 3 : __thw[p1][1][3] ;
    __thw[p1][2][2] =    (__thread_id[p1][3] == 2) ? 3 : __thw[p1][2][3] ;

    __thw[p1][0][1] =    (__thread_id[p1][2] == 0) ? 2 : __thw[p1][0][2] ;
    __thw[p1][1][1] =    (__thread_id[p1][2] == 1) ? 2 : __thw[p1][1][2] ;
    __thw[p1][2][1] =    (__thread_id[p1][2] == 2) ? 2 : __thw[p1][2][2] ;

    __thw[p1][0][0] =    (__thread_id[p1][1] == 0) ? 1 : __thw[p1][0][1] ;
    __thw[p1][1][0] =    (__thread_id[p1][1] == 1) ? 1 : __thw[p1][1][1] ;
    __thw[p1][2][0] =    (__thread_id[p1][1] == 2) ? 1 : __thw[p1][2][1] ;

    //for variable p
    __thw[p][0][4] = MEM_SIZE;
    __thw[p][1][4] = MEM_SIZE;
    __thw[p][2][4] = MEM_SIZE;
    __thw[p][0][3] =    (__thread_id[p][4] == 0) ? 4 : __thw[p][0][4] ;
    __thw[p][1][3] =    (__thread_id[p][4] == 1) ? 4 : __thw[p][1][4] ;
    __thw[p][2][3] =    (__thread_id[p][4] == 2) ? 4 : __thw[p][2][4] ;

    __thw[p][0][2] =    (__thread_id[p][3] == 0) ? 3 : __thw[p][0][3] ;
    __thw[p][1][2] =    (__thread_id[p][3] == 1) ? 3 : __thw[p][1][3] ;
    __thw[p][2][2] =    (__thread_id[p][3] == 2) ? 3 : __thw[p][2][3] ;

    __thw[p][0][1] =    (__thread_id[p][2] == 0) ? 2 : __thw[p][0][2] ;
    __thw[p][1][1] =    (__thread_id[p][2] == 1) ? 2 : __thw[p][1][2] ;
    __thw[p][2][1] =    (__thread_id[p][2] == 2) ? 2 : __thw[p][2][2] ;

    __thw[p][0][0] =    (__thread_id[p][1] == 0) ? 1 : __thw[p][0][1] ;
    __thw[p][1][0] =    (__thread_id[p][1] == 1) ? 1 : __thw[p][1][1] ;
    __thw[p][2][0] =    (__thread_id[p][1] == 2) ? 1 : __thw[p][2][1] ;

    //for variable a_cs_arr_0
    __thw[a_cs_arr_0][0][4] = MEM_SIZE;
    __thw[a_cs_arr_0][1][4] = MEM_SIZE;
    __thw[a_cs_arr_0][2][4] = MEM_SIZE;
    __thw[a_cs_arr_0][0][3] =    (__thread_id[a_cs_arr_0][4] == 0) ? 4 : __thw[a_cs_arr_0][0][4] ;
    __thw[a_cs_arr_0][1][3] =    (__thread_id[a_cs_arr_0][4] == 1) ? 4 : __thw[a_cs_arr_0][1][4] ;
    __thw[a_cs_arr_0][2][3] =    (__thread_id[a_cs_arr_0][4] == 2) ? 4 : __thw[a_cs_arr_0][2][4] ;

    __thw[a_cs_arr_0][0][2] =    (__thread_id[a_cs_arr_0][3] == 0) ? 3 : __thw[a_cs_arr_0][0][3] ;
    __thw[a_cs_arr_0][1][2] =    (__thread_id[a_cs_arr_0][3] == 1) ? 3 : __thw[a_cs_arr_0][1][3] ;
    __thw[a_cs_arr_0][2][2] =    (__thread_id[a_cs_arr_0][3] == 2) ? 3 : __thw[a_cs_arr_0][2][3] ;

    __thw[a_cs_arr_0][0][1] =    (__thread_id[a_cs_arr_0][2] == 0) ? 2 : __thw[a_cs_arr_0][0][2] ;
    __thw[a_cs_arr_0][1][1] =    (__thread_id[a_cs_arr_0][2] == 1) ? 2 : __thw[a_cs_arr_0][1][2] ;
    __thw[a_cs_arr_0][2][1] =    (__thread_id[a_cs_arr_0][2] == 2) ? 2 : __thw[a_cs_arr_0][2][2] ;

    __thw[a_cs_arr_0][0][0] =    (__thread_id[a_cs_arr_0][1] == 0) ? 1 : __thw[a_cs_arr_0][0][1] ;
    __thw[a_cs_arr_0][1][0] =    (__thread_id[a_cs_arr_0][1] == 1) ? 1 : __thw[a_cs_arr_0][1][1] ;
    __thw[a_cs_arr_0][2][0] =    (__thread_id[a_cs_arr_0][1] == 2) ? 1 : __thw[a_cs_arr_0][2][1] ;

    //for variable _cs_pointer_0
    __thw[_cs_pointer_0][0][4] = MEM_SIZE;
    __thw[_cs_pointer_0][1][4] = MEM_SIZE;
    __thw[_cs_pointer_0][2][4] = MEM_SIZE;
    __thw[_cs_pointer_0][0][3] =    (__thread_id[_cs_pointer_0][4] == 0) ? 4 : __thw[_cs_pointer_0][0][4] ;
    __thw[_cs_pointer_0][1][3] =    (__thread_id[_cs_pointer_0][4] == 1) ? 4 : __thw[_cs_pointer_0][1][4] ;
    __thw[_cs_pointer_0][2][3] =    (__thread_id[_cs_pointer_0][4] == 2) ? 4 : __thw[_cs_pointer_0][2][4] ;

    __thw[_cs_pointer_0][0][2] =    (__thread_id[_cs_pointer_0][3] == 0) ? 3 : __thw[_cs_pointer_0][0][3] ;
    __thw[_cs_pointer_0][1][2] =    (__thread_id[_cs_pointer_0][3] == 1) ? 3 : __thw[_cs_pointer_0][1][3] ;
    __thw[_cs_pointer_0][2][2] =    (__thread_id[_cs_pointer_0][3] == 2) ? 3 : __thw[_cs_pointer_0][2][3] ;

    __thw[_cs_pointer_0][0][1] =    (__thread_id[_cs_pointer_0][2] == 0) ? 2 : __thw[_cs_pointer_0][0][2] ;
    __thw[_cs_pointer_0][1][1] =    (__thread_id[_cs_pointer_0][2] == 1) ? 2 : __thw[_cs_pointer_0][1][2] ;
    __thw[_cs_pointer_0][2][1] =    (__thread_id[_cs_pointer_0][2] == 2) ? 2 : __thw[_cs_pointer_0][2][2] ;

    __thw[_cs_pointer_0][0][0] =    (__thread_id[_cs_pointer_0][1] == 0) ? 1 : __thw[_cs_pointer_0][0][1] ;
    __thw[_cs_pointer_0][1][0] =    (__thread_id[_cs_pointer_0][1] == 1) ? 1 : __thw[_cs_pointer_0][1][1] ;
    __thw[_cs_pointer_0][2][0] =    (__thread_id[_cs_pointer_0][1] == 2) ? 1 : __thw[_cs_pointer_0][2][1] ;

    //for variable _cs_pointer_1
    __thw[_cs_pointer_1][0][4] = MEM_SIZE;
    __thw[_cs_pointer_1][1][4] = MEM_SIZE;
    __thw[_cs_pointer_1][2][4] = MEM_SIZE;
    __thw[_cs_pointer_1][0][3] =    (__thread_id[_cs_pointer_1][4] == 0) ? 4 : __thw[_cs_pointer_1][0][4] ;
    __thw[_cs_pointer_1][1][3] =    (__thread_id[_cs_pointer_1][4] == 1) ? 4 : __thw[_cs_pointer_1][1][4] ;
    __thw[_cs_pointer_1][2][3] =    (__thread_id[_cs_pointer_1][4] == 2) ? 4 : __thw[_cs_pointer_1][2][4] ;

    __thw[_cs_pointer_1][0][2] =    (__thread_id[_cs_pointer_1][3] == 0) ? 3 : __thw[_cs_pointer_1][0][3] ;
    __thw[_cs_pointer_1][1][2] =    (__thread_id[_cs_pointer_1][3] == 1) ? 3 : __thw[_cs_pointer_1][1][3] ;
    __thw[_cs_pointer_1][2][2] =    (__thread_id[_cs_pointer_1][3] == 2) ? 3 : __thw[_cs_pointer_1][2][3] ;

    __thw[_cs_pointer_1][0][1] =    (__thread_id[_cs_pointer_1][2] == 0) ? 2 : __thw[_cs_pointer_1][0][2] ;
    __thw[_cs_pointer_1][1][1] =    (__thread_id[_cs_pointer_1][2] == 1) ? 2 : __thw[_cs_pointer_1][1][2] ;
    __thw[_cs_pointer_1][2][1] =    (__thread_id[_cs_pointer_1][2] == 2) ? 2 : __thw[_cs_pointer_1][2][2] ;

    __thw[_cs_pointer_1][0][0] =    (__thread_id[_cs_pointer_1][1] == 0) ? 1 : __thw[_cs_pointer_1][0][1] ;
    __thw[_cs_pointer_1][1][0] =    (__thread_id[_cs_pointer_1][1] == 1) ? 1 : __thw[_cs_pointer_1][1][1] ;
    __thw[_cs_pointer_1][2][0] =    (__thread_id[_cs_pointer_1][1] == 2) ? 1 : __thw[_cs_pointer_1][2][1] ;

    //for variable _cs_pointer_2
    __thw[_cs_pointer_2][0][4] = MEM_SIZE;
    __thw[_cs_pointer_2][1][4] = MEM_SIZE;
    __thw[_cs_pointer_2][2][4] = MEM_SIZE;
    __thw[_cs_pointer_2][0][3] =    (__thread_id[_cs_pointer_2][4] == 0) ? 4 : __thw[_cs_pointer_2][0][4] ;
    __thw[_cs_pointer_2][1][3] =    (__thread_id[_cs_pointer_2][4] == 1) ? 4 : __thw[_cs_pointer_2][1][4] ;
    __thw[_cs_pointer_2][2][3] =    (__thread_id[_cs_pointer_2][4] == 2) ? 4 : __thw[_cs_pointer_2][2][4] ;

    __thw[_cs_pointer_2][0][2] =    (__thread_id[_cs_pointer_2][3] == 0) ? 3 : __thw[_cs_pointer_2][0][3] ;
    __thw[_cs_pointer_2][1][2] =    (__thread_id[_cs_pointer_2][3] == 1) ? 3 : __thw[_cs_pointer_2][1][3] ;
    __thw[_cs_pointer_2][2][2] =    (__thread_id[_cs_pointer_2][3] == 2) ? 3 : __thw[_cs_pointer_2][2][3] ;

    __thw[_cs_pointer_2][0][1] =    (__thread_id[_cs_pointer_2][2] == 0) ? 2 : __thw[_cs_pointer_2][0][2] ;
    __thw[_cs_pointer_2][1][1] =    (__thread_id[_cs_pointer_2][2] == 1) ? 2 : __thw[_cs_pointer_2][1][2] ;
    __thw[_cs_pointer_2][2][1] =    (__thread_id[_cs_pointer_2][2] == 2) ? 2 : __thw[_cs_pointer_2][2][2] ;

    __thw[_cs_pointer_2][0][0] =    (__thread_id[_cs_pointer_2][1] == 0) ? 1 : __thw[_cs_pointer_2][0][1] ;
    __thw[_cs_pointer_2][1][0] =    (__thread_id[_cs_pointer_2][1] == 1) ? 1 : __thw[_cs_pointer_2][1][1] ;
    __thw[_cs_pointer_2][2][0] =    (__thread_id[_cs_pointer_2][1] == 2) ? 1 : __thw[_cs_pointer_2][2][1] ;

    //for variable _cs_pointer_3
    __thw[_cs_pointer_3][0][4] = MEM_SIZE;
    __thw[_cs_pointer_3][1][4] = MEM_SIZE;
    __thw[_cs_pointer_3][2][4] = MEM_SIZE;
    __thw[_cs_pointer_3][0][3] =    (__thread_id[_cs_pointer_3][4] == 0) ? 4 : __thw[_cs_pointer_3][0][4] ;
    __thw[_cs_pointer_3][1][3] =    (__thread_id[_cs_pointer_3][4] == 1) ? 4 : __thw[_cs_pointer_3][1][4] ;
    __thw[_cs_pointer_3][2][3] =    (__thread_id[_cs_pointer_3][4] == 2) ? 4 : __thw[_cs_pointer_3][2][4] ;

    __thw[_cs_pointer_3][0][2] =    (__thread_id[_cs_pointer_3][3] == 0) ? 3 : __thw[_cs_pointer_3][0][3] ;
    __thw[_cs_pointer_3][1][2] =    (__thread_id[_cs_pointer_3][3] == 1) ? 3 : __thw[_cs_pointer_3][1][3] ;
    __thw[_cs_pointer_3][2][2] =    (__thread_id[_cs_pointer_3][3] == 2) ? 3 : __thw[_cs_pointer_3][2][3] ;

    __thw[_cs_pointer_3][0][1] =    (__thread_id[_cs_pointer_3][2] == 0) ? 2 : __thw[_cs_pointer_3][0][2] ;
    __thw[_cs_pointer_3][1][1] =    (__thread_id[_cs_pointer_3][2] == 1) ? 2 : __thw[_cs_pointer_3][1][2] ;
    __thw[_cs_pointer_3][2][1] =    (__thread_id[_cs_pointer_3][2] == 2) ? 2 : __thw[_cs_pointer_3][2][2] ;

    __thw[_cs_pointer_3][0][0] =    (__thread_id[_cs_pointer_3][1] == 0) ? 1 : __thw[_cs_pointer_3][0][1] ;
    __thw[_cs_pointer_3][1][0] =    (__thread_id[_cs_pointer_3][1] == 1) ? 1 : __thw[_cs_pointer_3][1][1] ;
    __thw[_cs_pointer_3][2][0] =    (__thread_id[_cs_pointer_3][1] == 2) ? 1 : __thw[_cs_pointer_3][2][1] ;

    //for variable _cs_pointer_4
    __thw[_cs_pointer_4][0][4] = MEM_SIZE;
    __thw[_cs_pointer_4][1][4] = MEM_SIZE;
    __thw[_cs_pointer_4][2][4] = MEM_SIZE;
    __thw[_cs_pointer_4][0][3] =    (__thread_id[_cs_pointer_4][4] == 0) ? 4 : __thw[_cs_pointer_4][0][4] ;
    __thw[_cs_pointer_4][1][3] =    (__thread_id[_cs_pointer_4][4] == 1) ? 4 : __thw[_cs_pointer_4][1][4] ;
    __thw[_cs_pointer_4][2][3] =    (__thread_id[_cs_pointer_4][4] == 2) ? 4 : __thw[_cs_pointer_4][2][4] ;

    __thw[_cs_pointer_4][0][2] =    (__thread_id[_cs_pointer_4][3] == 0) ? 3 : __thw[_cs_pointer_4][0][3] ;
    __thw[_cs_pointer_4][1][2] =    (__thread_id[_cs_pointer_4][3] == 1) ? 3 : __thw[_cs_pointer_4][1][3] ;
    __thw[_cs_pointer_4][2][2] =    (__thread_id[_cs_pointer_4][3] == 2) ? 3 : __thw[_cs_pointer_4][2][3] ;

    __thw[_cs_pointer_4][0][1] =    (__thread_id[_cs_pointer_4][2] == 0) ? 2 : __thw[_cs_pointer_4][0][2] ;
    __thw[_cs_pointer_4][1][1] =    (__thread_id[_cs_pointer_4][2] == 1) ? 2 : __thw[_cs_pointer_4][1][2] ;
    __thw[_cs_pointer_4][2][1] =    (__thread_id[_cs_pointer_4][2] == 2) ? 2 : __thw[_cs_pointer_4][2][2] ;

    __thw[_cs_pointer_4][0][0] =    (__thread_id[_cs_pointer_4][1] == 0) ? 1 : __thw[_cs_pointer_4][0][1] ;
    __thw[_cs_pointer_4][1][0] =    (__thread_id[_cs_pointer_4][1] == 1) ? 1 : __thw[_cs_pointer_4][1][1] ;
    __thw[_cs_pointer_4][2][0] =    (__thread_id[_cs_pointer_4][1] == 2) ? 1 : __thw[_cs_pointer_4][2][1] ;



}
int main(int argc, char **argv)
{
    GuessedNumThreads = nondet_int();
    __CPROVER_assume(GuessedNumThreads <= MaxNumThreads);
    guessMU();

//cseq: simulation of the threads main
    main_thread(argc, argv);

    __CPROVER_assume(__thw[i][__cs_thread_index][__last_used_pos_of[i]] >= __memory_notvalid_of[i]) ;
    __CPROVER_assume(__thw[j][__cs_thread_index][__last_used_pos_of[j]] >= __memory_notvalid_of[j]) ;
    __CPROVER_assume(__thw[p1][__cs_thread_index][__last_used_pos_of[p1]] >= __memory_notvalid_of[p1]) ;
    __CPROVER_assume(__thw[p][__cs_thread_index][__last_used_pos_of[p]] >= __memory_notvalid_of[p]) ;
    __CPROVER_assume(__thw[a_cs_arr_0][__cs_thread_index][__last_used_pos_of[a_cs_arr_0]] >= __memory_notvalid_of[a_cs_arr_0]) ;
    __CPROVER_assume(__thw[_cs_pointer_0][__cs_thread_index][__last_used_pos_of[_cs_pointer_0]] >= __memory_notvalid_of[_cs_pointer_0]) ;
    __CPROVER_assume(__thw[_cs_pointer_1][__cs_thread_index][__last_used_pos_of[_cs_pointer_1]] >= __memory_notvalid_of[_cs_pointer_1]) ;
    __CPROVER_assume(__thw[_cs_pointer_2][__cs_thread_index][__last_used_pos_of[_cs_pointer_2]] >= __memory_notvalid_of[_cs_pointer_2]) ;
    __CPROVER_assume(__thw[_cs_pointer_3][__cs_thread_index][__last_used_pos_of[_cs_pointer_3]] >= __memory_notvalid_of[_cs_pointer_3]) ;
    __CPROVER_assume(__thw[_cs_pointer_4][__cs_thread_index][__last_used_pos_of[_cs_pointer_4]] >= __memory_notvalid_of[_cs_pointer_4]) ;
    __CPROVER_assume(GuessedNumThreads == n_treads_createad);
//cseq: Error check
    assert(__CS_error != 1);
}