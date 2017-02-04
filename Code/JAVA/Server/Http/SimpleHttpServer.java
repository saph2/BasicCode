package HttpServer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.URL;
import java.net.URLConnection;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.Headers;

public class SimpleHttpServer {
	
	static class TestHandle implements HttpHandler{

		public void handle(HttpExchange t) throws IOException {
			
			String str = "Massage";
			t.sendResponseHeaders(200, str.length());
			OutputStream out = t.getResponseBody();
			out.write(str.getBytes());
			out.close();
			
		}
		
	}
	
	static class InfoHandler implements HttpHandler {
		
		public void handle(HttpExchange t) throws IOException {
			String response = "Use /get to download a PDF";
			t.sendResponseHeaders(200, response.length()); //set the header: 200 + length of massage
			OutputStream output = t.getResponseBody();  //get body for massage
			output.write(response.getBytes()); //write the massage
			output.close();
		}
	}

	static class GetPdfHandler implements HttpHandler {
		
		public void handle(HttpExchange t) throws IOException {
			
			// add the required response header for a PDF file
			Headers h = t.getResponseHeaders();
			h.add("Content-Type", "application/pdf");		

			// a PDF (you provide your own!)
			Path path = FileSystems.getDefault().getPath("temp.pdf");
			byte [] bytearray = Files.readAllBytes(path);

			// OK, we are ready to send the response.
			t.sendResponseHeaders(200, bytearray.length);//set massage size as array length
			OutputStream output = t.getResponseBody(); //get the body
			output.write(bytearray);//write massage into body
			output.close();
		}
	}
	
static class GetUrlHandler implements HttpHandler {
		
		public void handle(HttpExchange t) throws IOException {
			
			// add the required response header for a PDF file
			Headers h = t.getResponseHeaders();
			h.add("Content-Type", "application");
			
			URL url=new URL("http://google.com");
			URLConnection con = url.openConnection();	
			InputStream in = con.getInputStream();
	
			int len = in.available();
			byte[] bytearray = new byte[len];
			in.read(bytearray,0,len);	
			
			// OK, we are ready to send the response.
			t.sendResponseHeaders(200, len);//set massage size as array length
			OutputStream output = t.getResponseBody(); //get the body
			output.write(bytearray,0,len);//write massage into body
			output.close();
		}
	}
	
	public static void main(String[] args) throws Exception {
		HttpServer server = HttpServer.create(new InetSocketAddress(8000), 0); //set the server on port 8000
		server.createContext("/test", new TestHandle());
		server.createContext("/info", new InfoHandler());
		server.createContext("/pdf", new GetPdfHandler());
		server.createContext("/url", new GetUrlHandler());
		server.setExecutor(null);
		server.start();
	}
}
