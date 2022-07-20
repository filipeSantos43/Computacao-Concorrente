#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"
#include<math.h>

long int tam_vet; 
int nthreads; 
float *vet; 
long int global = 0; 

float *vetSaidaSeq;
float *vetSaidaConc;

pthread_mutex_t mutex; 

int ehPrimo(long int n){
	
	if(n<2) return 0;
	
	if(n==2) return 1;
	
	if(n%2==0) return 0;
	
	for(int i=3; i<sqrt(n)+1; i+=2){
		if(n%i == 0) return 0; 
	}
	return 1; 
}

void raizesSeq (long long int tam_vet, float * vet) {
  for(long int i = 0; i < tam_vet; i++){
    if(ehPrimo(vet[i])) {
		vetSaidaSeq[i] = sqrt(vet[i]);
	}
    vetSaidaSeq[i] = vet[i];
  }
}

void *raizesConc () {
  pthread_mutex_lock(&mutex);
  int temp = global;
  global++;
  pthread_mutex_unlock(&mutex);

  while(temp < tam_vet){
    if(ehPrimo(vet[temp]))    vetSaidaConc[temp] = sqrt(vet[temp]);
    
    vetSaidaConc[temp] = vet[temp];

    pthread_mutex_lock(&mutex);
    temp = global;
    global++;
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

int main(int argc,  char *argv[]){

	pthread_t *tid; 

 	double inicio, fim, tempoSeq, tempoConc, desempenho; 
 	
	if(argc < 3){
		fprintf(stderr, "Digite: %s <tam_vetensao do vetor> <Número de threads>\n",argv[0]);
	        return 1;	
	}
	tam_vet = atoll(argv[1]);
	nthreads = atoi(argv[2]);

	vet= (float *) malloc(sizeof(float)*tam_vet); 
    if(vet== NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}
	
	vetSaidaSeq= (float *) malloc(sizeof(float)*tam_vet); 
    if(vetSaidaSeq== NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}
	
	
	vetSaidaConc= (float *) malloc(sizeof(float)*tam_vet); 
    if(vetSaidaConc== NULL) {
		fprintf(stderr, "ERRO--malloc\n");
	        return 2; 	
	}
	
	for(long int i =0; i<tam_vet; i++){
		vet[i] = i;
	}

	GET_TIME(inicio);  
	raizesSeq(tam_vet, vet);
	GET_TIME(fim); 
	tempoSeq = fim-inicio; 

	GET_TIME(inicio); 
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads); 
    if(tid == NULL){
	       	fprintf(stderr, "ERRO--malloc\n");
                return 2;
	}	

	 for( long int i = 0; i<nthreads; i++){ 	
		if(pthread_create(tid+i, NULL, raizesConc, (void*) i)){
			fprintf(stderr, "ERRO--pthread_create\n");
	                return 3;
		}
	 }

	for( long int i = 0; i<nthreads; i++){
                if(pthread_join(*(tid+i), NULL)){
                        fprintf(stderr, "ERRO--pthread_create\n");
                        return 3;
                }	
         }
         
	GET_TIME(fim);
    tempoConc = fim - inicio; 	

	desempenho = tempoSeq/tempoConc; 

	printf("\nO desempenho concorrete foi %lf melhor do que o concorrente\n", desempenho); 

	free(vet); 
	free(tid); 
	free(vetSaidaSeq);
	free(vetSaidaConc);
	pthread_mutex_destroy(&mutex);
	return 0;
}



