#include "types.h"

list_t *list()
{
    list_t *list = malloc(sizeof(list_t));
    list->head = malloc(sizeof(link_t));
    list->head->next = NULL;
    list->tail = NULL;
    return list;
}

link_t *link(int key)
{
    link_t *link = malloc(sizeof(link_t));
    link->key = key;
    link->next = NULL;
    return link;
}

int empty(list_t *list)
{
    return (list->tail ? 0 : 1);
}

int size(list_t *list)
{
    int count = 0;
    link_t *t = list->head->next;
    while (t) {
        t = t->next;
        count++;
    }
    return count;
}

link_t *peek_front(list_t *list)
{
    return list->head->next;
}

void push_front(list_t *list, int key)
{
    link_t *l = link(key);
    l->next = list->head->next;
    list->head->next = l;
    if (list->tail == NULL) {
        list->tail = l;
    }
}

link_t *pop_front(list_t *list)
{
    link_t *t = NULL;
    if (list->head->next) {
        t = list->head->next;
        list->head->next = t->next;
    }
    if (list->head->next == NULL) {
        list->tail = NULL;
    }
    return t;
}

void push_back(list_t *list, int key)
{
    link_t *l = link(key);
    if (list->tail) {
        list->tail = (list->tail->next = l);
    } else {
        l->next = list->head->next;
        list->tail = list->head->next = l;
    }
}

void reverse(list_t *list)
{
    link_t *p, *q, *r = NULL;
    p = list->head->next;
    // set tail here to first link
    list->tail = list->head->next;
    while (p) {
        q = p->next;
        p->next = r;
        r = p;
        p = q;
    }
    list->head->next = r;
}

void show(list_t *list)
{
    link_t *t = list->head->next;
    while (t) {
        printf("t->%d\n", t->key);
        t = t->next;
    }
    printf("\n");
}

int main()
{
    list_t *list = list();
    push_back(list, 42);
    push_back(list, 17);
    push_back(list, 2);
    push_back(list, 11);
    show(list);
    reverse(list);
    show(list);
    printf("size: %d\n", size(list));
    while (!empty(list)) {
        link_t *t = peek_front(list);
        pop_front(list);
        printf("t->%d\n", t->key);
    }
}
