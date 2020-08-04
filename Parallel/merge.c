
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mergePara.h"
#include <time.h>

int main(int argc, char** argv) {

	int rank, numrank;
	
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numrank);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	double startTime, endTime; 
	double scTime, ecTime; 
	
	startTime = MPI_Wtime(); 
	int n = 1000000;
	int size = n/numrank;
	

	int *array = malloc(n * sizeof(int));
	int c;
    srand(time(NULL));
    if(rank == 0)
    {
        printf("Unsorted array: ");
    }
    

	for(c = 0; c < n; c++) {
		
		array[c] = rand() % n;
        if(rank == 0){
        printf("%d ", array[c]);

        }
	}
    
	int *sub = malloc(size * sizeof(int));
	scTime = MPI_Wtime(); 
	MPI_Scatter(array, size, MPI_INT, sub, size, MPI_INT, 0, MPI_COMM_WORLD);
	
	int *tmp = malloc(size * sizeof(int));
	mergeSort(sub, tmp, 0, (size - 1));
	
	int *sorted = NULL;
	if(rank == 0) {
		
		sorted = malloc(n * sizeof(int));
		
		}
	
	MPI_Gather(sub, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);
	ecTime = MPI_Wtime(); 

	if(rank == 0) {
		
		int *narr = malloc(n * sizeof(int));
		mergeSort(sorted, narr, 0, (n - 1));
		
		
		printf("\nSorted array: ");
		for(c = 0; c < n; c++) {
			
			printf("%d ", sorted[c]);
			
			}
			
		printf("\n");
	
			
		free(sorted);
		free(narr);
			
		}
    free(array);
	free(sub);
	free(tmp);
	endTime = MPI_Wtime(); 
	if(rank == 0)
	{
		printf("Communication Time: %f \n", ecTime-scTime); 
		printf("Total Time: %f", endTime - startTime); 
	}
	MPI_Finalize();
	
	}


void merge(int *a, int *b, int l, int m, int r) {
	
	int h, i, j, k;
	h = l;
	i = l;
	j = m + 1;
	
	while((h <= m) && (j <= r)) {
		
		if(a[h] <= a[j]) {
			
			b[i] = a[h];
			h++;
			
			}
			
		else {
			
			b[i] = a[j];
			j++;
			
			}
			
		i++;
		
		}
		
	if(m < h) {
		
		for(k = j; k <= r; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
		
	else {
		
		for(k = h; k <= m; k++) {
			
			b[i] = a[k];
			i++;
			
			}
			
		}
		
	for(k = l; k <= r; k++) {
		
		a[k] = b[k];
		
		}
		
	}


void mergeSort(int *a, int *b, int l, int r) {
	
	int m;
	
	if(l < r) {
		
		m = (l + r)/2;
		
		mergeSort(a, b, l, m);
		mergeSort(a, b, (m + 1), r);
		merge(a, b, l, m, r);
		
		}
		
	}
