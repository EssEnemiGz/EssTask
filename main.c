#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>
#include <unistd.h>
#include "getkey.h"

int ansicode(const char* operation){ // To write ansicodes
	printf(operation);
	fflush(stdout);
};

char* manager(const char* filename, const char* operation, char* content) {
	// If error
	if (!strcmp(operation, "append") && content == NULL) {return "\033[1;31mERROR:\033[0m THE CONTENT CAN'T BE NULL WITH APPEND OPERATION";};

	if (!strcmp(operation, "read")){
		FILE* f = fopen(filename, "r");
		struct stat st;
		stat(filename, &st); // Creating a struct with the file size

		const size_t SIZE = st.st_size;
		char buff[SIZE];
		fread(buff, 1, SIZE, f);
		fclose(f);

		char *p = calloc(SIZE, 1); // Save the info in heap
		strcpy(p, buff);

		return p;
	}else if (!strcmp(operation, "append")){
		FILE* f = fopen(filename, "a");

		char *str = calloc(strlen(content)+6, 1); // Save the info heap
		strcpy(str, "[ ] "); // Copy 
		strcat(str, content); // Concat
		strcat(str, "\n"); // Concat
		const size_t SIZE = strlen(str);

		fwrite(str, 1, SIZE, f);
		fclose(f);

		return "DONE";
	};
};

int* get_pos(){
	fflush(stdin);
	ansicode("\033[6n");
	int yx[2] = {0,0};
	int c=-1;

	for (;;){
		char buff[1];
		read(0, buff, 1);
		if (buff[0] == 'R') break;
		if (buff[0] == '[' || buff[0] == ';') {c++;continue;}
		if (c==-1) continue;
		yx[c] *= 10; yx[c] += buff[0]-48;
	};
	
	int *p = calloc(2, 1); // Save the info in heap
	p[0] = yx[0]; p[1] = yx[1];

	return p;
};

int main() {	
	ansicode("\033[2J\033[H"); // Cleaning the terminal
	char *p = manager("./tasks", "read", NULL);
	puts(p); // Print the output
	free(p); // Free the heap
	ansicode("\033[1C");
	ansicode("\033[2A");
	
	struct termios tty, old;
	tcgetattr(0, &old);
	tty = old;
	cbreak(&tty);
	struct Key key;
	for (;;) {
		getkey(&key);
		if (!key.key) {
			get_pos();
			if (key.esc) {
				ansicode("\033[2J\033[0m");
				break;
			}else if (key.arrow==1) {
				ansicode("\033[1A");
			}else if (key.arrow==2) {
				ansicode("\033[1B");
			};
		} else {
			if (key.key == 10) {ansicode("\033[1C\033[1A");continue;}
			else if (key.key == ' ') continue
			puts("ANOTHER KEY");
		}
	}
	tcsetattr(0, TCSADRAIN, &old);
}
