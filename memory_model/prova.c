extern void __VERIFIER_error() __attribute__ ((__noreturn__));

#include <pthread.h>

int i = 1, j = 3;
int *p1;
int *p;
int a[1];

#define NUM 5

void * t1(void* arg)
{
  *p1 = *(&j);
  a[0] = *p1 + 1;
  i = a[0];
  __CPROVER_assume (i == 4);
  p = a;
  i = *p + 1;
  //j=p[0];  //non gestito
  return;
  p1 = &a[0];
  a[0] = 3;
}

void * t2(void* arg)
{
  p1 = &i;
}


int
main(int argc, char **argv)
{
  pthread_t id1, id2;

  pthread_create(&id1, NULL, t1, NULL);
  pthread_create(&id2, NULL, t2, NULL);

  if (((i != 1) && (i != 3) && (i != 4) && (i != 5))) {
ERROR: __VERIFIER_error();
  }

  return 0;
}
