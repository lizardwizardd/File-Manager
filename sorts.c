#include "header.h"

//
// TODO: redo all sorts to sort 1 array for greater efficency and simplicity
//

void swapInt(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void swapStr(char *str1, char *str2)
{
    char *temp = (char *)malloc((strlen(str1) + 1) * sizeof(char));
    strcpy(temp, str1);
    strcpy(str1, str2);
    strcpy(str2, temp);
    free(temp);
}

void SortBubble(char names[][100], char dates[][30], int sizes[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        for (j = 0; j < n - i - 1; j++)
            if (sizes[j] > sizes[j + 1])
            {
                swapInt(&sizes[j], &sizes[j + 1]);
                swapStr(names[j], names[j + 1]);
                swapStr(dates[j], dates[j + 1]);
            }
}


void SortSelection(int sizes[], int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (sizes[j] < sizes[min_idx])
            min_idx = j;
 
        // Swap the found minimum element with the first element
           if(min_idx != i)
            swapInt(&sizes[min_idx], &sizes[i]);
    }
}


void SortInsertion(int sizes[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = sizes[i];
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && sizes[j] > key) {
            sizes[j + 1] = sizes[j];
            j = j - 1;
        }
        sizes[j + 1] = key;
    }
}


/*int main()
{
    int sizes[4] = {2, 1, 7, 4};
    char names[4][100] = {"n2", "n1", "n7", "n4"};
    char dates[4][30] = {"d2", "d1", "d7", "d4"};

    sort_bubble(names, dates, sizes, 4);

    printf("%s %s %s %s\n%d %d %d %d", names[0],names[1],names[2],names[3], sizes[0], sizes[1], sizes[2], sizes[3]);
} */

