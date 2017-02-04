package loopAndRec;

public class fibunacci {
	
	public static int fibuRec(int n){
		if (n==1 || n==2) return 1;
		else{
			return (fibuRec(n-1)+fibuRec(n-2));
		}
	}
	
	public static int fibu(int n){
		int sumNew=1;
		int sumOld=1;
		int result=1;
		if (n==1 || n==2) return result;
		else{		
			for (int i=2; i<n; i++){
				sumNew=result;
				result=sumOld+sumNew;
				sumOld=sumNew;
			}
		}
		return result;
	}

	public static void main(String[] args) {
		StringBuilder str = new StringBuilder();
		StringBuilder str2 = new StringBuilder();
		str.append("Rec: ");
		str2.append("Iter:");
		for (int i=1; i<=10; i++){
			str.append(" "+fibuRec(i));
			str2.append(" "+fibu(i));
		}
		System.out.println(str.toString());
		System.out.println(str2.toString());
	}

}
