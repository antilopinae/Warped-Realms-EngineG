#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"

typedef struct node
{
  int candy;
  struct node *left, *right;
} node;


node* new_house(int candy)
{
  node* house = malloc(sizeof(node));
  if (house == NULL)
  {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  house->candy = candy;
  house->left = NULL;
  house->right = NULL;
  return house;
}

node* new_non_house(node *left, node *right)
{
  node *nonhouse = malloc(sizeof(node));
  if (nonhouse == NULL)
  {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  nonhouse->left = left;
  nonhouse->right = right;
  return nonhouse;
}

// Count all items in tree
// int tree_candy(node *tree)
// {
//   int total = 0;
//   stack *s = new_stack();
//   while (tree != NULL)
//   {
//     if (tree->left && tree->right)
//     {
//       push_stack(s, tree->left);
//       tree = tree->right;
//     } else
//     {
//       total = total + tree->candy;
//       if (is_empty_stack(s))
//       {
//         tree = NULL;
//       } else
//       {
//         tree = pop_stack(s);
//       }
//     }
//   }
//   return total;
// }

// Count all items in tree
int tree_candy(node *tree)
{
  if (!tree->left && !tree->right)
  {
    return tree->candy;
  }
  return tree_candy(tree->left) + tree_candy(tree->right);
}

// Count nodes in tree
int tree_nodes(node *tree)
{
  if (!tree->left && !tree->right)
    return 1;
  return 1 + tree_nodes(tree->left) + tree_nodes(tree->right);
}

// Count leaves in tree
int tree_leaves(node *tree)
{
  if (!tree->left && !tree->right)
    return 1;
  return tree_leaves(tree->left) + tree_leaves(tree->right);
}

// Count streets that have to walk when collecting all items
int tree_streets(node *tree)
{
  if (!tree->left && !tree->right)
    return 0;
  return tree_streets(tree->left) + tree_streets(tree->right) + 4;
}

int max(int v1, int v2)
{
  if (v1 > v2)
    return v1;
  else
    return v2;
}

int tree_height(node *tree)
{
  if (!tree->left && !tree->right)
  {
    return 0;
  }
  return 1 + max(tree_height(tree->left), tree_height(tree->right));
}

void tree_solve(node *tree)
{
  int candy = tree_candy(tree);
  int height = tree_height(tree);
  int num_streets = tree_streets(tree) - height;
  printf("%d %d\n", num_streets, candy);
}

node * read_tree_helper(char *line, int *pos)
{
  node *tree = malloc(sizeof(node));
  if (tree == NULL)
  {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  if (line[*pos] == '(')
  {
    (*pos)++;
    tree->left = read_tree_helper(line, pos);
    (*pos)++;
    tree->right = read_tree_helper(line, pos);
    (*pos)++;
    return tree;
  } else
  {
    tree->left = NULL;
    tree->right = NULL;
    tree->candy = line[*pos] - '0';
    (*pos)++;
    if (line[*pos] != ')' && line[*pos] != ' ' && line[*pos] != '\0')
    {
      tree->candy = tree->candy * 10 + line[*pos] - '0';
      (*pos)++;
    }
    return tree;
  }
}

node *read_tree(char *line)
{
  int pos = 0;
  return read_tree_helper(line, &pos);
}

#define SIZE 255
#define TEST_CASES 5

int main(int argc, char const *argv[]) {
  int i;
  char line[SIZE+1];
  node *tree;
  for (i = 0; i < TEST_CASES; i++)
  {
    fgets(line, 255, stdin);
    tree = read_tree(line);
    tree_solve(tree);
  }
  return 0;
  // node *four = new_house(4);
  // node *nine = new_house(9);
  //
  // node *B = new_non_house(four, nine);
  // node *fifteen = new_house(15);
  // node *C = new_non_house(B, fifteen);
  //
  // stack st;
  // st.highest_used = 12;
  // st.values[0] = B;
}