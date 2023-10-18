#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
#define MAX_SUGGESTIONS 10

typedef struct item {
    char *word;
    int weight;
} Item;

// Function to compare items based on weight
int compareItems(const void *a, const void *b) {
    return ((Item *)b)->weight - ((Item *)a)->weight;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <knowledge_base_file> <query_list_file>\n", argv[0]);
        return -1;
    }

    char *dictionaryFilePath = argv[1];
    char *queryFilePath = argv[2];
    int wordCount = 0;
    int queryCount = 0;

    // Read the knowledge base file and store the data in an array of Items
    FILE *fp = fopen(dictionaryFilePath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", dictionaryFilePath);
        return -1;
    }

    Item *dictionary = NULL;

    char word[BUFSIZE];
    int weight;

    while (fscanf(fp, "%s %d\n", word, &weight) != EOF) {
        wordCount++;
        dictionary = (Item *)realloc(dictionary, wordCount * sizeof(Item));
        dictionary[wordCount - 1].word = strdup(word);
        dictionary[wordCount - 1].weight = weight;
    }

    fclose(fp);

    // Read the query list file and process queries
    fp = fopen(queryFilePath, "r");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", queryFilePath);
        return -1;
    }

    char *line = NULL;
    size_t lineBuffSize = 0;

    while (getline(&line, &lineBuffSize, fp) != -1) {
        queryCount++;
        char *query = strdup(line);

        // Remove newline character from the query
        if (query[strlen(query) - 1] == '\n') {
            query[strlen(query) - 1] = '\0';
        }

        printf("Query word: %s\n", query);

        int suggestions = 0;
        // Search the dictionary for suggestions
        for (int i = 0; i < wordCount; i++) {
            if (strstr(dictionary[i].word, query) == dictionary[i].word) {
                printf("%s %d\n", dictionary[i].word, dictionary[i].weight);
                suggestions++;
            }
            if (suggestions >= MAX_SUGGESTIONS) {
                break;
            }
        }

        if (suggestions == 0) {
            printf("No suggestion!\n");
        }

        free(query);
    }

    free(line);
    fclose(fp);

    // Free memory
    for (int i = 0; i < wordCount; i++) {
        free(dictionary[i].word);
    }
    free(dictionary);

    return 0;
}
