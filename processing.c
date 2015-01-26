#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>
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
 * which stores both the number and the list of arguments
 *
 * The actual list size is size+1 to leave room for a terminating
 * NULL pointer as required by execvp */
int tokenize(char *input, cmd_ptr cmd_ref){
  int size = 1;
  char *token = NULL;

  for (token = strtok(input, COMMAND_SEPERATOR);
       token;
       token = strtok(NULL, COMMAND_SEPERATOR))
  {
    cmd_ref->argv = realloc(cmd_ref->argv, ++size * sizeof(char*));
    cmd_ref->argv[size-2] = token;
  }
  cmd_ref->argv[size-1] = NULL;
  cmd_ref->argc = size-1;

  return 0;
}

/* This could possibly use error checking see below:
 * http://www.linuxquestions.org/questions/programming-9/chdir-~-to-$home-in-c-programming-language-4175457202/
 */
char* get_home_dir(){
  uid_t uid = getuid();
  struct passwd *pwd = getpwuid(uid);
  return pwd->pw_dir;
}

void change_directory(cmd_ptr cmd_ref){
  if (cmd_ref->argc > 2){
    printf("Invalid command!\n");
  } else if (cmd_ref->argc == 1 || strcmp(cmd_ref->argv[1], "~") == 0){
    chdir(get_home_dir());
  } else {
    chdir(cmd_ref->argv[1]);
  }
}

void execute(cmd_ptr cmd_ref){
  int status, retval;
  pid_t cpid;

  cpid = fork();
  if (cpid >= 0){
    if (cpid == 0){
      retval = execvp(cmd_ref->argv[0], cmd_ref->argv);
      exit(retval);
    } else {
      waitpid(cpid, &status, 0);
    }
  } else {
    printf("Error: fork failed.\n");
  }
}

void process(char *input){
  cmd_ptr cmd_ref = initialize_cmd();
  tokenize(input, cmd_ref);

  if (strcmp("cd", cmd_ref->argv[0]) == 0){
    change_directory(cmd_ref);
  } else {
    execute(cmd_ref);
  }

  free_cmd(cmd_ref);
}
