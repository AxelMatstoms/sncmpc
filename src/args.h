#ifndef ARGS_H
#define ARGS_H

#define EXIT_ARGS 2

struct arguments;

#include <stdlib.h>

#include <argp.h>

#include "config.h"
#include "env.h"
#include "validate.h"

struct arguments {
    char *host;
    int port;
    int timeout;
};

void parse_args(struct arguments *arguments, int argc, char **argv);
//static error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif /* ARGS_H */
