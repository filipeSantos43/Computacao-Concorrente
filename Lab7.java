
package dcc.ufrj.br.compConc;

import java.util.Random;

//classe da estrutura de dados (recurso) compartilhado entre as threads
class Cont {

	private int tam; // Tamanho do vetor
	private int nThreads; // Número de threads
	private int[] vetor; // Vetor

	// recurso compartilhado
	private int r;

	// construtor
	public Cont(int tam, int nThreads, int[] vetor) {
		this.tam = tam;
		this.nThreads = nThreads;
		this.vetor = vetor;
		this.r = 0;
	}

	public synchronized void contaPares(int id) {
		int inicio, fim;
		int tamBloco = this.tam / this.nThreads;
		inicio = id * tamBloco;
		if (id == nThreads - 1)
			fim = this.tam;
		else
			fim = inicio + tamBloco;

		for (int i = inicio; i < fim; i++) {
			if (this.vetor[i] % 2 == 0) {
				r++;
			}
		}

	}

	public synchronized int get() {
		return this.r;
	}

}

//classe que estende Thread e implementa a tarefa de cada thread do programa 
class Th extends Thread {
	// identificador da thread
	private int id;
	// objeto compartilhado com outras threads
	Cont s;

	// construtor
	public Th(int tid, Cont s) {
		this.id = tid;
		this.s = s;
	}

	// metodo main da thread
	public void run() {
		System.out.println("Thread " + this.id + " iniciou!");
		this.s.contaPares(id);
		System.out.println("Thread " + this.id + " terminou!");
	}
}

//classe da aplicacao
class Lab7 {
	static final int N = 2;
	static final int TAM = 30; // Tamanho do vetor

	public static void main(String[] args) {

		Random rand = new Random();
		int[] vet = new int[TAM];
		for (int i = 0; i < TAM; i++) {
			vet[i] = rand.nextInt(TAM);
		}

		// reserva espaço para um vetor de threads
		Thread[] threads = new Thread[N];

		// cria uma instancia do recurso compartilhado entre as threads
		Cont s = new Cont(TAM, N, vet);

		// cria as threads da aplicacao
		for (int i = 0; i < threads.length; i++) {
			threads[i] = new Th(i, s);
		}

		// inicia as threads
		for (int i = 0; i < threads.length; i++) {
			threads[i].start();
		}

		// espera pelo termino de todas as threads
		for (int i = 0; i < threads.length; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				return;
			}
		}

		System.out.println("Valor de s = " + s.get());
	}
}
