#ifndef UTILS_H
#define UTILS_H

#include "types.h"

void split_string(char *buffer[], int buf_len, char *string, const char *delims);
bool string_to_int(int *n, char *s);
bool string_to_float(float *n, char *s);
void invalid_object_setup(int l);
void bee_append_to_linked_list(Bee *head, Bee *bee);

#endif

