#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define hashsize(n) ((unsigned int)1<<(n))
#define hashmask(n) (hashsize(n)-1)

// Key - data for hash, len - length of this data, bits - count of bits of hashcode
unsigned long oaat(char* key, unsigned long len, unsigned long bits){
  unsigned long hash, i;
  for(hash = 0, i = 0; i < len; i++){
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash & hashmask(bits);
}

typedef struct hashnode{
  struct node* data;
  struct hashnode* next;
} hashnode;

#define SIZE 1024

typedef struct node {
  char *name;
  int num_children;
  struct node **children;
  int score;
} node;

static void* safe_malloc(size_t n, unsigned long line)
{
  void* p = malloc(n);
  if (!p)
  {
    fprintf(stderr, "[%s:%ul]Out of memory(%ul bytes)\n",
            __FILE__, line, (unsigned long)n);
    exit(EXIT_FAILURE);
  }
  return p;
}

#define malloc_safe(n) safe_malloc(n, __LINE__)

// Inserting into hashtable
void insert_node(hashnode *nodes[], int num_cell, node *new_node) {
  hashnode *node1;
  node1 = nodes[num_cell];
  if(node1 == NULL) {
    nodes[num_cell] = (hashnode*)malloc_safe(sizeof(hashnode));
    nodes[num_cell]->data = new_node;
    nodes[num_cell]->next = NULL;
  }
  else {
    while(node1->next != NULL) {
      node1 = node1->next;
    }
    node1->next = (hashnode*)malloc_safe(sizeof(hashnode));
    node1->next->data = new_node;
    node1->next->next = NULL;
  }
}

#define MAX_NAME 10

node *new_node(char *name) {
  node *n = (node *)malloc_safe(sizeof(node));
  n->name = name;
  n->num_children = 0;
  return n;
}

// Hash search and inserting into hashtable and array
node *find_and_insert_node(hashnode *nodes[], int max_size, char *name, node *arr[], int* num_nodes) {
  int i;
  hashnode* node1;
  i = oaat(name, strlen(name), 10);
  if (i>max_size)
  {
    fprintf(stderr, "Out of size\n");
    exit(EXIT_FAILURE);
  }

  node1 = nodes[i];
  while(node1 != NULL) {
    if (strcmp(name, node1->data->name) == 0) {
      free(name);
      return node1->data;
    }
    node1 = node1->next;
  }

  node *node2 = new_node(name);
  insert_node(nodes, i, node2);

  arr[*num_nodes] = node2;
  *num_nodes += 1;

  return node2;
}

int read_tree(node *nodes[], int num_lines, hashnode *hashnodes[]) {
  node *parent_node, *child_node;
  char *parent_name, *child_name;
  int i, j, num_children;
  int num_nodes = 0;
  for(i = 0; i < num_lines; i++) {
    parent_name = (char*) malloc_safe(MAX_NAME+1);
    scanf("%s", parent_name);
    scanf("%d", &num_children);
    parent_node = find_and_insert_node(hashnodes, SIZE, parent_name, nodes, &num_nodes);
    parent_node->children = (node**)malloc_safe(num_children * sizeof(node*));
    parent_node->num_children = num_children;
    for(j = 0; j < num_children; j++) {
      child_name = (char*) malloc_safe(MAX_NAME + 1);
      scanf("%s", child_name);
      child_node = find_and_insert_node(hashnodes, SIZE, child_name, nodes, &num_nodes);
      parent_node->children[j] = child_node;
    }
  }
  return num_nodes;
}

int score_one(node *n, int d) {
  int total, i;
  if (d == 1)
    return n->num_children;
  total = 0;
  for (i = 0; i < n->num_children; i++) {
    total = total + score_one(n->children[i], d - 1);
  }
  return total;
}

void score_all(node *nodes[], int num_nodes, int d) {
  int i;
  for (i = 0; i < num_nodes; i++) {
    nodes[i]->score = score_one(nodes[i], d);
  }
}

int compare(const void *v1, const void *v2) {
  const node *n1 = *(const node **)v1;
  const node *n2 = *(const node **)v2;
  if (n1->score > n2->score)
    return -1;
  if (n1->score < n2->score)
    return 1;
  return strcmp(n1->name, n2->name);
}

void output_info(node *nodes[], int num_nodes) {
  int i = 0;
  while(i < 3 && i < num_nodes && nodes[i]->score > 0) {
    printf("%s %d\n", nodes[i]->name, nodes[i]->score);
    i++;
    while(i < num_nodes && nodes[i]->score == nodes[i-1]->score) {
      printf("%s %d\n", nodes[i]->name, nodes[i]->score);
      i++;
    }
  }
}

void clean_nodes(node* nodes[], int num_nodes)
{
  while (num_nodes--)
  {
    free(nodes[num_nodes]->name);
  }
  free(nodes);
}

void clean_hashnodes(hashnode* nodes[])
{
  int size = SIZE;
  hashnode *node1;
  while (size--)
  {
    if (nodes[size] != NULL && nodes[size]->next != NULL)
    {
      node1 = nodes[size]->next;
      while (node1!=NULL)
      {
        node1 = node1->next;
        free(node1);
      }
    }
  }
  free(nodes);
}

#define MAX_NODES 1000

/*
TEST CASES: INPUT:
1
7 2
Lucas 1 Enzo
Zara 1 Amber
Sana 2 Gabriel Lucas
Enzo 2 Min Becky
Kevin 2 Jad Cassie
Amber 4 Vlad Sana Ashley Kevin
Vlad 1 Omar
OUTPUT: // The biggest parent with childs on 2 distance
Tree 1:
Amber 5
Zara 4
Lucas 2
 */

int main(int argv, char* argc[]){
  hashnode **hashnodes;

  int num_cases, case_num;
  int n, d, num_nodes;
  node **nodes;
  scanf("%d", &num_cases);
  for(case_num = 1; case_num <= num_cases; case_num++) {
    nodes = (node**)malloc_safe(MAX_NODES * sizeof(node*));
    hashnodes = (hashnode**)malloc_safe(SIZE * sizeof(hashnode*));

    printf("Tree %d:\n", case_num);
    scanf("%d %d", &n, &d);
    num_nodes = read_tree(nodes, n, hashnodes);
    score_all(nodes, num_nodes, d);
    qsort(nodes, num_nodes, sizeof(node*), compare);
    output_info(nodes, num_nodes);
    if (case_num < num_cases)
      printf("\n");

    clean_nodes(nodes, num_nodes);
    clean_hashnodes(hashnodes);
  }
  return 0;
}
