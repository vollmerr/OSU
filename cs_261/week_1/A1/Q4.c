/* CS261- Assignment 1 - Q.4*/
/* Name:	Ryan Vollmer
 * Date:	09/28/16
 * Solution description:	Sorts random students by score then prints.
 */
 
#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#define MAX_SCORE 100

struct student{
	int id;
	int score;
};

void sort(struct student* students, int n){
	/*Sort the n students based on their score*/  
	struct student tmp;
	for (int i = 1; i < n; i++) {
		for (int j = 0; j < n - i; j++) {
			if (students[j].score > students[j + 1].score) {
				tmp = students[j];
				students[j] = students[j + 1];
				students[j + 1] = tmp;
			}
		}
	}
}

void generate(struct student* students, int n) {
	/*Generate random and unique ID and scores for ten students, ID being between 1 and 10, scores between 0 and 100*/
	int unique = 0;
	for (int i = 0; i < n; i++) {
		do {
			students[i].id = rand() % n + 1;
			unique = 1;
			for (int j = 0; j < i; j++) {
				if (students[i].id == students[j].id) {
					unique = 0;
				}
			}
		} while (!unique);
		students[i].score = rand() % (MAX_SCORE + 1);
	}
}

void output(struct student* students, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d\t%d\n", students[i].id, students[i].score);
	}
	printf("\n");
}

int main(){
	/*Declare an integer n and assign it a value.*/
	int n = 5;
	struct student* students = NULL;
	/*Allocate memory for n students using malloc.*/
	students = malloc(n * sizeof(struct student));
	/*Generate random and unique IDs and random scores for the n students, using rand().*/
	generate(students, n);
	/*Print the contents of the array of n students.*/
	output(students, n);
	/*Pass this array along with n to the sort() function*/
	sort(students, n);
	/*Print the contents of the array of n students.*/
	output(students, n);
	free(students);
	return 0;
}

