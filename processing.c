/* Standard library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/* Project includes */
#include "processing.h"
#include "process_list.h"

/* Initialize the struct that stores the parsed user input */
cmd_ptr initialize_cmd(){
  cmd_ptr cmd_ref = (cmd_ptr) malloc(sizeof(cmd));
  cmd_ref->argc = 0;
  cmd_ref->argv = NULL;
  cmd_ref->background = 0;

  return cmd_ref;
}

/* Deallocate memory used by command struct */
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

  for (token = strtok(input, " ");
       token;
       token = strtok(NULL, " "))
  {
    cmd_ref->argv = realloc(cmd_ref->argv, ++size * sizeof(char*));
    cmd_ref->argv[size-2] = token;
  }
  cmd_ref->argv[size-1] = NULL;
  cmd_ref->argc = size-1;

  if (!strcmp(cmd_ref->argv[size-2], "&")){
    cmd_ref->background = 1;
    cmd_ref->argv[size-2] = NULL;
    cmd_ref->argc--;
  }


  return 0;
}

/* More robust way to get home directory than environment variable */
char* get_home_dir(){
  uid_t uid = getuid();
  struct passwd *pwd = getpwuid(uid);
  return pwd->pw_dir;
}

/* Change the current working directory
 * Both "cd" and "cd ~" take the user to the home directory */
void change_directory(cmd_ptr cmd_ref){
  if (cmd_ref->argc > 2){
    printf("Invalid command!\n");
  } else if (cmd_ref->argc == 1 || !strcmp(cmd_ref->argv[1], "~")){
    chdir(get_home_dir());
  } else {
    chdir(cmd_ref->argv[1]);
  }
}

/* Execute the command input by the user.
 * Runs the program in the background if the user
 * specifies & as the last argumen */
void execute(cmd_ptr cmd_ref, llist_ref procs){
  int status, retval;
  pid_t cpid;

  cpid = fork();
  if (cpid >= 0){
    if (cpid == 0){
      retval = execvp(cmd_ref->argv[0], cmd_ref->argv);
      if (retval == -1) {
        fprintf(stderr, "RSI: ");
        perror(cmd_ref->argv[0]);
      }
      exit(retval);
    } else {
      /* Store the child process id if running in the background */
      if (cmd_ref->background){
        insert_at_head(procs, cpid);
      } else {
        waitpid(cpid, &status, 0);
      }
    }
  } else {
    perror("fork");
  }
}

/* Process the user's input:
 * - tokenize the input string
 * - handle changing directories since this isn't a program we can call
 * - otherwise call execute which runs the command through execvp */
void process(char *input, llist_ref procs){
  cmd_ptr cmd_ref = initialize_cmd();
  tokenize(input, cmd_ref);

  if (!strcmp("cd", cmd_ref->argv[0])){
    change_directory(cmd_ref);
  } else {
    execute(cmd_ref, procs);
  }

  free_cmd(cmd_ref);
}
