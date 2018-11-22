#include "sncmpc.h"

const char *argp_program_version =
    "sncmpc 0.1.0";
const char *argp_program_bug_address =
    "<axel.matstoms@gmail.com>";

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    struct arguments arguments;
    parse_args(&arguments, argc, argv);
    
    struct mpd_connection *conn;
    conn = mpd_connection_new(arguments.host, arguments.port, arguments.timeout);
    if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
	return handle_error(conn);
    }

    setup_screen();
    //addstr("fuck me in the arse");
    refresh();
    int s = setup_wins();
    
    //waddstr(main_win, "Hello World *dabs on curses*");
    mvwaddstr(status_win, 0, 0, "===============================>----------------------");
    mvwaddstr(status_win, 1, 20, "-| Return of the Black Placeholder |-");
    mvwaddstr(status_win, 2, 20, "Lorem Ipsum - Placeholder of the Black Death (1998)");
    wrefresh(status_win);
    display_queue(conn);
    wrefresh(main_win);
    refresh();

    FILE *f = fopen("log", "a");
    //fprintf(f, "setup_wins rEtURnEd SuCcEsSfuLlY: %d\n", s);
    fputs("disp_queue done\n", f);
    fclose(f);
    /*
    print_mpd_version(conn);
    struct mpd_status *status;
    struct mpd_song *song;

    mpd_command_list_begin(conn, 1);
    mpd_send_status(conn);
    mpd_send_current_song(conn);
    mpd_send_toggle_pause(conn);
    mpd_command_list_end(conn);

    status = mpd_recv_status(conn);
    if (status == NULL) {
	return handle_error(conn);
    }

    printf("volume: %d%%\n", mpd_status_get_volume(status));
    int state = mpd_status_get_state(status);
    if (state == MPD_STATE_PLAY) {
	puts("Playing");
    } else if (state == MPD_STATE_PAUSE) {
	puts("Paused");
    }
    mpd_status_free(status);
    mpd_response_next(conn);

    while ((song = mpd_recv_song(conn)) != NULL) {
	print_tag(song, MPD_TAG_ARTIST, "artist");
	print_tag(song, MPD_TAG_ALBUM, "album");
	print_tag(song, MPD_TAG_TITLE, "title");
	print_tag(song, MPD_TAG_TRACK, "track");
	mpd_song_free(song);
    }
    */
    
    mpd_connection_free(conn);
    endwin();
}

void print_tag(struct mpd_song *song, enum mpd_tag_type type, char *label)
{
    const char *value;
    int i = 0;
    while ((value = mpd_song_get_tag(song, type, i++)) != NULL) {
	printf("%s: %s\n", label, value);
    }
}

int handle_error(struct mpd_connection *conn)
{
    fprintf(stderr, "%s\n", mpd_connection_get_error_message(conn));
    mpd_connection_free(conn);
    return EXIT_FAILURE;
}

void print_mpd_version(struct mpd_connection *conn)
{
    const unsigned *version = mpd_connection_get_server_version(conn);
    printf("Connected to mpd v%u.%u.%u\n", version[0], version[1], version[2]);
}

void setup_screen()
{
    initscr();
    cbreak();
    noecho();
    //nonl();
    //raw();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    use_default_colors();
    init_pairs();
}

void init_pairs()
{
    for (int fg = 0; fg <= 7; fg++) {
	init_pair(fg + 1, fg, -1);
    }
}
