#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct item{
  char *word;
  int weight;
}Item;

// Comparison function for sorting items based on the 'word' field
int compareItems(const void *a, const void *b) {
    return strcmp(((Item *)a)->word, ((Item *)b)->word);
}

// Binary search function to find all items matching the query
Item* binarySearch(Item* items, int n, char* query) {

    Item* results = (Item*)malloc(10 * sizeof(Item));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    for (int i = 0; i < 10; i++) {
        results[i].word = NULL; // Initialize the word field to NULL
        results[i].weight = 0;
    }

    int count = 0;
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(items[mid].word, query);

        if (cmp == 0) {
            // Match found, add to results array
            results[count] = items[mid];
            count++;
            if (count >= 10) {
                break;
            }

            // Check for other matches on the left side
            int i = mid - 1;
            while (i >= 0 && strcmp(items[i].word, query) == 0) {
                results[count] = items[i];
                count++;
                if (count >= 10) {
                    break;
                }
                i--;
            }

            // Check for other matches on the right side
            i = mid + 1;
            while (i < n && strcmp(items[i].word, query) == 0) {
                results[count] = items[i];
                count++;
                if (count >= 10) {
                    break;
                }
                i++;
            }

            break;
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return results;
}

int main(int argc, char **argv) {
    //char *dictionaryFilePath = argv[1]; //this keeps the path to dictionary file
    //char *queryFilePath = argv[2]; //this keeps the path to the file that keeps a list of query wrods, 1 query per line
    int wordCount=0; //this variable will keep a count of words in the dictionary, telling us how much memory to allocate
    int queryCount=0; //this variable will keep a count of queries in the query file, telling us how much memory to allocate for the query words
    
    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// read dictionary file /////////////////////////
    ////////////////////////////////////////////////////////////////////////
    FILE *fp = fopen("simpsons_rand.txt", "r");
    char *line = NULL; //variable to be used for line counting
    size_t lineBuffSize = 0; //variable to be used for line counting
    ssize_t lineSize; //variable to be used for line counting
    
    //check if the file is accessible, just to make sure...
    if(fp == NULL){
        fprintf(stderr, "Error opening file:%s\n","dictionaryFilePath");
        return -1;
    }

    //First, let's count number of lines. This will help us know how much memory to allocate
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
    {
        wordCount++;
    }

    
    // Allocate memory for your data structure, by the size of "wordCount"
    Item* items = (Item*)malloc(sizeof(Item)*wordCount);
    if (items == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }

    
    //Read the file once more, this time to fill in the data into memory
    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    char word[BUFSIZE]; //to be used for reading lines in the loop below
    int weight;
    for(int i = 0; i < wordCount; i++)
    {
        fscanf(fp, "%s %d\n",word,&weight);
        
        // Store the dictionary words into your data structure
        items[i].word = strdup(word);
        items[i].weight = weight;
    }
    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// read query list file /////////////////////////
    ////////////////////////////////////////////////////////////////////////
    fp = fopen("queries_1.txt", "r");
        
    //check if the file is accessible, just to make sure...
    if(fp == NULL){
        fprintf(stderr, "Error opening file:%s\n","queryFilePath");
        return -1;
    }

    //First, let's count number of queries. This will help us know how much memory to allocate
    while((lineSize = getline(&line,&lineBuffSize,fp)) !=-1)
    {
        queryCount++;
    }
    free(line); //getline internally allocates memory, so we need to free it here so as not to leak memory!!


    
    // Allocate memory for storing query words, by the size of "queryCount"
    char** queries = (char**)malloc(sizeof(char*)*queryCount);
    if (queries == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return -1;
    }
    

    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    for(int i = 0; i < queryCount; i++)
    {
        fscanf(fp, "%s\n",word);

        // Store the query words in a list like data structure 
        queries[i] = strdup(word);  
    }
    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    //////////////////////// Sort the knowledge base ///////////////////////
    ////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < wordCount; i++)
    {
        for(int j = i+1; j < wordCount; j++)
        {
            if(strcmp(items[i].word, items[j].word) > 0)
            {
                char* temp = items[i].word;
                items[i].word = items[j].word;
                items[j].word = temp;
            }
        }
    }

    // Loop through the query words and list suggestions for each query word if there are any
    for(int i = 0; i < queryCount; i++)
    {
        printf("Query word:%s\n", queries[i]);
        //loop through the dictionary and find the words that are similar to the query word
        for(int j = 0; j < wordCount; j++)
        {
            int suggestionsCount = 0;
            if (strncmp(items[j].word, queries[i], strlen(queries[i])) == 0) {
                printf("%s %d\n", items[j].word, items[j].weight);
                suggestionsCount++;

                if (suggestionsCount == 10) {
                    break;  // Limit to the top 10 suggestions
                }
            }
        }
    }
    

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////// Free memory /////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < wordCount; i++)
    {
        free(items[i].word);
    }
    free(items); // Free dictionary
    for(int i = 0; i < queryCount; i++)
    {
        free(queries[i]);
    }
    free(queries); // Free queries
    
    
    //OUTPUT SPECS:
    // use the following if no word to suggest: printf("No suggestion!\n");
    // use the following to print a single line of outputs (assuming that the word and weight are stored in variables named word and weight, respectively): 
    // printf("%s %d\n",word,weight);
    // if there are more than 10 outputs to print, you should print the top 10 weighted outputs.
    
    printf("Done\n");
    return 0;
}
