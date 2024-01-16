#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
    char codigo[20];
    char cnpj[20];
    int peso;
    int diferencaPercentual;
    int diferenca;
} Container;

void montarArray(FILE *arq, int tamanho, Container containers[])
{
    for (int i = 0; i < tamanho; i++)
    {
        fscanf(arq, "%s %s %d", containers[i].codigo, containers[i].cnpj, &containers[i].peso);
        containers[i].diferencaPercentual = 0;
    }
}

// Função de comparação codigos
int comparaCODIGO(const void *a, const void *b)
{
    return strcmp(((Container *)a)->codigo, ((Container *)b)->codigo);
}


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

void mergep(Container arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    
    Container L[n1], R[n2];

    
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i].diferencaPercentual >= R[j].diferencaPercentual) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

//  Merge Sort diferenca percentual dos pesos
void mergeSortp(Container arr[], int l, int r) {
    if (l < r) {
        
        int m = l + (r - l) / 2;

        
        mergeSortp(arr, l, m);

        mergeSortp(arr, m + 1, r);

       
        mergep(arr, l, m, r);
    }
}

// realizar buscaBinaria arrayChecagem
int buscaBinaria(Container array[], int inicio, int fim, const char *chave)
{
    if (fim >= inicio)
    {
        int meio = inicio + (fim - inicio) / 2;

        // melhor caso chave no meio
        if (strcmp(array[meio].codigo, chave) == 0)
        {
            return meio;
        }

        // chave e menor esq
        if (strcmp(array[meio].codigo, chave) > 0)
        {
            return buscaBinaria(array, inicio, meio - 1, chave);
        }

        // chave e maior dir
        return buscaBinaria(array, meio + 1, fim, chave);
    }
    // nao achou
    return -1;
}

int main(int argc, char *argv[])
{
    (void)argc;
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
        mergesort(containers_checar, 0, num_total_container_checar - 1);

        // buscar os com o mesmo codigo
        for (int i = 0; i < num_total_container; i++)
        {
            containers[i].diferencaPercentual = 0;
            int indice = buscaBinaria(containers_checar, 0, num_total_container_checar, containers[i].codigo);
            if (indice != -1)
            {
                // calcular diferença no peso
                int diferenca = containers_checar[indice].peso - containers[i].peso;
                int diferencaPercentual = round(diferenca * 100 / (double)containers[i].peso);

                if (strcmp(containers[i].cnpj, containers_checar[indice].cnpj) != 0)
                {
                    printf("%s: %s<-->%s\n", containers[i].codigo, containers[i].cnpj, containers_checar[indice].cnpj);
                }
                else if (diferencaPercentual > 10)
                {
                    containers[i].diferenca = diferenca;
                    containers[i].diferencaPercentual = diferencaPercentual;
                }
            }
        }

        mergeSortp(containers, 0, num_total_container-1);
        for (int i = 0; i < num_total_container; i++)
            if (containers[i].diferencaPercentual > 10)
            {
                printf("%s: %dkg (%d%%)\n", containers[i].codigo, containers[i].diferenca, containers[i].diferencaPercentual);
            }
    }

    fclose(arquivo);
    return 0;
}