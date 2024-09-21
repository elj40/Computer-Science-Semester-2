#include <string.h>
#include <stdbool.h>

#include "utils.h"

void split_string(char *buffer[], int buf_len, char *string, const char *delims) {
	char* tok = strtok(string, delims);
	int tc = 0;

	while (tok != NULL && tc < buf_len) {
		buffer[tc] = tok;
		tc++;
		tok = strtok(NULL, delims);
	}
}

bool string_to_int(int *n, char *s) {
	int i = 0;
	*n = 0;
	char c;
	while (1) {
		c = *(s+i);	
		if (c == '\0') break;

		bool is_digit = '0' <= c && c <= '9';
		if (!is_digit) return false;

		int digit = c - '0';
		*n *= 10;
		*n += digit;
		i++;
	}
	return true;
}
