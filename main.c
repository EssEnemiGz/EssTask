#include <stdio.h>
#include <termios.h>
#include <sys/stat.h>
#include "getkey.h"

char* manager(const char* filename, const char* operation, const char* content) {
	// If error
	if (operation == "append" && content == NULL) {return "\033[1;31mERROR:\033[0m THE CONTENT CAN'T BE NULL WITH APPEND OPERATION";};

	if (operation == "read"){
		FILE* f = fopen(filename, "r");
		struct stat st;
		stat(filename, &st);

		const size_t SIZE = st.st_size;
		char buff[SIZE];
		fread(buff, 1, SIZE, f);
		fclose(f);

		return buff;
	}else if (operation == "append"){
		FILE* f = fopen(filename, "a");
		struct stat st;
		stat(filename, &st);

		const size_t SIZE = st.st_size;
		char buff[SIZE];
		fwrite(content, 1, SIZE, f);
		fclose(f);

		return "DONE";
	};
};

int main() {
	puts(manager("./test", "read", NULL));

	struct termios tty, old;
	tcgetattr(0, &old);
	tty = old;
	cbreak(&tty);
	struct Key key;
	for (;;) {
		getkey(&key);
		if (!key.key) {
			if (key.esc) {
				break;
			}else if (key.arrow==1) {
				puts("UP");
			}else if (key.arrow==2) {
				puts("DOWN");
			};
		} else {
			if (key.key == 10) {puts("ENTER");continue;}
			else if (key.key == ' ') {puts("SPACE");continue;}
			puts("ANOTHER KEY");
		}
	}
	tcsetattr(0, TCSADRAIN, &old);
}
