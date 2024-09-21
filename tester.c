#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include "validate.h"
#include "utils.h"
#include "display.h"

int main(int argc, char* argv[]) {

	int n;
	bool b;

	b = string_to_int(&n, "12345");
	assert(b && "Boolean not correct");
	if (b) assert(n = 12345 && "Number not correct");

	b = string_to_int(&n, "12f45");
	assert(!b && "Boolean not correct");
	if (b) assert(n = 12345 && "Number not correct");

	return 0;
}

