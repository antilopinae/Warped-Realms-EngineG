#ifndef STACK_H
#define STACK_H

#define SIZE 255

typedef struct stack
{
  struct node * values[SIZE];
  int highest_used;
} stack;

stack *new_stack(void)
{
  stack *s = malloc(sizeof(stack));
  if (s == NULL)
  {
    fprintf(stderr, "malloc error\n");
    exit(1);
  }
  s->highest_used = -1;
  return s;
}

void push_stack(stack *s, struct node *value)
{
  s->highest_used++;
  if (s->highest_used == SIZE)
  {
    fprintf(stderr, "Stack invalidate error\n");
  }
  s->values[s->highest_used] = value;
}

struct node * pop_stack(stack *s)
{
  if (s->highest_used <0)
  {
    fprintf(stderr, "Stack invalidate error\n");
  }
  struct node * ret = s->values[s->highest_used];
  s->highest_used--;
  return ret;
}

int is_empty_stack(stack *s)
{
  return s->highest_used == -1;
}

#endif //STACK_H
