// Taken from list_true-unreach-call.c of SV-COMP Heap Manipulation Category

extern int __VERIFIER_nondet_int();
/*
 * Simple example: build a list with only 1s, then 2s and finally
 * on 3 (arbitrary length); afterwards, go through it and check
 * if the the list does have the correct form, and in particular
 * finishes by a 3.
 */
#include <stdlib.h>


#define INTERVAL 10000000
#define TRACK_SIZE 31
#define HEAP_SIZE 31

// For the heap
unsigned int HEAP[HEAP_SIZE];
_Bool HEAP_ALLOCATED[HEAP_SIZE];
unsigned int HEAP_POINTER_SIZE[HEAP_SIZE];

// For the track variable
unsigned int TRACK_ADDRESS[TRACK_SIZE];
int TRACK_VALUE[TRACK_SIZE];

// A node is a structure of two variables
// one is a value
// another is a pointer

void init_MEM(void)
{

  // For the STATIC memory
  unsigned int TRACK_ADDRESS_tmp[57];

  TRACK_ADDRESS[0] = 0;    // For variable a
  TRACK_ADDRESS[1] = 1;    // For variable t
  TRACK_ADDRESS[2] = 2;    // For variable p

  TRACK_ADDRESS[3] = TRACK_ADDRESS_tmp[0];
  __CPROVER_assume((TRACK_ADDRESS[3] <= INTERVAL) & (TRACK_ADDRESS[3] > TRACK_ADDRESS[2]));
  TRACK_ADDRESS[4] = TRACK_ADDRESS_tmp[1];
  __CPROVER_assume((TRACK_ADDRESS[4] <= INTERVAL) & (TRACK_ADDRESS[4] > TRACK_ADDRESS[3]));
  TRACK_ADDRESS[5] = TRACK_ADDRESS_tmp[2];
  __CPROVER_assume((TRACK_ADDRESS[5] <= INTERVAL) & (TRACK_ADDRESS[5] > TRACK_ADDRESS[4]));
  TRACK_ADDRESS[6] = TRACK_ADDRESS_tmp[3];
  __CPROVER_assume((TRACK_ADDRESS[6] <= INTERVAL) & (TRACK_ADDRESS[6] > TRACK_ADDRESS[5]));
  TRACK_ADDRESS[7] = TRACK_ADDRESS_tmp[4];
  __CPROVER_assume((TRACK_ADDRESS[7] <= INTERVAL) & (TRACK_ADDRESS[7] > TRACK_ADDRESS[6]));
  TRACK_ADDRESS[8] = TRACK_ADDRESS_tmp[5];
  __CPROVER_assume((TRACK_ADDRESS[8] <= INTERVAL) & (TRACK_ADDRESS[8] > TRACK_ADDRESS[7]));
  TRACK_ADDRESS[9] = TRACK_ADDRESS_tmp[6];
  __CPROVER_assume((TRACK_ADDRESS[9] <= INTERVAL) & (TRACK_ADDRESS[9] > TRACK_ADDRESS[8]));
  TRACK_ADDRESS[10] = TRACK_ADDRESS_tmp[7];
  __CPROVER_assume((TRACK_ADDRESS[10] <= INTERVAL) & (TRACK_ADDRESS[10] > TRACK_ADDRESS[9]));
  TRACK_ADDRESS[11] = TRACK_ADDRESS_tmp[8];
  __CPROVER_assume((TRACK_ADDRESS[11] <= INTERVAL) & (TRACK_ADDRESS[11] > TRACK_ADDRESS[10]));
  TRACK_ADDRESS[12] = TRACK_ADDRESS_tmp[9];
  __CPROVER_assume((TRACK_ADDRESS[12] <= INTERVAL) & (TRACK_ADDRESS[12] > TRACK_ADDRESS[11]));
  TRACK_ADDRESS[13] = TRACK_ADDRESS_tmp[10];
  __CPROVER_assume((TRACK_ADDRESS[13] <= INTERVAL) & (TRACK_ADDRESS[13] > TRACK_ADDRESS[12]));
  TRACK_ADDRESS[14] = TRACK_ADDRESS_tmp[11];
  __CPROVER_assume((TRACK_ADDRESS[14] <= INTERVAL) & (TRACK_ADDRESS[14] > TRACK_ADDRESS[13]));
  TRACK_ADDRESS[15] = TRACK_ADDRESS_tmp[12];
  __CPROVER_assume((TRACK_ADDRESS[15] <= INTERVAL) & (TRACK_ADDRESS[15] > TRACK_ADDRESS[14]));
  TRACK_ADDRESS[16] = TRACK_ADDRESS_tmp[13];
  __CPROVER_assume((TRACK_ADDRESS[16] <= INTERVAL) & (TRACK_ADDRESS[16] > TRACK_ADDRESS[15]));
  TRACK_ADDRESS[17] = TRACK_ADDRESS_tmp[14];
  __CPROVER_assume((TRACK_ADDRESS[17] <= INTERVAL) & (TRACK_ADDRESS[17] > TRACK_ADDRESS[16]));
  TRACK_ADDRESS[18] = TRACK_ADDRESS_tmp[15];
  __CPROVER_assume((TRACK_ADDRESS[18] <= INTERVAL) & (TRACK_ADDRESS[18] > TRACK_ADDRESS[17]));
  TRACK_ADDRESS[19] = TRACK_ADDRESS_tmp[16];
  __CPROVER_assume((TRACK_ADDRESS[19] <= INTERVAL) & (TRACK_ADDRESS[19] > TRACK_ADDRESS[18]));
  TRACK_ADDRESS[20] = TRACK_ADDRESS_tmp[17];
  __CPROVER_assume((TRACK_ADDRESS[20] <= INTERVAL) & (TRACK_ADDRESS[20] > TRACK_ADDRESS[19]));
  TRACK_ADDRESS[21] = TRACK_ADDRESS_tmp[18];
  __CPROVER_assume((TRACK_ADDRESS[21] <= INTERVAL) & (TRACK_ADDRESS[21] > TRACK_ADDRESS[20]));
  TRACK_ADDRESS[22] = TRACK_ADDRESS_tmp[19];
  __CPROVER_assume((TRACK_ADDRESS[22] <= INTERVAL) & (TRACK_ADDRESS[22] > TRACK_ADDRESS[21]));
  TRACK_ADDRESS[23] = TRACK_ADDRESS_tmp[20];
  __CPROVER_assume((TRACK_ADDRESS[23] <= INTERVAL) & (TRACK_ADDRESS[23] > TRACK_ADDRESS[22]));
  TRACK_ADDRESS[24] = TRACK_ADDRESS_tmp[21];
  __CPROVER_assume((TRACK_ADDRESS[24] <= INTERVAL) & (TRACK_ADDRESS[24] > TRACK_ADDRESS[23]));
  TRACK_ADDRESS[25] = TRACK_ADDRESS_tmp[22];
  __CPROVER_assume((TRACK_ADDRESS[25] <= INTERVAL) & (TRACK_ADDRESS[25] > TRACK_ADDRESS[24]));
  TRACK_ADDRESS[26] = TRACK_ADDRESS_tmp[23];
  __CPROVER_assume((TRACK_ADDRESS[26] <= INTERVAL) & (TRACK_ADDRESS[26] > TRACK_ADDRESS[25]));
  TRACK_ADDRESS[27] = TRACK_ADDRESS_tmp[24];
  __CPROVER_assume((TRACK_ADDRESS[27] <= INTERVAL) & (TRACK_ADDRESS[27] > TRACK_ADDRESS[26]));
  TRACK_ADDRESS[28] = TRACK_ADDRESS_tmp[25];
  __CPROVER_assume((TRACK_ADDRESS[28] <= INTERVAL) & (TRACK_ADDRESS[28] > TRACK_ADDRESS[27]));
  TRACK_ADDRESS[29] = TRACK_ADDRESS_tmp[26];
  __CPROVER_assume((TRACK_ADDRESS[29] <= INTERVAL) & (TRACK_ADDRESS[29] > TRACK_ADDRESS[28]));
  // TRACK_ADDRESS[30] = TRACK_ADDRESS_tmp[27];
  // __CPROVER_assume((TRACK_ADDRESS[30] <= INTERVAL) & (TRACK_ADDRESS[30] > TRACK_ADDRESS[29]));
  // TRACK_ADDRESS[31] = TRACK_ADDRESS_tmp[28];
  // __CPROVER_assume((TRACK_ADDRESS[31] <= INTERVAL) & (TRACK_ADDRESS[31] > TRACK_ADDRESS[30]));
  // TRACK_ADDRESS[32] = TRACK_ADDRESS_tmp[29];
  // __CPROVER_assume((TRACK_ADDRESS[32] <= INTERVAL) & (TRACK_ADDRESS[32] > TRACK_ADDRESS[31]));
  // TRACK_ADDRESS[33] = TRACK_ADDRESS_tmp[30];
  // __CPROVER_assume((TRACK_ADDRESS[33] <= INTERVAL) & (TRACK_ADDRESS[33] > TRACK_ADDRESS[32]));
  // TRACK_ADDRESS[34] = TRACK_ADDRESS_tmp[31];
  // __CPROVER_assume((TRACK_ADDRESS[34] <= INTERVAL) & (TRACK_ADDRESS[34] > TRACK_ADDRESS[33]));
  // TRACK_ADDRESS[35] = TRACK_ADDRESS_tmp[32];
  // __CPROVER_assume((TRACK_ADDRESS[35] <= INTERVAL) & (TRACK_ADDRESS[35] > TRACK_ADDRESS[34]));
  // TRACK_ADDRESS[36] = TRACK_ADDRESS_tmp[33];
  // __CPROVER_assume((TRACK_ADDRESS[36] <= INTERVAL) & (TRACK_ADDRESS[36] > TRACK_ADDRESS[35]));
  // TRACK_ADDRESS[37] = TRACK_ADDRESS_tmp[34];
  // __CPROVER_assume((TRACK_ADDRESS[37] <= INTERVAL) & (TRACK_ADDRESS[37] > TRACK_ADDRESS[36]));
  // TRACK_ADDRESS[38] = TRACK_ADDRESS_tmp[35];
  // __CPROVER_assume((TRACK_ADDRESS[38] <= INTERVAL) & (TRACK_ADDRESS[38] > TRACK_ADDRESS[37]));
  // TRACK_ADDRESS[39] = TRACK_ADDRESS_tmp[36];
  // __CPROVER_assume((TRACK_ADDRESS[39] <= INTERVAL) & (TRACK_ADDRESS[39] > TRACK_ADDRESS[38]));
  // TRACK_ADDRESS[40] = TRACK_ADDRESS_tmp[37];
  // __CPROVER_assume((TRACK_ADDRESS[40] <= INTERVAL) & (TRACK_ADDRESS[40] > TRACK_ADDRESS[39]));
  // TRACK_ADDRESS[41] = TRACK_ADDRESS_tmp[38];
  // __CPROVER_assume((TRACK_ADDRESS[41] <= INTERVAL) & (TRACK_ADDRESS[41] > TRACK_ADDRESS[40]));
  // TRACK_ADDRESS[42] = TRACK_ADDRESS_tmp[39];
  // __CPROVER_assume((TRACK_ADDRESS[42] <= INTERVAL) & (TRACK_ADDRESS[42] > TRACK_ADDRESS[41]));
  // TRACK_ADDRESS[43] = TRACK_ADDRESS_tmp[40];
  // __CPROVER_assume((TRACK_ADDRESS[43] <= INTERVAL) & (TRACK_ADDRESS[43] > TRACK_ADDRESS[42]));
  // TRACK_ADDRESS[44] = TRACK_ADDRESS_tmp[41];
  // __CPROVER_assume((TRACK_ADDRESS[44] <= INTERVAL) & (TRACK_ADDRESS[44] > TRACK_ADDRESS[43]));
  // TRACK_ADDRESS[45] = TRACK_ADDRESS_tmp[42];
  // __CPROVER_assume((TRACK_ADDRESS[45] <= INTERVAL) & (TRACK_ADDRESS[45] > TRACK_ADDRESS[44]));
  // TRACK_ADDRESS[46] = TRACK_ADDRESS_tmp[43];
  // __CPROVER_assume((TRACK_ADDRESS[46] <= INTERVAL) & (TRACK_ADDRESS[46] > TRACK_ADDRESS[45]));
  // TRACK_ADDRESS[47] = TRACK_ADDRESS_tmp[44];
  // __CPROVER_assume((TRACK_ADDRESS[47] <= INTERVAL) & (TRACK_ADDRESS[47] > TRACK_ADDRESS[46]));
  // TRACK_ADDRESS[48] = TRACK_ADDRESS_tmp[45];
  // __CPROVER_assume((TRACK_ADDRESS[48] <= INTERVAL) & (TRACK_ADDRESS[48] > TRACK_ADDRESS[47]));
  // TRACK_ADDRESS[49] = TRACK_ADDRESS_tmp[46];
  // __CPROVER_assume((TRACK_ADDRESS[49] <= INTERVAL) & (TRACK_ADDRESS[49] > TRACK_ADDRESS[48]));
  // TRACK_ADDRESS[50] = TRACK_ADDRESS_tmp[47];
  // __CPROVER_assume((TRACK_ADDRESS[50] <= INTERVAL) & (TRACK_ADDRESS[50] > TRACK_ADDRESS[49]));
  // TRACK_ADDRESS[51] = TRACK_ADDRESS_tmp[48];
  // __CPROVER_assume((TRACK_ADDRESS[51] <= INTERVAL) & (TRACK_ADDRESS[51] > TRACK_ADDRESS[50]));
  // TRACK_ADDRESS[52] = TRACK_ADDRESS_tmp[49];
  // __CPROVER_assume((TRACK_ADDRESS[52] <= INTERVAL) & (TRACK_ADDRESS[52] > TRACK_ADDRESS[51]));
  // TRACK_ADDRESS[53] = TRACK_ADDRESS_tmp[50];
  // __CPROVER_assume((TRACK_ADDRESS[53] <= INTERVAL) & (TRACK_ADDRESS[53] > TRACK_ADDRESS[52]));
  // TRACK_ADDRESS[54] = TRACK_ADDRESS_tmp[51];
  // __CPROVER_assume((TRACK_ADDRESS[54] <= INTERVAL) & (TRACK_ADDRESS[54] > TRACK_ADDRESS[53]));
  // TRACK_ADDRESS[55] = TRACK_ADDRESS_tmp[52];
  // __CPROVER_assume((TRACK_ADDRESS[55] <= INTERVAL) & (TRACK_ADDRESS[55] > TRACK_ADDRESS[54]));
  // TRACK_ADDRESS[56] = TRACK_ADDRESS_tmp[53];
  // __CPROVER_assume((TRACK_ADDRESS[56] <= INTERVAL) & (TRACK_ADDRESS[56] > TRACK_ADDRESS[55]));
  // TRACK_ADDRESS[57] = TRACK_ADDRESS_tmp[54];
  // __CPROVER_assume((TRACK_ADDRESS[57] <= INTERVAL) & (TRACK_ADDRESS[57] > TRACK_ADDRESS[56]));
  // TRACK_ADDRESS[58] = TRACK_ADDRESS_tmp[55];
  // __CPROVER_assume((TRACK_ADDRESS[58] <= INTERVAL) & (TRACK_ADDRESS[58] > TRACK_ADDRESS[57]));
  // TRACK_ADDRESS[59] = TRACK_ADDRESS_tmp[56];
  // __CPROVER_assume((TRACK_ADDRESS[59] <= INTERVAL) & (TRACK_ADDRESS[59] > TRACK_ADDRESS[58]));
  // TRACK_ADDRESS[60] = INTERVAL + 1;
  TRACK_ADDRESS[30] = INTERVAL + 1;

  // For the HEAP (MUST be larger than the last element in static address TRACK_ADDRESS[8])
  unsigned int HEAP_tmp[HEAP_SIZE - 1];

  HEAP[0] = HEAP_tmp[0];
  __CPROVER_assume((HEAP[0] <= INTERVAL) & (HEAP[0] > TRACK_ADDRESS[2]));
  HEAP[1] = HEAP_tmp[1];
  __CPROVER_assume((HEAP[1] <= INTERVAL) & (HEAP[1] > HEAP[0]));
  HEAP[2] = HEAP_tmp[2];
  __CPROVER_assume((HEAP[2] <= INTERVAL) & (HEAP[2] > HEAP[1]));
  HEAP[3] = HEAP_tmp[3];
  __CPROVER_assume((HEAP[3] <= INTERVAL) & (HEAP[3] > HEAP[2]));
  HEAP[4] = HEAP_tmp[4];
  __CPROVER_assume((HEAP[4] <= INTERVAL) & (HEAP[4] > HEAP[3]));
  HEAP[5] = HEAP_tmp[5];
  __CPROVER_assume((HEAP[5] <= INTERVAL) & (HEAP[5] > HEAP[4]));
  HEAP[6] = HEAP_tmp[6];
  __CPROVER_assume((HEAP[6] <= INTERVAL) & (HEAP[6] > HEAP[5]));
  HEAP[7] = HEAP_tmp[7];
  __CPROVER_assume((HEAP[7] <= INTERVAL) & (HEAP[7] > HEAP[6]));
  HEAP[8] = HEAP_tmp[8];
  __CPROVER_assume((HEAP[8] <= INTERVAL) & (HEAP[8] > HEAP[7]));
  HEAP[9] = HEAP_tmp[9];
  __CPROVER_assume((HEAP[9] <= INTERVAL) & (HEAP[9] > HEAP[8]));
  HEAP[10] = HEAP_tmp[10];
  __CPROVER_assume((HEAP[10] <= INTERVAL) & (HEAP[10] > HEAP[9]));
  HEAP[11] = HEAP_tmp[11];
  __CPROVER_assume((HEAP[11] <= INTERVAL) & (HEAP[11] > HEAP[10]));
  HEAP[12] = HEAP_tmp[12];
  __CPROVER_assume((HEAP[12] <= INTERVAL) & (HEAP[12] > HEAP[11]));
  HEAP[13] = HEAP_tmp[13];
  __CPROVER_assume((HEAP[13] <= INTERVAL) & (HEAP[13] > HEAP[12]));
  HEAP[14] = HEAP_tmp[14];
  __CPROVER_assume((HEAP[14] <= INTERVAL) & (HEAP[14] > HEAP[13]));
  HEAP[15] = HEAP_tmp[15];
  __CPROVER_assume((HEAP[15] <= INTERVAL) & (HEAP[15] > HEAP[14]));
  HEAP[16] = HEAP_tmp[16];
  __CPROVER_assume((HEAP[16] <= INTERVAL) & (HEAP[16] > HEAP[15]));
  HEAP[17] = HEAP_tmp[17];
  __CPROVER_assume((HEAP[17] <= INTERVAL) & (HEAP[17] > HEAP[16]));
  HEAP[18] = HEAP_tmp[18];
  __CPROVER_assume((HEAP[18] <= INTERVAL) & (HEAP[18] > HEAP[17]));
  HEAP[19] = HEAP_tmp[19];
  __CPROVER_assume((HEAP[19] <= INTERVAL) & (HEAP[19] > HEAP[18]));
  HEAP[20] = HEAP_tmp[20];
  __CPROVER_assume((HEAP[20] <= INTERVAL) & (HEAP[20] > HEAP[19]));
  HEAP[21] = HEAP_tmp[21];
  __CPROVER_assume((HEAP[21] <= INTERVAL) & (HEAP[21] > HEAP[20]));
  HEAP[22] = HEAP_tmp[22];
  __CPROVER_assume((HEAP[22] <= INTERVAL) & (HEAP[22] > HEAP[21]));
  HEAP[23] = HEAP_tmp[23];
  __CPROVER_assume((HEAP[23] <= INTERVAL) & (HEAP[23] > HEAP[22]));
  HEAP[24] = HEAP_tmp[24];
  __CPROVER_assume((HEAP[24] <= INTERVAL) & (HEAP[24] > HEAP[23]));
  HEAP[25] = HEAP_tmp[25];
  __CPROVER_assume((HEAP[25] <= INTERVAL) & (HEAP[25] > HEAP[24]));
  HEAP[26] = HEAP_tmp[26];
  __CPROVER_assume((HEAP[26] <= INTERVAL) & (HEAP[26] > HEAP[25]));
  HEAP[27] = HEAP_tmp[27];
  __CPROVER_assume((HEAP[27] <= INTERVAL) & (HEAP[27] > HEAP[26]));
  HEAP[28] = HEAP_tmp[28];
  __CPROVER_assume((HEAP[28] <= INTERVAL) & (HEAP[28] > HEAP[27]));
  HEAP[29] = HEAP_tmp[29];
  __CPROVER_assume((HEAP[29] <= INTERVAL) & (HEAP[29] > HEAP[28]));
  // Enclosing of HEAP
  HEAP[30] = INTERVAL + 1;

}


unsigned int my_malloc(int size)
{

  int pos;
  __CPROVER_assume((pos >= 0) & (pos < HEAP_SIZE - 1));

  __CPROVER_assume(!HEAP_ALLOCATED[pos]
                   && (HEAP[pos] + size < HEAP[pos + 1])
                  );
  // Allocated the memory
  HEAP_ALLOCATED[pos] = 1;
  HEAP_POINTER_SIZE[pos] = size;

  return HEAP[pos];
}

int READ(int codevar)
{
  return TRACK_VALUE[codevar];
}

void WRITE(int codevar, int value)
{
  TRACK_VALUE[codevar] = value;
}

int READ_ADDR(unsigned int address)
{
  // Find out if this belong to the heap first
  int pos;
  __CPROVER_assume((pos >= 0) & (pos < TRACK_SIZE - 1) );
  // Find a position on the track to read
  __CPROVER_assume(TRACK_ADDRESS[pos] == address);
  return TRACK_VALUE[pos];
}


void WRITE_ADDR(unsigned int address, int value)
{
  int pos;
  __CPROVER_assume((pos >= 0) & (pos < TRACK_SIZE - 1) );

  __CPROVER_assume((address >= TRACK_ADDRESS[pos]) & (address < TRACK_ADDRESS[pos + 1]));
  if (TRACK_ADDRESS[pos] == address)
  {
    TRACK_VALUE[pos] = value;
  }
}

#define CONST 50000

// typedef struct node {
//   int h;     // index 0
//   struct node *n;   // index 1
// } *List;

// List is the type of pointer to (node)


// There are three variables to track here
// a      - 0
// t      - 1
// p      - 2



int main() {

  // Important
  init_MEM();

  /* Build a list of the form 1->...->1->2->....->2->3 */
  // List a =  malloc(CONST*sizeof(struct node));
  TRACK_VALUE[0] = my_malloc(CONST * 2); // 3 is the size of a node
  //
  // List t;
  TRACK_VALUE[1] = __VERIFIER_nondet_int();

  // List p = a;
  TRACK_VALUE[2] = TRACK_VALUE[0];

  while (__VERIFIER_nondet_int()) {
    // p->h = 1;
    WRITE_ADDR(TRACK_VALUE[2], 1);

    // t = (List) malloc(CONST*sizeof(struct node));
    TRACK_VALUE[1] = my_malloc(CONST * 2);

    // p->n = t;
    WRITE_ADDR(TRACK_VALUE[2] + 1, TRACK_VALUE[1]);
    // p = p->n;
    TRACK_VALUE[2] = READ_ADDR(TRACK_VALUE[2] + 1);
  }


  while (__VERIFIER_nondet_int()) {
    // p->h = 2;
    WRITE_ADDR(TRACK_VALUE[2], 2);

    // t = (List) malloc(CONST*sizeof(struct node));
    TRACK_VALUE[1] = my_malloc(CONST * 2);

    // p->n = t;
    WRITE_ADDR(TRACK_VALUE[2] + 1, TRACK_VALUE[1]);
    // p = p->n;
    TRACK_VALUE[2] = READ_ADDR(TRACK_VALUE[2] + 1);
  }
  // p->h = 3;
  WRITE_ADDR(TRACK_VALUE[2], 3);

  /* Check it */
  // p = a;
  TRACK_VALUE[2] = TRACK_VALUE[0];

  // while (p->h == 1)
  while (READ_ADDR(TRACK_VALUE[2]) == 1)
  {
    // p = p->n;
    TRACK_VALUE[2] = READ_ADDR(TRACK_VALUE[2] + 1);
  }
  // while (p->h == 2)
  while (READ_ADDR(TRACK_VALUE[2]) == 2)
  {
    // p = p->n;
    TRACK_VALUE[2] = READ_ADDR(TRACK_VALUE[2] + 1);
  }
  // if (p->h == 3)
  if (READ_ADDR(TRACK_VALUE[2]) != 3)
  {
    assert(0);
  }

  return 0;
}
