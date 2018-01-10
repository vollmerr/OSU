/* CS261- Assignment 1 - Q.5*/
/* Name:	Ryan Vollmer
 * Date:	09/28/16
 * Solution description:	Prints enetred text as Sticky Caps.
 */
 
#include <stdio.h>
#include <stdlib.h>

/*converts ch to upper case, assuming it is in lower case currently*/
char toUpperCase(char ch){
     return ch-'a'+'A';
}

/*converts ch to lower case, assuming it is in upper case currently*/
char toLowerCase(char ch){
     return ch-'A'+'a';
}

void sticky(char* word){
	/*Convert to sticky caps*/
	int i = 0;
	while (word[i] != '\0') {
		if (i % 2) {
			if (word[i] < 'a') {	// is uppercase	
				word[i] = toLowerCase(word[i]);
			}
		}
		else {
			if (word[i] > 'Z') {	// is lowercase
				word[i] = toUpperCase(word[i]);
			}
		}
		i++;
	}
}

int main(){
	/*Read word from the keyboard using scanf*/
	char* str;
	str = malloc(sizeof(int)*100);	
	scanf("%s", str);
	/*Call sticky*/
    	sticky(str);
	/*Print the new word*/
    	printf("%s\n", str);
	free(str);
	return 0;
}
