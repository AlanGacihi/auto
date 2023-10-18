#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct item {
    char *word;
    int weight;
} Item;

// Function to compare items by weight for sorting
int compareItems(const void *a, const void *b) {
    return ((Item *)b)->weight - ((Item *)a)->weight;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <knowledge_base_file> <query_list_file>\n", argv[0]);
        return 1;
    }

    char *dictionaryFilePath = argv[1];
    char *queryFilePath = argv[2];

    int wordCount = 0;
    int queryCount = 0;

    // Read the dictionary file
    FILE *fp = fopen(dictionaryFilePath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", dictionaryFilePath);
        return -1;
    }

    char word[BUFSIZE];
    int weight;
    while (fscanf(fp, "%s %d\n", word, &weight) == 2) {
        wordCount++;
    }

    // Allocate memory for dictionary items
    Item *dictionary = (Item *)malloc(wordCount * sizeof(Item));
    if (dictionary == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }

    // Read the dictionary file again and store data
    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < wordCount; i++) {
        fscanf(fp, "%s %d\n", word, &weight);
        dictionary[i].word = strdup(word);
        dictionary[i].weight = weight;
    }
    fclose(fp);

    // Read the query list file
    fp = fopen(queryFilePath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", queryFilePath);
        return -1;
    }

    char *line = NULL;
    size_t lineBuffSize = 0;
    ssize_t lineSize;

    while ((lineSize = getline(&line, &lineBuffSize, fp) != -1)) {
        queryCount++;
    }
    free(line);

    // Allocate memory for query words
    char **queryWords = (char **)malloc(queryCount * sizeof(char *));
    if (queryWords == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }

    fseek(fp, 0, SEEK_SET);
    for (int i = 0; i < queryCount; i++) {
        getline(&queryWords[i], &lineBuffSize, fp);
        // Remove newline character from the query word
        queryWords[i][strcspn(queryWords[i], "\n")] = '\0';
    }
    fclose(fp);

    // Process queries and suggest words
    for (int i = 0; i < queryCount; i++) {
        printf("Query word:%s\n", queryWords[i]);

        int suggestionsCount = 0;
        for (int j = 0; j < wordCount; j++) {
            if (strncmp(dictionary[j].word, queryWords[i], strlen(queryWords[i])) == 0) {
                printf("%s %d\n", dictionary[j].word, dictionary[j].weight);
                suggestionsCount++;

                if (suggestionsCount == 10) {
                    break;  // Limit to the top 10 suggestions
                }
            }
        }

        if (suggestionsCount == 0) {
            printf("No suggestion!\n");
        }
    }

    // Free allocated memory
    for (int i = 0; i < queryCount; i++) {
        free(queryWords[i]);
    }
    free(queryWords);

    for (int i = 0; i < wordCount; i++) {
        free(dictionary[i].word);
    }
    free(dictionary);

    return 0;
}