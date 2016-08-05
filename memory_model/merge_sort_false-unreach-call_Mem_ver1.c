#include <stdlib.h>

extern int __VERIFIER_nondet_int(void);
#define assume __CPROVER_assume

#define INTERVAL 1000
#define TRACK_SIZE 31

// For the track variable
unsigned int TRACK_ADDRESS[TRACK_SIZE];
int TRACK_VALUE[TRACK_SIZE];
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
    assume((TRACK_ADDRESS[17] <= INTERVAL) & (TRACK_ADDRESS[17] > TRACK_ADDRESS[16]));
    TRACK_ADDRESS[18] = TRACK_ADDRESS_tmp[1];
    assume((TRACK_ADDRESS[18] <= INTERVAL) & (TRACK_ADDRESS[18] > TRACK_ADDRESS[17]));
    TRACK_ADDRESS[19] = TRACK_ADDRESS_tmp[2];
    assume((TRACK_ADDRESS[19] <= INTERVAL) & (TRACK_ADDRESS[19] > TRACK_ADDRESS[18]));
    TRACK_ADDRESS[20] = TRACK_ADDRESS_tmp[3];
    assume((TRACK_ADDRESS[20] <= INTERVAL) & (TRACK_ADDRESS[20] > TRACK_ADDRESS[19]));
    TRACK_ADDRESS[21] = TRACK_ADDRESS_tmp[4];
    assume((TRACK_ADDRESS[21] <= INTERVAL) & (TRACK_ADDRESS[21] > TRACK_ADDRESS[20]));
    TRACK_ADDRESS[22] = TRACK_ADDRESS_tmp[5];
    assume((TRACK_ADDRESS[22] <= INTERVAL) & (TRACK_ADDRESS[22] > TRACK_ADDRESS[21]));
    TRACK_ADDRESS[23] = TRACK_ADDRESS_tmp[6];
    assume((TRACK_ADDRESS[23] <= INTERVAL) & (TRACK_ADDRESS[23] > TRACK_ADDRESS[22]));
    TRACK_ADDRESS[24] = TRACK_ADDRESS_tmp[7];
    assume((TRACK_ADDRESS[24] <= INTERVAL) & (TRACK_ADDRESS[24] > TRACK_ADDRESS[23]));
    TRACK_ADDRESS[25] = TRACK_ADDRESS_tmp[8];
    assume((TRACK_ADDRESS[25] <= INTERVAL) & (TRACK_ADDRESS[25] > TRACK_ADDRESS[24]));
    TRACK_ADDRESS[26] = TRACK_ADDRESS_tmp[9];
    assume((TRACK_ADDRESS[26] <= INTERVAL) & (TRACK_ADDRESS[26] > TRACK_ADDRESS[25]));
    TRACK_ADDRESS[27] = TRACK_ADDRESS_tmp[10];
    assume((TRACK_ADDRESS[27] <= INTERVAL) & (TRACK_ADDRESS[27] > TRACK_ADDRESS[26]));
    TRACK_ADDRESS[28] = TRACK_ADDRESS_tmp[11];
    assume((TRACK_ADDRESS[28] <= INTERVAL) & (TRACK_ADDRESS[28] > TRACK_ADDRESS[27]));
    TRACK_ADDRESS[29] = TRACK_ADDRESS_tmp[12];
    assume((TRACK_ADDRESS[29] <= INTERVAL) & (TRACK_ADDRESS[29] > TRACK_ADDRESS[28]));
    TRACK_ADDRESS[30] = INTERVAL + 1;

}


unsigned int my_malloc(size_t size)
{
    unsigned int tmp = num_malloc;

    num_malloc += size + 1;

    return tmp + 1;
}


void my_free(unsigned int p)
{
    // TODO
}


int READ(int codevar)
{
    return TRACK_VALUE[codevar];
}

void WRITE(int codevar, int value)
{
    TRACK_VALUE[codevar] = value;
}

int READ_NORMAL(unsigned int address)
{
    // Find out if this belong to the heap first
    int pos;
    assume((pos >= 0) & (pos < TRACK_SIZE - 1) );
    // Find a position on the track to read
    assume(TRACK_ADDRESS[pos] == address);
    return TRACK_VALUE[pos];
}

// int READ_NORMAL(unsigned int address)
// {
//     // Find out if this belong to the heap first
//     int pos;

//     if (TRACK_ADDRESS[17] == address)
//     {
//         pos = 17;
//     }
//     else if (TRACK_ADDRESS[18] == address)
//     {
//         pos = 18;
//     }
//     else if (TRACK_ADDRESS[19] == address)
//     {
//         pos = 19;
//     }
//     else if (TRACK_ADDRESS[20] == address)
//     {
//         pos = 20;
//     }
//     else if (TRACK_ADDRESS[21] == address)
//     {
//         pos = 21;
//     }
//     else if (TRACK_ADDRESS[22] == address)
//     {
//         pos = 22;
//     }
//     else if (TRACK_ADDRESS[23] == address)
//     {
//         pos = 23;
//     }
//     else if (TRACK_ADDRESS[24] == address)
//     {
//         pos = 24;
//     }
//     else if (TRACK_ADDRESS[25] == address)
//     {
//         pos = 25;
//     }
//     else if (TRACK_ADDRESS[26] == address)
//     {
//         pos = 26;
//     }
//     else if (TRACK_ADDRESS[27] == address)
//     {
//         pos = 27;
//     }
//     else if (TRACK_ADDRESS[28] == address)
//     {
//         pos = 28;
//     }
//     else if (TRACK_ADDRESS[29] == address)
//     {
//         pos = 29;
//     }
//     else
//     {
//         assume(0);
//     }
//     return TRACK_VALUE[pos];
// }


void WRITE_NORMAL(unsigned int address, int value)
{
    int pos;
    assume((pos >= 0) && (pos < (TRACK_SIZE - 1)));
    assume((address >= TRACK_ADDRESS[pos]) & (address < TRACK_ADDRESS[pos + 1]));
    if (TRACK_ADDRESS[pos] == address)
    {
        TRACK_VALUE[pos] = value;
    }
    else
    {
        assume(0);
    }
}

// void WRITE_NORMAL(unsigned int address, int value)
// {
//     // Find out if this belong to the heap first
//     int pos;

//     if (TRACK_ADDRESS[17] == address)
//     {
//         pos = 17;
//     }
//     else if (TRACK_ADDRESS[18] == address)
//     {
//         pos = 18;
//     }
//     else if (TRACK_ADDRESS[19] == address)
//     {
//         pos = 19;
//     }
//     else if (TRACK_ADDRESS[20] == address)
//     {
//         pos = 20;
//     }
//     else if (TRACK_ADDRESS[21] == address)
//     {
//         pos = 21;
//     }
//     else if (TRACK_ADDRESS[22] == address)
//     {
//         pos = 22;
//     }
//     else if (TRACK_ADDRESS[23] == address)
//     {
//         pos = 23;
//     }
//     else if (TRACK_ADDRESS[24] == address)
//     {
//         pos = 24;
//     }
//     else if (TRACK_ADDRESS[25] == address)
//     {
//         pos = 25;
//     }
//     else if (TRACK_ADDRESS[26] == address)
//     {
//         pos = 26;
//     }
//     else if (TRACK_ADDRESS[27] == address)
//     {
//         pos = 27;
//     }
//     else if (TRACK_ADDRESS[28] == address)
//     {
//         pos = 28;
//     }
//     else if (TRACK_ADDRESS[29] == address)
//     {
//         pos = 29;
//     }
//     else
//     {
//         assume(0);
//     }

//     TRACK_VALUE[pos] = value;
// }


/////////////////////////////



// struct node {
//     struct node     *next;
//     int             value;
// };

// A node contains two elements
// a pointer to next structure
// a value


// struct list {
//     struct node     *slist;
//     struct list     *next;
// };

// A list contains two elements
// A pointer to a node
// A pointer to another list

// static void merge_single_node(struct node ***dst, struct node **data)
static void merge_single_node(unsigned int dst, unsigned int data)
{
    TRACK_VALUE[12] = dst;
    TRACK_VALUE[13] = data;

    // pick up the current item and jump to the next one
    // node : INDEX 14
    // struct node *node = *data;
    TRACK_VALUE[14] = READ_NORMAL(TRACK_VALUE[13]);
    // *data = node->next;
    WRITE_NORMAL(TRACK_VALUE[13], READ_NORMAL(TRACK_VALUE[14]));
    // node->next = NULL;
    WRITE_NORMAL(TRACK_VALUE[14], 0);

    // insert the item into dst and move cursor
    // **dst = node;
    WRITE_NORMAL(READ_NORMAL(TRACK_VALUE[12]), TRACK_VALUE[14]);

    // *dst = &node->next;
    WRITE_NORMAL(TRACK_VALUE[12], TRACK_VALUE[14]);
}

// static void merge_pair(struct node **dst, struct node *sub1, struct node *sub2)
static void merge_pair(unsigned int dst, unsigned int sub1, unsigned int sub2)
{
    TRACK_VALUE[9] = dst;
    TRACK_VALUE[10] = sub1;
    TRACK_VALUE[11] = sub2;
    // merge two sorted sub-lists into one
    // while (sub1 || sub2) {
    while ((TRACK_VALUE[10] || TRACK_VALUE[11])) {
        // if (!sub2 || (sub1 && sub1->value < sub2->value))
        if (!TRACK_VALUE[11] || (TRACK_VALUE[10] && READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1)))
        {
            // merge_single_node(&dst, &sub1);
            merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
        }
        else
        {
            // merge_single_node(&dst, &sub2);
            merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
        }
    }
}

// static struct list* seq_sort_core(struct list *data)
static unsigned int seq_sort_core(unsigned int data)
{
    TRACK_VALUE[6] = data;

    // dst : INDEX 7
    // struct list *dst = NULL;
    TRACK_VALUE[7] = 0;

    // while (data) {
    while (TRACK_VALUE[6]) {
        // next : INDEX 8
        // struct list *next = data->next;
        TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
        // if (!next) {
        if (!TRACK_VALUE[8]) {
            // take any odd/even padding as it is
            // data->next = dst;
            WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
            // dst = data;
            TRACK_VALUE[7] = TRACK_VALUE[6];
            break;
        }

        // take the current sub-list and the next one and merge them into one
        // merge_pair(&data->slist, data->slist, next->slist);
        merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));

        // data->next = dst;
        WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);

        // dst = data;
        TRACK_VALUE[7] = TRACK_VALUE[6];

        // free the just processed sub-list and jump to the next pair
        // data = next->next;
        TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
        // free(next);
        my_free(TRACK_VALUE[8]);
    }

    // return dst;
    return TRACK_VALUE[7];
}

// static void inspect_before(struct list *shape)
static void inspect_before(unsigned int shape)   // INDEX 5
{
    // Parameter passing
    TRACK_VALUE[5] = shape;

    /* we should get a list of sub-lists of length exactly one */
    assert(TRACK_VALUE[5]);

    // for (; shape->next; shape = shape->next) {
    for (; READ_NORMAL(TRACK_VALUE[5] + 1); TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1))
    {
        assert(TRACK_VALUE[5]);
        // assert(shape->next);
        assert(READ_NORMAL(TRACK_VALUE[5] + 1));
        // assert(shape->slist);
        assert(READ_NORMAL(TRACK_VALUE[5]));
        // assert(shape->slist->next == NULL);
        assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
    }

    /* check the last node separately to make the exercising more fun */
    assert(TRACK_VALUE[5]);
    // assert(shape->next == NULL);
    assert(READ_NORMAL(TRACK_VALUE[5] + 1) == 0);
    // assert(shape->slist);
    assert(READ_NORMAL(TRACK_VALUE[5]));
    // assert(shape->slist->next == NULL);
    assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
}

// static void inspect_after(struct list *shape)
static void inspect_after(unsigned int shape)
{
    TRACK_VALUE[15] = shape;
    /* we should get exactly one node at the top level and one nested list */
    // assert(shape);
    assert(TRACK_VALUE[15]);

    // assert(shape->next == NULL);
    assert(READ_NORMAL(TRACK_VALUE[15] + 1) == 0);

    // assert(shape->slist != NULL);
    assert(READ_NORMAL(TRACK_VALUE[15]) != 0);

    /* the nested list should be zero terminated (iterator back by one node) */
    // struct node *pos;
    // for (pos = shape->slist; pos->next; pos = pos->next);
    for (TRACK_VALUE[16] = READ_NORMAL(TRACK_VALUE[15]); READ_NORMAL(TRACK_VALUE[16]); TRACK_VALUE[16] = READ_NORMAL(TRACK_VALUE[16]));
    {
        // assert(!pos->next);
        assert(!READ_NORMAL(TRACK_VALUE[16]));
    }
}

#define FACTOR 1

int main()
{
    init_MEM();

    // data : INDEX 0
    // struct list *data = NULL;   // data = 0 here, just a position when it comes to static variables
    TRACK_VALUE[0] = 0;

    while (__VERIFIER_nondet_int()) {
        // node : INDEX 1
        // struct node *node = malloc(sizeof *node);
        TRACK_VALUE[1] = my_malloc(FACTOR * 2); // size of node is 2

        // if (!node)
        // if (TRACK_VALUE[1] == 0)
        // {
        //     abort();
        // }

        // node->next = node;
        WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);

        // node->value = __VERIFIER_nondet_int();
        WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());

        // item : INDEX 2
        // struct list *item = malloc(sizeof *item);
        TRACK_VALUE[2] = my_malloc(FACTOR * 2);

        // if (!item)
        // if (TRACK_VALUE[2] == 0)
        // {
        //     abort();
        // }

        // item->slist = node;
        WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);

        // item->next = data;
        WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);

        // data = item;
        TRACK_VALUE[0] = TRACK_VALUE[2];
    }

    // // if (!data)
    // if (TRACK_VALUE[0] == 0)
    // {
    //     // return EXIT_SUCCESS;
    //     return 0;
    // }

    // inspect_before(data);
    inspect_before(TRACK_VALUE[0]);

    // do O(log N) iterations
    // while (data->next)
    while (READ_NORMAL(TRACK_VALUE[0] + 1))
    {
        // data = seq_sort_core(data);
        TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
    }

    // inspect_after(data);
    inspect_after(TRACK_VALUE[0]);

    // node : INDEX 3
    // struct node *node = data->slist;
    TRACK_VALUE[3] = READ_NORMAL(TRACK_VALUE[0]);
    // free(data);
    my_free(TRACK_VALUE[0]);

    // while (node) {
    while (TRACK_VALUE[3]) {
        // snext : INDEX 4
        // struct node *snext = node->next;
        TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
        // free(node);
        my_free(TRACK_VALUE[4]);
        // node = snext;
        TRACK_VALUE[3] = TRACK_VALUE[4];
    }

    // return EXIT_SUCCESS;
    return 0;
}
