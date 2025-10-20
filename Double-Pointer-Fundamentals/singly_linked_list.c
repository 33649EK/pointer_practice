#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int x;
    struct Node *next;
};

// Need double pointer for head node
void push_front(struct Node **head, int value)
{
    // struct Node new_head = {value, *head};
    // *head = &new_head;

    struct Node *new = malloc(sizeof *new);
    if (!new)
    {
        perror("Malloc");
        exit(1);
    }

    new->x = value;
    new->next = *head;
    *head = new;
}

int main(void)
{

    struct Node list = {12, NULL};
    struct Node *head = &list;

    int new_value = 49;

    push_front(&head, new_value);

    struct Node *next_node = list.next;
    struct Node *current_node = &list;
    while (current_node != NULL)
    {
        printf("%d ", current_node->x);
        current_node = next_node;
        next_node = current_node->next;
    }

    return 0;
}