# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 1 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/weirdo.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/weirdo.c"


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

int aaa[10];
int *bbb;

int ccc = 10;

STACK stack;
struct StackItem item;

int main(int argc, char const *argv[])
{
# 71 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/weirdo.c"
    stack.array = malloc(3*sizeof(struct StackItem));

    stack.count = 1;
    bbb = &stack.head;

    stack.array[1].next = 0;
# 101 "/home/trucnguyenlam/Development/cseq/regression/pointertranslation/weirdo.c"
}
