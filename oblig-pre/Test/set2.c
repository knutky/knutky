

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

void merge_sort(Set *set);
void merge_sort_recursion(int a[], int l, int r, Set *set);
void merge_sorted_arrays(int a[], int l, int m, int r, Set *set);

int main(){
    Set *set = init();

    insert(set, 9);
    insert(set, 4);
    insert(set, 8);
    insert(set, 2);
    insert(set, 7);
    insert(set, 0);
    insert(set, 0);
    insert(set, 2);
    insert(set, 2);
    insert(set, 6);

    printf("Set before sorting: ");
    print_set(set);

    // Sort the set
    merge_sort(set);

    printf("Set after sorting: ");
    print_set(set);

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

void merge_sort(Set *set) {
    merge_sort_recursion(set->members, 0, set->length - 1, set);
}

void merge_sort_recursion(int a[], int l, int r, Set *set) {
    if (l < r) {
        int m = l + (r - l) / 2;

        merge_sort_recursion(a, l, m, set);
        merge_sort_recursion(a, m + 1, r, set);

        merge_sorted_arrays(a, l, m, r, set);
    }
}

void merge_sorted_arrays(int a[], int l, int m, int r, Set *set) {
    int left_length = m - l + 1;
    int right_length = r - m;

    int temp_left[left_length];
    int temp_right[right_length];

    int i, j, k;

    for (i = 0; i < left_length; i++)
        temp_left[i] = a[l + i];

    for (i = 0; i < right_length; i++)
        temp_right[i] = a[m + 1 + i];

    i = 0;
    j = 0;
    k = l;

    while (i < left_length && j < right_length) {
        if (temp_left[i] <= temp_right[j]) {
            a[k] = temp_left[i];
            i++;
        } else {
            a[k] = temp_right[j];
            j++;
        }
        k++;
    }

    while (i < left_length) {
        a[k] = temp_left[i];
        i++;
        k++;
    }

    while (j < right_length) {
        a[k] = temp_right[j];
        j++;
        k++;
    }

    // Update set length after sorting
    set->length = r + 1;
}

