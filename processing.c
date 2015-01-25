#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processing.h"

const char *COMMAND_SEPERATOR = " ";

cmd_ptr initialize_cmd(){
  cmd_ptr cmd_ref = (cmd_ptr) malloc(sizeof(cmd));
  cmd_ref->argc = 0;
  cmd_ref->argv = NULL;

  return cmd_ref;
}

void free_cmd(cmd_ptr cmd_ref){
  free(cmd_ref->argv);
  free(cmd_ref);
}

/* Tokenizes the user input and stores it in a struct cmd_ref
 * This is so that we can return both the parsed input
 * array, and also its length which is unknown at compile time */
int tokenize(char *input, cmd_ptr cmd_ref){
  int size = 0;
  char *token = NULL;

  for (token = strtok(input, COMMAND_SEPERATOR);
       token;
       token = strtok(NULL, COMMAND_SEPERATOR))
  {
    cmd_ref->argv = realloc(cmd_ref->argv, ++size * sizeof(char*));
    cmd_ref->argv[size-1] = token;
  }
  cmd_ref->argc = size;

  return 0;
}

void process(char *input){
  int i;
  cmd_ptr cmd_ref = initialize_cmd();
  tokenize(input, cmd_ref);

  for (i=0; i < cmd_ref->argc; i++) {
    printf("Argument %d: %s\n", i+1, cmd_ref->argv[i]);
  }

  free_cmd(cmd_ref);
}
