#include <stdio.h> 
#include <stdlib.h> 
#include "quickHeader.h"

void swap(int* a, int* b) { 
	int t = *a; 
	*a = *b; 
	*b = t; 
} 

int partition (int arr[], int low, int high) { 
	int pivot = arr[high]; 
	int i = (low - 1); 

	for (int j = low; j <= high- 1; j++) { 
		
		if (arr[j] < pivot) { 
			i++;  
			swap(&arr[i], &arr[j]); 
		} 
	} 
	swap(&arr[i + 1], &arr[high]); 
	return (i + 1); 
} 

void quickSort(int arr[], int l, int h) { 
	if (l < h) { 
		
		int pi = partition(arr, l, h); 

		quickSort(arr, l, pi - 1); 
		quickSort(arr, pi + 1, h); 
	} 
} 

void printArray(int arr[], int size) { 
	int i; 
	for (i=0; i < size; i++) {
        printf("%d ", arr[i]); 
    }
		
	printf("\n");
} 

int * randomArray(int size){
    int *array = (int*)malloc(size*sizeof(int));

	int k = 0;
    for(int i=0;i<size;i++){
        int random = (rand()%1000);
		array[k] = random;
		k++;
    }

    return (int*)array;
}

int main() { 
    int size = 1000000; 
	int * arr = randomArray(size); 

    printf("Unsorted arry: \n");

	for (int i=0; i < size; i++) {
        printf("%d ", arr[i]);
    }

	quickSort(arr, 0, size); 

	printf("\n");
	printf("Sorted array: \n"); 
	printArray(arr, size); 

	return 0; 
} 