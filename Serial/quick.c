#include <stdio.h> 
#include <stdlib.h> 
#include "quickHeader.h"


void swap(int* a, int* b) 
{ 
	int t = *a; 
	*a = *b; 
	*b = t; 
} 


int partition (int arr[], int low, int high) 
{ 
	int pivot = arr[high]; 
	int i = (low - 1); 

	for (int j = low; j <= high- 1; j++) 
	{ 
		
		if (arr[j] < pivot) 
		{ 
			i++;  
			swap(&arr[i], &arr[j]); 
		} 
	} 
	swap(&arr[i + 1], &arr[high]); 
	return (i + 1); 
} 


void quickSort(int arr[], int l, int h) 
{ 
	if (l < h) 
	{ 
		
		int pi = partition(arr, l, h); 

		quickSort(arr, l, pi - 1); 
		quickSort(arr, pi + 1, h); 
	} 
} 

void printArray(int arr[], int size) 
{ 
	int i; 
	for (i=0; i < size; i++) 
		printf("%d ", arr[i]); 
	printf("\n");
} 


int main() 
{ 
    int size = 6; 
	int arr[] = {9, 2, 5, 8, 3, 1}; 
    int i; 
    printf("Unsorted arry: ");
	for (i=0; i < size; i++) 
		printf("%d ", arr[i]);
	int n = sizeof(arr)/sizeof(arr[0]); 
	quickSort(arr, 0, n-1); 
	printf("Sorted array: "); 
	printArray(arr, n); 
	return 0; 
} 
