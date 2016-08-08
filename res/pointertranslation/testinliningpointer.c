// #include "stdio.h"
struct small
{
    int ss;
    int s[10];
};

struct larger
{
    int two;
    int three[10];
    struct small one;
};

typedef struct A
{
    int b;
	int a[10];
    int *f;
} _A;

struct StackItem
{
    int value;
    int next;
};

typedef struct STACK
{
    struct StackItem * array;
    int head;
    int count;
} STACK;

// int aaa[10];
int *bbb;

// int ccc = 10;

STACK stack;
//
struct StackItem item;

int __VERIFIER_atomic_f(int *ll, struct StackItem * kkk)
{
	struct StackItem * nnn;

	kkk->value = *ll;
	nnn = kkk;
	return nnn->next;
}

int main(int argc, char const *argv[])
{

	*bbb = 10;

	*bbb = __VERIFIER_atomic_f(bbb, &item);


    // int fff[10][10];

    // fff[2][2] = 0;

    // sizeof(struct small);
    // sizeof(struct larger);

 //    struct larger blah;

 //    blah.one.ss = 10;

 //    _A c[10];
	// _A zz;

 //    ((c[1]).a)[2] = 10;
 //    (c[1]).f = 10;
 //    zz.a[2] = 10;

    // _A * z = malloc(sizeof(_A));

    stack.array = malloc(3*sizeof(struct StackItem));

    // stack.count = 1;
    // bbb = &stack.head;

    // stack.array[1].next = 0;
    // stack.array[1].value = 0;

 //    int d = 10, *e, **y;

 //    e = malloc(sizeof(int));

 //    y = malloc(sizeof(int *));

    // c[2].a[2] = 10;

 //    c[2].b = d++;

 //    c[d].b = *e;

 //    (*e) = 2;

    // &c[d].b += f(c[d].b, c[*e].a[2]);

    // (&c[d])->b = 2;

	// *c[d].f = *e;
	// printf("%d\n", c[2].a[2]);

	// return 0;
}