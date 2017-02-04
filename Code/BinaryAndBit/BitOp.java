package BitOp;

public class BitOp {
	
	public static void printNeg(long num){
		String bit = "";
		num = Math.abs(num);
		long div=num;
		long mod=0;
		while (div!=0){
			div=num/2;
			mod=num%2;
			bit=mod+bit;
			num=div;
		}
		while (bit.length()<32){
			bit='0'+bit;
		}
		char[] bitArray = bit.toCharArray();
		for (int i=0; i<bitArray.length;i++){
			if (bitArray[i]=='1'){bitArray[i]='0';}
			else {bitArray[i]='1';}
		}
		String numStr=String.valueOf(bitArray);
		long newNum = Long.parseLong(numStr,2);
		newNum=newNum+1;
		printPos(newNum);
	}
	
	public static void printPos(long num){
		String bit = "";
		long div=num;
		long mod=0;
		while (div!=0){
			div=num/2;
			mod=num%2;
			bit=mod+bit;
			num=div;
		}
		while (bit.length()<32){
			bit='0'+bit;
		}
		System.out.println(bit);
	}
	
	public static void printBinary(long num){
		if (num<0){
			printNeg(num);
		}
		else{
			printPos(num);
		}
		
	}
	
	public static long updateBits(long n, long m, long i, long j) {
		 long max = ~0; /* All 1’s */
		 
		 // 1’s through position j, then 0’s
		 long left = max - ((1 << j+1) - 1);
		 
		 printBinary(left);

		 // 1’s after position i
		 long right = ((1 << i) - 1);
		 
		 printBinary(right);

		 // 1’s, with 0s between i and j
		 long mask = left | right;
		 
		 printBinary(mask);

		 // Clear i through j, then put m in there

		 printBinary(n&mask); //give me n only with 0 it i to j
		 printBinary(m << i); // give me m that stars from place i

		 return (n & mask) | (m << i); //add m that starts from i to the 0's in n from i to j
	}

	public static void main(String[] args) {
		long n=1024;
		long m=21;
		long result = updateBits(n, m, 2, 6);
		System.out.println("Result:");
		printBinary(n);
		printBinary(m);
		printBinary(result);
	}

}


