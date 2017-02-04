#include <stdio.h>
#include <math.h>
#include <limits.h>


int getMax(int arr[],int len){
	int max = INT_MIN;
	int secMax = INT_MIN;
	int i = 0;
	if  (len < 2) {//no second
		return -1;
	}
	while (i < len){
		if (max < arr[i]){ //on max update, update secMax
			secMax = max;
			max = arr[i];
		}
		else{ //if max not updated, update secMax only (if was grater than max then max would get updated)
			if (secMax < arr[i]){
				secMax = arr[i];
			}
		}
		i++;
	}
	return secMax;
}

/*int main(){
	int arr[] = { 10, 11, 2, 0, 19 };
	int len = sizeof(arr) / sizeof(arr[0]);
	printf("%d", getMax(arr,len));
	getchar();
	return 1;
}*/