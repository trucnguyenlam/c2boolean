// Taken from list_true-unreach-call.c of SV-COMP Heap Manipulation Category

extern int __VERIFIER_nondet_int();
/*
 * Simple example: build a list with only 1s, then 2s and finally
 * on 3 (arbitrary length); afterwards, go through it and check
 * if the the list does have the correct form, and in particular
 * finishes by a 3.
 */
#include <stdlib.h>

#define CONST 1


typedef struct node {
  int h;
  struct node *n;
} *List;

int main() {
  /* Build a list of the form 1->...->1->2->....->2->3 */
  List a = (List) malloc(CONST*sizeof(struct node));
  List t;
  List p = a;
  while (__VERIFIER_nondet_int()) {
    p->h = 1;
    t = (List) malloc(CONST*sizeof(struct node));

    p->n = t;

    p = p->n;
  }
  while (__VERIFIER_nondet_int()) {
    p->h = 2;
    t = (List) malloc(CONST*sizeof(struct node));
    p->n = t;
    p = p->n;
  }
  p->h = 3;

  /* Check it */
  p = a;
  while (p->h == 1)
    p = p->n;
  while (p->h == 2)
    p = p->n;

  if(p->h == 3)
    assert(0);

  return 0;
}
