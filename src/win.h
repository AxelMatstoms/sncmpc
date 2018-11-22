#ifndef WIN_H
#define WIN_H

#include <curses.h>

#define STATUS_HEIGHT 4

extern WINDOW *main_win;
extern WINDOW *status_win;

int resize_windows();
int setup_wins();

#endif /* WIN_H */
