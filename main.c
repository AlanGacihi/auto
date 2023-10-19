#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct item{
  char *word;
  int weight;
}Item;


// Swap two items in the 'items' array.
void swap(Item* items, int i, int j) 
{
    Item temp = items[i];
    items[i] = items[j];
    items[j] = temp;
}

// Partition the 'items' array
int partition(Item* items, int low, int high)
{
    char* pivot = items[high].word;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (strcmp(items[j].word, pivot) < 0) {
            i++;
            swap(items, i, j);
        }
    }
    swap(items, i + 1, high);
    return (i + 1);
}

// Sort an array of Item structures
void sort(Item* items, int low, int high)
{
    if (low < high) {
        int pi = partition(items, low, high);
        sort(items, low, pi - 1);
        sort(items, pi + 1, high);
    }
}

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
        int mid = left + (right - left) / 2; // To avoid potential integer overflow
        int cmp = strcmp(items[mid].word, query);

        if (cmp == 0) {
            // Match found, add to results array
            results[count].word = strdup(items[mid].word);
            results[count].weight = items[mid].weight;
            count++;
            if (count >= 10) {
                break;
            }

            // Check for other matches on the left side
            int i = mid - 1;
            while (i >= 0 && strcmp(items[i].word, query) == 0) {
                results[count].word = strdup(items[i].word);
                results[count].weight = items[i].weight;
                count++;
                if (count >= 10) {
                    break;
                }
                i--;
            }

            // Check for other matches on the right side
            i = mid + 1;
            while (i < n && strcmp(items[i].word, query) == 0) {
                results[count].word = strdup(items[i].word);
                results[count].weight = items[i].weight;
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

    printf("Count: %d   N: %d\n", count, n);

    // Sort results
    sort(results, 0, count - 1);

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
    sort(items, 0, wordCount-1);

    ////////////////////////////////////////////////////////////////////////
    //////////////////////////// Process queries ///////////////////////////
    ////////////////////////////////////////////////////////////////////////
    for(int i = 0; i < queryCount; i++)
    {
        // Search in knowledge base
        Item* results = binarySearch(items, wordCount, queries[i]);

        int suggestionsCount = 0; // Store number of suggestions
        printf("Query word:%s\n", queries[i]);
        
        // Loop through the results
        for (int i = 0; i < 10 && results[i].word != NULL; i++)
        {
            printf("%s %d\n", results[i].word, results[i].weight);
            suggestionsCount++;
            free(results[i].word);
        }
        if (suggestionsCount == 0) {
            printf("No suggestion!\n");
        }
        free(results);
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
