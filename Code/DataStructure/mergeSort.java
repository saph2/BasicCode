package sort;

import java.util.Arrays;

public class mergeSort {
	
	public static int[] merge(int[] left, int[] right){
		int[] array=new int[left.length+right.length];
		int i=0;
		int j=0;
		int k=0;
		while (k<array.length){
			if (i<left.length && j<right.length){
				if (left[i]<right[j]){
					array[k]=left[i];
					i++;
				}
				else{
					array[k]=right[j];
					j++;
				}
			}
			else if (i<left.length){
				array[k]=left[i];
				i++;
			}
			else{
				array[k]=right[j];
				j++;
			}
			k++;
		}
		return array;
	}
	
	public static int[] mergeSort(int[] array){
		if (array.length<=1){return array;}
		else{
			int middle=(int) Math.floor(array.length/2);
			return merge(mergeSort(Arrays.copyOfRange(array,0, middle)),mergeSort(Arrays.copyOfRange(array,middle, array.length)));
		}
	}

	public static void main(String[] args) {
		int[] array={3,0,14,2,1,5,1};
		System.out.print(Arrays.toString(mergeSort(array)));
	}

}
