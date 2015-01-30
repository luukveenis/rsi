#ifndef PROCESSING_H_
#define PROCESSING_H_

#include "process_list.h"

typedef struct command *cmd_ptr;
typedef struct command {
  int argc;
  char **argv;
  int background;
} cmd;

int tokenize(char*, cmd_ptr);
void process(char*, llist_ref);

#endif
