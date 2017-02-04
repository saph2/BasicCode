package filesAndApi;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.FileAlreadyExistsException;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Scanner; 

public class readWriteToFile {
	
	public static final String filePath="new.txt";
	public static int count=1;
	
	public static File createFile(){
		Path file = FileSystems.getDefault().getPath("new.txt");
		try {
			Files.createFile(file);
		}
		catch (FileAlreadyExistsException e){} 
		catch (IOException e) {
			e.printStackTrace();
		}
		return file.toFile(); 
	}
	
	public static void writeToFile(File file) throws IOException{
		FileWriter ft = new FileWriter(file.getAbsoluteFile(),true);
		BufferedWriter out = new BufferedWriter(ft);
		out.write("Hello: "+count+"\n");
		out.close();
	}
	
	public static void readFile(File file) throws FileNotFoundException{
		Scanner sc = new Scanner(file);
		while (sc.hasNextLine()){
			System.out.println(sc.nextLine().toString());
		}
		sc.close();
	}
	
	public static void deleteFile(File file) throws IOException{
		Path path=file.toPath();
		Files.delete(path);
		System.out.println("File deleted!");
	}

	public static void main(String[] args) throws IOException {
		File file = createFile();
		writeToFile(file);
		readFile(file);
		deleteFile(file);
	}

}
