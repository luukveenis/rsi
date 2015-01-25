/*
 * Author: Luuk Veenis
 * Date: January 14, 2015
 */

/* Standard library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <readline/readline.h>

/* Personal includes */
#include "processing.h"

char* build_prompt(){
  char buf[_POSIX_PATH_MAX];
  char *prefix = "RSI: ";
  char *suffix = " > ";
  char *cwd = getcwd(buf, _POSIX_PATH_MAX);
  char *prompt = (char*) malloc(strlen(prefix) + strlen(suffix) + strlen(cwd) + 1);

  strcpy(prompt, prefix);
  strcat(prompt, cwd);
  strcat(prompt, suffix);

  return prompt;
}

void run(){
  char *prompt, *input;

  for(;;free(prompt),free(input)){
    prompt = build_prompt();
    input = readline(prompt);

    if (!input){
      free(prompt);
      free(input);
      printf("\nbye bye\n");
      exit(0);
    } else {
      process(input);
    }
  }
}

int main(int argc, char **argv){
  run();

  return 0;
}
