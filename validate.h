#ifndef VALIDATE_H
#define VALIDATE_H

#include "types.h"

#define CONFIG_LINE_LEN 50
#define CONFIG_OPTIONS_LEN 4

void invalid_object_setup(int l);
bool validate_gui_mode(int argc,char* argv[], int *gm);
bool validate_config_line(char *config_line, int config_len, int *s, Config *c);

#endif
