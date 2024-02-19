

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int *members; // A pointer to an integer. indicating a dynamic array or set of integers.
    int length;
} Set;



Set* init();    //This function declaration is for an init function that returns a pointer to a Set. It is likely meant to initialize a new set.
                //The init function is expected to return a pointer to a newly initialized Set structure.


bool is_empty(Set *set);
bool set_containes(Set *set, int member);
void set_add(Set *set, int member); 
void print_set(Set *set);
void delete_set(Set *set);
Set *set_union(Set *setA, Set *setB); // Return a new pointer to the union of two sets
Set *set_intersection(Set *setA, Set *setB); // Return a new pointer to the union of two sets
Set *set_difference(Set *setA, Set *setB);
Set *copy_set(Set *original);
int get_size(Set *setA);




int main(){
    Set *setA = init();     //Set *setA: Declares a variable named setA with a type of pointer to Set.
                            //= init();: Calls the init function, which should return a pointer to a newly created Set, and assigns this pointer to setA.
                            // setA points to a newly initialized set

    if (is_empty(setA)) printf("Set A is empty.\n");
    else printf("Set A is not empty.\n");

    set_add(setA,8);
    set_add(setA,2);
    set_add(setA,3); 
    set_add(setA,9);
    set_add(setA,6); 


    printf("Set A: ");
    print_set(setA);

    Set *setB = init();
    set_add(setB,7);
    set_add(setB,4); 
    set_add(setB,9);
    set_add(setB,1); 

    printf("Set B: ");
    print_set(setB);

    Set *setC = set_union(setA, setB);
    
    printf("Set A union B: ");
    print_set(setC);

    Set *setD = set_intersection(setA, setB);
    
    printf("Set A intersection set B: ");
    print_set(setD);

    Set *setE = set_difference(setA, setB);
    
    printf("Set A - B: ");
    print_set(setE);

    Set *setF = set_difference(setB, setA);
    
    printf("Set F =  B - A: ");
    print_set(setF);

    if (set_containes(setA, 9)) printf("9 \n");
    if (set_containes(setF, 3)) printf("3");

    int i = get_size(setA);
    printf("length of set A is: %d\n", i);

    Set *setG = copy_set(setA);
    print_set(setG);

    return 0;

}   

Set* init(){
    Set *new_set = malloc(sizeof(Set));
    new_set->length = 0;
    new_set->members = malloc(sizeof(int)); //Setter av plass til en integer
    return new_set;




}
// argument: pointer to a Set structure
// returns: True if set is empty
bool is_empty(Set *set){
    return (set->length == 0);
}

void delete_set(Set *set) {
    if (set != NULL) {
        free(set->members);
        free(set);
    }
}



bool set_containes(Set *set, int member){ // Take advantage of the sorted structur
    int low = 0, high = set->length - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (set->members[mid] == member) {
            // Member already exists, no need to    set_add again
            return true;
        } else if (set->members[mid] < member) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return false;
}

// Parameters:
//   - set: A pointer to a Set structure.
//   - member: The integer to be    added into the set.
// Note: This function assumes that 'set' is a valid pointer to a Set structure.
void    set_add(Set *set, int member){
    int low = 0, high = set->length - 1;

    // Perform binary search to find the correct index to   set_add the member
    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (set->members[mid] == member) {
            // Member already exists, no need to    set_add again
            return;
        } else if (set->members[mid] < member) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Now, 'low' is the correct index to   set_add the member
    // Shift elements to make space for the new member at the correct position
    set->members = realloc(set->members, sizeof(int) * (set->length + 1));
    for (int i = set->length; i > low; i--) {
        set->members[i] = set->members[i - 1];
    }

    //  set_add the new member at the correct position
    set->members[low] = member;

    // Increment the length of the set
    set->length++;
}




void print_set(Set *set) //loop over each member of the set and print out each one of them
{
    for (int i = 0; i < set->length; i++)
        if (i == (set->length - 1))
            printf("%d\n", set->members[i]);
        else
            printf("%d,", set->members[i]);
}



Set *set_union(Set *setA, Set *setB) // Return a new pointer to the union of two sets
{
    Set *newSet = init();

    int i = 0, j = 0;

    while (i < setA->length && j < setB->length) {
        if (setA->members[i] < setB->members[j]) {
            set_add(newSet, setA->members[i]);
            i++;
        } else if (setA->members[i] > setB->members[j]) {
            set_add(newSet, setB->members[j]);
            j++;
        } else {
            // Elements are equal,  set_add one and advance both indices
            set_add(newSet, setA->members[i]);
            i++;
            j++;
        }
    }

    //  set_add remaining elements from setA
    while (i < setA->length) {
        set_add(newSet, setA->members[i]);
        i++;
    }

    //  set_add remaining elements from setB
    while (j < setB->length) {
        set_add(newSet, setB->members[j]);
        j++;
    }

    return newSet;

}


Set *set_intersection(Set *setA, Set *setB) // Return a new pointer to the union of two sets
{
    Set *newSet = init();

    int i = 0, j = 0;

    while (i < setA->length && j < setB->length) {
        if (setA->members[i] < setB->members[j]) {
            i++;
        } else if (setA->members[i] > setB->members[j]) {
            j++;
        } else {
            // Elements are equal,  set_add one and advance both indices
            set_add(newSet, setA->members[i]);
            i++;
            j++;
        }
    }

    return newSet;
}

Set *set_difference(Set *setA, Set *setB)
{
    Set *newSet = init();

    int i = 0, j = 0;

    while (i < setA->length && j < setB->length) {
        if (setA->members[i] < setB->members[j]) {
            set_add(newSet, setA->members[i]);
            i++;
        } else if (setA->members[i] > setB->members[j]) {
            j++;
        } else {
            // Elements are equal, skip both and advance indices
            i++;
            j++;
        }
    }

    //  set_add remaining elements from setA
    while (i < setA->length) {
        set_add(newSet, setA->members[i]);
        i++;
    }

    return newSet;
}

Set *copy_set(Set *original){
    Set *copy = init();
    
    // Copy the length
    copy->length = original->length;

    // Allocate memory for the members and copy the content
    copy->members = malloc(sizeof(int) * copy->length);
    for (int i = 0; i < copy->length; i++) {
        copy->members[i] = original->members[i];
    }

    return copy;
}




int get_size(Set *setA){
    return setA->length;
}