#include <stdio.h>
#include <math.h>
#include <limits.h>


int isSorted(int arr[], int len){
	int start = -1;
	int end = -1;
	int i = 0;
	if (len < 2) {//no second
		return 0;
	}
	while (i+1 < len){
		if (arr[i] > arr[i + 1]){
			if (start < 0){
				start = i;
			}
			else if (end < 0){
				end = i+1;
			}
			else{
				return 0;
			}
		}
		i++;			
	}
	if (end < 0 && start < 0){
		return 1;
	}
	if (end < 0 && start > 0){
		end = len - 1;
	}
	if (arr[end] > arr[start + 1] || (start > 0 && arr[start - 1] > arr[end])){
		return 0;
	}
	if ((end < len-1 && arr[start] > arr[end + 1] ) || arr[end - 1] > arr[start]){
		return 0;
	}
	return 1;
}

/*int main(){
int arr[] = { 1, 7, 3, 2, 8 };
int len = sizeof(arr) / sizeof(arr[0]);
printf("1: %d\n", isSorted(arr, len));

int arr2[] = { 1, 7, 9, 2, 8 };
printf("0: %d\n", isSorted(arr2, len));

int arr3[] = { 1, 7, 2, 2, 2 };
printf("1: %d\n", isSorted(arr3, len));

int arr4[] = { 1, 2, 3, 4, 4 };
printf("1: %d\n", isSorted(arr4, len));

getchar();
return 1;
}*/