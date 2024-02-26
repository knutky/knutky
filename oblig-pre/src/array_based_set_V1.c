#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <common.h>
#include <set_test.h>

// Define the structures
struct set {
    void** members;
    int length;
    cmpfunc_t cmpfunc;
};

struct set_iter {
    void** members;
    int length;
    int current_index;
};

// Define the comparison functions
int compare_ints(void *a, void *b) {
    int *ia = a;
    int *ib = b;
    return (*ia) - (*ib);
}

set_t* set_create(cmpfunc_t cmpfunc) {
    // Function to create a new set
    set_t *new_set = malloc(sizeof(set_t));
    new_set->length = 0;
    new_set->members = malloc(sizeof(void*));
    new_set->cmpfunc = cmpfunc;
    return new_set;
}

bool is_empty(set_t *set) {
    // Function to check if the set is empty
    return (set->length == 0);
}

void set_destroy(set_t *set) {
    if (set != NULL) {
        // Free individual elements if needed
        for (int i = 0; i < set->length; i++) {
            free(set->members[i]);
        }

        // Free the members array and the set itself
        free(set->members);
        free(set);
    }
}


int set_contains(set_t *set, void *member) {
    // Function to check if the set contains a given member
    // Uses binary search since the set is assumed to be sorted
    int low = 0, high = set->length - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp_result = set->cmpfunc(&set->members[mid], member);
        if (cmp_result == 0) {
            return 1;  // Member found
        } else if (cmp_result < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return 0;  // Member not found
}

void set_add(set_t *set, void *member) {
    // Function to add a member to the set while maintaining the order
    // Uses binary search to find the correct position for insertion
    int low = 0, high = set->length - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp_result = set->cmpfunc(set->members[mid], member);
        if (cmp_result == 0) {
            return;  // Member already in the set
        } else if (cmp_result < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Perform insertion at the correct position
    set->members = realloc(set->members, sizeof(void*) * (set->length + 1));
    for (int i = set->length; i > low; i--) {
        set->members[i] = set->members[i - 1];
    }
    set->members[low] = member;
    set->length++;
}

void print_set(set_t *set) {
    // Function to print the elements of the set
    for (int i = 0; i < set->length; i++) {
        if (set->cmpfunc == compare_ints) {
            printf(" %d", *(int *)set->members[i]);
        } else if (set->cmpfunc == *compare_strings) {
            printf(" %s", set->members[i]);
        }
        if (i == (set->length - 1)) {
            printf("\n");
        }
    }
}

set_t *set_union(set_t *setA, set_t *setB) {
    // Function to compute the union of two sets
    // Iterates through the two sets in parralell and add smallest element
    set_t *newSet = set_create(setA->cmpfunc);
    int i = 0, j = 0;
    while (i < setA->length && j < setB->length) {
        int cmp_result = setA->cmpfunc(setA->members[i], setB->members[j]);
        if (cmp_result < 0) {
            set_add(newSet, setA->members[i]);
            i++;
        } else if (cmp_result > 0) {
            set_add(newSet, setB->members[j]);
            j++;
        } else {
            set_add(newSet, setA->members[i]);
            i++;
            j++;
        }
    }

    // Add remaining elements from setA
    while (i < setA->length) {
        set_add(newSet, setA->members[i]);
        i++;
    }

    // Add remaining elements from setB
    while (j < setB->length) {
        set_add(newSet, setB->members[j]);
        j++;
    }
    return newSet;
}

set_t *set_intersection(set_t *setA, set_t *setB){          
    set_t *newSet = set_create(setA->cmpfunc);  
    int i = 0, j = 0;
    while (i < setA->length && j < setB->length) {  
        int cmp_result = setA->cmpfunc(setA->members[i], setB->members[j]);  
        if (cmp_result == 0) {
            set_add(newSet, setA->members[i]);  
            i++;
            j++;
        } else if (cmp_result > 0) {
            j++;
        } else {
            i++;

        }
    }
    return newSet;
}

set_t *set_difference(set_t *setA, set_t *setB){            
    set_t *newSet = set_create(setA->cmpfunc);     
    int i = 0, j = 0;
    while (i < setA->length && j < setB->length) {  
        int cmp_result = setA->cmpfunc(setA->members[i], setB->members[j]);  
        if (cmp_result < 0) {          
            set_add(newSet, setA->members[i]);
            i++;
        } else if (cmp_result > 0) {
            j++;
        } else {    
            i++;
            j++;
        }
    }
}

set_t *copy_set(set_t *original) {
    // Function to create a copy of a set
    set_t *copy = set_create(original->cmpfunc);
    copy->length = original->length;
    copy->members = malloc(sizeof(void*) * copy->length);
    for (int i = 0; i < copy->length; i++) {
        copy->members[i] = original->members[i];
    }
    return copy;
}

int set_size(set_t *setA) {
    // Function to get the size of a set
    return setA->length;
}

set_iter_t *set_createiter(set_t *set) {
    // Function to create an iterator for the set
    set_iter_t *iter = malloc(sizeof(set_iter_t));
    iter->members = set->members;
    iter->length = set->length;
    iter->current_index = 0;
    return iter;
}

void set_destroyiter(set_iter_t *iter) {
    // Function to destroy the iterator
    free(iter);
}

int set_hasnext(set_iter_t *iter) {
    // Function to check if there is a next element in the iteration
    return iter->current_index < iter->length;
}

void *set_next(set_iter_t *iter) {
    // Function to get the next element in the iteration
    if (set_hasnext(iter)) {
        void *next_element = (iter->members[iter->current_index]);
        iter->current_index++;
        return next_element;
    } else {
        return NULL;
    }
}

