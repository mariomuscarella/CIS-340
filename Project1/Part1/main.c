// Description: Builds a doubly linked list of integers and sorts them in ascending order

#include <stdlib.h>
#include <stdio.h>
#include "node.h"


int main(void)
{
    struct mynode *list = (struct mynode *)malloc(sizeof(struct mynode));
    struct mynode *node = list; //this helps to go through the linked list
    struct mynode *previous = (struct mynode *)malloc(sizeof(struct mynode));
    //printf("Please enter list data:\n");
    
    int datalist = 1;
    //this ensures that input is no longer requested from user after a 0 is entered
    while(datalist!=0)
    {
        scanf("%d", &(node->value));
        datalist = node->value;
        if (datalist !=0) //this ensures that the next node is not made if the entered value is 0
        {
            node -> next = (struct mynode *)malloc(sizeof(struct mynode));
            previous = node;
            node = node->next;
            node -> prev = previous;
        }
    }
    
    
    
    printf("The list before sorting: \n");
    printlist(list);
    
    //sort the list
    list = quicksort(list);
    
    printf("The list after sorting: \n");
    printlist(list);
    
    //this will clear the list, helps avoid memory leaks
    freelist(list);
    
    return 0;
}
