#include <stdio.h> 
#include <stdlib.h> 
#include "mpi.h"
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

int main(int argc, char **argv) {
    int rank, numranks;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numranks);

    int size = 6; 
	int * arr = randomArray(size);
    int * done = NULL;

    if(rank==0){
        printf("Unsorted array: \n");

        for (int i=0; i < size; i++) {
            printf("%d ", arr[i]);
        }

        done = (int*)malloc(size*sizeof(int));
    }

    int * subArr = (int*)malloc(size*sizeof(int));
    MPI_Scatter(arr, size, MPI_INT, subArr, size, MPI_INT, 0, MPI_COMM_WORLD);

    quickSort(subArr, 0, size-1);
    
    MPI_Gather(arr, size, MPI_INT, done, size, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank==0){
        printf("\n");
        printf("Sorted array: \n"); 
        printArray(subArr, size);
    }
	
    MPI_Finalize();

	return 0; 
} 