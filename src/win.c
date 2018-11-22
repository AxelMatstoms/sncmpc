#include "win.h"

WINDOW *main_win = NULL;
WINDOW *status_win = NULL;

int resize_windows()
{
    int maxx, maxy;
    getmaxyx(stdscr, maxy, maxx);
    int err = 0;
    err += wresize(main_win, maxy - STATUS_HEIGHT + 1, maxx);
    err += mvwin(status_win, maxy - (STATUS_HEIGHT - 1), 0);
    err += wresize(status_win, 3, maxx);
    return err;
}

int setup_wins()
{
    int maxx, maxy;
    getmaxyx(stdscr, maxy, maxx);
    main_win = newwin(maxy - STATUS_HEIGHT + 1, maxx, 0, 0);
    status_win = newwin(3, maxx, maxy - (STATUS_HEIGHT - 1), 0);
    if (main_win == NULL || status_win == NULL) {
	return 1;
    }
    return 0;
}
