#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {

    int tam = 10;

    int arr[] = {8, 4, 5, 2, 9, 10, 3, 1, 6, 7};
    int count[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int sortedArr[tam];

    for(int i = 0; i < tam; i++){

        for (int j = 0; j < tam; j++){
            if(arr[i] > arr[j]){

                count[i]++;
            }
        }
    }

    for (int i = 0; i < tam; i++){
        for (int j = 0; j < tam; j++)
        {

            if(count[i] == j){

                sortedArr[j] = arr[i];
            }
        }
    }
    
    for (int i = 0; i < tam; i++)
    {
        printf("%d, ", sortedArr[i]);
    }
}
