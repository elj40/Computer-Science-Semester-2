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
	if (b) assert(n == 12345 && "Number not correct");

	b = string_to_int(&n, "12f45");
	assert(!b && "Boolean not correct");
	if (b) assert(n == 12345 && "Number not correct");

	float f;
	float eps = 0.00001;

	b = string_to_float(&f, "213");
	assert(b && "string_to_float failed");
	if (b) assert(f == 213.0 && "Number not correct");

	b = string_to_float(&f, "213.12");
	assert(b && "string_to_float failed");
	printf("%f\n", f);
	if (b) assert(f > 213.12 - eps && f < 213.2 + eps && "Number not correct");

	b = string_to_float(&f, "0.123456");
	assert(b && "string_to_float failed");
	printf("%f\n", f);
	if (b) assert(f > 0.123456 - eps && f < 0.123456 + eps && "Number not correct");

	
	b = string_to_float(&f, "0.123.456");
	assert(!b && "string_to_float failed");
	printf("%f\n", f);
	if (b) assert(f > 0.123456 - eps && f < 0.123456 + eps && "Number not correct");
	return 0;
}

