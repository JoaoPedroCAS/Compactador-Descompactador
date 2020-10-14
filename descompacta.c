#include <stdio.h> //BIBLIOTECA IN E OUT
#include <stdlib.h> //BIBLIOTECA PADRÃO
#include <math.h>
#include <string.h>
#define alpha 29 //NOSSO ALFABETO TERA 29 ELEMENTOS, DE a À z, ' ', '\n', 'EOF'. 1 INDEXADO
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
}

nodo *criaArvore(nodo *raiz, int *caminho, int tam, int letra, int iteracao){
    if(iteracao == tam) return raiz;
    if(caminho[iteracao] == 1){
        if(raiz->filhoDireito == NULL){
            if(iteracao == tam-1){
                if(letra == 26) raiz->filhoDireito = criaNodo(' ');
                else if(letra == 27) raiz->filhoDireito = criaNodo('\n');
                else if(letra == 28) raiz->filhoDireito = criaNodo(0);
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
                else if(letra == 28) raiz->filhoEsquerdo = criaNodo(0);
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

int *escreveTxt(nodo *raiz, int *bin, FILE *e, char *argv2, int tam){
    int i = 0;
    while(raiz->letra == '+'){
        if(bin[i] == 1){
            i++;
            raiz = raiz->filhoDireito;
        }
        else{
            i++;
            raiz = raiz->filhoEsquerdo;
        }
    }
    e = fopen(argv2, "a");
    fprintf(e, "%c", raiz->letra);
    fclose(e);
}

int main(int argc, char* argv[]){
    int i, j = 0;
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
        j=0;
        int aux = (i*5)+1;
        int *bin = (int*)malloc(8*sizeof(int));
        int tam = bytes[i*5];
        while(j < tam){
            int decimal = bytes[aux];
            bin = conversaoBinario(bin, decimal);
            if(tam>8){
                for(j=0;j<8;j++){
                    printf("%d", bin[j]);
                }
                j=0;
                tam = tam - 8;
            }
            else{
                for(j=0;j<tam;j++){
                    printf("%d", bin[j]);
                }
            }
            raiz = criaArvore(raiz, bin, tam, i, 0);
            aux++;
        }
        free(bin);
        printf("\n");
    }
    int aux = i*5;
    char letra = 'a';
    FILE *escrita;
    escrita = fopen(argv[2], "w");
    fclose(escrita);
    while(letra != 0){
        int *bin = (int*)malloc(8*sizeof(int));
        int decimal = bytes[aux];
        bin = conversaoBinario(bin, decimal);
        bin = escreveTxt(raiz, bin, escrita, argv[2], 8);
        break;
    }

    return 0;
}
