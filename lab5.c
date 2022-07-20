#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5


int cond = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;


void * t1 (void * t) {

  pthread_mutex_lock(&x_mutex);

  while (cond < 4) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }

  printf("Volte sempre!\n");

  pthread_mutex_unlock(&x_mutex); 
  
  pthread_exit(NULL);
}

void * t2 (void * t) {
  sleep(0.100);
  printf("Fique a vontade.\n");

  pthread_mutex_lock(&x_mutex);
  cond++;

  if (cond < 1) {
  pthread_cond_wait(&x_cond, &x_mutex);
  }

  if (cond == 4) {
    pthread_cond_signal(&x_cond);
  }

  pthread_mutex_unlock(&x_mutex); 
  
  pthread_exit(NULL);
}

void * t3 (void * t) {
  sleep(0.100);
  printf("Sente-se por favor.\n");

  pthread_mutex_lock(&x_mutex);
  cond++;

  if (cond < 1) {
  pthread_cond_wait(&x_cond, &x_mutex);
  }

  if (cond == 4) {
    pthread_cond_signal(&x_cond);
  }

  pthread_mutex_unlock(&x_mutex); 
  
  pthread_exit(NULL);
}

void * t4 (void * t) {
  sleep(0.100);
  printf("Aceita um copo d’água?\n");

  pthread_mutex_lock(&x_mutex);
  cond++;

  if (cond < 1) {
  pthread_cond_wait(&x_cond, &x_mutex);
  }

  if (cond == 4) {
    pthread_cond_signal(&x_cond);
  }

  pthread_mutex_unlock(&x_mutex); 

  pthread_exit(NULL);
}

void * t5 (void * t) {
  printf("Seja bem-vindo!\n"); 
  
  pthread_mutex_lock(&x_mutex);
  cond++;
  
  pthread_cond_broadcast(&x_cond);
  
  pthread_mutex_unlock(&x_mutex); 

  pthread_exit(NULL);
}



int main(int argc, char *argv[]) {
  int i; 
  pthread_t tid[NTHREADS];

  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  if (pthread_create( & tid[0], NULL, t5, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[1], NULL, t4, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[2], NULL, t3, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[3], NULL, t2, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  if (pthread_create( & tid[4], NULL, t1, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
