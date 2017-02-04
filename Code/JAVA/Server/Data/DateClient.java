package getReady;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

/**
 * Trivial client for the date server.
 */
public class DateClient {

    /**
     * Runs the client as an application.  First it displays a dialog
     * box asking for the IP address or hostname of a host running
     * the date server, then connects to it and displays the date that
     * it serves.
     */
    public static void main(String[] args) throws IOException {
        System.out.print("Enter IP Address of a machine that is\n" +
                "running the date service on port 9090:");
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in)); //buffer to read input from user
		String serverAddress = br.readLine();//read next line input from user
        Socket s = new Socket(serverAddress, 9090);
        BufferedReader input =
            new BufferedReader(new InputStreamReader(s.getInputStream()));
        String answer = input.readLine();
        System.exit(0);
    }
}
