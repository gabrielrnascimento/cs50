// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// ? Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    char lowercase_word[strlen(word)];
    strcpy(lowercase_word, word);
    for (int i = 0; i < strlen(word); i++)
    {
        lowercase_word[i] = tolower(word[i]);
    }

    int index = hash(word);
    node *travelptr = table[index];
    while (true) // ? only iterate through the list from the hash function searching for word
    {
        if (table[index] == NULL)
        {
            break;
        }
        else if (strcmp(travelptr->word, lowercase_word) == 0)
        {
            return true;
        }
        else if (travelptr->next == NULL)
        {
            break;
        }
        else
        {
            travelptr = travelptr->next;
        }
    }
    return false;
}

// ? Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    if (strlen(word) == 1)
    {
        return toupper(word[0]) - 'A';
    }
    int hash_code = 27;
    for (int i = 0; i < strlen(word); i++)
    {
        hash_code = hash_code + toupper(word[i]) - 'A';
    }
    return hash_code;

}

// ? Reference: https://stackoverflow.com/questions/31930046/what-is-a-hash-table-and-how-do-you-make-it-in-c
void insert_node(int key, char *value)
{
    // ? try to instantiate node to insert word
    node *newptr = malloc(sizeof(node));
    if (newptr == NULL)
    {
        return;
    }
    // ? make a new pointer
    strcpy(newptr->word, value);
    newptr->next = NULL;

    // ? check if list is empty
    if (table[key] == NULL)
    {
        table[key] = newptr;
    }
    else // ? if not first item then append to linked list (head)
    {
        node *travelptr = table[key];
        table[key] = newptr;
        newptr->next = travelptr;
    }
    // else // ? if not first item then append to linked list (tail)
    // {
    //     node *travelptr = table[key];
    //     while (true)
    //     {
    //         if (travelptr->next == NULL)
    //         {
    //             travelptr->next = newptr;
    //             break;
    //         }
    //         travelptr = travelptr->next;
    //     }
    // }
}

//? Loads dictionary into memory, returning true if successful, else false
// TODO
bool load(const char *dictionary)
{
    //* TODO Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    //* TODO Read strings from file one at a time
    char scan_word[LENGTH + 1];
    while (fscanf(file, "%s", scan_word) != EOF)
    {
        //* TODO Create a new node for each word
        //* TODO Hash word to obtain a hash value
        int hash_value = hash(scan_word);
        //* TODO Insert node into hash table at that location
        insert_node(hash_value, scan_word);
    }
    fclose(file);
    return true;
}

// ? Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    int word_counter = 0;
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *travelptr = table[i];
            while (true)
            {
                word_counter++;
                if (travelptr->next == NULL)
                {
                    break;
                }
                else
                {
                    travelptr = travelptr->next;
                }
            }
        }
    }
    // printf("%i\n", word_counter);
    return word_counter;
    // return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
void free_list(node *list)
{
    if (list == NULL)
    {
        return;
    }
    free_list(list->next);
    free(list);
}
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        free_list(table[i]);
    }
    return true;
}
