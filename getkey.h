#ifndef GETKEY_H
#define GETKEY_H
struct Key {
	char key;
	char esc;
	char arrow;
};
void cbreak(struct termios *tty);
void getkey(struct Key *k);
#endif
