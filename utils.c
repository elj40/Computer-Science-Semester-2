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
	int x = 0;
	char c;
	while (1) {
		c = *(s+i);	
		if (c == '\0') break;

		bool is_digit = '0' <= c && c <= '9';
		if (!is_digit) return false;

		int digit = c - '0';
		x *= 10;
		x += digit;
		i++;
	}

	*n = x;
	return true;
}

bool string_to_float(float *f, char *s) {
	int i = 0;
	int pow_10 = 0;
	bool decimal_seen = false;

	float x = 0;
	char c;
	while (1) {
		c = *(s+i);	
		if (c == '\0') break;

		bool is_digit = '0' <= c && c <= '9';
		bool is_point = c == '.';
		if (!is_digit && !is_point) return false;
		if (is_point && decimal_seen) return false;

	
		if (decimal_seen) pow_10--;
		if (is_point && !decimal_seen) {
			decimal_seen = true;
			i++;
			continue;
		}

		int digit = c - '0';
		x *= 10;
		x += digit;
		i++;
	}

	for (int i = 0; i > pow_10; i--) {
		x /= 10;
	}
	*f = x;
	return true;
}
