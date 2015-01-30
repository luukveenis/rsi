/*
 * Author: Luuk Veenis
 * Date: January 14, 2015
 */

/* Standard library includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

/* Project includes */
#include "processing.h"
#include "process_list.h"

#define PATH_MAX 120 /* Buffer size to store path */

/* Creates shell prompt with current working directory */
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

/* Iterate over background jobs and display any status changes */
void report_statuses(llist_ref procs){
  int status;
  node_ptr current = NULL;
  node_ptr next = NULL;

  for(current = procs->head; current; current = next){
    next = current->next; /* Store next in case the node gets deleted */

    /* We need to set the WUNTRACED flag to get WIFSTOPPED information */
    if (waitpid(current->pid, &status, WNOHANG | WUNTRACED)){
      if (WIFEXITED(status)){
        printf("Process %d exited with status: %d\n", current->pid, WEXITSTATUS(status));
        delete_node(procs, search(procs, current->pid));
      } else if (WIFSIGNALED(status)){
        printf("Process %d killed by signal: %d\n", current->pid, WTERMSIG(status));
        delete_node(procs, search(procs, current->pid));
      } else if (WIFSTOPPED(status)){
        printf("Process %d stopped by signal: %d\n", current->pid, WSTOPSIG(status));
      } else if (WIFCONTINUED(status)){
        printf("Process %d continued\n", current->pid);
      }
    }
  }
}

void run(){
  char *prompt, *input;
  llist procs = { NULL, NULL, 0 };

  for(;; free(input)){
    report_statuses(&procs);

    prompt = build_prompt();
    input = readline(prompt);
    free(prompt);

    /* Input is NULL (EOF) allows ctrl+d to quit */
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
