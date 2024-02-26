#include <stdio.h>
#include <stdbool.h>
#include "../include/set.h"
#include <stdlib.h>
#include "../include/common.h"
#include "../include/set_test.h"





void set_sort(set_t *set);
void merge_sort_recursion(set_t *set, int l, int r); 
void merge_sorted_arrays(set_t *set, int l, int m, int r);


struct set{
    void** members;
    int allocated_size;
    int length;
    int sorted;
    cmpfunc_t cmpfunc; 
};


struct set_iter{
    void** members;  
    int length;
    int current_index;
};

// Used in mergesort to replace duplicates
struct duplicate {
};



int compare_ints(void *a, void *b)
{
    int *ia = a;
    int *ib = b;

    return (*ia) - (*ib);
}
/*

int compare_strings(void *a, void *b)
{
    return strcmp(a, b);
}


*/




set_t* set_create(cmpfunc_t cmpfunc){

    set_t *new_set = malloc(sizeof(set_t));         
    new_set->length = 0;                  
    new_set->allocated_size = 10;  
    new_set->members = malloc(sizeof(void*)*new_set->allocated_size);         
    new_set->cmpfunc = cmpfunc;
    new_set->sorted = 1;
    return new_set;                                 
}


bool is_empty(set_t *set){               
    return (set->length == 0);
}


void set_destroy(set_t *set) {                   
    if (set != NULL) {
        free(set->members);
        free(set);
    }
}

// implement comparison
int set_contains(set_t *set, void *member){         
    set_sort(set);
    int low = 0, high = set->length - 1;            
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp_result = set->cmpfunc(&set->members[mid], member);    
        if (cmp_result == 0) {         
            return 1;                            
        } else if (cmp_result < 0) {   
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return 0;
}

//
void set_add(set_t *set, void *member){                 
    set->sorted = 0;
    if (set->length >= set->allocated_size-1) {
        set->allocated_size *= 2;
        set->members = realloc(set->members, sizeof(void*) * set->allocated_size);
    }
    set->members[set->length] = member;                    
    set->length++;                                
}


void print_set(set_t *set) {       
    set_sort(set);
    for (int i = 0; i < set->length; i++){
        if (set->cmpfunc == compare_ints) {
            printf(" %d", *(int *)set->members[i]);
        } else if (set->cmpfunc == compare_strings) {
            printf(" %s", set->members[i]);
        } 
        
        
    }
    printf("\n");
}


set_t *set_union(set_t *setA, set_t *setB){         
    set_sort(setA);
    set_sort(setB);
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
    
    while (i < setA->length) {                      
        set_add(newSet, setA->members[i]);
        i++;
    }

    while (j < setB->length) {                      
        set_add(newSet, setB->members[j]);
        j++;
    }
    newSet->sorted = 1;
    return newSet;
}

set_t *set_intersection(set_t *setA, set_t *setB){         
    set_sort(setA);
    set_sort(setB);
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

// Comparison
set_t *set_difference(set_t *setA, set_t *setB){            
    set_sort(setA);
    set_sort(setB);
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

    while (i < setA->length) {                              
        set_add(newSet, setA->members[i]);
        i++;
    }
    return newSet;
}


set_t *copy_set(set_t *original){
    set_sort(original);
    set_t *copy = set_create(original->cmpfunc);                                   
    copy->length = original->length;                        
    copy->members = malloc(sizeof(void*) * copy->length);     
    for (int i = 0; i < copy->length; i++) {                
        copy->members[i] = original->members[i];            
    }
    return copy;
}


int set_size(set_t *setA)                           
{
    return setA->length;
}


set_iter_t *set_createiter(set_t *set){
    set_iter_t *iter = malloc(sizeof(set_iter_t));                                              
    iter->members = set->members;                  
    iter->length = set->length;         
    iter->current_index = 0;
    return iter;
}


void set_destroyiter(set_iter_t *iter){         
    free(iter->members);     
    free(iter);                                     
}


int set_hasnext(set_iter_t *iter){                 
    return iter->current_index < iter->length;
}


void *set_next(set_iter_t *iter){                  
    if (set_hasnext(iter)) {                       
        void *next_element = (iter->members[iter->current_index]);  
        iter->current_index++;                      
        return next_element;
    } else {
        return NULL;
    }
}

/*
int main() {
    // Example usage of set_merge_sort
    set_t *mySet = set_create(compare_ints);
    set_t *mySet2 = set_create(compare_ints);

    // Add elements to mySet
    
    int a = 6;
    int b = 1;
    int c = 57;
    int d = 6;
    int e = 7;

    int a2 = 6;
    int b2 = 1;
    int c2 = 67;
    int d2 = 1;
    int e2 = 6;
    int f2 = 8;
    int g2 = 7;
    
    set_add(mySet2, &a);
    set_add(mySet2, &b);
    set_add(mySet2, &c);
    set_add(mySet2, &d);
    set_add(mySet2, &e);
    
    set_add(mySet2, &a2);
    set_add(mySet2, &b2);
    set_add(mySet2, &c2);
    set_add(mySet2, &d2);
    //set_add(mySet2, &e2);
    //set_add(mySet2, &g2);
    
    print_set(mySet2);


    set_t *A = set_union(mySet, mySet2);
    set_t *B = set_difference(mySet, mySet2);
    set_t *C =  set_intersection(mySet, mySet2);

    print_set(A);
    print_set(B);
    print_set(C);
    
    print_set(mySet);

    return 0;
}
*/

void set_sort(set_t *set) {
    if (set->sorted==0){
        merge_sort_recursion(set, 0, set->length-1);
        // Should remove the nulls 
        int last = set->length - 1;
        while (last >= 0 && set->members[last] == NULL) {
            last--;
        }

        // Adjust the size of the set
        set->length = last + 1;

    }
}


void merge_sort_recursion(set_t *set, int l, int r)
{
    // If more then one element
    if (l < r)
    {
        int m = l + (r - l) / 2;

        // Recursively do merge sort on the left and right halves
        merge_sort_recursion(set, l, m);
        merge_sort_recursion(set, m + 1, r);

        // Merge the sorted halves
        merge_sorted_arrays(set, l, m, r);
    }
}


// Function to merge two sorted arrays into one sorted array 
void merge_sorted_arrays(set_t *set, int l, int m, int r)
{
    // Calculate the lengths of the left and right segments
    int left_length = m - l + 1;
    int right_length = r - m;

    // Create a temporary array to store the merged result
    void **temp = malloc(sizeof(void *) * (r - l + 1));

    // Initialize indices for iterating through temp, left, and right
    int i = 0, j = 0, k = 0;

    // Merge the two sorted arrays into the temp array
    while (i < left_length && j < right_length) {  
        int cmp_result = 0;
        void *left_element = set->members[l + i];
        void *right_element = set->members[m + 1 + j];

        // Handling Null cases
        if (left_element == NULL && right_element == NULL) {
            cmp_result = 0;
        }
        else if (left_element == NULL) {
            cmp_result = 1;
        }
        else if (right_element == NULL) {
            cmp_result = -1;
        }
        else{
        cmp_result = set->cmpfunc(left_element, right_element);
        }
        // Add elements to temp
        if (cmp_result < 0){
            // left element smaller
            temp[k++] = left_element;
            i++;
        }
        else if (cmp_result > 0 ) {
            // right element smaller
            temp[k++] = right_element;
            j++;
        }
        else if (left_element != NULL) {
            // Both element equal
            temp[k++] = left_element;
            j++;
            i++;
        } else {
            // Both elements NULL, 
            i++;
            j++;
            
        }   
    }
    // Copy any remaining elements from the left array to temp
    while (i < left_length) {
        temp[k++] = set->members[l + i++];
    }
    // Copy any remaining elements from the right array to temp
    while (j < right_length) {
        temp[k++] = set->members[m + 1 + j++];
    }
    // Fill remaining positions in temp with NULL
    while (k < r -l + 1) {
        temp[k++] = NULL;
    }

    // Copy elements from temp back to the set
    for (int s = 0; s < r- l +1; s++) {
        set->members[l + s] = temp[s];
    }

    // Free the allocated memory for the temporary array
    free(temp);
}

