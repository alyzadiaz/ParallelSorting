
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mergePara.h"
#include <time.h>

int main(int argc, char** argv) {

	//MPI
	int rank, numrank;
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numrank);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	//Timings 
	double startTime, endTime; 
	double scTime, ecTime; 
	
	startTime = MPI_Wtime(); 
	int n = 1000000;
	int size = n/numrank;
	
	//Creating the rand values to sort
	int *array = malloc(n * sizeof(int));
	int i;
    srand(time(NULL));
    if(rank == 0)
    {
        printf("Unsorted array: ");
    }
	

	for(i = 0; i < n; i++) {
		
		array[i] = rand() % n;
        if(rank == 0){
        printf("%d ", array[i]);

        }
	}
    
	//Splitting up the values 
	int *sub = malloc(size * sizeof(int));
	scTime = MPI_Wtime(); 
	MPI_Scatter(array, size, MPI_INT, sub, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	int *tmp = malloc(size * sizeof(int));
	mergeSort(sub, tmp, 0, (size - 1));
	
	int *sorted = NULL;
	if(rank == 0) {
		
		sorted = malloc(n * sizeof(int));
		
		}
	
	//Collecting the sub arrays
	MPI_Gather(sub, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	ecTime = MPI_Wtime(); 

	//Doing the Final sort and printing the sorted array
	if(rank == 0) {
		
		int *narr = malloc(n * sizeof(int));
		mergeSort(sorted, narr, 0, (n - 1));
		
		
		printf("\nSorted array: ");
		for(i = 0; i < n; i++) {
			
			printf("%d ", sorted[i]);
			
			}
			
		printf("\n");
	
			
	
			
		}
    
	endTime = MPI_Wtime(); 
	if(rank == 0)
	{
		printf("Communication Time: %f \n", ecTime-scTime); 
		printf("Total Time: %f", endTime - startTime); 
	}
	MPI_Finalize();
	
	}

//Merging the array after dividing 
void merge(int *x, int *y, int n, int m, int r) {
	
	int h;
	int i;
	int j;
	int k;
	h = n;
	i = n;
	j = m + 1;
	
	while((j <= r) && (h <= m)) {
		
		if(x[h] <= x[j]) {
			
			y[i] = x[h];
			h++;
			
			}
			
		else {
			
			y[i] = x[j];
			j++;
			
			}
			
		i++;
		
		}
		
	if(m < h) {
		
		for(k = j; k <= r; k++) {
			
			y[i] = x[k];
			i++;
			
			}
			
		}
		
	else {
		
		for(k = h; k <= m; k++) {
			
			y[i] = x[k];
			i++;
			
			}
			
		}
		
	for(k = n; k <= r; k++) {
		
		x[k] = y[k];
		
		}
		
	}

//Sorting values withing the subarray
void mergeSort(int *x, int *y, int n, int r) {
	
	int m;
	
	if(n < r) {
		
		m = (n + r)/2;
		
		mergeSort(x, y, n, m);
		mergeSort(x, y, (m + 1), r);
		merge(x, y, n, m, r);
		
		}
		
	}
