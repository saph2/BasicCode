package multiServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class SocketThread implements Runnable{
	

	protected Socket clientSocket = null;
	
	public SocketThread(Socket clientSocket){
		this.clientSocket=clientSocket;
	}

	public void run() {
		try {
			OutputStream output = this.clientSocket.getOutputStream();
			output.write("Start Connection\n".getBytes());

			InputStream input = this.clientSocket.getInputStream();
			byte[] byteArray = new byte[input.available()];
			input.read(byteArray,0,byteArray.length);
			
			output.write(byteArray);
			
			String str = new String(byteArray);
			System.out.println(str);

			output.close();
			
		} catch (IOException e) {
			e.printStackTrace();
		}		
	}

}
