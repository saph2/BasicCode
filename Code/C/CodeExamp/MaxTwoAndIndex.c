#include <stdio.h>
#include <math.h>
#include <limits.h>


int maxTwo(int A[], int len){
	int i;
	int j = 0;
	int result = INT_MIN;
	int total = 0;
	for (i = 0; i < len; i++){
		total = A[i] + i + A[j] - j;
		if (result < total){ //keep maximum result
			result = total;
		}
		if (A[i] - i > A[j] - j){//find largest j (while j < i)
			j = i;
		}
	}
	return result;
}

/*int main(){
	int arr[] = { 1, 2, 0, 5, 0 };
	int len = sizeof(arr) / sizeof(arr[0]);
	printf("9: %d\n", maxTwo(arr, len));

	int arr2[] = { 1, 0, 1, 0, 0 };
	printf("5: %d\n", maxTwo(arr2, len));

	int arr3[] = { 0, 0, 2, 0, 0 };
	printf("4: %d\n", maxTwo(arr3, len));

	getchar();
	return 1;
}*/