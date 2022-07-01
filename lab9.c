
#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#include <semaphore.h>

#define NTHREADS 5

int cond = 0;
sem_t sem1, thread1;

void * t1(void * arg) {
  sem_wait( & thread1);

  printf("Volte sempre!\n");

  pthread_exit(NULL);
}

void * t2(void * arg) {
  sem_wait( & sem1);
  printf("Fique a vontade.\n");

  cond++;

  if (cond == 4) {
    sem_post( & thread1);
  }
  sem_post( & sem1);

  pthread_exit(NULL);
}

void * t3(void * arg) {
  sem_wait( & sem1);
  printf("Sente-se por favor.\n");

  cond++;

  if (cond == 4) {
    sem_post( & thread1);
  }

  sem_post( & sem1);

  pthread_exit(NULL);
}

void * t4(void * arg) {

  sem_wait( & sem1);
  printf("Aceita um copo d’água?\n");

  cond++;

  if (cond == 4) {
    sem_post( & thread1);
  }

  sem_post( & sem1);

  pthread_exit(NULL);
}

void * t5(void * threadid) {
  printf("Seja bem-vindo!\n");

  sem_post( & sem1);
  cond++;
  pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
  pthread_t tid[NTHREADS];
  int *idt1, *idt2,*idt3, *idt4, *idt5, t;
  
  idt1=malloc(sizeof(int)); 
  idt2=malloc(sizeof(int)); 
  idt3=malloc(sizeof(int)); 
  idt4=malloc(sizeof(int)); 
  idt5=malloc(sizeof(int)); 
  *idt1=1, *idt2=2, *idt3=3; *idt4=4, *idt5=5;


  sem_init( & sem1, 0, 0);
  sem_init( & thread1, 0, 0);

  if (pthread_create( & tid[0], NULL, t5, (void *)idt1)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[1], NULL, t4, (void *)idt2)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[2], NULL, t3, (void *)idt3)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[3], NULL, t2, (void *)idt4)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[4], NULL, t1, (void *)idt5)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  for (t = 0; t < NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
      printf("--ERRO: pthread_join() \n");
      exit(-1);
    }
  }

  pthread_exit(NULL);
}
