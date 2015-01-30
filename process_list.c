#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_list.h"

/* Creates a new node
 * Both back and prev pointers will be NULL */
node_ptr create_node(int pid)
{
  /* Create the node */
  node_ptr new_node = malloc(sizeof(node));
  new_node->next = NULL;
  new_node->prev = NULL;
  new_node->pid = pid;
  return new_node;
}

/* Destroys the given node, freeing the memory 
 * All pointers are set to NULL */
void deallocate_node(node_ptr node)
{
  node->next = NULL;
  node->prev = NULL;
  free(node);
}

/* Deletes the given node */
void delete_node(llist_ref list, node_ptr node)
{
  /* Case 1: It is the only node in the list */
  if (NULL == node->prev && NULL == node->next)
  {
    list->head = NULL;
    list->tail = NULL;
    deallocate_node(node);
  }
  /* Case 2: It is the head of the list */
  else if (NULL == node->prev && NULL != node->next)
  {
    list->head = node->next;
    node->next->prev = NULL;
    deallocate_node(node);
  }
  /* Case 3: It is the tail of the list */
  else if (NULL != node->prev && NULL == node->next)
  {
    list->tail = node->prev;
    node->prev->next = NULL;
    deallocate_node(node);
  }
  /* Case 4: It is in the middle of the list */
  else
  {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    deallocate_node(node);
  }
  list->size--;
}

/* Searches for and returns the node corresponding to pid */
node_ptr search(llist_ref list, int pid)
{
  node_ptr current = list->head;
  while (NULL != current)
  {
    if (current->pid == pid)
    {
      return current;
    }
    current = current->next;
  }
  return current;
}

/* Inserts a node with the given pid at the head of the list */
void insert_at_head(llist_ref list, int pid)
{
  node_ptr new_node = create_node(pid);
  if (NULL == list->head)
  {
    list->head = new_node;
    if (NULL == list->tail)
    {
      list->tail = new_node;
    }
  }
  else
  {
    list->head->prev = new_node;
    new_node->next = list->head;
    list->head = new_node;
  }
  list->size++;
}

/* Simply prints all the elements in the list */
/* Primarily to assist with development */
void print_list(llist_ref list)
{
  int counter = 1;
  node_ptr current = list->head;
  while(NULL != current)
  {
    printf("Node %d: %d\n", counter, current->pid);
    counter++;
    current = current->next;
  }
}
