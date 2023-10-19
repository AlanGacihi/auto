#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct item {
    char *word;
    int weight;
} Item;

// Comparison function for sorting items based on the 'word' field
int compareItems(const void *a, const void *b) {
    return strcmp(((Item *)a)->word, ((Item *)b)->word);
}

// Binary search function to find all items matching the query
Item* binarySearch(Item* items, int n, char* query) {
    // Sort the array of items based on the 'word' field
    qsort(items, n, sizeof(Item), compareItems);

    Item* results = (Item*)malloc(10 * sizeof(Item));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
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

int main() {
    Item items[] = {
        {"apple", 10},
        {"banana", 20},
        {"cherry", 15},
        {"date", 12},
        {"fig", 18},
        {"grape", 25},
        {"kiwi", 22},
        {"mango", 30},
        {"orange", 8},
        {"pear", 17}
    };
    int n = sizeof(items) / sizeof(Item);

    char query[] = "kiwi";
    Item* result = binarySearch(items, n, query);

    printf("Items matching the query '%s':\n", query);
    for (int i = 0; i < 10 && result[i].word != NULL; i++) {
        printf("%s (%d)\n", result[i].word, result[i].weight);
    }

    free(result); // Don't forget to free the allocated memory

    return 0;
}
