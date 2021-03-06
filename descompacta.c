#include <stdio.h> //BIBLIOTECA IN E OUT
#include <stdlib.h> //BIBLIOTECA PADR�O
#include <math.h>
#include <string.h>
#define alpha 29 //NOSSO ALFABETO TERA 29 ELEMENTOS, DE a � z, ' ', '\n', 'EOF'. 1 INDEXADO
#define max 1000000

typedef struct Nodo{
    char letra;
    struct Nodo *filhoDireito;
    struct Nodo *filhoEsquerdo;
}nodo;

int *conversaoBinario(int *binario, int decimal){
    for(int i=7;i>=0;i--){
        if(decimal%2 == 0) binario[i] = 0;
        else binario[i] = 1;
        decimal = decimal/2;
    }
    return binario;
}

nodo *criaNodo(char letra){
    nodo *novo = (nodo*)malloc(sizeof(nodo));
    novo->letra = letra;
    novo->filhoDireito = NULL;
    novo->filhoEsquerdo = NULL;
    return novo;
}

nodo *criaArvore(nodo *raiz, int *caminho, int tam, int letra, int iteracao){
    if(iteracao == tam) return raiz;
    if(caminho[iteracao] == 1){
        if(raiz->filhoDireito == NULL){
            if(iteracao == tam-1){
                if(letra == 26) raiz->filhoDireito = criaNodo(' ');
                else if(letra == 27) raiz->filhoDireito = criaNodo('\n');
                else if(letra == 28) raiz->filhoDireito = criaNodo(-1);
                else raiz->filhoDireito = criaNodo(letra+97);
            }
            else{
                raiz->filhoDireito = criaNodo('+');
            }
        }
        raiz->filhoDireito = criaArvore(raiz->filhoDireito, caminho, tam, letra, iteracao+1);
    }
    else{
        if(raiz->filhoEsquerdo == NULL){
            if(iteracao == tam-1){
                if(letra == 26) raiz->filhoEsquerdo = criaNodo(' ');
                else if(letra == 27) raiz->filhoEsquerdo = criaNodo('\n');
                else if(letra == 28) raiz->filhoEsquerdo = criaNodo(-1);
                else raiz->filhoEsquerdo = criaNodo(letra+97);
            }
            else{
                raiz->filhoEsquerdo = criaNodo('+');
            }
        }
        raiz->filhoEsquerdo = criaArvore(raiz->filhoEsquerdo, caminho, tam, letra, iteracao+1);
    }
    return raiz;
}

void escreveTxt(nodo *raiz, int *bin, FILE *e, char *argv2, int tam, int aux, unsigned char *bytes, int total){
    int i = 0;
    int decimal = bytes[aux];
    bin = conversaoBinario(bin, decimal);
    nodo *temp = raiz;
    while(aux<total && raiz->letra != -1){
        if(raiz->letra != '+' && i<8){
            e = fopen(argv2, "a");
            fprintf(e, "%c", raiz->letra);
            fclose(e);
            raiz = temp;
        }
        if(i==8){
            aux++;
            decimal = bytes[aux];
            bin = conversaoBinario(bin, decimal);
            i=0;
        }
        else if(bin[i] == 1 && i<8){
            //printf("%d ", bin[i]);
            i++;
            raiz = raiz->filhoDireito;
        }
        else if(bin[i]==0 && i<8){
            //printf("%d ", bin[i]);
            i++;
            raiz = raiz->filhoEsquerdo;
        }
    }
    //printf("%d", raiz->letra);
}

int main(int argc, char* argv[]){
    int i;
    FILE *leitura;
    nodo *raiz = NULL;
    raiz = criaNodo('+');
    unsigned char bytes[max];
    leitura = fopen(argv[1], "rb");
    int total = fread(bytes, sizeof(unsigned char), max, leitura);
    for(i=0;i<alpha;i++){
        if(i == 26) printf(" : ");
        else if(i == 27)printf("\\n: ");
        else if(i == 28)printf("EOF: ");
        else printf("%c: ", i+97);
        int aux = (i*5)+1;
        int tam = bytes[i*5];
        int j = 0;
        while(j<tam){
            if(tam>0 && tam <=8){
                int *buffer = (int*)malloc(tam*sizeof(int));
                int *bin = (int*)malloc(8*sizeof(int));
                int decimal = bytes[aux];
                for(int k=0;k<8;k++){
                    if(decimal%2 == 0) bin[k] = 0;
                    else bin[k] = 1;
                    decimal = decimal/2;
                }
                for(int k=0;k<tam;k++){
                    buffer[k] = bin[7-k];
                    printf("%d", buffer[k]);
                }
                j=8;
                raiz = criaArvore(raiz, buffer, tam, i, 0);
                free(bin);
                free(buffer);
            }
            else if(tam>8 && tam<=16){
                int *buffer = (int*)malloc(tam*sizeof(int));
                int *bin = (int*)malloc(8*sizeof(int));
                int decimal = bytes[aux];
                for(int k=0;k<8;k++){
                    if(decimal%2 == 0) bin[k] = 0;
                    else bin[k] = 1;
                    decimal = decimal/2;
                }
                for(int k=0;k<8;k++){
                    buffer[k] = bin[7-k];
                    printf("%d", buffer[k]);
                }
                aux++;
                decimal = bytes[aux];
                for(int k=0;k<8;k++){
                    if(decimal%2 == 0) bin[k] = 0;
                    else bin[k] = 1;
                    decimal = decimal/2;
                }
                j = 7;
                for(int k=8;k<tam;k++){
                    buffer[k] = bin[j];
                    j--;
                    printf("%d", buffer[k]);
                }
                j=16;
                raiz = criaArvore(raiz, buffer, tam, i, 0);
                free(bin);
                free(buffer);
            }
        }
        printf("\n");
    }
    int aux = i*5;
    FILE *escrita;
    escrita = fopen(argv[2], "w");
    fclose(escrita);
    int *bin = (int*)malloc(8*sizeof(int));
    escreveTxt(raiz, bin, escrita, argv[2], 8, aux, bytes, total);

    return 0;
}
