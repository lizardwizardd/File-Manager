#include "libs.h"

void swapFiles(struct _finddata_t* file1, struct _finddata_t* file2)
{
    struct _finddata_t temp = *file1;
    *file1 = *file2;
    *file2 = temp;
}

// Bubble sort
void sort_bubble(struct _finddata_t arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
 
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j].size > arr[j + 1].size)
                swapFiles(&arr[j], &arr[j + 1]);
}

// Selection sort
void sort_selection(struct _finddata_t arr[], int n)
{
    int i, j, min_idx;
 
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j].size < arr[min_idx].size)
            min_idx = j;
 
        // Swap the found minimum element with the first element
           if(min_idx != i)
            swapFiles(&arr[min_idx], &arr[i]);
    }
}

// Insertion sort
void sort_insertion(struct _finddata_t arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i].size;
        j = i - 1;
 
        /* Move elements of arr[0..i-1], that are
          greater than key, to one position ahead
          of their current position */
        while (j >= 0 && arr[j].size > key) {
            arr[j + 1].size = arr[j].size;
            j = j - 1;
        }
        arr[j + 1].size = key;
    }
}

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void sort_merge_arrays(struct _finddata_t arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    struct _finddata_t L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].size <= R[j].size) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void sort_merge(struct _finddata_t arr[], int l, int r)
{
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        sort_merge(arr, l, m);
        sort_merge(arr, m + 1, r);
 
        sort_merge_arrays(arr, l, m, r);
    }
}

void sort_quick(struct _finddata_t arr[], int size) 
{
    // Pointers to the start and the end of an array
    int i = 0;
    int j = size - 1;

    // Central element of an array
    struct _finddata_t mid = arr[size / 2];

    // Divide an array
    do {
        while(arr[i].size < mid.size) {
            i++;
        }
        // Skip elements that are higher than the central array
        while(arr[j].size > mid.size) {
            j--;
        }

        // Change places of elements
        if (i <= j) {
            struct _finddata_t tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);

    //Recursive calls, if there's still something to sort 
    if(j > 0) {
        //"Left part"
        sort_quick(arr, j + 1);
    }
    if (i < size) {
        //"Right part"
        sort_quick(arr, size - i);
    }
}

/* function to sort arr using shellSort */
int sort_shell(struct _finddata_t arr[], int n)
{
    // Start with a big gap, then reduce the gap
    for (int gap = n/2; gap > 0; gap /= 2)
    {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements a[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is
        // gap sorted
        for (int i = gap; i < n; i += 1)
        {
            // add a[i] to the elements that have been gap sorted
            // save a[i] in temp and make a hole at position i
            struct _finddata_t temp = arr[i];
 
            // shift earlier gap-sorted elements up until the correct
            // location for a[i] is found
            int j;           
            for (j = i; j >= gap && arr[j - gap].size > temp.size; j -= gap)
                arr[j] = arr[j - gap];
             
            //  put temp (the original a[i]) in its correct location
            arr[j] = temp;
        }
    }
    return 0;
}
/*
void sort_shell(struct _finddata_t arr[], int n)
{
    int i, j, step;
    struct _finddata_t tmp;
    for (step = n / 2; step > 0; step /= 2)
        for (i = step; i < n; i++)
        {
            tmp = arr[i];
            for (j = i; j >= step; j -= step)
            {
                if (tmp.size < arr[j - step].size)
                    arr[j] = arr[j - step];
                else
                    break;
            }
            arr[j] = tmp;
        }
}
*/