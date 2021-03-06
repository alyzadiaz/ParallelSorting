#include <stdio.h> 
#include <stdlib.h> 
#include "merge.h"


int main() 
{ 
    int arr[] = { 12, 11, 13, 5, 6, 7 }; 
    int arr_size = sizeof(arr) / sizeof(arr[0]); 
  
    printf("Given array is \n"); 
    printArray(arr, arr_size); 
  
    mergeSort(arr, 0, arr_size - 1); 
  
    printf("\nSorted array is \n"); 
    printArray(arr, arr_size); 
    return 0; 
} 

void mergeSort(int arr[], int x, int z) 
{ 
    if (x < z) { 
         
        int y = x + (z - x) / 2; 
  
        mergeSort(arr, x, y); 
        mergeSort(arr, y + 1, z); 
  
        merge(arr, x, y, z); 
    } 
} 


void merge(int arr[], int x, int y, int z) 
{ 
    int i;
    int j;
    int k; 
    int v1 = y - x + 1; 
    int v2 = z - y; 

    int L[v1], R[v2]; 
  
    for (i = 0; i < v1; i++) 
        L[i] = arr[x + i]; 
    for (j = 0; j < v2; j++) 
        R[j] = arr[y + 1 + j]; 
  
   
    i = 0; 
    j = 0; 
    k = x;  
    while (i < v1 && j < v2) { 
        if (L[i] <= R[j]) { 
            arr[k] = L[i]; 
            i++; 
        } 
        else { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    while (i < v1) { 
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    
    while (j < v2) { 
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  

  

void printArray(int A[], int size) 
{ 
    int i; 
    for (i = 0; i < size; i++) 
        printf("%d ", A[i]); 
    printf("\n"); 
} 
