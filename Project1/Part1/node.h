// Description: Header file to include quicksort functions

#ifndef __NODE_H__
#define __NODE_H__

struct mynode {
    int  const value;
    struct mynode *next;
    struct mynode *prev;
};

struct mynode* quicksort(struct mynode *);
struct mynode* _quicksort(struct mynode **, struct mynode *);
void printlist(struct mynode *);
void freelist(struct mynode *);
#endif
