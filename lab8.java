
class LE {
	private int leit, escr, aux;

	LE() {
		this.leit = 0;
		this.escr = 0;
		this.aux = 0;
	}

	public synchronized void EntraLeitor(int id) {
		try {
			while (this.escr > 0) {
				System.out.println("le.leitorBloqueado(" + id + ")");
				wait();
			}
			this.leit++;
			System.out.println("le.leitorLendo(" + id + ")");
		} catch (InterruptedException e) {
		}
	}

	public synchronized void SaiLeitor(int id) {
		this.leit--;
		if (this.leit == 0)
			this.notify();
		System.out.println("le.leitorSaindo(" + id + ")");
	}

	public synchronized void EntraEscritor(int id) {
		try {
			while ((this.leit > 0) || (this.escr > 0)) {
				System.out.println("le.escritorBloqueado(" + id + ")");
				wait();
			}
			this.escr++;
			System.out.println("le.escritorEscrevendo(" + id + ")");
		} catch (InterruptedException e) {
		}
	}

	public synchronized void SaiEscritor(int id) {
		this.escr--;
		notifyAll();
		System.out.println("le.escritorSaindo(" + id + ")");
	}

}

class T1 extends Thread {
	int id;
	int delay;
	LE monitor;

	T1 (int id, int delayTime, LE m) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
	
	 public void run () {
		    double j=777777777.7, i;
		    try {
		      for (;;) {
		    	  this.monitor.EntraEscritor(this.id); 
		    	  for (i=0; i<100000000; i++) {j=j/2;} 
		          this.monitor.SaiEscritor(this.id); 
		          sleep(this.delay); 
		      }
		    } catch (InterruptedException e) { return; }
		  }
	
}


class T2 extends Thread {
	  int id; 
	  int delay; 
	  LE monitor;
	  
	  T2 (int id, int delayTime, LE m) {
	    this.id = id;
	    this.delay = delayTime;
	    this.monitor = m;
	  }

	
	  public void run () {
	    double j=777777777.7, i;
	    try {
	      for (;;) {
	        this.monitor.EntraLeitor(this.id); 
	        for (i=0; i<100000000; i++) {j=j/2;} 
	        this.monitor.SaiLeitor(this.id); 
	        sleep(this.delay); 
	      }
	    } catch (InterruptedException e) { return; }
	  }
	}


class T3 extends Thread {
	int id; 
	int delay; 
	LE monitor; 

	T3 (int id, int delayTime, LE m) {
		this.id = id;
		this.delay = delayTime;
		this.monitor = m;
	}
	public void run () {
		double j=777777777.7, i;
		try {
			for (;;) {
				this.monitor.EntraLeitor(this.id); 
				for (i=0; i<100000000; i++) {j=j/2;} 
				this.monitor.SaiLeitor(this.id); 
				for (i=0; i<100000000; i++) {j=j/2;} 
		    	this.monitor.EntraEscritor(this.id); 
		    	for (i=0; i<100000000; i++) {j=j/2;} 
		    	this.monitor.SaiEscritor(this.id);
				sleep(this.delay); 
			}
		} catch (InterruptedException e) { return; }
	}
}

class Lab1 {
	  static final int v1 = 1;
	  static final int v2 = 2;
	  static final int v3 = 3;


	  public static void main (String[] args) {
	    int i;
	    LE monitor = new LE();            
	    T1[] a = new T1[v1];       
	    T2[] b = new T2[v2];  
	    T3[] c = new T3[v3]; 

	    System.out.println ("import verificaLE");
	    System.out.println ("le = verificaLE.LE()");
	    
	    for (i=0; i<v1; i++) {
	       a[i] = new T1(i+1, (i+1)*500, monitor);
	       a[i].start(); 
	    }
	    for (i=0; i<v2; i++) {
	       b[i] = new T2(i+1, (i+1)*500, monitor);
	       b[i].start(); 
	    }
	    
	    for (i=0; i<v3; i++) {
	        c[i] = new T3(i+1, (i+1)*500, monitor);
	        c[i].start(); 
	     }
	    
	  }
	}













