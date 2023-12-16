#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
    char codigo[20];
    char cnpj[20];
    int peso;
} Container;

void montarArray(FILE *arq, int tamanho, Container containers[])
{
    for (int i = 0; i < tamanho; i++)
    {
        fscanf(arq, "%s %s %d", containers[i].codigo, containers[i].cnpj, &containers[i].peso);
    }
}

// Função de comparação para o mergesort
int comparaCODIGO(const void *a, const void *b)
{
    return strcmp(((Container *)a)->codigo, ((Container *)b)->codigo);
}

// Função para mesclar duas partes do array durante o mergesort
void merge(Container *container, int inicio, int meio, int fim)
{
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    Container *temp1 = (Container *)malloc(n1 * sizeof(Container));
    Container *temp2 = (Container *)malloc(n2 * sizeof(Container));

    for (int i = 0; i < n1; i++)
        temp1[i] = container[inicio + i];
    for (int j = 0; j < n2; j++)
        temp2[j] = container[meio + 1 + j];

    int i = 0, j = 0, k = inicio;
    while (i < n1 && j < n2)
    {
        if (comparaCODIGO(&temp1[i], &temp2[j]) <= 0)
        {
            container[k] = temp1[i];
            i++;
        }
        else
        {
            container[k] = temp2[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        container[k] = temp1[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        container[k] = temp2[j];
        j++;
        k++;
    }

    free(temp1);
    free(temp2);
}

// Função mergesort
void mergesort(Container *arr, int inicio, int fim)
{
    if (inicio < fim)
    {
        int meio = (inicio + fim) / 2;
        mergesort(arr, inicio, meio);
        mergesort(arr, meio + 1, fim);
        merge(arr, inicio, meio, fim);
    }
}

void imprimirRepetidos(Container arr1[], Container arr2[], int size1, int size2) {
    int i = 0, j = 0;
    while (i < size1 && j < size2) {
        if (strcmp(arr1[i].codigo, arr2[j].codigo) < 0)
            i++;
        else if (strcmp(arr1[i].codigo, arr2[j].codigo) > 0)
            j++;
        else if(strcmp(arr1[i].codigo, arr2[j].codigo) == 0){
            if (strcmp(arr1[i].cnpj, arr2[j].cnpj) != 0)
            {
                printf("Codigo: %s, CNPJ: %s, Peso: %d\n", arr1[i].codigo, arr1[i].cnpj, arr1[i].peso);
                printf("Codigo: %s, CNPJ: %s, Peso: %d\n", arr2[j].codigo, arr2[j].cnpj, arr2[j].peso);
            }
            
            i++;
            j++;
        }
    }
}

int main(int argc, char *argv[])
{
    // argumentos de programa
    printf("#ARGS = %i\n", argc);
    FILE *arquivo = fopen(argv[1], "r");

    if (arquivo == NULL)
    {
        printf("Falha na leitura!");
    }
    else
    {
        // lista containers
        int num_total_container;
        fscanf(arquivo, "%d", &num_total_container);
        Container containers[num_total_container];
        montarArray(arquivo, num_total_container, containers);
        // lista containers a checar
        int num_total_container_checar;
        fscanf(arquivo, "%d", &num_total_container_checar);
        Container containers_checar[num_total_container_checar];
        montarArray(arquivo, num_total_container_checar, containers_checar);

        // aplicando ordenação por cnpj ordem crescente
        mergesort(containers, 0, num_total_container - 1);
        mergesort(containers_checar, 0, num_total_container_checar  -1);


        // imprimir repetidos
        imprimirRepetidos(containers, containers_checar, num_total_container, num_total_container_checar);

    }

    fclose(arquivo);
    return 0;
}