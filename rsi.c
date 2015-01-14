/*
 * Author: Luuk Veenis
 * Date: January 14, 2015
 */

#include <stdio.h>
#include <readline/readline.h>

int main(int argc, char *argv[]){
  for(;;){
    char *line = readline("RSI:> ");
    printf("%s\n", line);
  }

  return 0;
}
