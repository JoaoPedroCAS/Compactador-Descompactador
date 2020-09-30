#include<stdio.h>
#include<stdlib.h>

typedef struct Nodo{
    int frequencia;
    int chave;
    char letra;
    struct Nodo *pai, *FE, *FD;
}nodo;

nodo* criaNodo(int f, char c){
    nodo *novo = (nodo*)malloc(sizeof(nodo));
    novo->frequencia = f;
    novo->letra = c;
    novo->pai = NULL;
    novo->FE = NULL;
    novo->FD = NULL;
    novo->chave = 0;
    return novo;
}

nodo* criaPai(nodo *aux1, nodo *aux2, nodo *pai){
    aux1->pai = pai;
    aux2->pai = pai;
    aux1->chave = 0;
    aux2->chave = 1;
    pai->letra = '+';
    pai->FE = aux1;
    pai->FD = aux2;
    pai->pai = NULL;
    pai->chave = 0;
    pai->frequencia = aux1->frequencia + aux2->frequencia;
    return pai;
}

int* contaLetra(int *alfabeto, char *c, int index){
    if(c[index] == '\0'){
        alfabeto[28]++;
        return alfabeto;
    }
    else if(c[index] == '\n'){
        alfabeto[27]++;
        contaLetra(alfabeto, c, index+1);
    }
    else if(c[index] == ' '){
        alfabeto[26]++;
        contaLetra(alfabeto, c, index+1);
    }
    else{
        alfabeto[c[index]-97]++;
        contaLetra(alfabeto, c, index+1);
    }
}

nodo **heapfy(nodo **novo, int i, int tam){
    int j = i;
    while(2*j < tam){
        int aux = 2*j;
        if(aux+1<tam && novo[aux]->frequencia > novo[aux + 1]->frequencia){
            aux = aux + 1;
        }
        if(novo[j]->frequencia <= novo[aux]->frequencia){
            j = tam;
        }
        else{
            char temp = novo[j]->letra;
            novo[j]->letra = novo[aux]->letra;
            novo[aux]->letra = temp;

            int troca = novo[j]->frequencia;
            novo[j]->frequencia = novo[aux]->frequencia;
            novo[aux]->frequencia = troca;

            j = aux;
        }
    }

    return novo;
}

nodo **Heap(nodo **novo, int tam){
    int i;
    for(i=(tam/2); i>=0; i--){
        novo = heapfy(novo, i, tam);
    }
    return novo;
}

/*void imprimeBits(nodo *pai){
    printf("%c\n", pai->letra);
    if(pai->FE == NULL && pai->FD == NULL){
        return;
    }
    imprimeBits(pai->FE);
    imprimeBits(pai->FD);
}*/

int main(){
    //ARMAZENAR UMA FRASE NA STRING, CRIAR AS VARIAVEIS USADAS EM ITERAÇOES, CRIAR O VETOR COM O TAMANHO DO ALFABETO USADO.
    char *palavra = "aaabbc";
    int i, qtdLetra =0;
    int *alfabeto = (int*)malloc(29*sizeof(int));
    for(i=0; i<29;i++){
        alfabeto[i] = 0;
    }
    //A FUNÇAO CONTA LETRAS VAI INCREMENTAR EM CERTA POSIÇAO DO VETOR, PARA TER O TOTAL DE LETRAS E SUA FREQUENCIA
    alfabeto = contaLetra(alfabeto, palavra, 0);
    for(i=0;i<29;i++){
            qtdLetra++;
    }
    //CRIANDO MAIS ALGUMAS VARIAVEIS. UM VETOR INTEIRO DE TAMNHO QTDDELETRAS E UMA STRING APENAS COM AS LETRAS USADAS
    int *letrasUsadasF = (int*)malloc(qtdLetra*sizeof(int));
    char *caracter = (char*)malloc(qtdLetra*sizeof(char));

    //ATRIBUINDO AOS VETORES SEUS DEVIDOS VALORES
    for(i=0;i<29;i++){
            if(i==26){
                letrasUsadasF[i] = alfabeto[i];
                caracter[i] = ' ';
            }
            else if(i==27){
                letrasUsadasF[i] = alfabeto[i];
                caracter[i] = '\n';
            }
            else if(i==28){
                letrasUsadasF[i] = alfabeto[i];
                caracter[i] = '\0';
            }
            else{
                letrasUsadasF[i] = alfabeto[i];
                caracter[i] = i+97;
            }
    }
    //CRIANDO A ESTRUTURA DAS FOLHAS
    nodo **novo = (nodo**)malloc(29*sizeof(nodo*));
    //TABELA DE FREQUENCIA E CRIAÇAO DAS FOLHAS
    for(i=0;i<qtdLetra;i++){
        if(i==26){
            printf("%c: %d\n", caracter[i], letrasUsadasF[i]);
            novo[i] = criaNodo(letrasUsadasF[i], caracter[i]);
        }
        else if(i==27){
            printf("\\n: %d\n", letrasUsadasF[i]);
            novo[i] = criaNodo(letrasUsadasF[i], caracter[i]);
        }
        else if(i==28){
            printf("EOF: %d\n", letrasUsadasF[i]);
            novo[i] = criaNodo(letrasUsadasF[i], caracter[i]);
        }
        else{
            printf("%c: %d\n", caracter[i], letrasUsadasF[i]);
            novo[i] = criaNodo(letrasUsadasF[i], caracter[i]);
        }
    }
    while(qtdLetra>1){
        novo = Heap(novo, qtdLetra);
        nodo *aux1 = (nodo*)malloc(sizeof(nodo));
        aux1 = criaNodo(novo[0]->frequencia, novo[0]->letra);
        printf("%c %c",novo[0]->letra, aux1->letra);
        qtdLetra--;
        novo[0] = novo[qtdLetra];
        novo = Heap(novo, qtdLetra-1);
        nodo *aux2 = (nodo*)malloc(sizeof(nodo));
        aux2 = criaNodo(novo[0]->frequencia, novo[0]->letra);
        printf("%c %c\n",novo[0]->letra, aux2->letra);
        qtdLetra--;
        novo[0] = novo[qtdLetra-1];
        qtdLetra++;
        novo[qtdLetra] = criaPai(aux1, aux2, novo[qtdLetra]);
    }
    return 0;
}
