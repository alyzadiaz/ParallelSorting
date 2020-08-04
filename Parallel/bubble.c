#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void swap(int * v, int i, int j)
{
  int t = v[i];
  v[i] = v[j];
  v[j] = t;
}

void bubblesort(int * v, int n)
{
  int i, j;
  for (i = n-2; i >= 0; i--)
    for (j = 0; j <= i; j++)
      if (v[j] > v[j+1])
        swap(v, j, j+1);
}

int * merge(int * v1, int n1, int * v2, int n2)
{
  int * result = (int *)malloc((n1 + n2) * sizeof(int));
  int i = 0;
  int j = 0;
  int k;
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


int main(int argc, char ** argv)
{
  int n = 10000;
  int * data = (int*)malloc(n*sizeof(int));
  int c, s;
  int * chunk;
  int o;
  int * other;
  int step;
  int p, id;
  MPI_Status status;
  double elapsed_time;
  int i;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  if (id == 0) {
      for(int i=0;i<n;i++){
          data[i] = rand()%1000;
      }

    c = n/p; if (n%p) c++;
  }

  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = - MPI_Wtime();

  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  c = n/p; if (n%p) c++;
  chunk = (int *)malloc(c * sizeof(int));

  MPI_Scatter(data, c, MPI_INT, chunk, c, MPI_INT, 0, MPI_COMM_WORLD);
  free(data);
  data = NULL;

  s = (n >= c * (id+1)) ? c : n - c * id;

  bubblesort(chunk, s);

  for (step = 1; step < p; step = 2*step) {
    if (id % (2*step)!=0) {
     
      MPI_Send(chunk, s, MPI_INT, id-step, 0, MPI_COMM_WORLD);
      break;
    }
    
    if (id+step < p) {
      o = (n >= c * (id+2*step)) ? c * step : n - c * (id+step);
      other = (int *)malloc(o * sizeof(int));
      MPI_Recv(other, o, MPI_INT, id+step, 0, MPI_COMM_WORLD, &status);

      data = merge(chunk, s, other, o);
      free(chunk);
      free(other);
      chunk = data;
      s = s + o;
    }
  }

  elapsed_time += MPI_Wtime();

  if (id == 0) {
    for(int i=0;i<n;i++){
        printf("%d ",chunk[i]);
    }
    printf("\n");

    printf("Time: %.5f secs\n", elapsed_time);
  }

  MPI_Finalize();
  return 0;
}