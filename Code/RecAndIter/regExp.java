package loopAndRec;

import java.util.ArrayList;

public class regExp {
	
	public static final char PLUS ='+';
	public static final char ALL = '*';
	
	public static ArrayList<String> getParts(String reg){
		// Regular expression -> parts //
		ArrayList<String> parts = new ArrayList<String>();
		if (reg.length()==0){
			return parts;
		}
		String str="";
		int i=0;
		while (i<reg.length()){
			if (i+1<reg.length() && reg.charAt(i+1)==PLUS){
				str=""+reg.charAt(i)+reg.charAt(i+1);//a+
				i++;
			}
			else{
				str=""+reg.charAt(i);//a
			}
			parts.add(str);//[a,a+,*]
			i++;
		}
		return parts;
	}
	
	public static boolean checkCurrent(String reg, String word){
		// Return true is the string part matches the regular expression part //
		if (reg.length()<2 && (word==null || word.length()<1)){
			return false; //if reg is * or a and word is {} or null -> false
		}
		if (reg.length()<2 && reg.charAt(0)!=ALL && word!=null){
			if (word.length()==1 && reg.equals(word)){
				return true; //if reg is 'a' and word is 'a' -> true
			}
			else{
				return false; //if reg is 'a' and word is 'b' -> false
			}
		}
		if (reg.length()==2 && word.length()>1){ //if reg is 'a+' and word is 'aa' -> false 
			return false;
		}
		return true;// if reg is * -> true
	}
	

	
	public static boolean checkReg(ArrayList<String> parts, String word){
		// checks if the word belong to this regular expression //
		if (parts.size()==0 && (word==null || word.length()==0)){//end of the reg and word
			return true;
		}
		if (parts.size()==0){//reg ended but word did not
			return false;
		}
		ArrayList<String> newParts=(ArrayList<String>) parts.clone();//reg expression parts
		String part=parts.get(0); //* : a
		newParts.remove(0);//a : ''
		int j=0;
		while (j<word.length()+1){//go over word(start->end) to find a match to reg(0)
			String first=word.substring(0, j);//a : a : aa
			String last = word.substring(j,word.length());//aa : a : a
			if (checkCurrent(part,first)){//*->a , a->aa : *->aa
				// if the word(0->j) matches reg(0) --> check match reg(1->last) and word(j+1->last)
				if (checkReg(newParts,last)){ //(a,aa) : (a,a)
					// if rest of parts matched -> word belongs to reg --> break to return true
					break;
				}
			}
			j++;//if rest of parts didn't match --> check reg(0) and word(0->j+1)
		}
		if (j<word.length()+1){//match found
			return true;
		}
		return false;//no match found
	}
	
	public static boolean checkMatch(String reg, String word){
		ArrayList<String> parts = getParts(reg);
		return checkReg(parts,word);
	}
	

	public static void main(String[] args) {
		System.out.println(checkMatch("a","a"));
		System.out.println(checkMatch("a+","a"));
		System.out.println(checkMatch("*a","aaaa"));
		System.out.println(checkMatch("a+a","a"));
		System.out.println(checkMatch("a+","ba"));
		System.out.println(checkMatch("ba+b","bb"));
		

	}

}
