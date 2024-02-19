

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int *members; // A pointer to an integer, indicating a dynamic array or set of integers.

    int length;
} Set;

Set* init();    //This function declaration is for an init function that returns a pointer to a Set. It is likely meant to initialize a new set.
                //The init function is expected to return a pointer to a newly initialized Set structure.


bool is_empty(Set *set);
void insert(Set *set, int member); 
void print_set(Set *set);
Set *set_union(Set *setA, Set *setB); // Return a new pointer to the union of two sets
Set *set_intersection(Set *setA, Set *setB); // Return a new pointer to the union of two sets
Set *set_difference(Set *setA, Set *setB);

int main(){
    Set *setA = init();     //Set *setA: Declares a variable named setA with a type of pointer to Set.
                            //= init();: Calls the init function, which should return a pointer to a newly created Set, and assigns this pointer to setA.
                            // setA points to a newly initialized set

    if (is_empty(setA)) printf("Set A is empty.\n");
    else printf("Set A is not empty.\n");

    insert(setA,5);
    insert(setA,6);
    insert(setA,7); 
    insert(setA,7);
    insert(setA,8); 


    printf("Set A: ");
    print_set(setA);

    Set *setB = init();
    insert(setB,7);
    insert(setB,8); 
    insert(setB,9);
    insert(setB,10); 

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

// Parameters:
//   - set: A pointer to a Set structure.
//   - member: The integer to be inserted into the set.
// Note: This function assumes that 'set' is a valid pointer to a Set structure.
void insert(Set *set, int member){
    bool in_set = false; 

    // Iterate through the existing members of the set
    for (int i = 0; i < set->length;i++) //
        if (set->members[i] == member)
            in_set = true;


    if (!in_set){
        // Allocate space for an additional integer in the set
        set->members =
            realloc(set->members, sizeof(int) * (set->length + 1)); //Legger til plass til enda en integer
        set->members[set->length] = member;   // Add a member of end of list
        set->length = set->length +1; // increase length of set by 1
        
    }

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
    Set *new = init();

    for (int i = 0; i < setA->length; i++)    // Just using insertion. That will also take care of overlap
        insert(new, setA->members[i]);

    for (int i = 0; i < setB->length; i++)
        insert(new, setB->members[i]);

    return new;
}


Set *set_intersection(Set *setA, Set *setB) // Return a new pointer to the union of two sets
{
    Set *new = init();
    for (int i = 0; i < setA->length; i++)
        for (int j = 0; j < setB->length; j ++)
            if (setA->members[i] == setB->members[j])
                insert(new, setA->members[i]);
    return new;
}

Set *set_difference(Set *setA, Set *setB)
{
    Set *new = init();
    for (int i = 0; i < setA->length; i++)
    {
        bool inB = false;
        for (int j = 0; j < setB->length; j ++)
            if ((setA-> members[i]) == setB->members[j])
                inB = true;
        if (!inB) insert(new, setA->members[i]);
    }
    return new;
}

