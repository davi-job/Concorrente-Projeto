#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

#define tam 10000

int main(int argc, char** argv) {

    MPI_Init(NULL, NULL);
    int nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int meu_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_rank);

    int i = 0;
    int partialCount[tam];
    int count[tam];
    int *arr = (int *)malloc(sizeof(int) * tam);
    int sortedArr[tam];
    time_t t;
    srand(time(NULL));

    if(meu_rank == 0){
        //gerando o vetor aleatório
        for (i=0;i<tam;i++) {
            int num = (rand() % tam);
            arr[i] = num;
        }
    }

    MPI_Bcast(arr, tam, MPI_INT, 0, MPI_COMM_WORLD);

    for(i = 0; i < tam; i++){
        partialCount[i] = 0;
        count[i] = 0;
        sortedArr[i] = 0;
        for (int j = meu_rank; j < tam; j+=nprocs){
            if(arr[i] > arr[j]){
                partialCount[i]++;
            }
        }
    }

    MPI_Reduce(partialCount, count, tam, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(meu_rank == 0){
        for (i = 0; i < tam; i++){
            sortedArr[count[i]] = arr[i];
        }
        int ultimoNaoZero = 0;

        for(int i = 0; i < tam; i++){
            if (sortedArr[i] != 0)
            {
                ultimoNaoZero = sortedArr[i];
            }else
            {
                sortedArr[i] = ultimoNaoZero;
            }
        }
        /*
        for (int i = 0; i < tam; i++)
        {
            printf("%d,", arr[i]);
        }
        printf("\n");
        for (int i = 0; i < tam; i++)
        {
            printf("%d, ", sortedArr[i]);
        }
        printf("\n");
        */
    }
    MPI_Finalize();
}
