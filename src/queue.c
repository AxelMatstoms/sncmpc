#include "queue.h"

int fetch_queue(struct queue_song **dest, struct mpd_connection *conn,
		int queue_len)
{
    /*
    int err;
    FILE *f = fopen("log", "a");
    err = mpd_command_list_begin(conn, 1);
    fprintf(f, "e0: %d, msg: %s\n", err, mpd_connection_get_error_message(conn));
    err = mpd_send_list_queue_meta(conn);
    fprintf(f, "e1: %d, msg: %s\n", err, mpd_connection_get_error_message(conn));

    err = mpd_command_list_end(conn);
    fprintf(f, "e2: %d, msg: %s\n", err, mpd_connection_get_error_message(conn));
    */
    int err = mpd_send_list_queue_meta(conn);

    int n = queue_len;
    int i = 0;
    struct queue_song *songs = malloc(sizeof(struct queue_song) * n);
    struct mpd_song *song;
    struct queue_song *cur;
    while ((song = mpd_recv_song(conn)) != NULL) {
	// Realloc if there are more songs than fit in the array
	// SHOULD NOT HAPPEN IF CALLEE PROVIDES CORRECT QUEUE_LEN 
	if (i + 1 >= n) {
	    songs = realloc(songs, sizeof(struct queue_song) * (n *= 2));
	}
	cur = songs + i;
	
	// Copy artist to queue_song struct
	const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
	if (artist != NULL) {
	    cur->artist = malloc(strlen(artist) + 1);
	    strcpy(cur->artist, artist);
	} else {
	    cur->artist = NULL;
	}
	
	// Write track to queue_song struct
	const char *track = mpd_song_get_tag(song, MPD_TAG_TRACK, 0);
	if (track != NULL) {
	    char *ptr;
	    int track_no = strtol(track, &ptr, 10);
	    cur->track = malloc(5);
	    snprintf(cur->track, 4, "%02d", track_no);
	} else {
	    cur->artist = NULL;
	}
	
	// Write title to queue_song struct
	const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
	if (title != NULL) {
	    cur->title = malloc(strlen(title) + 1);
	    strcpy(cur->title, title);
	} else {
	    cur->title = NULL;
	}

	// Write album to queue_song struct
	const char *album = mpd_song_get_tag(song, MPD_TAG_ALBUM, 0);
	if (album != NULL) {
	    cur->album = malloc(strlen(album) + 1);
	    strcpy(cur->album, album);
	} else {
	    cur->album = NULL;
	}

	// Write duration to queue_song struct
	const unsigned duration = mpd_song_get_duration(song);
	if (duration != 0) {
	    cur->duration = malloc(10);
	    unsigned min = duration / 60;
	    unsigned sec = duration % 60;
	    if (duration < 3600) {
		sprintf(cur->duration, "%02hhu:%02hhu", min, sec);
	    } else {
		unsigned hrs = min / 60;
		min = min % 60;
		sprintf(cur->duration, "%02hhu:%02hhu:%02hhu", hrs, min, sec);
	    }
	} else {
	    cur->duration = NULL;
	}
	mpd_song_free(song);
	i++;
    }
    mpd_response_finish(conn);
    songs = realloc(songs, sizeof(struct queue_song) * i);
    *dest = songs;
    return i;
}

struct bounds *calculate_bounds(WINDOW *win)
{
    int maxx, maxy;
    getmaxyx(win, maxy, maxx);
    struct bounds *bounds = malloc(sizeof(struct bounds));
    int left = maxx - 15;
    int part = left / 5;
    bounds->artist = 0;
    bounds->track = part + (left - part * 5);
    bounds->title = bounds->track + 6;
    bounds->album = bounds->title + 2 * part;
    bounds->duration = bounds->album + 2 * part;
    bounds->stop = maxx;
    return bounds;
}

int display_queue(struct mpd_connection *conn)
{
    mpd_send_status(conn);
    
    struct mpd_status *status;
    status = mpd_recv_status(conn);
    unsigned queue_len = mpd_status_get_queue_length(status);
    int curr_song_pos = mpd_status_get_song_pos(status);

    mpd_status_free(status);
    mpd_response_finish(conn);
    
    struct queue_song *songs;
    int n_songs = fetch_queue(&songs, conn, queue_len);

    int selected_pos = curr_song_pos;
    //struct bounds bounds = {0, 20, 25, 50, 70, 80};
    
    
    struct bounds *bounds = calculate_bounds(main_win);
    
    int lines, cols;
    getmaxyx(main_win, lines, cols);
    int key = 0;
    int start_pos = 0;
    /*
    struct queue_flags flags = { .need_redisplay = 1,
				 .need_rechgattr = 1,
				 .need_recalc_bounds = 0,
				 .need_move_start = 1};
    */
    /*
    int need_redisplay = 1;
    int need_rechgattr = 1;
    int need_recalc_bounds = 0;
    */
    int flags = FLAG_REDISPLAY | FLAG_RECHGATTR | FLAG_MOVE_START;
    do {
	if (flags & FLAG_BOUNDS) {
	    getmaxyx(main_win, lines, cols);
	    if (bounds != NULL) {
		free(bounds);
	    }
	    bounds = calculate_bounds(main_win);
	    flags &= ~FLAG_BOUNDS;
	}
	if (flags & FLAG_MOVE_START) {
	    start_pos = selected_pos - lines / 2;
	    if ((start_pos + lines) > n_songs) {
		start_pos = n_songs - lines;
	    }
	    if (start_pos < 0) {
		start_pos = 0;
	    }
	    flags |= FLAG_REDISPLAY | FLAG_RECHGATTR;
	    flags &= ~FLAG_MOVE_START;
	}
	if (flags & FLAG_REDISPLAY) {
	    werase(main_win);
	    display_songs(main_win, songs, n_songs, bounds, curr_song_pos, start_pos);
	    flags &= ~FLAG_REDISPLAY;
	}
	if (flags & FLAG_RECHGATTR) {
	    chgattr_songs(main_win, n_songs, bounds, curr_song_pos, selected_pos, start_pos);
	    flags &= ~FLAG_RECHGATTR;
	}
	key = getch();
	switch (key) {
	case 'j':
	case KEY_DOWN:
	    selected_pos++;
	    if (selected_pos >= n_songs) {
		selected_pos = n_songs - 1;
	    }
	    if (selected_pos >= (start_pos + lines)) {
		flags |= FLAG_MOVE_START;
	    }
	    flags |= FLAG_RECHGATTR;
	    break;
	case 'k':
	case KEY_UP:
	    selected_pos--;
	    if (selected_pos < 0) {
		selected_pos = 0;
	    }
	    if (selected_pos < start_pos) {
		flags |= FLAG_MOVE_START;
	    }
	    flags |= FLAG_RECHGATTR;
	    break;
	case KEY_RESIZE:
	    resize_windows();
	    flags |= FLAG_REDISPLAY | FLAG_RECHGATTR | FLAG_BOUNDS;
	    break;
	case 'p':
	    mpd_run_toggle_pause(conn);
	    break;
	case '\n':
	    mpd_run_play_pos(conn, selected_pos);
	    break;
	case 'g':
	    selected_pos = 0;
	    flags |= FLAG_MOVE_START;
	    break;
	case 'G':
	    selected_pos = n_songs - 1;
	    flags |= FLAG_MOVE_START;
	    break;
	case 'H':
	    selected_pos = start_pos;
	    flags |= FLAG_RECHGATTR;
	    break;
	case 'M':
	    selected_pos = start_pos + lines / 2;
	    flags |= FLAG_RECHGATTR;
	    break;
	case 'L':
	    selected_pos = start_pos + lines - 1;
	    flags |= FLAG_RECHGATTR;
	    break;
	}
    } while(key != 'q');
}

void display_songs(WINDOW *win, struct queue_song *songs, int n_songs,
		   struct bounds *bounds, int playing_pos, int start_pos)
{
    FILE *f = fopen("log", "a");
    fprintf(f, "dssp: %d\n", start_pos);
    fclose(f);
    int maxx, maxy;
    getmaxyx(win, maxx, maxy);
    int songs_after_start = n_songs - start_pos;
    maxy = (songs_after_start < maxy) ? songs_after_start : maxy;
    for (int y = 0; y < maxy; y++) {
	int i = y + start_pos;
	int is_playing = (i == playing_pos);	
	display_song(win, songs + i, y, bounds, is_playing);
    }
    wrefresh(win);
}

void chgattr_songs(WINDOW *win, int n_songs, struct bounds *bounds,
		   int playing_pos, int selected_pos, int start_pos)
{
    int maxx, maxy;
    getmaxyx(win, maxx, maxy);
    int songs_after_start = n_songs - start_pos;
    maxy = (songs_after_start < maxy) ? songs_after_start : maxy;
    for (int y = 0; y < maxy; y++) {
	int i = y + start_pos;
	int is_current = (i == playing_pos);
	attr_t attrs = A_NORMAL;
	if (i == selected_pos) {
	    attrs |= A_REVERSE;
	}
	if (is_current) {
	    attrs |= A_BOLD;
	}
	color_row(win, y, bounds, attrs);
    }
    wrefresh(win);
}
		   

void display_song(WINDOW *win, struct queue_song *song, int y,
	       struct bounds *bounds, int is_cur)
{
    display_field(win, song->artist, y, bounds->artist,
		  MAX_FIELD_WIDTH(bounds->artist, bounds->track));
    display_field(win, song->track, y, bounds->track,
		  MAX_FIELD_WIDTH(bounds->track, bounds->title));
    char *title = malloc(strlen(song->title) + 3);
    title[0] = '\0';
    if (is_cur) {
	strcat(title, "* ");
    }
    strcat(title, song->title);
    display_field(win, title, y, bounds->title,
		 MAX_FIELD_WIDTH(bounds->title, bounds->album));
    free(title);
    display_field(win, song->album, y, bounds->album,
		  MAX_FIELD_WIDTH(bounds->album, bounds->duration));
    display_field(win, song->duration, y, bounds->duration,
		  MAX_FIELD_WIDTH(bounds->duration, bounds->stop));
}

void color_row(WINDOW *win, int y, struct bounds *bounds, attr_t attrs)
{
    mvwchgat(win, y, bounds->artist,
	     MAX_FIELD_WIDTH(bounds->artist, bounds->track) + 1, attrs,
	     ARTIST_PAIR, NULL);
    mvwchgat(win, y, bounds->track,
	     MAX_FIELD_WIDTH(bounds->track, bounds->title) + 1, attrs,
	     TRACK_PAIR, NULL);
    mvwchgat(win, y, bounds->title,
	     MAX_FIELD_WIDTH(bounds->title, bounds->album) + 1, attrs,
	     TITLE_PAIR, NULL);
    mvwchgat(win, y, bounds->album,
	     MAX_FIELD_WIDTH(bounds->album, bounds->duration) + 1, attrs,
	     ALBUM_PAIR, NULL);
    mvwchgat(win, y, bounds->duration,
	     MAX_FIELD_WIDTH(bounds->duration, bounds->stop) + 1, attrs,
	     DURATION_PAIR, NULL);
}

void display_field(WINDOW *win, char *value, int y, int x, int max_width)
{
    if (value != NULL) {
	int n = utf8_strnlen(value, max_width);
	mvwaddnstr(win, y, x, value, n);
    } else {
	mvwaddnstr(win, y, x, "n/a", max_width);
    }
}
