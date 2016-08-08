#include <stdio.h>
#include <stdlib.h>

int v1;

int __VERIFIER_nondet_int(void);

int *data;
int len;

void *thr_0(void)
{
    data[0] = 0;
    assert(data[0] == 0);
}
void main_thread(void)
{
    if (v1 <= 0) goto tmain_1;
    len = __VERIFIER_nondet_int();
    __CPROVER_assume(len > 0);
    data = malloc(len * sizeof(int));
tmain_1:
    return;
}
int main(void)
{
    v1 = __VERIFIER_nondet_int();
    __CPROVER_assume( (v1 > 0 ) & (v1 <= 2));
    main_thread();
    thr_0();
    return 0;
}