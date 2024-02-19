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

/*
 * Main entry point.
 */
int main(int argc, char **argv)
{
    char *spamdir, *nonspamdir, *maildir;

    if (argc != 4)
    {
        DEBUG_PRINT("usage: %s <spamdir> <nonspamdir> <maildir>\n", argv[0]);
        return 1;
    }
    spamdir = argv[1];
    nonspamdir = argv[2];
    maildir = argv[3];

    return 0;
}
