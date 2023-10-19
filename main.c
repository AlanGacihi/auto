#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

typedef struct item{
  char *word;
  int weight;
}Item;

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
    
    //Read the file once more, this time to fill in the data into memory
    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    char word[BUFSIZE]; //to be used for reading lines in the loop below
    int weight;
    for(int i = 0; i < wordCount; i++)
    {
        fscanf(fp, "%s %d\n",word,&weight);
        
        // Store the dictionary words into your data structure
        items[i].word = (char*)malloc(sizeof(char)*strlen(word));
    }
    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// read query list file /////////////////////////
    ////////////////////////////////////////////////////////////////////////
    fp = fopen("queries_1.txt", "r");
        
    //check if the file is accessible, just to make sure...
    if(fp == NULL){
        fprintf(stderr, "Error opening file:%s\n","queries_1.txt");
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
    

    fseek(fp, 0, SEEK_SET);// rewind to the beginning of the file, before reading it line by line.
    for(int i = 0; i < queryCount; i++)
    {
        fscanf(fp, "%s\n",word);

        // Store the query words in a list like data structure 
        queries[i] = (char*)malloc(sizeof(char)*strlen(word));  
    }
    //close the input file
    fclose(fp);

    ////////////////////////////////////////////////////////////////////////
    ///////////////////////// reading input is done ////////////////////////
    ////////////////////////////////////////////////////////////////////////
    
    //Now it is your turn to do the magic!!!
    //do search/sort/print, whatever you think you need to do to satisfy the requirements of the assignment!
    //loop through the query words and list suggestions for each query word if there are any
    //don't forget to free the memory before you quit the program!

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
