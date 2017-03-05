#!/usr/bin/python

# http://interactivepython.org/courselib/static/pythonds/index.html

from time import sleep

def findSmallest(arr):
    smallest = arr[0]
    smallest_index = 0
    for i in range(1, len(arr)):
        if (arr[i] < smallest):
            smallest = arr[i]
            smallest_index = i
    return smallest_index

def selectionSort(arr):
    newArr = []
    for i in range(len(arr)):
        smallest = findSmallest(arr)
        newArr.append(arr.pop(smallest))
    return newArr

def selection(arr):
    n = len(arr)
    for i in range(0,n-1):
        min = i
        for j in range(i+1,n):
            if (arr[j] < arr[min]):
                min = j
        arr[i], arr[min] = arr[min], arr[i] #swap
    return arr

def bubble(arr):
    n = len(arr)
    for i in range(0,n-1):
        for j in range(0, n-1-i):
            if (arr[j+1] < arr[j]):
                arr[j],arr[j+1]=arr[j+1],arr[j] #swap
    return arr

def insertion(arr):
    n = len(arr)
    for c in range(1, n):
        d = c
        while (d > 0 and arr[d] < arr[d-1]):
            arr[d],arr[d-1]=arr[d-1],arr[d] #swap
            d = d - 1
        c = c - 1
    return arr
              
def merge_sort(arr):
    #print("Splitting ",arr)
    if len(arr)>1:
        mid = len(arr)//2
        lefthalf = arr[:mid]
        righthalf = arr[mid:]
        merge_sort(lefthalf)
        merge_sort(righthalf)
        i=0
        j=0
        k=0
        while i < len(lefthalf) and j < len(righthalf):
            if lefthalf[i] < righthalf[j]:
                arr[k]=lefthalf[i]
                i=i+1
            else:
                arr[k]=righthalf[j]
                j=j+1
            k=k+1
        while i < len(lefthalf):
            arr[k]=lefthalf[i]
            i=i+1
            k=k+1
        while j < len(righthalf):
            arr[k]=righthalf[j]
            j=j+1
            k=k+1
    return arr

def sequential_search(arr, k):
    i = 0
    while (i < len(arr) and arr[i] != k):
        i = i + 1
    if (i < len(arr)):
       return i
    else:
        return -1

def string_match(str, patt):
    if (len(patt) > len(str)):
        return -1
    i = 0
    for i in range(0,len(str)-len(patt)+2):
        j = 0
        while (j < len(patt) and patt[j] == str[i+j]):
            j = j + 1
            if (j == len(patt)):
                return i
        i = i + 1
    return -1

def palindrome(s):
    n = len(s) - 1
    for i in range(0,len(s)/2):
        if (s[i] != s[n]):
            return 0
        i = i + 1
        n = n - 1
    return 1 

def anagram(s1, s2):
    ascii = [0 for x in range(256)] # hash of ascii
    p1 = 0
    p2 = 0
    if (len(s1) != len(s2)):
        return False
    for i in range(0,len(s1)): # register asciis from s1
        ascii[ord(s1[i])] = ascii[ord(s1[i])] + 1
    for i in range(0,len(s2)):
        if (ascii[ord(s2[i])] == 0): # if ascii from s2 not registered fail 
            return False
        ascii[ord(s2[i])] = ascii[ord(s2[i])] - 1 # unregister 1 s2
    for i in range(0,256):
        if (ascii[i] != 0): # check for any still registered
            return False    # if so,fail
    return True

def siftup(arr, count):
    i = count
    x = arr[count]
    while (i > 1):
        if (arr[i] > arr[i/2]):
            arr[i], arr[i/2] = arr[i/2], arr[i]
            i = i / 2

def heapify(arr):
    heap = [0 for x in range(len(arr)+1)]
    count = 0
    for i in range(0, len(arr)):
        count = count + 1
        heap[count] = arr[i]
        siftup(heap, count)
    return heap

# recursion
def countdown(i):
    print i
    if i <= 0:
        return
    else:
        countdown(i-1)

# stack demo
def bye():
    print "ok bye!"

def greet2(name):
    print "how are you, " + name + "?"

def greet(name):
    print "hello, " + name + "!"
    greet2(name)
    print "getting ready to say bye..."
    bye()

#stack for recursion
def fact(x):
    if x == 1:
        return 1;
    else:
        return x * fact(x-1)

def gcd(a, b):
    while b != 0:
       t = b
       b = a % b
       a = t
    return a

def rec_sum(arr):
    if arr == []:
        return 0
    else:
        return arr[0] + rec_sum(arr[1:])

def countem(arr):
    if arr == []:
        return 0
    else:
        return 1 + countem(arr[1:])

def maxit(arr):
    if (len(arr) == 1):
        return arr[0]
    else:
        m = maxit(arr[1:])
        if (m > arr[0]):
            return m
        else:
            return arr[0]

def binsearch(arr, x):
    m = (len(arr))/2
    if (x == arr[m]):
        return m
    elif (x < arr[m]):
        binsearch(arr[:m],x)
    else:
        binsearch(arr[m:],x)

def swap(arr, i, j):
    t = arr[i]
    arr[i] = arr[j]
    arr[j] = t


def quicksort(arr):
    if (len(arr) < 2):
        return arr
    else:
        pivot = arr[0]
        less = [i for i in arr[1:] if i <= pivot]
        grtr = [i for i in arr[1:] if i > pivot]
        return quicksort(less) + [pivot] + quicksort(grtr)
    return arr

def print_items(list):
    for item in list:
        #sleep(1)
        print item

from collections import deque

def find_goal(node, goal):
    #print "node: " + node + ", goal: " + goal
    return node == goal

def activity_is_wakey(node, goal):
    print "node: " + node + ", goal: " + goal
    return node == goal

def person_is_seller(name, goal):
    print "person seller? " + name
    return name[-1] == goal # silly, use last letter as flag for mango dealer

# note: parent list will give the shortest path
def bfs(g, start, end, size, condition, notice):
    search_queue = deque()
    search_queue += g[start]
    searched = []
    parent = [0 for x in range(size)]
    for i in range(size):
        parent[i] = -1
    while search_queue:
        item = search_queue.popleft()
        if not item in searched:
            #print "children of parent {}".format(item)
            for v in g[item]:
                #print "{}, ".format(v)
                if not v in searched:
                    parent[v] = item
            search_queue += g[item]
            searched.append(item)
    return parent

def top_sort(graph_unsorted):
    graph_sorted = []
    # convert unsorted graph to hash table
    print graph_unsorted
    graph_unsorted = dict(graph_unsorted)
    print graph_unsorted
    while graph_unsorted:
        acyclic = False
        for node, edges in list(graph_unsorted.items()):
            for edge in edges:
                if edge in graph_unsorted:
                    break
            else:
                acyclic = True
            del graph_unsorted[node]
            graph_sorted.append((node, edges))
        if not acyclic:
            print "error, cyclic"
    return graph_sorted

def find_path(parent, start, end):
    if ((start == end) or (end == -1)):
        print "start => {}".format(start)
    else:
        find_path(parent, start, parent[end])
        print "         {} <=".format(end)

def test_recursion():        
    #countdown(5)
    #greet("jack")
    #print "fact: {}".format(fact(7))
    #print "gcd: {}".format(gcd(1680,640))
    #print "sum: {}".format(rec_sum([2,4,6]))
    #print "countem: {}".format(countem([2,4,6,3,1,9,1]))
    #print "maxit: {}".format(maxit([2,4,6,3,42,1,9,1]))
    print "binsearch: {}".format(binsearch([2,3,4,9,42,99,132,763,1023],42))
    #print_items([33,15,42,199])

def test_graph():                
    graph = {}
    graph["you"]    = ["alice", "bob", "claire"]
    graph["bob"]    = ["anuj", "peggy"]
    graph["alice"]  = ["peggy"]
    graph["claire"] = ["thom", "jonny"]
    graph["anuj"]   = [""]
    graph["peggy"]  = [""]
    graph["thom"]   = [""]
    graph["jonny"]  = [""]

    graph2 = {}
    graph2["wake up"] = [""]
    graph2["shower"] = ["wake up"]
    graph2["brush teeth"] = ["wake up"]
    graph2["eat breakfast"] = ["brush teeth"]

    
    #bfs(graph, "you", 'm', person_is_seller, " => is a mango seller")
    #bfs(graph2, "eat breakfast", "wake up", activity_is_wakey, " => is wakey")
    #tsort = top_sort(graph2)
    #for i in tsort:
    #    print i

    routes = {}
    routes[1] = [2,4]
    routes[2] = [3]
    routes[3] = [7]
    routes[4] = [5,6]
    routes[5] = [3]
    routes[6] = [3]
    routes[7] = []

    start = 1
    end = 7
    max_vertex = end+1
    #s = bfs(routes, start, end, max_vertex, find_goal, " => found")
    s = shortest(routes, start, end, max_vertex)
    find_path(s, start, end)
    #for i in s:
    #    print i

    ###### dijkstra
    graph = {}
    graph["start"] = {}
    graph["start"]["a"] = 6 # edge start->a has weight of 6
    graph["start"]["b"] = 2
    graph["a"] = {}
    graph["a"]["fin"] = 1
    graph["b"] = {}
    graph["b"]["a"] = 3
    graph["b"]["fin"] = 5
    graph["fin"] = {}

    infinity = float("inf")
    costs = {}
    costs["a"] = 6
    costs["b"] = 2
    costs["fin"] = infinity

    parents = {}
    parents["a"] = "start"
    parents["b"] = "start"
    parents["fin"] = None

    processed = []

def test_sort():
    print "test sort"
    #print selectionSort([5,3,6,2,10])
    #print selection([5,3,6,2,10])
    #print "insertion {}".format(insertion([5,3,6,2,10]))
    #print bubble([5,3,6,2,10])
    #print sequential_search([33,15,10,42,99,6],19)
    #print string_match("bart", "rt")
    #print "palindrome {}".format(palindrome("bartsab"))
    #print "anagram {}".format(anagram("bart", "zart"))
    #print "quicksort: {}".format(quicksort([33,15,10,42,99,6]))
    #print "merge_sort {}".format(merge_sort([54,26,93,17,77,31,44,55,20]))
    print "heap {}".format(heapify([34, 5, 23, 12, 33, 98, 4, 13, 44, 37, 1, 86, 8]))
                                  #[98, 44, 86, 33, 37, 34, 4, 5, 13, 12, 1, 23, 8] 



def main():
    test_sort()
    #test_recursion()

if __name__ == '__main__':
    main()