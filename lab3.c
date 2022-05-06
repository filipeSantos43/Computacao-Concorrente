#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

long int tam_vet; 
int nthreads; 
float *vet; 
float maxS;
float minS;

typedef struct{
   float max; 
   float min; 
} tArgs;

//fluxo das threads
void * tarefa(void * arg) {
    long int id = (long int) arg; 
    tArgs *args = malloc(sizeof(tArgs));
   
    long int fim; 
    if(id == nthreads-1) 
        fim = tam_vet;
    else 
        fim = (id * tam_vet/nthreads) + tam_vet/nthreads; 

    args->max=vet[id * tam_vet/nthreads];
    args->min=vet[id * tam_vet/nthreads];
    
   
    for(long int i=(id * tam_vet/nthreads); i<fim; i++){
        if(vet[i]>args->max){
            args->max = vet[i];
        }
        if(vet[i]<args->min){
            args->min = vet[i];
        }
   }
     
   pthread_exit((void *) args); 
}

void seqMaxMin(int tam_vet){
    float min = vet[0];
    float max = vet[0];
    for(int i=0; i<tam_vet; i++){
        if (min > vet[i]) {
            min = vet[i];
        }

        if (max < vet[i]) {
            max = vet[i];        
        }
    }
    
    maxS = max;
    minS = min;
   // printf("Execução Sequencial: Minimo = %.5f e Maximo = %.5f \n", min, max);
}

int main(int argc, char *argv[]) {
    double ini, fim, tempoC, tempoS;
    float  maxCon, minCon; 
    pthread_t *tid; 
    tArgs *retorno; 

    if(argc < 3) {
        fprintf(stderr, "Digite: %s <tamanho vetor> <numero threads>\n", argv[0]);
        return 1; 
    }
    tam_vet = atoi(argv[1]);
    nthreads = atoi(argv[2]);



	vet = (float*) malloc(sizeof(float)*tam_vet);
	
	srand(time(NULL));
	
	for(int i=0;i<tam_vet;i++){
		float num = (rand()%1000)*0.8f;
		vet[i] = num;
	}       
        
    
    GET_TIME(ini);
	seqMaxMin(tam_vet);
    GET_TIME(fim);
    tempoS=fim-ini;
    printf("Tempo sequencial:  %lf\n", fim-ini);

    
    GET_TIME(ini);
    maxCon=vet[0];
    minCon=vet[0];

    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
 
    for(long int i=0; i<nthreads; i++) {
        if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
    }
	

    for(long int i=0; i<nthreads; i++) {
        if(pthread_join(*(tid+i), (void**) &retorno)){
            fprintf(stderr, "ERRO--pthread_create\n");
            return 3;
        }
        

        if(retorno->max>maxCon)
        maxCon=retorno->max;
        if(retorno->min<minCon)
        minCon=retorno->min;    

    }
    GET_TIME(fim);
    tempoC=fim-ini;
    printf("Tempo concorrente:  %lf\n", fim-ini);


      
    printf("A execução concorrente é %lf vezes melhor do que a sequencial\n", tempoS/tempoC);

    free(vet);
    free(tid);

    return 0;
}

   
