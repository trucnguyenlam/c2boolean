/*
 * This source code is licensed under the GPL license, see License.GPLv2.txt
 * for details.  The list implementation is taken from the Linux kernel.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

extern int __VERIFIER_nondet_int(void);

struct list_head {
    struct list_head *next, *prev;
};

struct node {
    int value;
    struct list_head linkage;
    struct list_head nested;
};

struct list_head gl_list = { &(gl_list), &(gl_list) };

static void inspect(const struct list_head *head)
{

    assert(head);
    assert(head->next != head);
    assert(head->prev != head);


    head = head->prev;
    assert(head);
    assert(head->next != head);
    assert(head->prev != head);


    const struct node *node = ((struct node *)((char *)(head) - (unsigned long)(&((struct node *)0)->linkage)));
    assert(node);
    assert(node->nested.next == &node->nested);
    assert(node->nested.prev == &node->nested);
    assert(node->nested.next != &node->linkage);
    assert(node->nested.prev != &node->linkage);


    assert(node != (const struct node *)head);
    assert(node != (const struct node *)&node->linkage);
    assert(node == (const struct node *)&node->value);
    assert(head == node->linkage.next->prev);
    assert(head == node->linkage.prev->next);


    for (head = head->next; &node->linkage != head; head = head->next);
    assert(((struct node *)((char *)(head) - (unsigned long)(&((struct node *)0)->linkage))) == node);
}

static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

static void gl_insert(int value)
{
    struct node *node = malloc(sizeof * node);
    if (!node)
        abort();

    node->value = value;
    list_add(&node->linkage, &gl_list);
    do { (&node->nested)->next = (&node->nested); (&node->nested)->prev = (&node->nested); } while (0);
}

static void gl_read()
{
    do {
        gl_insert(__VERIFIER_nondet_int());
    }
    while (__VERIFIER_nondet_int());
}

static void gl_destroy()
{
    struct list_head *next;
    while (&gl_list != (next = gl_list.next)) {
        gl_list.next = next->next;
        free(((struct node *)((char *)(next) - (unsigned long)(&((struct node *)0)->linkage))));
    }
}

static int val_from_node(struct list_head *head) {
    struct node *entry = ((struct node *)((char *)(head) - (unsigned long)(&((struct node *)0)->linkage)));
    return entry->value;
}

static bool gl_sort_pass()
{
    bool any_change = false;

    struct list_head *pos0 = gl_list.next;
    struct list_head *pos1;
    while (&gl_list != (pos1 = pos0->next)) {
        const int val0 = val_from_node(pos0);
        const int val1 = val_from_node(pos1);
        if (val0 <= val1) {

            pos0 = pos1;
            continue;
        }

        any_change = true;
        list_move(pos0, pos1);
    }

    return any_change;
}

static void gl_sort()
{
    while (gl_sort_pass())
        ;
}

int main()
{
    gl_read();
    inspect(&gl_list);

    gl_sort();
    inspect(&gl_list);

    gl_destroy();

    return 0;
}
