/* CS261- Assignment 1 - Q.3*/
/* Name:	Ryan Vollmer
 * Date:	09/28/16
 * Solution description:	Sort an array of random integers.	
 */
 
#include <stdio.h>
#include<stdlib.h>
#include<math.h>

void sort(int* number, int n){
	/*Sort the given array number , of length n*/     
	int cur, j;
	for (int i = 0; i < n; i++) {
		j = i;
		cur = number[j];
		while (j > 0 && number[j -1] > cur) {
			number[j] = number[j - 1];
			j = j - 1;
		}
		number[j] = cur;
	}
}

int main(){
	/*Declare an integer n and assign it a value of 20.*/
	int n = 20;
	/*Allocate memory for an array of n integers using malloc.*/
	int* nums = malloc(sizeof(n) * n);
	/*Fill this array with random numbers, using rand().*/
	for (int i = 0; i < n; i++) {
		nums[i] = rand();
	}
	/*Print the contents of the array.*/
	for (int i = 0; i < n; i++) {
		if (i) {
			printf(", %d", nums[i]);
		}
		else {
			printf("%d", nums[i]);
		}
	}
	printf("\n\n");
	/*Pass this array along with n to the sort() function of part a.*/
	sort(nums, n);
	/*Print the contents of the array.*/
	for (int i = 0; i < n; i++) {
		if (i) {
			printf(", %d", nums[i]);
		}
		else {
			printf("%d", nums[i]);
		}
	}
	free(nums);
	return 0;
}
