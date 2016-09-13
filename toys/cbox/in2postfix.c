#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    struct node *next;
};

struct node *head, *z, *t;

void stackinit()
{
    head = (struct node *) malloc(sizeof(struct node));
    z = (struct node *) malloc(sizeof(struct node));
    head->next = z;
    head->key = 0;
    z->next = z;
}

void push(int v)
{
    t = (struct node *) malloc(sizeof(struct node));
    t->key = v;
    t->next = head->next;
    head->next = t;
}

int pop()
{
    int x;
    t = head->next;
    head->next = t->next;
    x = t->key;
    free(t);
    return x;
}

int stackempty()
{
    return (head->next == z);
}

void dump()
{
    printf("\n");
    while (!stackempty()) {
        char c = pop();
        printf("%c ", c);
    }
    printf("\n");
}

int main()
{
    char c;
    for (stackinit(); scanf("%c", &c) != EOF; ) {
        printf("c=%c\n", c);
        switch (c) {
            case ')': printf("%c", (char) pop()); break;
            case '+': push((int) c); break;
            case '-': push((int) c); break; 
            case '*': push((int) c); break; 
            case '/': push((int) c); break; 
            default:
                while (c >= '0' && c <= '0') {
                    printf("%1c", c);
                    scanf("%1c", &c);
                }
                if (c != '(') printf(" ");
                break;
        }
   }
    printf("\n");
    //dump();
}
