#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, HashMap* map)
{
	if (!file) {
		printf("Error opening file.");
	}
	char* word;
	while (word = nextWord(file)) {
		hashMapPut(map, word, 1);
		free(word);
	}
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
    HashMap* map = hashMapNew(1000);

	int idx;
	HashLink* link;
	char fn[150] = PATH;	//	for visual studio, it needs the path before it
	strcat(fn, "dictionary.txt");	//	how do I avoid having to do this, or not possible to avoid?
    FILE* file = fopen(fn, "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);
    
    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        
		if (hashMapContainsKey(map, inputBuffer)) {
			printf("Looks good to me.\n");
		}
		else {
			idx = HASH_FUNCTION(inputBuffer) % hashMapCapacity(map);
			if (idx < 0) { idx += hashMapCapacity(map); }
			link = map->table[idx];
			printf("Did you mean:\n");
			for (int i = 0; i < 5; i++) {
				if (link) {
					printf("%s\n", link->key);
				}
				else { break; }
				link = link->next;
			}
		}

        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
    }
    
    hashMapDelete(map);
    return 0;
}