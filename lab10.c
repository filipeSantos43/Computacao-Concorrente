/*
 
 A) Para que o algoritmo funcione corretamente, os valores iniciais de todos os semáforos deve ser igual a 1, tanto para os escritores quanto leitores. 
 * 
 B) Sim, é possível eliminar alguns semáforos, deixando restar apenas os relativos a leitores e escritores. No caso, no codigo, poderia se reescrever eliminando o em_e e o em_l
 * 
 C) Código abaixo:
 
  */


#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

#define L 4
#define E 4

int l=0, e = 0;
sem_t em_e, em_l, leit, escr;


void * escritor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("Escritora %d quer escrever\n", *id);
    sem_wait(&em_e); e++;
    if(e==1){
        sem_wait(&leit);
    } 
    sem_post(&em_e);
    sem_wait(&escr);
    printf("Escritora %d esta escrevendo\n", *id);
    sem_post(&escr);
    sem_wait(&em_e); e--;
    if(e==0) sem_post(&leit);
    sem_post(&em_e);
    sleep(3);
  } 
  pthread_exit(NULL);
}



void * leitor (void * arg) {
  int *id = (int *) arg;
  while(1) {
    printf("Leitora %d quer ler.\n", *id);
    sem_wait(&leit);
    sem_wait(&em_l);
    l++;
    if(l==1){
        sem_wait(&escr);
    } 
    sem_post(&em_l);
    sem_post(&leit);
    printf("Leitora %d esta lendo\n", *id);
    sleep(1);
    sem_wait(&em_l);
    l--;
    if(l==0){
		printf("Leitora %d libera escritores.\n", *id);
		sem_post(&escr);
	} 
    sem_post(&em_l);
  } 
  pthread_exit(NULL);
}


int main(void) {
	
  pthread_t tid[L+E];
  int id[L+E];

  sem_init(&leit, 0, 1);
  sem_init(&escr, 0, 1);
  sem_init(&em_l, 0, 1);
  sem_init(&em_e, 0, 1);

  for(int i=0; i<L; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
  }

  for(int i=0; i<E; i++) {
    id[i+L] = i+1;
    if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
  }

  for (int i = 0; i < (L+E); i++) {
    if (pthread_join(tid[i], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  pthread_exit(NULL);
  return 0;
}

