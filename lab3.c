#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


long int tam_vet;  
int nthreads;  
float *vet; 
float *maxLocal;
float *minLocal;


typedef struct{
   int id; 
   int tam_vet;
   float min;
   float max;
} tArgs;

void * tarefa(void * arg) {
    tArgs *args = (tArgs*) arg;
    float min = vet[0]; 
    float max = vet[0];
    
    for(int i = args->id * (tam_vet/nthreads); i < (args->id + 1) * (tam_vet/nthreads); i++){
        if (min > vet[i]) {
            min = vet[i];
        }
        if (max < vet[i]) {
           max = vet[i];   
         }
         minLocal[(args->id)++] = min;
         maxLocal[(args->id)++] = max;
    }

    
    pthread_exit(NULL); 
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
    printf("Execução Sequencial: Minimo = %.5f e Maximo = %.5f \n", min, max);
}

int main(int argc, char *argv[]) {
	
	if(argc < 3) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
       return 1; 
	}
	
	tam_vet = atoll(argv[1]); 
	nthreads = atoi(argv[2]);
	
	vet = (float*) malloc(sizeof(float)*tam_vet);
	
	srand(time(NULL));
	
	for(int i=0;i<tam_vet;i++){
		float num = (rand()%1000)*0.8f;
		vet[i] = num;
	}
	
	for(int i=0;i<tam_vet;i++){
		printf("%.5f\n", vet[i]);
	}
	
	seqMaxMin(tam_vet);
	
			
	pthread_t *tid; 
	tArgs *args;
	
	tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {puts("ERRO--malloc"); return 2;}
	args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
	if(args==NULL) {puts("ERRO--malloc"); return 2;}
	
		printf("12");
	
	maxLocal =(float*) malloc(sizeof(float)*nthreads);	
	minLocal = (float*) malloc(sizeof(float)*nthreads);
	
	printf("13");
	
	 for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   
    for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   
   float maxTotal = 0.0f;
   float minTotal =0.0f;
   for (int i = 0; i < nthreads; i++) {
        if (maxLocal[i] > maxTotal){
            maxTotal = maxLocal[i];
		}
		 if (minLocal[i] < minTotal){
            minTotal = minLocal[i];
		}
    }
   
   printf("Execução Concorrente: Minimo = %.5f e Maximo = %.5f \n", minTotal, maxTotal);
   free(vet);
   free(args);
   free(tid);
   
   return 0;
   
}
