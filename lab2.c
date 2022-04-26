/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat1; //matriz de entrada
//float *mat2; //vetor de entrada
float *saidaSeq; //matriz de saida sequencial
float *saidaCon;
int nthreads; //numero de threads

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada
} tArgs;

//funcao que as threads executarao
void * tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   //printf("Thread %d\n", args->id);
   for(int i=args->id; i<args->dim; i+=nthreads){
      	for(int j=0; j<args->dim; j++){
		for(int k=0;k<args->dim;k++){
	   		saidaCon[i*(args->dim)+j] += mat1[k*(args->dim)+j] * mat1[k]; 		
      		} 
      	}
   }
         
   pthread_exit(NULL);
}

void multiplicaMatrizSeq(int dim){
	for(int i =0;i<dim;i++){
		for(int j=0;j<dim;j++){
			for(int k=0; k<dim;k++){
				saidaSeq[i*dim+j] += mat1[k*dim+j] * mat1[k];
			}
		}
	}
}

//fluxo principal
int main(int argc, char* argv[]) {
   int dim; //dimensao da matriz de entrada
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta;
   
    //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //alocacao de memoria para as estruturas de dados
   mat1 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat1 == NULL) {printf("ERRO--malloc\n"); return 2;}
   saidaSeq = (float *) malloc(sizeof(float) * dim * dim);
   if (saidaSeq == NULL) {printf("ERRO--malloc\n"); return 2;}
   saidaCon = (float *) malloc(sizeof(float) * dim * dim);
   if (saidaCon == NULL) {printf("ERRO--malloc\n"); return 2;}

   //inicializacao das estruturas de dados de entrada e saida
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
      	mat1[i*dim+j] = 1;    //equivalente mat[i][j]
      	saidaSeq[i*dim+j] = 0; 
      	saidaCon[i*dim+j] = 0;
      }
   }
   

   //multiplicacao da matriz pelo vetor
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   float concT = delta;
   printf("Tempo multiplicacao concorrente:%lf\n", delta);

   //exibicao dos resultados
  /* puts("Vetor de saida:");
   for(int j=0; j<dim; j++){
      printf("%.1f ", saidaCon[j]);
   }
   puts("");*/
  
   GET_TIME(inicio);
   multiplicaMatrizSeq(dim);
   GET_TIME(fim);
   delta = fim - inicio;
   float seqT = delta;
   printf("Tempo multiplicacao sequencial: %lf\n", delta);

   printf("A multiplicação de forma concorrente é  %lf vezes mais rápida do que a sequencial\n", (seqT/concT));

   for(int i =0;i<dim;i++){
                for(int j=0;j<dim;j++){
                        for(int k=0; k<dim;k++){
                                if(saidaSeq[i*dim+j] != saidaCon[i*dim+j]){
					printf("As matrizes de saída ficaram diferentes. Há um erro no código");
					exit(1);
				}
                        }
                }
        }

   printf("As matrizes resultantes da multiplicação sequencial e concorrente foram comparadas e estão iguais.\n");


   //liberacao da memoria
   free(mat1);
   free(saidaSeq);
   free(saidaCon);
   free(args);
   free(tid);  

   return 0;
}
