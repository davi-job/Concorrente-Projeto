#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int *gerar_vetor_inteiro(int n);

int tam = 100000;

int main(int argc, char** argv) {

    // Inicializar o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obter o número de processos e o rank de cada um
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Alocar memória para os vetores locais de cada processo
    int local_tam = tam / size;
    int *local_arr = malloc(local_tam * sizeof(int));
    int *local_count = malloc(local_tam * sizeof(int));
    int *local_sortedArr = malloc(local_tam * sizeof(int));

    // Gerar o vetor inteiro no processo raiz e distribuir para os demais processos
    int *arr;
    
    if (rank == 0) {
        arr = gerar_vetor_inteiro(tam);
        
        // Imprimir o vetor original
        printf("Vetor original:\n");
        for (int i = 0; i < tam; i++)
        {
            printf("%d,", arr[i]);
        }
        printf("\n");
        
    }

// Distribuir o vetor inteiro para os vetores locais de cada processo
    MPI_Scatter(arr, local_tam, MPI_INT, local_arr, local_tam, MPI_INT, 0, MPI_COMM_WORLD);

    // Inicializar os vetores locais de contagem e ordenação com zeros
    for (int i = 0; i < local_tam; i++){
        local_count[i] = 0;
        local_sortedArr[i] = 0;
    }

    // Calcular a contagem local de cada elemento do vetor local
    for(int i = 0; i < local_tam; i++){
        for (int j = 0; j < tam; j++){
            if(local_arr[i] > arr[j]){

                local_count[i]++;
            }
        }
    }

    // Comunicar as contagens locais entre os processos
    int *global_count = malloc(tam * sizeof(int));
    MPI_Allgatherv(local_count, local_tam, MPI_INT, global_count, local_tam, MPI_INT, 0, MPI_COMM_WORLD);

    // Ordenar o vetor local de acordo com as contagens globais
    for (int i = 0; i < local_tam; i++){
        for (int j = 0; j < tam; j++)
        {
            if(local_count[i] == j){
                local_sortedArr[j] = local_arr[i];
            }
        }
    }

    // Comunicar os vetores locais ordenados entre os processos
    int *sortedArr;
    
    if (rank == 0) {
        sortedArr = malloc(tam * sizeof(int));
    }

    MPI_Gather(local_sortedArr, local_tam, MPI_INT, sortedArr, local_tam, MPI_INT, 0, MPI_COMM_WORLD);

    // Corrigir os elementos nulos no vetor ordenado no processo raiz
    if (rank == 0) {
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

        // Imprimir o vetor ordenado
        printf("Vetor ordenado:\n");
        for (int i = 0; i < tam; i++)
        {
            printf("%d,", sortedArr[i]);
        }
        printf("\n");
        
    }

    // Liberar a memória alocada
    free(local_arr);
    free(local_count);
    free(local_sortedArr);
    free(global_count);
    
    if (rank == 0) {
        free(arr);
        free(sortedArr);
    }

    // Finalizar o ambiente MPI
    MPI_Finalize();

}