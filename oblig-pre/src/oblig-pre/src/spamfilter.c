/* Author: Steffen Viken Valvaag <steffenv@cs.uit.no> */
#include "common.h"
#include "list.h"
#include "printing.h"
#include "set.h"

/*
 * Case-insensitive comparison function for strings.
 */
static int compare_words(void *a, void *b)
{
    return strcasecmp(a, b);
}

/*
 * Returns the set of (unique) words found in the given file.
 */
static set_t *tokenize(char *filename)
{
    set_t *wordset = set_create(compare_words);
    list_t *wordlist = list_create(compare_words);
    list_iter_t *it;
    FILE *f;

    f = fopen(filename, "r");
    if (f == NULL)
    {
        perror("fopen");
        ERROR_PRINT("fopen() failed");
    }
    tokenize_file(f, wordlist);

    it = list_createiter(wordlist);
    while (list_hasnext(it))
    {
        set_add(wordset, list_next(it));
    }
    list_destroyiter(it);
    list_destroy(wordlist);
    fclose(f); // Close the file when done

    return wordset;
}

/*
 * Prints a set of words.
 */
static void printwords(char *prefix, set_t *words)
{
    set_iter_t *it;

    it = set_createiter(words);
    INFO_PRINT("%s: ", prefix);
    while (set_hasnext(it))
    {
        INFO_PRINT(" %s", (char *)set_next(it));
    }
    printf("\n");
    set_destroyiter(it);
}


int spam_filter(set_t *spam_fil, char *filename){
    set_t *M = tokenize(filename);
    set_t *T = set_intersection(spam_fil, M);
    size_t L = set_size(T);
    
    /*if (L > 0){
        printf("mail/mail5.txt: %zu spam word(s) -> SPAM\n", L);
    } else
        printf("mail/mail5.txt: %zu spam word(s) -> Not spam\n", L);*/
    set_destroy(M);
    set_destroy(T);

}


/*
 * Main entry point.
 */
int main()
{   
    for (int i = 0; i<1000; i++){
    // Formula: M ∩ ((S1 ∩ S2 ∩ ... ∩ Sn ) - (N1 ∪ N2 ∪ ... ∪ Nm)) ≠ ∅
    //Spam
        set_t *S1 = tokenize( "./data/spam/spam1.txt");
        set_t *S2 = tokenize( "./data/spam/spam2.txt");
        set_t *S3 = tokenize( "./data/spam/spam3.txt");
        set_t *S4 = tokenize( "./data/spam/spam4.txt");
        //nonspam
        set_t *N1 = tokenize( "./data/nonspam/nonspam1.txt");
        set_t *N2 = tokenize( "./data/nonspam/nonspam2.txt");
        set_t *N3 = tokenize( "./data/nonspam/nonspam3.txt");
        set_t *N4 = tokenize( "./data/nonspam/nonspam4.txt");
        // Union
        set_t *U1 = set_union(N1, N2);
        set_t *U2 = set_union(N3, N4);
        set_t *U = set_union(U1, U2);
        // Intersection
        set_t *I1 = set_intersection(S1, S2);
        set_t *I2 = set_intersection(S3, S4);
        set_t *I = set_intersection(I1, I2);
        //Differense
        set_t *D = set_difference(I, U);

        spam_filter(D, "./data/mail/mail1.txt");
        spam_filter(D, "./data/mail/mail2.txt");
        spam_filter(D, "./data/mail/mail3.txt");
        spam_filter(D, "./data/mail/mail4.txt");
        spam_filter(D, "./data/mail/mail5.txt");

        set_destroy(S1);
        set_destroy(S2);
        set_destroy(S3);
        set_destroy(S4);
        set_destroy(N1);
        set_destroy(N2);
        set_destroy(N3);
        set_destroy(N4);
        set_destroy(U1);
        set_destroy(U2);
        set_destroy(U);
        set_destroy(I1);
        set_destroy(I2);
        set_destroy(I);
        set_destroy(D);
        }
    return 0;
    //mail/mail5.txt: 0 spam word(s) -> Not spam
}
