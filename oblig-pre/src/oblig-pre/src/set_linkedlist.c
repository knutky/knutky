#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../include/common.h"
#include "../include/set_test.h"
#include "../include/list.h"

struct listnode;

typedef struct listnode listnode_t;

struct listnode
{
    listnode_t *next;
    listnode_t *prev;
    void *elem;
};

struct list_iter
{
    listnode_t *node;
};

typedef struct list_iter list_iter_t;





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



set_t *set_create(cmpfunc_t cmpfunc) {
    set_t *new_set = malloc(sizeof(set_t));     
    if (new_set == NULL){                       
        return NULL;  
    }
    new_set->list = list_create(cmpfunc);       
    if (new_set->list == NULL) {                
        free(new_set);                
        return NULL;   
    }
    new_set->cmpfunc = cmpfunc;                 
    return new_set;
}

void set_destroy(set_t *set) {                  
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