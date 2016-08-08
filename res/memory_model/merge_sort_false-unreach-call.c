#include <stdlib.h>

extern int __VERIFIER_nondet_int(void);

struct node {
    struct node     *next;
    int             value;
};

struct list {
    struct node     *slist;
    struct list     *next;
};

static void merge_single_node(struct node ***dst, /*INDEX 12*/
    struct node **data)  /*INDEX 13*/
{
    // pick up the current item and jump to the next one
    struct node *node = *data; // INDEX 14
    *data = node->next;
    node->next = NULL;

    // insert the item into dst and move cursor
    **dst = node;
    *dst = &node->next;
}

static void merge_pair(struct node **dst, /*INDEX 9*/
    struct node *sub1, /*INDEX 10*/
    struct node *sub2) /*INDEX 11*/
{
    // merge two sorted sub-lists into one
    while (sub1 || sub2) {
        if (!sub2 || (sub1 && sub1->value < sub2->value))
            merge_single_node(&dst, &sub1);
        else
            merge_single_node(&dst, &sub2);
    }
}

static struct list* seq_sort_core(struct list *data)  // INDEX 6
{
    struct list *dst = NULL;  // INDEX 7

    while (data) {
        struct list *next = data->next;  // INDEX 8
        if (!next) {
            // take any odd/even padding as it is
            data->next = dst;
            dst = data;
            break;
        }

        // take the current sub-list and the next one and merge them into one
        merge_pair(&data->slist, data->slist, next->slist);
        data->next = dst;
        dst = data;

        // free the just processed sub-list and jump to the next pair
        data = next->next;
        free(next);
    }

    return dst;
}

static void inspect_before(struct list *shape)  //  INDEX 5
{
    /* we should get a list of sub-lists of length exactly one */
    assert(shape);

    for (; shape->next; shape = shape->next) {
        assert(shape);
        assert(shape->next);
        assert(shape->slist);
        assert(shape->slist->next == NULL);
    }

    /* check the last node separately to make the exercising more fun */
    assert(shape);
    assert(shape->next == NULL);
    assert(shape->slist);
    assert(shape->slist->next == NULL);
}

static void inspect_after(struct list *shape)  // INDEX 15
{
    /* we should get exactly one node at the top level and one nested list */
    assert(shape);
    assert(shape->next == NULL);
    assert(shape->slist != NULL);

    /* the nested list should be zero terminated (iterator back by one node) */
    struct node *pos;  // INDEX 16
    for (pos = shape->slist; pos->next; pos = pos->next);
    assert(!pos->next);
}

#define FACTOR 1

int main()
{
    // Create the list
    struct list *data = NULL;   // INDEX 0
    while (__VERIFIER_nondet_int()) {
        struct node *node = malloc(FACTOR*sizeof(struct node)); // INDEX 1
        // if (!node)
        //     abort();

        node->next = node;
        node->value = __VERIFIER_nondet_int();

        struct list *item = malloc(FACTOR*sizeof(struct list));  // INDEX 2
        // if (!item)
        //     abort();

        item->slist = node;
        item->next = data;
        data = item;
    }

    if (!data)
        return EXIT_SUCCESS;

    inspect_before(data);

    // Sort the list
    //
    // do O(log N) iterations
    while (data->next)
    {
        data = seq_sort_core(data);
    }
    inspect_after(data);

    struct node *node = data->slist;  // INDEX 3
    free(data);

    while (node) {
        struct node *snext = node->next; // INDEX 4
        free(node);
        node = snext;
    }

    return EXIT_SUCCESS;
}
