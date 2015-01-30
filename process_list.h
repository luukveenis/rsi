#ifndef PROCESS_LIST_H_
#define PROCESS_LIST_H_

typedef struct Node* node_ptr;
typedef struct Node
{
  node_ptr next;
  node_ptr prev;
  int pid;
} node;

typedef struct LinkedList* llist_ref;
typedef struct LinkedList
{
  node_ptr head;
  node_ptr tail;
  int size;
} LList;

void delete_node(llist_ref, node_ptr);
node_ptr search(llist_ref, int);
void insert_at_head(llist_ref, int);

#endif
