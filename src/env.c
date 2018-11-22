#include "env.h"

void read_env_args(struct arguments *arguments)
{
    char *host = getenv("SNCMPC_HOST");
    char *port_str = getenv("SNCMPC_PORT");
    if (host) {
	host = host_ok(host);
	if (!host) {
	    print_host_errmsg(0, "$SNCMPC_HOST");
	    exit(EXIT_ARGS);
	}
	arguments->host = host;
    } else {
#if defined(DEBUG)
	puts("Ignoring $SNCMPC_HOST as it is not set...");
#endif
    }
    if (port_str) {
	int port = port_ok(port_str);
	if (port >= 0) {
	    arguments->port = port;
	} else {
	    print_port_errmsg(port, "$SNCMPC_PORT");
	    exit(EXIT_ARGS);
	}
    } else {
#if defined DEBUG
	puts("Ignoring $SNCMPC_PORT as it is not set...");
#endif
    }
}
