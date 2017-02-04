package filesAndApi;
import java.io.*;
import java.net.*;

public class api {
	
	public static final String Url = "http://google.com";
	
	public static String getHTML(String urlToRead) throws Exception {
		
		URL url = new URL(urlToRead); //get url from string
		HttpURLConnection con = (HttpURLConnection) url.openConnection(); // connect to url
		con.setRequestMethod("GET");//get info from connection
		
		BufferedReader bt = new BufferedReader (new InputStreamReader(con.getInputStream()));
		StringBuilder str = new StringBuilder();
		
		String line;
		while ((line=bt.readLine())!=null){
			str.append(line);
		}
		
		bt.close();
		return str.toString();
		
	}

   public static void main(String[] args) throws Exception   {
     System.out.println(getHTML(Url));
   }
}
