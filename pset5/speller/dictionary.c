// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
#define N 17576 // 26*26*26

// Hash table
node *table[N];

// Word counter
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char temp[LENGTH + 1];
    int i = 0;
    for (; word[i]; i++)
        temp[i] = tolower(word[i]);
    temp[i] = '\0';

    unsigned int index = hash(temp);
    for (node *cursor = table[index]; cursor != NULL; cursor = cursor->next)
    {
        if (strcmp(cursor->word, temp) == 0)
            return true;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash_value = 0;
    for (int i = 0; i < 3 && word[i]; i++)
    {
        if (isalpha(word[i]))
            hash_value = hash_value * 26 + (tolower(word[i]) - 'a');
        else
            hash_value = hash_value * 26; // treat non-alpha as 0
    }
    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (!file)
        return false;

    char word[LENGTH + 1];

    while (fscanf(file, "%45s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (!new_node)
        {
            fclose(file);
            return false;
        }

        // Lowercase the word inline
        int i = 0;
        for (; word[i]; i++)
            new_node->word[i] = tolower(word[i]);
        new_node->word[i] = '\0';

        unsigned int index = hash(new_node->word);
        new_node->next = table[index];
        table[index] = new_node;

        word_count++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        table[i] = NULL;
    }
    return true;
}
