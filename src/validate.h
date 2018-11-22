#ifndef VALIDATE_H
#define VALIDATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "sncmpc.h"

#define MAX_PORT 65536
#define E_PORT_STRTOL -1
#define E_PORT_NODIGIT -2
#define E_PORT_RANGE -3
#define E_PORT_SUFFIX -4

/*
enum level {
    LEVEL_FATAL,
    LEVEL_ERROR,
    LEVEL_WARN,
    LEVEL_INFO,
    LEVEL_DEBUG,
    LEVEL_ALL
    };*/

char *host_ok(char *host);
int port_ok(char *port);
void print_host_errmsg(int status, char *name);
void print_port_errmsg(int status, char *name);

#endif /* VALIDATE_H */
