#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <poll.h>
#include "getkey.h"

void cbreak(struct termios *tty) {
	tty->c_cc[VTIME] = 0; tty->c_cc[VMIN] = 1;
	tty->c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSADRAIN, tty);
}

void handleEsc(struct pollfd *rdfd, struct Key *k) {
	if (!poll(rdfd, 1, 5)) {
		k->esc = 1;
		return;
	}
	char key[2];
	for (int i=0; i<2; i++) {
		char ch[1];
		read(0, ch, 1);
		key[i] = ch[0];
	}
	if (key[0]==91) {
		if (key[1]==65) {
			k->arrow = 1;
		}
		else if (key[1]==66) {
			k->arrow = 2;
		}
	}
}

void getkey(struct Key *k) {
	k->arrow = 0;
	k->esc = 0;
	k->key = 0;
	struct pollfd rdfd[1];
	rdfd[0].fd = 0;
	rdfd[0].events = POLLIN;
	char ch[1];
	poll(rdfd, 1, -1);
	read(0, ch, 1);
	if (ch[0] == 27) {  /*ESC*/
		handleEsc(rdfd, k);
		return;
	}
	k->key = ch[0];
}
