#include "utf8.h"

int utf8_strlen(char *utf8_str)
{
    int n = 0;
    char *ptr = utf8_str;
    while (*ptr != '\0') {
	if ((*ptr & 0x80) == 0) {
	    ++ptr;
	    ++n;
	} else if ((*ptr & 0xe0) == 0xc0) {
	    ptr += 2;
	    ++n;
	} else if ((*ptr & 0xf0) == 0xe0) {
	    ptr += 3;
	    ++n;
	} else if ((*ptr & 0xf8) == 0xf0) {
	    ptr += 4;
	    ++n;
	}
    }
    return n;
}

int utf8_strnlen(char *utf8_str, int ncp)
{
    int n = 0;
    char *ptr = utf8_str;
    while (n < ncp && *ptr != '\0') {
	if ((*ptr & 0x80) == 0) {
	    ptr += 1;
	    ++n;
	} else if ((*ptr & 0xe0) == 0xc0) {
	    ptr += 2;
	    ++n;
	} else if ((*ptr & 0xf0) == 0xe0) {
	    ptr += 3;
	    ++n;
	} else if ((*ptr & 0xf8) == 0xf0) {
	    ptr += 4;
	    ++n;
	}
    }
    return (int)(ptr - utf8_str);
}
