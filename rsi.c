/*
 * Author: Luuk Veenis
 * Date: January 14, 2015
 */

/* Standard library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>

/* Personal includes */
#include "processing.h"
#include "process_list.h"

#define PATH_MAX 120

char* build_prompt(){
  char buf[PATH_MAX];
  char *prefix = "RSI: ";
  char *suffix = " > ";
  char *cwd = getcwd(buf, PATH_MAX);
  char *prompt = malloc(strlen(prefix) + strlen(suffix) + strlen(cwd) + 1);

  strcpy(prompt, prefix);
  strcat(prompt, cwd);
  strcat(prompt, suffix);

  return prompt;
}

void run(){
  char *prompt, *input;
  llist procs = { NULL, NULL, 0 };

  for(;; free(input)){
    prompt = build_prompt();
    input = readline(prompt);
    free(prompt);

    if (!input){
      printf("\nbye bye\n");
      exit(EXIT_SUCCESS);
    } else {
      /* Ignore empty inputs (pressing enter) */
      if (strcmp(input, "")) {
        process(input, &procs);
      }
    }
  }
}

int main(int argc, char **argv){
  run();

  return 0;
}
