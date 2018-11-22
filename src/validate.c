#include "validate.h"

char *host_ok(char *host)
{
    if (host != NULL && strlen(host)) {
	return host;
    } else {
	return NULL;
    }
}

int port_ok(char *portstr)
{
    char *ptr;
    errno = 0;
    long port = strtol(portstr, &ptr, 10);
    if ((errno == ERANGE && (port == LONG_MAX || port == LONG_MIN))
	|| (errno != 0 && port == 0)) {
	return E_PORT_RANGE;
    }
    if (ptr == portstr) {
	return E_PORT_NODIGIT;
    }
    if (port < 0 || port >= MAX_PORT) {
	return E_PORT_RANGE;
    }
    if (*ptr != '\0') {
	return E_PORT_SUFFIX;
    }
    return port;
}

void print_host_errmsg(int status, char *name)
{
    switch (status) {
    case 0:
	printf("Error: %s is empty", name);
	break;
    default:
	printf("Error: an unknown error occured when parsing %s", name);
    }
}

void print_port_errmsg(int status, char *name)
{
    switch (status) {
    case E_PORT_STRTOL:
	printf("Error: %s is not a number or does not fit in long\n", name);
	break;
    case E_PORT_NODIGIT:
	printf("Error: %s contained no digits\n", name);
	break;
    case E_PORT_RANGE:
	printf("Error: %s is out of range (0-65535)\n", name);
	break;
    case E_PORT_SUFFIX:
	printf("Error: %s contains additional chars after port\n", name);
	break;
    default:
	printf("Error: an unknown error occurred when parsing %s\n", name);
	break;
    }
}
