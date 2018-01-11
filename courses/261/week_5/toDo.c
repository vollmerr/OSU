/*
 * CS 261 Assignment 5
 * Name:
 * Date:
 */

#include "dynamicArray.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//#define VISUAL_STUDIO
#ifdef VISUAL_STUDIO
#define PATH "C:\\Users\\goon\\Documents\\cs_261\\week_1\\A1\\A1\\A5\\"
#else
#define PATH "./"
#endif

 /**
  * Loads into heap a list from a file with lines formatted like
  * "priority, name".
  * @param heap
  * @param file
  */
void listLoad(DynamicArray* heap, FILE* file) {
	enum { FORMAT_LENGTH = 256 };
	char format[FORMAT_LENGTH];
	snprintf(format, FORMAT_LENGTH, "%%d, %%%d[^\n]", TASK_NAME_SIZE);

	Task temp;
	while (fscanf(file, format, &temp.priority, &temp.name) == 2) {
		dyHeapAdd(heap, taskNew(temp.priority, temp.name), taskCompare);
	}
}

/**
 * Writes to a file all tasks in heap with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listSave(DynamicArray* heap, FILE* file) {
	for (int i = 0; i < dySize(heap); i++) {
		Task* task = dyGet(heap, i);
		fprintf(file, "%d, %s\n", task->priority, task->name);
	}
}

/**
 * Prints every task in heap.
 * @param heap
 */
void listPrint(DynamicArray* heap) {
	DynamicArray* temp = dyNew(1);
	dyCopy(heap, temp);
	while (dySize(temp) > 0) {
		Task* task = dyHeapGetMin(temp);
		printf("\n");
		taskPrint(task);
		printf("\n");
		dyHeapRemoveMin(temp, taskCompare);
	}
	dyDelete(temp);
}

/**
 * Handles the given command.
 * @param list
 * @param command
 */
void handleCommand(DynamicArray* list, char command) {
	FILE* file = NULL;
	char fn[100] = PATH;	// added for visual studio.
	char tmp[50];
	int priority = 0;
	switch (command) {
		/*  LOAD FILE  */
	case 'l':
		printf("Load File: ");
		scanf("%s", tmp);
		strcat(fn, tmp);
		file = fopen(fn, "r");

		if (file) {
			listLoad(list, file);
			printf("File %s loaded", fn);
		}
		else printf("Error opening %s.", fn);
		while (getchar() != '\n');
		break;
		/*  SAVE FILE  */
	case 's':
		printf("Save As: ");
		scanf("%s", tmp);
		strcat(fn, tmp);
		file = fopen(fn, "w");

		if (file) {
			listSave(list, file);
			printf("File %s saved", fn);
		}
		else printf("Error saving %s.", fn);
		while (getchar() != '\n');
		break;
		/*  ADD TASK  */
	case 'a':
		printf("New Task Name: ");
		scanf("%[^\n]%*c", tmp);
		printf("New Task Priority [int]: ");
		scanf("%d", &priority);
		dyHeapAdd(list, taskNew((int)priority, (char*)tmp), taskCompare);
		printf("%s has been added.", tmp);
		while (getchar() != '\n');
		break;
		/*  GET FIRST TASK  */
	case 'g':
		if (dySize(list)) {
			printf("First Task: ");
			taskPrint(dyHeapGetMin(list));
		}
		else printf("Nothing in list.");
		break;
		/*  REMOVE FIRST TASK  */
	case 'r':
		if (dySize(list)) {
			printf("Removed: ");
			taskPrint(dyHeapGetMin(list));
			dyHeapRemoveMin(list, taskCompare);
		}
		else printf("Nothing in list.");
		break;
		/*  PRINT ALL TASKS  */
	case 'p':
		if (dySize(list)) listPrint(list);
		else printf("Nothing in list.");
		break;
		/*  EXIT  */
	case 'e':
		printf("Exiting...");
		break;
	default:
		break;
	}
	if (file) fclose(file);
}

int main() {

	// Implement
	printf("\n\n** TO-DO LIST APPLICATION **\n\n");
	DynamicArray* list = dyNew(8);
	char command = ' ';
	do {
		printf("\n\nPress:\n"
			"'l' to load to-do list from a file\n"
			"'s' to save to-do list to a file\n"
			"'a' to add a new task\n"
			"'g' to get the first task\n"
			"'r' to remove the first task\n"
			"'p' to print the list\n"
			"'e' to exit the program\n\n"
			);
		command = getchar();
		// Eat newlines
		while (getchar() != '\n');
		handleCommand(list, command);
	} while (command != 'e');
	dyDelete(list);

	return 0;
}