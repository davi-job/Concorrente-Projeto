#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int *gerar_vetor_inteiro(int n);

int tam = 10000;

int main(int argc, char** argv) {

    int count[tam];
    int *arr;

    int sortedArr[tam];
    
    arr = gerar_vetor_inteiro(tam);

    #pragma omp parallel num_threads(3)
    {


        #pragma omp for
        for (int i = 0; i < tam; i++){
            count[i] = 0;
        }


        #pragma omp for
        for (int i = 0; i < tam; i++){
            sortedArr[i] = 0;
        }


        #pragma omp for
        for(int i = 0; i < tam; i++){
            for (int j = 0; j < tam; j++){
                if(arr[i] > arr[j]){

                 count[i]++;
                }
            }
        }

        #pragma omp for
        for (int i = 0; i < tam; i++){
            for (int j = 0; j < tam; j++)
            {
               if(count[i] == j){
                    sortedArr[j] = arr[i];
                }
            }
        }

        int ultimoNaoZero = 0;

        #pragma omp for
        for(int i = 0; i < tam; i++){
            if (sortedArr[i] != 0)
            {
                ultimoNaoZero = sortedArr[i];
            }else
            {
                sortedArr[i] = ultimoNaoZero;
            }
        }
    }



    for (int i = 0; i < tam; i++)
    {
        printf("%d,", arr[i]);
    }
    printf("\n");
    for (int i = 0; i < tam; i++)
    {
        printf("%d, ", sortedArr[i]);
    }

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