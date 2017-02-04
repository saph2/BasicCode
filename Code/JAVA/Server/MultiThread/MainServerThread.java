package multiServer;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class MainServerThread implements Runnable{
	
	protected static final int PORT = 9000;
	protected ServerSocket listner = null;
	protected Thread currentThread = null;
	protected boolean stop=false;
	
	public void run() {
		synchronized (this){
			this.currentThread = Thread.currentThread();
		}
		try {
			this.listner=new ServerSocket(PORT);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		while (!isStopped()){
			Socket client= null;
			try{
				client=this.listner.accept();
			}
			catch(IOException e){
				if(isStopped()) {
                    System.out.println("Server Stopped.") ;
                    return;
                }
			}
			Thread ht = new Thread(new SocketThread(client));
			ht.start();	
		}						
	}
	
	private synchronized boolean isStopped(){
		return this.stop;
	}
	
	public synchronized void stop(){		
		this.stop=true;
		try {
			this.listner.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	

}
