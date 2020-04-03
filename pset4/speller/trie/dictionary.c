// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Number of word in dictionary
int words_in_dictionary = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // A temp node to keep in mind in wich children i am.
        node *tmp = root;

        // Loop every char in the word
        for (char *ptr = word; *ptr != '\0'; ptr++)
        {
            // Hash function
            int char_index = 26;
            if (*ptr != '\'')
            {
                char_index = *ptr - 97;
            }

            if (tmp->children[char_index] == NULL)
            {
                node *new_children = malloc(sizeof(node));
                if (new_children == NULL)
                {
                    unload();
                    return false;
                }
                // Init the new node
                new_children->is_word = false;
                for (int i = 0; i < N; i++)
                {
                    new_children->children[i] = NULL;
                }

                tmp->children[char_index] = new_children;
            }

            tmp = tmp->children[char_index];
        }
        tmp->is_word = true;
        words_in_dictionary++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return words_in_dictionary;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *tmp = root;
    for (const char *ptr = word; *ptr != '\0'; ptr++)
    {
        // Hash function
        int char_index = 26;
        if (*ptr != '\'')
        {
            char_index = tolower(*ptr) - 97;
        }
        // If is not the end of the word
        if (tmp->children[char_index] == NULL)
        {
            return false;
        }
        tmp = tmp->children[char_index];
    }
    return tmp->is_word;
}

bool unload_node(node *nodo)
{
    for ( int n = 0; n < N; n++)
    {
        if (nodo->children[n] != NULL)
        {
            unload_node(nodo->children[n]);
        }
    }
    free(nodo);
    return true;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return unload_node(root);
}