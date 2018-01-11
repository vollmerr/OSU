#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>

//#define VISUAL_STUDIO
#ifdef VISUAL_STUDIO
#define PATH "C:\\Users\\goon\\Documents\\cs_261\\week_1\\A1\\A1\\A6\\"
#else
#define PATH "./"
#endif

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    const char* fileName = "input1.txt";
    if (argc > 1)
    {
        fileName = argv[1];
    }
    printf("Opening file: %s\n", fileName);
    
    clock_t timer = clock();
    
    HashMap* map = hashMapNew(10);
    
    // --- Concordance code begins here ---
    // Be sure to free the word after you are done with it here.
	int count = 0;
	char fn[150] = PATH;	//	for visual studio, it needs the path before it
	strcat(fn, fileName);	//	how do I avoid having to do this, or not possible to avoid?
	FILE* fp = fopen(fn, "r");
	if (!fp) {
		printf("Error opening file.");
		return -1;
	}
	char* word;
	while (word = nextWord(fp)) {
		if (!hashMapContainsKey(map, word)) {
			count = 1;
		}
		else {
			count = *hashMapGet(map, word) + 1;
		}
		hashMapPut(map, word, count);
		free(word);
	}
	fclose(fp);

	for (int i = 0; i < map->capacity; i++) {
		HashLink* link = map->table[i];
		if (link) {
			while (link) {
				printf("%s: %d\n", link->key, link->value);
				link = link->next;
			}
		}
	}
	// --- Concordance code ends here ---
    
    hashMapPrint(map);
    
    timer = clock() - timer;
    printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
    printf("Number of links: %d\n", hashMapSize(map));
    printf("Number of buckets: %d\n", hashMapCapacity(map));
    printf("Table load: %f\n", hashMapTableLoad(map));
    
    hashMapDelete(map);
    return 0;
}