package BitOp;

public class killerQueen {
	
	static int columnForRow[] = new int [8];
	static int count=0;
	
	static void printBoard(){
		StringBuilder str = new StringBuilder();
		for (int i=0; i<8; i++){
			str.append("|");
			for (int j=0; j<8; j++){
				if (j!=columnForRow[i]){
					str.append(" |");
				}
				else{
					str.append("Q|");
				}
			}
			str.append("\n");
		}
		System.out.println(str);
	}

	
	static boolean check(int row) {
		for (int i = 0; i < row; i++) {
			int diff = Math.abs(columnForRow[i] - columnForRow[row]);
			if (diff == 0 || diff == row - i) return false;
		}
		return true;
	}

	static void PlaceQueen(int row){
		if (row == 8) {
			printBoard();
			count++;
			return;
		}
		for (int i = 0; i < 8; i++) {
			columnForRow[row]=i;
			if(check(row)){
				PlaceQueen(row+1);
			}
		}
	}

	public static void main(String[] args) {
		PlaceQueen(0);
		System.out.println("Total: "+count);

	}

}
