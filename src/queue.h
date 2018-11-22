#ifndef QUEUE_H
#define QUEUE_H

#define INITIAL_QUEUE_LEN 8
#define MAX_FIELD_WIDTH(S, NS) ((NS) - (S) - 1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>
#include <mpd/client.h>

#include "utf8.h"
#include "config.h"
#include "win.h"

struct queue_song {
    char *artist;
    char *track;
    char *title;
    char *album;
    char *duration;
};

struct bounds {
    int artist;
    int track;
    int title;
    int album;
    int duration;
    int stop;
};

#define FLAG_REDISPLAY 0x1
#define FLAG_RECHGATTR 0x2
#define FLAG_BOUNDS 0x4
#define FLAG_MOVE_START 0x8

struct queue_flags {
    int need_redisplay;
    int need_rechgattr;
    int need_recalc_bounds;
    int need_move_start;
};

int fetch_queue(struct queue_song **dest, struct mpd_connection *conn,
		int queue_len);
int display_queue(struct mpd_connection *conn);
void display_song(WINDOW *win, struct queue_song *song, int y,
	       struct bounds *bounds, int is_cur);
void display_songs(WINDOW *win, struct queue_song *songs, int n_songs,
		   struct bounds *bounds, int playing_pos, int start_pos);
void chgattr_songs(WINDOW *win, int n_songs, struct bounds *bounds,
		   int playing_pos, int selected_pos, int start_pos);
void display_field(WINDOW *win, char *value, int y, int x, int max_width);
void color_row(WINDOW *win, int y, struct bounds *bounds, attr_t attrs);

#endif /* QUEUE_H */
