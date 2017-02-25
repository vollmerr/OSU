/* CS261- Assignment 1 - Q.1*/
/* Name:	Ryan Vollmer
 * Date:	09/22/16
 * Solution description:	Creates students with scores and prints values.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_STUDENTS 10
#define MAX_SCORE 100

struct student{
	int id;
	int score;
};

struct student* allocate(){
	/*Allocate memory for ten students*/
	struct student* p = malloc(sizeof(struct student) * MAX_STUDENTS);
	/*return the pointer*/
	return p;
}

void generate(struct student* students){
	/*Generate random and unique ID and scores for ten students, ID being between 1 and 10, scores between 0 and 100*/
	int unique = 0;
	for (int i = 0; i < MAX_STUDENTS; i++) {
		do {
			students[i].id = rand() % MAX_STUDENTS + 1;
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

void output(struct student* students){
	/*Output information about the ten students in the format:
              ID1 Score1
              ID2 score2
              ID3 score3
              ...
              ID10 score10*/
	for (int i = 0; i < MAX_STUDENTS; i++) {
		printf("%d\t%d\n", students[i].id, students[i].score);
	}
}

void summary(struct student* students){
	/*Compute and print the minimum, maximum and average scores of the ten students*/
	int min = MAX_SCORE, max = 0;
	float avg = 0.0;
	for (int i = 0; i < MAX_STUDENTS; i++) {
		if (students[i].score < min) {
			min = students[i].score;
		}
		if (students[i].score > max) {
			max = students[i].score;
		}
		avg += students[i].score;
	}
	avg = avg / MAX_STUDENTS;
	printf("Min: %d\nMax: %d\nAvg: %.2f\n", min, max, avg);
}

void deallocate(struct student* stud){
	/*Deallocate memory from stud*/
	if (stud != NULL) {
		free(stud);
	}
}

int main(){
	struct student* stud = NULL;
	/*call allocate*/
	stud = allocate();
	/*call generate*/
	generate(stud);
	/*call output*/
	output(stud);
	/*call summary*/
	summary(stud);
	/*call deallocate*/
	deallocate(stud);
	return 0;
}
