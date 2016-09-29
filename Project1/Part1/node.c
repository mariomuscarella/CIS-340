// Description: Runs the recursive quicksort function to sort a doubly linked list
//include header files
#include <stdlib.h>
#include <stdio.h>
#include "node.h"

struct mynode* quicksort(struct mynode *head) {
    struct mynode *last = head;
    
    while (last -> next != NULL) {
        last = last -> next;
    }
    
    return _quicksort(&head, last);
}

//method uses recursion to quickly sort through the elements in the linked list
struct mynode* _quicksort(struct mynode **top, struct mynode *last)
{
    
    struct mynode *temporary;
    struct mynode *reference_s = *top;
    
    int reference_i = reference_s->value;
    struct mynode *current=reference_s->next;
    struct mynode *previous = reference_s;
  
    
    if (*top == last)
    {
        return 0;
    }
    else
    {
        while (current != last)
        {
            if (current->value <= reference_i)
            {
                temporary = current->next; //reassigns links, not values of mynode (constant)
                current->next = *top;
                previous->next = temporary;
                *top = current;
                current = temporary;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
    }
    
    
    _quicksort(top, reference_s);
    _quicksort(&reference_s->next, last);
    
    return *top;
    
}


void printlist(struct mynode *top)
{
    struct mynode *node = top;
    
    while (node->next != NULL)
    {
        printf("%d ", node->value);
        node = node->next;
    }
    printf("\n");
}


//ensures memory leaks don't occcur, deletes linked list
void freelist(struct mynode *top)
{
    struct mynode *node, *temporary = top;
    top = NULL;
    
    while (temporary != NULL)
    {
        node = temporary;
        temporary = temporary->next;
        free(node);
    } //cycles through list, freeing space
    
}
