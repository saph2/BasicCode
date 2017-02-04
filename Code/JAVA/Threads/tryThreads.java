package threads;

import java.util.concurrent.locks.Lock;

public class tryThreads {
	
	static class ThreadA extends Thread{
		
		public static int counter=0;
		
		public ThreadA(String name){
			super(name);
		}
		
		public void run(){
			synchronized(this){
				while (counter<100 && counter>=0){
					System.out.println(this.getName()+": "+counter);
					if (this.getName().equals("t1")){
						counter++;
					}
					else if (this.getName().equals("t2")){
						counter--;
					}
				}
			}
		}
	}
	
	public static void main(String[] args) {
		ThreadA t1=new ThreadA("t1");
		ThreadA t2=new ThreadA("t2");
		t1.start();
		t2.start();
	}

}
