// Zain Mukati COP 3502
// Spring 2019 za465651

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "TriePrediction.h"


void string_print(char *t)
{
    while (*t)
        printf("%c", *(t++));
    printf("\n");
}

// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
    int i;
    if (root == NULL)
    {
        return;
    }
    if (root->count > 0)
        printf("%s (%d)\n", buffer, root->count);

        buffer[k + 1] = '\0';

        for (i = 0; i < 26; i++)
        {
            buffer[k] = 'a' + i;
            printTrieHelper(root->children[i], buffer, k + 1);
        }
        buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
    char buffer[1026];
    if (useSubtrieFormatting)
    {
        strcpy(buffer, "- ");
        printTrieHelper(root, buffer, 2);
    }
    else
    {
        strcpy(buffer, "");
        printTrieHelper(root, buffer, 0);
    }
}

// Modified version of printTrieHelper (Credit: Dr.S)
void process_print_helper(TrieNode *root, char *buffer, int k)
{
    int i;

    if (root == NULL)
    {
        return;
    }
    if (root->count > 0)
        printf("%s ", buffer);

        buffer[k + 1] = '\0';

        for (i = 0; i < 26; i++)
        {
            buffer[k] = 'a' + i;
            process_print_helper(root->children[i], buffer, k + 1);
        }
        buffer[k] = '\0';
}

// Modified version of printTrie (Credit: Dr.S)
void procss_print(TrieNode *root, int useSubtrieFormatting)
{
    char buffer[1026];

    if (useSubtrieFormatting)
    {
        strcpy(buffer, "- ");
        process_print_helper(root, buffer, 2);
    }
    else
    {
        strcpy(buffer, "");
        process_print_helper(root, buffer, 0);
    }
}

// Credits go to Dr.S
TrieNode *createNode(void)
{
    int i;
    TrieNode *n = malloc(sizeof(TrieNode));
    n->subtrie = NULL;
    n->count = 0;

    for (i = 0; i < 26; i++)
    {
        n->children[i] = NULL;
    }
    return n;
}

void stripPunctuators(char *about_to_be_naked_string)
{
    int i = 0;
    int x = 0;
    int length = strlen(about_to_be_naked_string);
    char *buffer = malloc(sizeof(char) * (length + 1));

    for (i = 0; i < length; i++)
    {
        if (isalpha(about_to_be_naked_string[i]) != 0)
        {
            buffer[x++] = tolower(about_to_be_naked_string[i]);
        }
    }
    buffer[x] = '\0';

    strcpy(about_to_be_naked_string, buffer);
    free(buffer);
    return;
}

// Credits go to Dr.S
TrieNode *insert(TrieNode *root, char *str)
{
    int i, index, len;
    TrieNode *wizard;
    if (root == NULL)
        root = createNode();

    wizard = root;
    len = strlen(str);

    for (i = 0; i < len; i++)
    {
        if (isalpha(str[i]) != 0)
        {
            index = tolower(str[i]) - 'a';

            if (wizard->children[index] == NULL)
            {
                wizard->children[index] = createNode();
            }
            wizard = wizard->children[index];
        }
    }
    wizard->count++;
    return root;
}

// Modified version of buildTrie (Credits go to Dr.S)
TrieNode *buildTrie(char *filename)
{
    TrieNode *root = NULL;
    char buffer[1026];
    char prev[1026];
    TrieNode *prev_loc = NULL;
    int flag = 0;
    int flag2 = 0;
    int flag3 = 0;
    int len;
    FILE *ifp;

    // Used to check for end of line punctuation
    char *check_for_periods = malloc(sizeof(char));
    char *check_for_periods2 = malloc(sizeof(char));
    char *check_for_periods3 = malloc(sizeof(char));
    check_for_periods[0]= 'X';
    check_for_periods2[0] = 'X';
    check_for_periods3[0] = 'X';

    if ((ifp = fopen(filename, "r")) == NULL)
    {
        return NULL;
    }
    // Insert strings one-by-one into the trie.
    while (fscanf(ifp, "%s", buffer) != EOF)
    {
        root = insert(root, buffer);
        len = strlen(buffer);

        if (flag3 == 1)
        {
            strcpy(prev, buffer);
            flag3 = 0;
            continue;
        }

        // if flag is on next word wont be inserted into this words subtrie
        if (buffer[len - 1] == '!' || buffer[len - 1] == '.' || buffer[len - 1] == '?')
        {
            flag2 = 1;
        }

        // Flag for the first word in a sentence into a subtrie
        if (flag == 1)
        {
            if (flag3 != 1)
            {
                prev_loc = getNode(root, prev);
                prev_loc->subtrie = insert(prev_loc->subtrie, buffer);
            }
            if (flag2 == 1)
            {
                flag2 = 0;
                flag3 = 1;
            }
        }
        strcpy(prev, buffer);
        flag = 1;
    }
    fclose(ifp);
    free(check_for_periods);
    free(check_for_periods2);
    free(check_for_periods3);
    return root;
}

int processInputFile(TrieNode *root, char *filename)
{
    char buffer[1025];
    char freqbuffer[1025];
    char fodder[1025];
    FILE *ifp;
    TrieNode *ptr;
    TrieNode *location;
    int flag = 0;
    int flag2 = 0;
    int flag3 = 0;
    int command_count = 0;
    int i = 0;
    int num_length = 0 ;
    int x;
    if ((ifp = fopen(filename, "r")) == NULL)
    {
        return 1;
    }
    // Insert strings one-by-one into the trie.
    while (fscanf(ifp, "%s", buffer) != EOF)
    {
        // condition for @ (starts the sequence)
        if (strcmp(buffer, "@") == 0)
        {
            flag = 1;
        }
        // condition for !
        else if (strcmp(buffer, "!") == 0)
        {
            printTrie(root, 0);
        }
        // Once we hit the number for the @ command
        else if (flag2 == 1)
        {
            command_count = atoi(buffer);
            if (command_count == 0)
            {
                printf("%s\n", fodder);
                continue;
            }
            printf("%s ", fodder);;
            location = getNode(root, fodder);

            if (location->subtrie == NULL )
            {
                flag2 = 0;
                flag = 0;
                continue;
            }
            while (command_count > i)
            {
                getMostFrequentWord(location->subtrie, freqbuffer);
                location = getNode(root, freqbuffer);

                if (location->subtrie == NULL || i == (command_count - 1))
                {
                    printf("%s", freqbuffer);
                    flag2 = 0;
                    flag = 0;
                    break;
                }
                printf("%s ", freqbuffer);
                i++;
            }
            i = 0;
            printf("\n");
            flag2 = 0;
            flag = 0;
        }
        // string copys for to prepare for the number
        else if (containsWord(root, buffer) == 1 && flag == 1)
        {
            strcpy(fodder, buffer);
            flag2 = 1;
            flag = 0;
        }
        // @ command but word not in trie
        else if (containsWord(root, buffer) == 0 && flag == 1)
        {
            printf("%s\n", buffer);
            flag = 0;
            continue;
        }
        // str condition
        else if (containsWord(root,buffer) == 1 && flag == 0 )
        {
            string_print(buffer);
            ptr = getNode(root, buffer);
            if (ptr->subtrie == NULL)
            {
                printf("(EMPTY)\n");
            }
            else
            {
                printTrie(ptr->subtrie, 1);
            }
        }
        // str not in trie
        else if (containsWord(root,buffer) == 0 && flag == 0)
        {
            num_length = strlen(buffer);
            for (x = 0; x < num_length; x++)
            {
                if (isdigit(buffer[x]) != 0)
                {
                    flag3 = 1;
                    break;
                }
            }
            if (flag3)
            {
                flag3 = 0;
                continue;
            }
            string_print(buffer);
            printf("(INVALID STRING)\n");
        }
    }
    fclose(ifp);
    return 0;
}

TrieNode *destroyTrie(TrieNode *root)
{
    int i;
    if (root == NULL)
    {
        return NULL;
    }
    for (i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
        {
            root->children[i] = destroyTrie(root->children[i]);
        }
        if (root->subtrie != NULL)
        {
            root->subtrie = destroyTrie(root->subtrie);
        }
    }
    free(root);
    root = NULL;
    return NULL;
}

// recursivley finds all words with this prefix
void search_and_retrieve(TrieNode *root,int *counter)
{
    int i;
    TrieNode* hunter;
    if (root == NULL)
    {
        return;
    }
    hunter = root;
    for (i = 0; i < 26; i++)
    {
        if (hunter->children[i] != NULL)
        {
            search_and_retrieve(hunter->children[i],counter);
            *counter += hunter->children[i]->count;
        }
    }
    return;
}

TrieNode *getNode(TrieNode *root, char *str)
{
    int i, index, len;
    TrieNode *wizard;

    if (root == NULL)
    {
        return NULL;
    }
    wizard = root;
    len = strlen(str);

    for (i = 0; i < len; i++)
    {
        if (isalpha(str[i]) != 0)
        {
            index = tolower(str[i]) - 'a';

            if (wizard->children[index] == NULL)
            {
                return NULL;
            }
            wizard = wizard->children[index];
        }
    }
    if (wizard->count > 0)
    {
        return wizard;
    }
    return NULL;
}

// keeps copying the strings till highest count is found
void helper_mostFreq(TrieNode *root, char *str, char* buffer, int *count_for_most, int k)
{
    int i;
    if (root == NULL)
    {
        return;
    }
    buffer[k + 1] = '\0';
    if (root->count > *count_for_most)
    {
        *count_for_most = root->count;
        strcpy(str,buffer);
    }
    for (i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
        {
            buffer[k] = 'a' + i;
            helper_mostFreq(root->children[i],str, buffer,count_for_most, k + 1);
        }
    }
    buffer[k] = '\0';
    return;
}

void getMostFrequentWord(TrieNode *root, char *str)
{
    char buffer[1025];
    int i;
    int count[1];
    int k = 0;
    *count = 0;

    if (root == NULL)
    {
        strcpy(str, "");
        return;
    }
    helper_mostFreq(root, str, buffer, count,k);
    if (*count == 0)
    {
        strcpy(str, "");
    }
    return;
}

int containsWord(TrieNode *root, char *str)
{
    int i, index, len = strlen(str);
    TrieNode *wizard;
    char *my_copy = malloc(sizeof(char) * (len + 1));
    if (root == NULL)
    {
        return 0;
    }

    wizard = root;
    strcpy(my_copy, str);
    len = strlen(my_copy);
    for (i = 0; i < len; i++)
    {
        if (isalpha(str[i]) != 0)
        {
            index = tolower(str[i]) - 'a';

            if (wizard->children[index] == NULL)
            {
                free(my_copy);
                return 0;
            }
            wizard = wizard->children[index];
        }
    }
    if (wizard->count > 0)
    {
        free(my_copy);
        return 1;
    }
    free(my_copy);
    return 0;
}

int prefixCount(TrieNode *root, char *str)
{
    int i, index, len = strlen(str);
    int *string_count = malloc(sizeof(int));
    int stringcnt = 0;
    TrieNode *wizard;
    *string_count = 0;

    if (root == NULL)
    {
        return 0;
    }
    wizard = root;
    len = strlen(str);
    for (i = 0; i < len; i++)
    {
        if (isalpha(str[i]) != 0)
        {
            index = tolower(str[i]) - 'a';
            if (wizard->children[index] == NULL)
            {
                free(string_count);
                return 0;
            }
            wizard = wizard->children[index];
        }
    }
    *string_count += wizard->count;
    search_and_retrieve(wizard, string_count);
    stringcnt = *string_count;
    free(string_count);
    return stringcnt;
}

double difficultyRating(void)
{
    return 5.0;
}

double hoursSpent(void)
{
    return 40.0;
}

int main(int argc, char **argv)
{
    char *corpus = argv[1];
    char *input = argv[2];
    TrieNode *root = buildTrie(corpus);
    processInputFile(root, input);
    destroyTrie(root);
    return 0;
}
