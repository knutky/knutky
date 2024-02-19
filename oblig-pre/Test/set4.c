

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct set;

typedef struct set{
    int *members; // A pointer to an integer. indicating a dynamic array or set of integers.
    int length;
} set_t;

struct Set_Iter;

typedef struct Set_Iter{
    int *members;
    int length;
    int current_index;
} set_iter_t;

set_t* init();    //This function declaration is for an init function that returns a pointer to a set_t. It is likely meant to initialize a new set.
                //The init function is expected to return a pointer to a newly initialized set_t structure.

// set_t functionalities
bool is_empty(set_t *set);
bool set_containes(set_t *set, int member);
void set_add(set_t *set, int member); 
void print_set(set_t *set);
void delete_set(set_t *set);
set_t *set_union(set_t *setA, set_t *setB); // Return a new pointer to the union of two sets
set_t *set_intersection(set_t *setA, set_t *setB); // Return a new pointer to the union of two sets
set_t *set_difference(set_t *setA, set_t *setB);
set_t *copy_set(set_t *original);
int get_size(set_t *setA);


// Iteratiar functionalities
set_iter_t *set_createiter(set_t *set);
void set_destroyiter(set_iter_t *iter);
int set_hasnext(set_iter_t *iter);
void *set_next(set_iter_t *iter);


int main(){
    set_t *setA = init();   //set_t *setA: Declares a variable named setA with a type of pointer to set_t.
                            //= init();: Calls the init function, which should return a pointer to a newly created set_t, and assigns this pointer to setA.
                            // setA points to a newly initialized set

    // Testing code:
    if (is_empty(setA)) printf("set_t A is empty.\n");
    else printf("set_t A is not empty.\n");

    set_add(setA,8);
    set_add(setA,2);
    set_add(setA,3); 
    set_add(setA,9);
    set_add(setA,6); 


    printf("set_t A: ");
    print_set(setA);

    set_t *setB = init();
    set_add(setB,7);
    set_add(setB,4); 
    set_add(setB,9);
    set_add(setB,1); 

    printf("set_t B: ");
    print_set(setB);

    set_t *setC = set_union(setA, setB);
    
    printf("set_t A union B: ");
    print_set(setC);

    set_t *setD = set_intersection(setA, setB);
    
    printf("set_t A intersection set B: ");
    print_set(setD);

    set_t *setE = set_difference(setA, setB);
    
    printf("set_t A - B: ");
    print_set(setE);

    set_t *setF = set_difference(setB, setA);
    
    printf("set_t F =  B - A: ");
    print_set(setF);

    if (set_containes(setA, 9)) printf("9 \n");
    if (set_containes(setF, 3)) printf("3");

    int i = get_size(setA);
    printf("length of set A is: %d\n", i);

    set_t *setG = copy_set(setA);
    print_set(setG);

    printf("set A: ");
    print_set(setA);

    set_iter_t *iter = set_createiter(setA);

    printf("Iterating set A: ");
    while (set_hasnext(iter))
    {
        int *next_element = (int *)set_next(iter);
        printf("%d ", *next_element);
    }
    printf("\n");

    set_destroyiter(iter);


    return 0;
}   


set_t* init(){
    set_t *new_set = malloc(sizeof(set_t));         // Allocate memory for a set_t structure
    new_set->length = 0;                            // Start with a empty set
    new_set->members = malloc(sizeof(int));         // Starts with allocate place for 1 element
    return new_set;                                 // Return pointer
}


bool is_empty(set_t *set){                          // Returns true if empty
    return (set->length == 0);
}


void delete_set(set_t *set) {                       // Frees up space when you are finished working with a set
    if (set != NULL) {
        free(set->members);
        free(set);
    }
}


bool set_containes(set_t *set, int member){         // Take advantage of the sorted structur
    int low = 0, high = set->length - 1;            // Binary search tree?
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (set->members[mid] == member) {          // If element is found
            return true;                            
        } else if (set->members[mid] < member) {    // Decrease the search area
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return false;
}


void set_add(set_t *set, int member){               // Add element to end of set , could maby used set_containes
    int low = 0, high = set->length - 1;
    while (low <= high) {                           // Perform binary search to find the correct index to add the member to
        int mid = low + (high - low) / 2;
        if (set->members[mid] == member) {          //
            return;
        } else if (set->members[mid] < member) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    
    set->members = realloc(set->members, sizeof(int) * (set->length + 1));
                                                    // Allocates place
    for (int i = set->length; i > low; i--) {       // Shifts elements to make place
        set->members[i] = set->members[i - 1];
    }

    set->members[low] = member;                     // Sets in the new member
    set->length++;                                  // Updates the length of the set
}


void print_set(set_t *set)                          //loop over each member of the set and print out each one of them
{
    for (int i = 0; i < set->length; i++)
        if (i == (set->length - 1))
            printf("%d\n", set->members[i]);
        else
            printf("%d,", set->members[i]);
}


set_t *set_union(set_t *setA, set_t *setB){         // Return a new pointer to the union of two sets
    set_t *newSet = init();                         // Makes new set
    int i = 0, j = 0;
    while (i < setA->length && j < setB->length) {  // Iterate both sets and add elements that ar larger then the previous
        if (setA->members[i] < setB->members[j]) {
            set_add(newSet, setA->members[i]);
            i++;
        } else if (setA->members[i] > setB->members[j]) {
            set_add(newSet, setB->members[j]);
            j++;
        } else {
            set_add(newSet, setA->members[i]);      // Elements are equal, add element and advance both indices
            i++;
            j++;
        }
    }
    
    while (i < setA->length) {                      // Adds potential remaining elements from set A
        set_add(newSet, setA->members[i]);
        i++;
    }

    while (j < setB->length) {                      // Adds potential remaining elements from setA
        set_add(newSet, setB->members[j]);
        j++;
    }
    return newSet;
}


set_t *set_intersection(set_t *setA, set_t *setB){          // Return a new pointer to the union of two sets
    set_t *newSet = init();                                 // Makes new set
    int i = 0, j = 0;
    while (i < setA->length && j < setB->length) {          // Iterate both sets and add elemens 
        if (setA->members[i] < setB->members[j]) {
            i++;
        } else if (setA->members[i] > setB->members[j]) {
            j++;
        } else {
            set_add(newSet, setA->members[i]);              // Elements are equal,  set_add one and advance both indices
            i++;
            j++;
        }
    }
    return newSet;
}


set_t *set_difference(set_t *setA, set_t *setB){            // A-B
    set_t *newSet = init();                                 // Makes a new set instance
    int i = 0, j = 0;
    while (i < setA->length && j < setB->length) {          // Iterates both sets and add those who are in A and not B to the new set
        if (setA->members[i] < setB->members[j]) {          
            set_add(newSet, setA->members[i]);
            i++;
        } else if (setA->members[i] > setB->members[j]) {
            j++;
        } else {                                            // Elements are equal, skip both and advance indices
            i++;
            j++;
        }
    }

    while (i < setA->length) {                              // Adds remaining elements from setA
        set_add(newSet, setA->members[i]);
        i++;
    }
    return newSet;
}


set_t *copy_set(set_t *original){                           // Returns a copy of a set
    set_t *copy = init();                                   // Makes new set
    copy->length = original->length;                        // Copy the length 
    copy->members = malloc(sizeof(int) * copy->length);     // Allocate memory for the members and copy the content
    for (int i = 0; i < copy->length; i++) {                // Iterate and copies elements
        copy->members[i] = original->members[i];            // 
    }
    return copy;
}


int get_size(set_t *setA)                           // Gets the cardinality of a set
{
    return setA->length;
}


set_iter_t *set_createiter(set_t *set){
    set_iter_t *iter = malloc(sizeof(set_iter_t));  // Declares a variable iter, pointer to an instance of type set_iter_t.
                                                    // malloc allocates memory dynamically 
    iter->members = set->members;                   // Pointing to the mebers of the original set
    iter->length = set->length;         
    iter->current_index = 0;
    return iter;
}


void set_destroyiter(set_iter_t *iter){             // Function made for deleting the iterator instance
    free(iter);                                     // Use free function to free up the space
}


int set_hasnext(set_iter_t *iter){                  // Checking if set has another element after thi or not
    return iter->current_index < iter->length;
}


void *set_next(set_iter_t *iter){                   // Getting the next element in an iteration
    if (set_hasnext(iter)) {                        // Make sure that it has not reached the end of the sorted set
        int *next_element = &(iter->members[iter->current_index]);  // Save adress to current element in the iteration ??
        iter->current_index++;                      // Increment iter
        return next_element;
    } else {
        return NULL;
    }
}