package multiServer;

public class RunServer {

	public static void main(String[] args) {
		MainServerThread sv = new MainServerThread();
		Thread svThread = new Thread (sv);
		svThread.start();
		try {
			Thread.sleep(20 * 1000);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		sv.stop();
	}

}
