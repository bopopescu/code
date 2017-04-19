#include <stdio.h>
#include <stdlib.h>

#define exchg(A, B) { int t = A; A = B; B = t; }

// todo - recursive:
// [x] 4.1 write sum(a,n) recursively
// [x] 4.2 count items in list -> count(a,n)
// [x] 4.3 find max in list -> max(a,n)
// [ ] 4.4 binsearch recursively

// binary search
// selection sort
// divide and conquer: gcd


void show(int a[], int n)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%d, ", a[i]);
    }
    printf("\n");
}

int sum(int a[], int p, int n)
{
    show(&a[p],n);
    //if (n == 0) return 0; // skip the zero case
    if (n == 1) return a[p];
    return a[p] + sum(a,p+1,n-1);
    
// iterative
//    int i, s = 0;
//    for (i = 0; i < n; i++) {
//        show(&a[i],n-i);
//        s += a[i];
//    }
//    return s;
}

typedef struct _list {
    struct _list *next;
    int v;
} list;

list *new(int v)
{
    list *l = malloc(sizeof(list));
    l->v = v;
    l->next = NULL;
    return l;
}

void insert(list *head, int v)
{
    list *n = new(v);
    list *tmp = head->next;
    head->next = n;
    n->next = tmp;
}

void showl(list *head)
{
    printf("show list\n");
    list *p = head->next;
    while (p) {
        printf("v:%d\n", p->v);
        p = p->next;
    }
}

int count(list *head)
{
    if (head->next == NULL) return 0;
    return 1 + count(head->next);
}

void max(list *head, int *m)
{
    if (head->next == NULL) return;
    if (*m < head->next->v) {
        *m = head->next->v;
    }
    max(head->next, m);
}

void partition(int a[], int p, int n)
{
    int i, j;
    int pivot = a[p];
    printf("partition: %d\n", pivot);

    for (i = 0; i < n-1; i++) {
        if (i == p) continue;
        if (a[i] > pivot) {
            exchg(a[i],a[p]);
            p = i;
        }
    }
}

#if 0 // python
def quicksort(arr):
    if (len(arr) < 2):
        return arr
    else:
        pivot = arr[0]
        less = [i for i in arr[1:] if i <= pivot]
        grtr = [i for i in arr[1:] if i > pivot]
        return quicksort(less) + [pivot] + quicksort(grtr)
    return arr
#endif

void quicksort(int a[], int n)
{
    if (n < 2) return;
    int pvt = 0;
    partition(a, pvt, n-1);
}

void selection(int a[], int n)
{
    int i, j, min;
    for (i = 0; i < n; i++) {
        min = i;
        for (j = i+1; j < n; j++) {
            if (a[j] < a[min]) {
                min = j;
            }
        }
        exchg(a[i], a[min]);
    }
}

int gcd(int m, int n)
{
    if (m < n) exchg(m,n); //assert(m > n);
    printf("m:%4d, n:%4d, mod:%4d\n", m,n,m%n);
    return (m%n == 0) ? n : gcd(n, m%n);
}

int main()
{
    int g = gcd(1680, 640);
    printf("gcd: %d\n", g);

    int a[] = {4,1,8,3,42,2};
    int n = sizeof(a)/sizeof(a[0]);
    show(a,n);
    //selection(a,n);
    quicksort(a,n);
    show(a,n);

    return 0;

    printf("sum: %d\n", sum(a,0,n));

    list head;
    insert(&head, 4);
    insert(&head, 42);
    insert(&head, 3);
    insert(&head, 5);
    insert(&head, 6);
    showl(&head);
    int c = count(&head);
    printf("list count: %d\n", c);

    int m = 0;
    max(&head, &m);
    printf("list max: %d\n", m);

}
