#include "args.h"

static char doc[] =
    "sncmpc - simple ncurses music player client";

static char args_doc[] = "";

static struct argp_option options[] = {
    {"host",    'h', "HOST", 0,
     "Connect to HOST instead of host defined in config.h"},
    {"port",    'p', "PORT", 0,
     "Connect to PORT instead of port defined in config.h"},
    {"timeout", 't', "TIMEOUT", 0,
     "Use TIMEOUT ms as timeout instead of timeout defined in config.h"},
    { 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key) {
    case 'h':
	if (!host_ok(arg)) {
	    print_host_errmsg(0, "--host");
	    exit(EXIT_ARGS);
	}
	arguments->host = arg;
	break;
    case 'p':
    {
	int port = port_ok(arg);
	if (port < 0) {
	    print_port_errmsg(port, "--port");
	    exit(EXIT_ARGS);
	}
	arguments->port = port;
	break;
    }
    case 't':
    {
	char *ptr;
	arguments->timeout = strtol(arg, &ptr, 10);
	break;
    }
    case ARGP_KEY_ARG:
	/* fuck you *dab* */
	argp_usage(state);
	break;
    case ARGP_KEY_END:
	break;
    default:
	return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

void parse_args(struct arguments *arguments, int argc, char **argv)
{
    arguments->host = MPD_HOST;
    arguments->port = MPD_PORT;
    arguments->timeout = MPD_TIMEOUT_MS;
    read_env_args(arguments);
    argp_parse(&argp, argc, argv, 0, 0, arguments);
}
