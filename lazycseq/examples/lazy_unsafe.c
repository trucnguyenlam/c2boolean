extern void __VERIFIER_error() __attribute__ ((__noreturn__));

#include <pthread.h>
#include <assert.h>

pthread_mutex_t  dddd;
int data = 0;

void *thread1(void *arg)
{
  pthread_mutex_lock(&dddd);
  data++;
  pthread_mutex_unlock(&dddd);
}


void *thread2(void *arg)
{
  pthread_mutex_lock(&dddd);
  data+=2;
  pthread_mutex_unlock(&dddd);
}


void *thread3(void *arg)
{
  pthread_mutex_lock(&dddd);
  if (data >= 3){
    ERROR: __VERIFIER_error();
    ;
  }
  pthread_mutex_unlock(&dddd);    
}


int main()
{
  pthread_mutex_init(&dddd, 0);

  pthread_t t1, t2, t3;

  pthread_create(&t1, 0, thread1, 0);
  pthread_create(&t2, 0, thread2, 0);
  pthread_create(&t3, 0, thread3, 0);

  pthread_join(t1, 0);
  pthread_join(t2, 0);
  pthread_join(t3, 0);
  
  return 0;
}
