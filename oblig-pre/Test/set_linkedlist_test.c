#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/common.h"
#include "../include/set_test.h"

// set_size
// union
// intersection
// difference
// set copy





/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include "../include/list.h"

#include <stdlib.h>

struct listnode;

typedef struct listnode listnode_t;

struct listnode
{
    listnode_t *next;
    listnode_t *prev;
    void *elem;
};

struct list
{
    listnode_t *head;
    listnode_t *tail;
    int size;
    cmpfunc_t cmpfunc;
};

struct list_iter
{
    listnode_t *node;
};

typedef struct list_iter list_iter_t;


static listnode_t *newnode(void *elem)
{
    listnode_t *node = malloc(sizeof(listnode_t));
    if (node == NULL){
        printf("m");
        return NULL;
    }
    node->next = NULL;
    node->prev = NULL;
    node->elem = elem;
    return node;
}

list_t *list_create(cmpfunc_t cmpfunc)
{
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL)
        return NULL;

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->cmpfunc = cmpfunc;
    return list;
}

void list_destroy(list_t *list)
{
    listnode_t *node = list->head;
    while (node != NULL)
    {
        listnode_t *tmp = node;
        node = node->next;
        free(tmp);
    }
    free(list);
}

int list_size(list_t *list)
{
    return list->size;
}

int list_addfirst(list_t *list, void *elem)
{
    listnode_t *node = newnode(elem);
    if (node == NULL)
        return 0;

    if (list->head == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    }
    list->size++;
    return 1;
}


int list_addanywhere(list_t *list, void *elem, list_iter_t *iter) {
    listnode_t *node = newnode(elem);
    printf("d");
    // If node not created
    if (node == NULL){
        printf("f");
        return 0;
    }if (list->head == NULL) {
        // If list is currently empty
        printf("x");
        list->head = list->tail = node;
        printf("e");
    }else if (iter->node == NULL){
        // If element should be the last on the list
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
        printf("Y");
    }else if (iter->node == list->head){
        // If element should be first on list
        printf("w");
        list->head->prev = node;
        node->next = list->head;
        list->head = node;
    } else {
        // Insert the new node before the iter node
        printf("i");
        node->next = iter->node;
        node->prev = iter->node->prev;
        iter->node->prev = node;
        node->prev->next = node;
        node->elem = elem;
        printf("j");
        }
    list->size++;
    return 1;
}



int list_addlast(list_t *list, void *elem)
{
    listnode_t *node = newnode(elem);
    if (node == NULL)
        return 0;

    if (list->head == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }
    list->size++;
    return 1;
}

void *list_popfirst(list_t *list)
{
    if (list->head == NULL)
    {
        return NULL;
    }
    else
    {
        void *elem = list->head->elem;
        listnode_t *tmp = list->head;
        list->head = list->head->next;
        if (list->head == NULL)
        {
            list->tail = NULL;
        }
        else
        {
            list->head->prev = NULL;
        }
        list->size--;
        free(tmp);
        return elem;
    }
}

void *list_poplast(list_t *list)
{
    if (list->tail == NULL)
    {
        return NULL;
    }
    else
    {
        void *elem = list->tail->elem;
        listnode_t *tmp = list->tail;
        list->tail = list->tail->prev;
        if (list->tail == NULL)
        {
            list->head = NULL;
        }
        else
        {
            list->tail->next = NULL;
        }
        free(tmp);
        list->size--;
        return elem;
    }
}

int list_contains(list_t *list, void *elem)
{
    listnode_t *node = list->head;
    while (node != NULL)
    {
        if (list->cmpfunc(elem, node->elem) == 0)
            return 1;
        node = node->next;
    }
    return 0;
}

/*
 * Merges two sorted lists a and b using the given comparison function.
 * Only assigns the next pointers; the prev pointers will have to be
 * fixed by the caller.  Returns the head of the merged list.
 */
static listnode_t *merge(listnode_t *a, listnode_t *b, cmpfunc_t cmpfunc)
{
    listnode_t *head, *tail;

    /* Pick the smallest head node */
    if (cmpfunc(a->elem, b->elem) < 0)
    {
        head = tail = a;
        a = a->next;
    }
    else
    {
        head = tail = b;
        b = b->next;
    }
    /* Now repeatedly pick the smallest head node */
    while (a != NULL && b != NULL)
    {
        if (cmpfunc(a->elem, b->elem) < 0)
        {
            tail->next = a;
            tail = a;
            a = a->next;
        }
        else
        {
            tail->next = b;
            tail = b;
            b = b->next;
        }
    }
    /* Append the remaining non-empty list (if any) */
    if (a != NULL)
    {
        tail->next = a;
    }
    else
    {
        tail->next = b;
    }
    return head;
}

/*
 * Splits the given list in two halves, and returns the head of
 * the second half.
 */
static listnode_t *splitlist(listnode_t *head)
{
    listnode_t *slow, *fast, *half;

    /* Move two pointers, a 'slow' one and a 'fast' one which moves
     * twice as fast.  When the fast one reaches the end of the list,
     * the slow one will be at the middle.
     */
    slow = head;
    fast = head->next;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    /* Now 'cut' the list and return the second half */
    half = slow->next;
    slow->next = NULL;
    return half;
}

/*
 * Recursive merge sort.  This function is named mergesort_ to avoid
 * collision with the mergesort function that is defined by the standard
 * library on some platforms.
 */
static listnode_t *mergesort_(listnode_t *head, cmpfunc_t cmpfunc)
{
    if (head->next == NULL)
    {
        return head;
    }
    else
    {
        listnode_t *half = splitlist(head);
        head = mergesort_(head, cmpfunc);
        half = mergesort_(half, cmpfunc);
        return merge(head, half, cmpfunc);
    }
}

void list_sort(list_t *list)
{
    if (list->head != NULL)
    {
        listnode_t *prev, *n;

        /* Recursively sort the list */
        list->head = mergesort_(list->head, list->cmpfunc);

        /* Fix the tail and prev links */
        prev = NULL;
        for (n = list->head; n != NULL; n = n->next)
        {
            n->prev = prev;
            prev = n;
        }
        list->tail = prev;
    }
}

list_iter_t *list_createiter(list_t *list)
{
    list_iter_t *iter = malloc(sizeof(list_iter_t));
    if (iter == NULL)
        return NULL;

    iter->node = list->head;
    return iter;
}

void list_destroyiter(list_iter_t *iter)
{
    free(iter);
    iter->node = NULL;
}

int list_hasnext(list_iter_t *iter)
{
    if (iter->node == NULL)
        return 0;
    else
        return 1;
}

void *list_next(list_iter_t *iter)
{
    if (iter->node == NULL)
    {
        return NULL;
    }
    else
    {
        void *elem = iter->node->elem;
        iter->node = iter->node->next;
        return elem;
    }
}
















                      

typedef struct set {                            // Makes a set with the linked list
    list_t *list;                               // Use the linked list implementation for set storage
    cmpfunc_t cmpfunc;                          
}set_t;

typedef struct set_iter{                        // Define an iterator for the set
    list_iter_t *list_iter;                     // Use the linked list iterator
}set_iter_t;


int compare_ints(void *a, void *b)             
{
    int *ia = a;
    int *ib = b;

    return (*ia) - (*ib);
}



int compare_strings(void *a, void *b)
{
    return strcmp(a, b);
}






set_t *set_create(cmpfunc_t cmpfunc) {
    set_t *new_set = malloc(sizeof(set_t));     // Allocate memory in the size of a new set
    if (new_set == NULL){                       // Tests if the memory allocation was successful
        return NULL;  
    }
    new_set->list = list_create(cmpfunc);       // Creates a linked list in the set to store the elements
    if (new_set->list == NULL) {                // Checks if the the list was successfully created
        free(new_set);                
        return NULL;   
    }
    new_set->cmpfunc = cmpfunc;                 // Define the comparison function
    return new_set;
}

void set_destroy(set_t *set) {                  // Function for deliting the set
    if (set != NULL) {
        list_destroy(set->list);
        free(set);
    }
}


bool is_empty(set_t *set) {                     
    return list_size(set->list) == 0;
}

int set_contains(set_t *set, void *elem) {
    // Stops when coming to bigger or equal element
    // If equal, then element is contained
    set_iter_t *iter = set_createiter(set);
    while (set_hasnext(iter)) {
        void *current_elem = set_next(iter);
        int cmp_result = set->cmpfunc(elem, current_elem);
        if (cmp_result == 0) {                  
            set_destroyiter(iter); 
            return 1;
        } else if (cmp_result < 0) {           
            set_destroyiter(iter); 
            return 0;
        }
    }
    if (iter != NULL){
        set_destroyiter(iter);
    }
    return 0;
}


void set_add(set_t *set, void *elem) {
    set_iter_t *iter = set_createiter(set);
    while (set_hasnext(iter)) {
        // Iterates and compare with elements.
        void *current_elem = iter->list_iter->node->elem;
        int cmp_result = set->cmpfunc(elem, current_elem);
        if (cmp_result == 0) {
            set_destroyiter(iter);
            return;
        } else if (cmp_result < 0) {  
            break;
        }
        // Iterate without returning anything
        iter->list_iter->node = iter->list_iter->node->next;
    }
    //Adds element in a sorted way
    list_addanywhere(set->list, elem, iter->list_iter);
    set_destroyiter(iter);

}

void print_set(set_t *set) {
    // Iterates and prints
    set_iter_t *iter = set_createiter(set);
    while (set_hasnext(iter)) {
        void *elem = set_next(iter);
        if (set->cmpfunc == compare_ints) {
            printf(" %d", *((int *)elem));
        }
        else if (set->cmpfunc == compare_strings) {
            printf(" %s", (char *)elem);
        }
    }
    printf("\n");
    set_destroyiter(iter); 
}


int set_size(set_t *set) {                      
    return list_size(set->list);
}


set_t *set_union(set_t *a, set_t *b) {
    set_t *new_set = set_create(a->cmpfunc);    
    if (new_set == NULL) {
        return NULL;  
    }
    // Iterating through in parrallell and adding smallest element
    set_iter_t *iter_a = set_createiter(a);
    set_iter_t *iter_b = set_createiter(b);
    void *elem_a = set_next(iter_a);            
    void *elem_b = set_next(iter_b);            
    while (elem_a != NULL && elem_b != NULL) {
        int cmp_result = a->cmpfunc(elem_a, elem_b);
        if (cmp_result == 0) {                  
            list_addlast(new_set->list, elem_a);
            elem_a = set_next(iter_a);
            elem_b = set_next(iter_b);
        } else if (cmp_result < 0) {      
            list_addlast(new_set->list, elem_a);
            elem_a = set_next(iter_a);
        } else {                               
            list_addlast(new_set->list, elem_b);
            elem_b = set_next(iter_b);
        }
    // Add remainding elements
    } while (elem_a != NULL) {
        list_addlast(new_set->list, elem_a);
        elem_a = set_next(iter_a);
    } while (elem_b != NULL) {
        list_addlast(new_set->list, elem_b);
        elem_b = set_next(iter_b);
    }
    set_destroyiter(iter_a);
    set_destroyiter(iter_b);
    return new_set;
}


// Dont know if this work  !!!!
set_t *set_intersection(set_t *a, set_t *b) {
    set_t *new_set = set_create(a->cmpfunc);    
    if (new_set == NULL) {
        return NULL;  
    }
    // Iterating through in parrallell and adding equal elements
    set_iter_t *iter_a = set_createiter(a);
    set_iter_t *iter_b = set_createiter(b);
    void *elem_a = set_next(iter_a);            
    void *elem_b = set_next(iter_b);            
    while (elem_a != NULL && elem_b != NULL) {
        int cmp_result = a->cmpfunc(elem_a, elem_b);
        if (cmp_result == 0) {                  
            list_addlast(new_set->list, elem_a);
            elem_a = set_next(iter_a);
            elem_b = set_next(iter_b);
        } else if (cmp_result < 0) {      
            elem_a = set_next(iter_a);
        } else {                               
            elem_b = set_next(iter_b);
        }
    }
    set_destroyiter(iter_a);
    set_destroyiter(iter_b);
    return new_set;
}


set_t *set_difference(set_t *a, set_t *b) {
    set_t *new_set = set_create(a->cmpfunc);    
    if (new_set == NULL) {
        return NULL;  
    }
    // Iterating through in parrallell and adding a if a is smallest
    set_iter_t *iter_a = set_createiter(a);
    set_iter_t *iter_b = set_createiter(b);
    void *elem_a = set_next(iter_a);            
    void *elem_b = set_next(iter_b);            
    while (elem_a != NULL && elem_b != NULL) {
        int cmp_result = a->cmpfunc(elem_a, elem_b);
        if (cmp_result == 0) {                  
            elem_a = set_next(iter_a);
            elem_b = set_next(iter_b);
        } else if (cmp_result < 0) {      
            list_addlast(new_set->list, elem_a);
            elem_a = set_next(iter_a);
        } else {                               
            elem_b = set_next(iter_b);
        }
    // Add remainding elements of a
    } while (elem_a != NULL) {
        list_addlast(new_set->list, elem_a);
        elem_a = set_next(iter_a);
    }
    set_destroyiter(iter_a);
    set_destroyiter(iter_b);
    return new_set;
}


set_t *set_copy(set_t *set) {
    set_t *new_set = set_create(set->cmpfunc);
    if (new_set == NULL) {
        return NULL;
    }
    set_iter_t *iter = set_createiter(set);
    while (set_hasnext(iter)) {
        set_add(new_set, set_next(iter));
    }
    set_destroyiter(iter);

    return new_set;
}

set_iter_t *set_createiter(set_t *set) {
    set_iter_t *iter = malloc(sizeof(set_iter_t));      
    if (iter == NULL) {
        return NULL;
    }
    iter->list_iter = list_createiter(set->list);       
    if (iter->list_iter == NULL) {
        free(iter);
        return NULL;
    }                                                     
    return iter;
}


void set_destroyiter(set_iter_t *iter) {                
    if (iter != NULL) {
        list_destroyiter(iter->list_iter);
        free(iter);        
    }
}


int set_hasnext(set_iter_t *iter) {
    return list_hasnext(iter->list_iter);
}


void *set_next(set_iter_t *iter) {                      
    return list_next(iter->list_iter);
}



int main(){
    set_t *setA = set_create(compare_ints);
    set_t *setB = set_create(compare_ints);
    
    int x1 = 5;
    int x2 = 10;
    
    int x3 = 13;
    int y1 = 1;
    int y2 = 9;
    int y3 = 4;
    int y4 = 3;
    int y5 = 9;
    int y6 = 11;
    

    set_add(setA, &x1);
    
    
    set_add(setA, &x2);
    
    
    set_add(setA, &x3);
    
    set_add(setB, &y1);
    set_add(setB, &y2);
    
    set_add(setB, &y3);
    
    set_add(setB, &y4);
    set_add(setB, &y5);
    set_add(setB, &y6);
    
    set_t *setC = set_union(setA, setB);
    set_t *setD = set_copy(setA);


    print_set(setA);
    print_set(setB);
    print_set(setC);
    printf("size of set B: %d\n", set_size(setB));
    print_set(setD);
    int z = 13;
    printf("%d\n", set_contains(setA, &z));

    set_t *setE = set_intersection(setA, setB);
    set_t *setF = set_difference(setA, setB);
    printf("E and F");
    print_set(setE);
    print_set(setF);



}