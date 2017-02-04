package multiServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class RunClient {

	public static void main(String[] args) throws IOException {
			
			long time = System.currentTimeMillis();
	
			Socket client = new Socket("localhost",9000);
			
			OutputStream output = client.getOutputStream();	
			output.write(("Client "+time).getBytes());

	}

}
