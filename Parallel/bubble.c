#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

void swap(int * v, int i, int j){
  int t = v[i];
  v[i] = v[j];
  v[j] = t;
}

void bubblesort(int * v, int n){
  int i, j;
  for (i = n-2; i >= 0; i--)
    for (j = 0; j <= i; j++)
      if (v[j] > v[j+1])
        swap(v, j, j+1);
}

int * merge(int * v1, int n1, int * v2, int n2){
  int * result = (int *)malloc((n1 + n2) * sizeof(int));
  int i = 0, j = 0, k;
  
  for (k = 0; k < n1 + n2; k++) {
    if (i >= n1) {
      result[k] = v2[j];
      j++;
    }
    else if (j >= n2) {
      result[k] = v1[i];
      i++;
    }
    else if (v1[i] < v2[j]) {
      result[k] = v1[i];
      i++;
    }
    else {
      result[k] = v2[j];
      j++;
    }
  }
  return result;
}


int main(int argc, char ** argv){
  int n = 100000, c, s, o, step, i, rank, numranks;
  int * data = (int*)malloc(n*sizeof(int));
  int * chunk, other;
  MPI_Status status;
  double time;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numranks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
      for(int i=0;i<n;i++){
          data[i] = rand()%1000;
      }

    c = n/numranks; if (n%numranks) c++;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  time = - MPI_Wtime();

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  c = n/numranks; if (n%numranks) c++;
  chunk = (int*)malloc(c * sizeof(int));

  MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
  free(data);
  data = NULL;

  s = (n >= c * (rank+1)) ? c : n - c * rank;

  bubblesort(chunk, s);

  for (step = 1; step < numranks; step = 2*step) {
    if (rank % (2*step)!=0) {
      MPI_Send(chunk, s, MPI_INT, rank-step, 0, MPI_COMM_WORLD);
      break;
    }
    
    if (rank+step < numranks) {
      o = (n >= c * (rank+2*step)) ? c * step : n - c * (rank+step);
      other = (int *)malloc(o * sizeof(int));
      MPI_Recv(other, o, MPI_INT, rank+step, 0, MPI_COMM_WORLD, &status);

      data = merge(chunk, s, other, o);
      free(chunk);
      free(other);
      chunk = data;
      s = s + o;
    }
  }

  time += MPI_Wtime();

  if (rank == 0) {
    for(int i=0;i<n;i++){
      printf("%d ", chunk[i]);
    }
    printf("\n");

    printf("Time: %.5f secs\n", time);
  }

  MPI_Finalize();
  return 0;
}
