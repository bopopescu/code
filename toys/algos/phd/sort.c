#include <stdio.h>

void show(int a[], int n)
{
    int i;
    //for (i = 0; i < sz; ++i) {
    //    printf("S[%d] = %d, ", i, a[i]);
    //}
    //printf("\n");

    for (i = 0 ; i < n; i++) {
        printf("%3d", a[i]);
    }
    printf("\n\n\n ");
}

int findmax(int a[], int from) {
    int i, max = 0, maxval = 0;
    for (i = from; i >= 0; --i) {
        if (a[i] > maxval) {
            max = i;
            maxval = a[i];
        }
    }
    return max;
}


int binsearch(int x, int *a, int n) {
/*
 * binary search of array a of size n
 * for the value x.
 */
    int low, high, mid;
    low = 0;
    high = n-1;
    while (low <= high) {
        mid = (low+high)/2;
        if (x > a[mid]) low = mid + 1;
        else if (x < a[mid]) high = mid - 1;
        else return x;
    }
    return -1;
}

void swap(int a[], int i, int j) {
    int tmp;
    tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

int partition(int *A, int lo, int hi) {
    int i, p, firsthigh;
    p = hi; /* pick a partition (=hi) */
    firsthigh = lo; /* firsthigh is the partition boundary */
    for (i = lo; i < hi; ++i) {
        if (A[i] < A[p]) {
            swap(A, i, firsthigh);
            firsthigh++;
        }
    }
    swap(A, p, firsthigh); /* finally put pivot where it belongs */
    return firsthigh;
}

void quicksort(int *A, int lo, int hi) {
    int p;
    if (lo < hi) {
        p = partition(A, lo, hi);
        quicksort(A, lo, p-1);
        quicksort(A, p+1, hi);
    }
}

void isort(int *a, int sz) {
/*
 * insertion into ordered list
 * p is the partition between the ordered
 * upper portion and the unordered lower
 * portion of the list. as new items are 
 * encountered, a reverse scan is performed 
 * from the point of the new item to the 
 * beginning of the list to maintain the 
 * invariant of an ordered upper portion.
 */
    int i, j;
    for (i = 1; i < sz; ++i) {
        j = i;
        while (j > 0 && (a[j] < a[j-1])) {
            swap(a, j, j-1);
            --j;
        }
    }
}

void ssort(int A[], int sz) {
/**
 * This algorithm partitions the array into a sorted and unsorted partition,
 * similar to insertion sort, except, it defines the sorted partition starting
 * at the bottom of the array.  It places "max" values there, decrementing the
 * sorted index as values are placed in their final position.  This is faster
 * than insertion sort, which may move keys many times when inserting one element.
 * This algorithm places the key in the final position in one move, ie, the 
 * "max" value for a given linear scan of the unsorted upper partition into
 * it's final resting place at the sorted index.
 *
 */
    int s; /* sorted index */
    int max; /* index of maximum */
    for (s = sz-1; s > 0; --s) {
        max = findmax(A, s);
        swap(A, s, max);
    }
}

void sort_interval(int a[], int sz, int start, int inc) {
/*
 * insertion into ordered list
 * p is the partition between the ordered
 * upper portion and the unordered lower
 * portion of the list. as new items are 
 * encountered, a reverse scan is performed 
 * from the point of the new item to the 
 * beginning of the list to maintain the 
 * invariant of an ordered upper portion.
 */
    int p, i;
    for (i = start; i < sz; i+=inc) {
        for (p = i; p-inc >= 0; p-=inc) {
            if (a[p] < a[p-inc]) {
                swap(a, p, p-inc);
            }
        }
    }
}

#define key(A) (A)
#define less(A, B) (key(A) < key(B))
#define exchg(A, B) { int t = A; A = B; B = t; } 
#define cmpexchg(A, B) if (less(B, A)) exchg(A, B)

void shell(int A[], int sz) {
    int increment, start, i;
    increment = sz;
    do {
        increment = increment/3 + 1;
        for (start = 0; start < increment; ++start) {
            sort_interval(A, sz, start, increment);
        }   
    } while (increment > 1);
}

void selection(int a[], int n)
{
    int i, j, min, swap;
    for (i = 0 ; i <= (n - 2) ; i++) {
        min = i;
        // Find the smallest element
        for ( j = i + 1 ; j <= n-1 ; j++ ) {
            if ( a[j] < a[min] )
                min = j; 
        }
        exchg(a[i], a[min]);
    }
}

void insertion(int a[], int n)
{
    int c, d, temp;
    for (c = 1 ; c <= n - 1; c++) { 
        d = c;
        while ( d > 0 && a[d] < a[d-1]) { 
            temp = a[d];
            a[d] = a[d-1];
            a[d-1] = temp;
            d--; 
        }
    }
}

void bubble(int a[], int n)
{
    int i, d, pos, swap;
    for (i = 0; i < (n-1); i++) {
        pos = i;
        for (d = i+1; d < n; d++) {
            if (a[pos] > a[d]) pos = d;
        }
        if (pos != i) {
            exchg(a[i], a[pos]);
        } 
    }
}

int main() {
    int a[] = {26,33,35,29,19,12,22,15,42,69,1};
    int sz = sizeof(a)/sizeof(a[0]);
    show(a, sz);
    //selection2(a, sz);
    //bubble(a, sz);
    insertion(a, sz);
    show(a, sz);
    //isort(a, sz);
    //ssort(a, sz);
    //shell(a, sz);
    //quicksort(a, 0, sz-1);
    //show(a, sz);
    printf("binsearch %d\n", binsearch(42, a, sz));
  
    return 0;
}
