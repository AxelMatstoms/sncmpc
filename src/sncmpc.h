#ifndef SNCMPC_H
#define SNCMPC_H

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include <curses.h>
#include <mpd/client.h>

#include "args.h"
#include "config.h"
#include "validate.h"
#include "env.h"
#include "queue.h"
#include "win.h"

int main(int argc, char **argv);
void print_mpd_version(struct mpd_connection *conn);
int handle_error(struct mpd_connection *conn);
void print_tag(struct mpd_song *song, enum mpd_tag_type type, char *label);
void setup_screen();
//int setup_wins(WINDOW **main, WINDOW **status);
void init_pairs();
#endif /* SNCMPC_H */
