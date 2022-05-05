#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>


long int tam_vet;  
int nthreads;  
float *vet; 


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
   
}
