#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define tam 100

int *gerar_vetor_inteiro(int n);

int main(int argc, char** argv) {
    int rank, size;
    int *arr;
    int *sortedArr;
    int *count;
    int *local_count;
    int *local_sortedArr;
    int local_size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calcula o tamanho do vetor para cada processo
    local_size = tam / size;

    // Aloca espaço para os vetores locais
    arr = gerar_vetor_inteiro(tam);
    local_count = (int*) calloc(local_size, sizeof(int));
    local_sortedArr = (int*) malloc(local_size * sizeof(int));

    // Scatter do vetor original
    MPI_Scatter(arr, local_size, MPI_INT, local_sortedArr, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo conta as ocorrências dos elementos em seu trecho do vetor
    for(int i = 0; i < local_size; i++){
        for(int j = 0; j < tam; j++){
            if(local_sortedArr[i] > arr[j]){
                local_count[i]++;
            }
        }
    }

    // Gather das contagens parciais em um único vetor global
    if(rank == 0){
        count = (int*) calloc(tam, sizeof(int));
    }
    MPI_Gather(local_count, local_size, MPI_INT, count, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Processo 0 calcula as posições dos elementos no vetor ordenado
    if(rank == 0){
        sortedArr = (int*) malloc(tam * sizeof(int));
        for(int i = 0; i < tam; i++){
            for(int j = 0; j < tam; j++){
                if(arr[i] > arr[j]){
                    count[i]++;
                }
            }
        }
        for(int i = 0; i < tam; i++){
            sortedArr[count[i]] = arr[i];
        }
    }

    // Scatter do vetor ordenado
    MPI_Scatter(sortedArr, local_size, MPI_INT, local_sortedArr, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo constroi seu trecho do vetor ordenado com base nas posições calculadas
    for(int i = 0; i < local_size; i++){
        int pos = local_count[i];
        for(int j = 0; j < tam; j++){
            if(count[j] == pos){
                local_sortedArr[i] = arr[j];
                break;
            }
        }
    }

    // Gather dos vetores ordenados parciais em um único vetor global
    MPI_Gather(local_sortedArr, local_size, MPI_INT, sortedArr, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Processo 0 atualiza os elementos nulos do vetor ordenado final
    if(rank == 0){
        int ultimoNaoZero = 0;

        for(int i = 0; i < tam; i++){
            if (sortedArr[i] != 0){
                ultimoNaoZero = sortedArr[i];
            }else{
                sortedArr[i] = ultimoNaoZero;
            }
        }

        printf("Vetor original:\n");
        for (int i = 0; i < tam; i++)
        {
            printf("%d,", arr[i]);
        }
        printf("\n");

        printf("Vetor ordenado:\n");
        for (int i = 0; i < tam; i++)
        {
            printf("%d, ", sortedArr[i]);
        }
    }

    MPI_Finalize();
    return 0;
}

int *gerar_vetor_inteiro(int n) {
    int *vetor;
    int i;
    vetor = (int *)malloc(sizeof(int) * n);
    for (i=0;i<n;i++) {
        int num = (rand() % tam);
        vetor[i] = num;
    }
    return vetor;
}
