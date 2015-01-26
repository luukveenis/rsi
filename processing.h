#ifndef PROCESSING_H_
#define PROCESSING_H_

typedef struct command *cmd_ptr;
typedef struct command {
  int argc;
  char **argv;
  int background;
} cmd;

int tokenize(char*, struct command*);
void process(char*);

#endif
