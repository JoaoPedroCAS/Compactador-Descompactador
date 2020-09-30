#include <stdio.h> //BIBLIOTECA IN E OUT
#include <stdlib.h> //BIBLIOTECA PADRÃO
#define alpha 29 //NOSSO ALFABETO TERA 29 ELEMENTOS, DE a À z, ' ', '\n', 'EOF'.

typedef struct Nodo{
    int frequencia, chave; //FREQUENCIA E CHAVE: NUMERO DE VEZES QUE A LETRA APARECE E SE VAI SER 1 OU 0 NA TRIE
    int caminho[29]; //CAMINHO A SER PERCORRIDO NA TRIE PARA ENCONTRAR UMA LETRA
    char letra; //A LETRA
    struct Nodo *pai, *filhoEsquerdo, *filhoDireito; //CADA NODO TEM UM PAI, UM FILHO ESQUERDO E UM FILHO DIREITO
}nodo;

int *contaLetra(int *Vetor, char *palavra, int indice){
    if(palavra[indice] == '\0'){ //SE FOR O FIM DO ARQUIVO
        Vetor[28]++; //INCREMENTA A FREQUENCIA
        return Vetor;
    }
     else if(palavra[indice] == '\n'){
        Vetor[27]++; //INCREMENTA A FREQUENCIA
        contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }
    else if(palavra[indice] == ' '){
        Vetor[26]++; //INCREMENTA A FREQUENCIA
        contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }
    else{
        Vetor[palavra[indice]-97]++; //INCREMENTA A FREQUENCIA
        contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }

}

nodo *criaNodo(int f, char c, int i){
    int j; //VARIAVEL DE ITERAÇAO
    nodo *novo = (nodo*)malloc(sizeof(nodo)); //LIBERANDO A MEMORIA PRO NODO
    novo->frequencia = f; //DEFININDO A FREQUENCIA DO NODO
    novo->chave = 0; //DEFININDO A CHAVE DO NODO
    for(j=0;j<alpha;j++){ //DEFININDO O VETOR CAMINHO
        if(j==i){
            novo->caminho[i] = 1;
        }
        else novo->caminho[j] = 0;
    }
    novo->letra = c; //DEFININDO A LETRA DO NODO
    novo->pai = NULL; //PAI E FILHOS INICIALMENTE VAZIOS, NAS FOLHAS OS FILHOS SEMPRE SERÃO VAZIOS
    novo->filhoEsquerdo = NULL;
    novo->filhoDireito = NULL;

}

nodo **heapfy(nodo **novo, int i, int tam){
    int j = i;
    while(2*j < tam){ //ENQUANTO 2*J FOR MENOR QUE O TAMANHO DO 'VETOR'
        int aux = 2*j;
        if(aux+1<tam && novo[aux]->frequencia > novo[aux + 1]->frequencia){ //SE AUX + 1 FOR MAIOR QUE O TAMANHO ESTAREMOS VERIFICANDO UMA POSIÇAO INEXISTENTE
            aux = aux + 1;
        }
        if(novo[j]->frequencia <= novo[aux]->frequencia){//SE O ELEMENTO EM J FOR O MENOR, TROQUE J POR TAM E ACABE A ITERAÇAO
            j = tam;
        }
        else{
            char temp = novo[j]->letra;
            novo[j]->letra = novo[aux]->letra;  //TROCANDO AS LETRAS DOS 2 NODOS
            novo[aux]->letra = temp;

            int troca = novo[j]->frequencia;
            novo[j]->frequencia = novo[aux]->frequencia; //TROCANDO A FREQUENCIA DOS 2 NODOS
            novo[aux]->frequencia = troca;

            j = aux; //J RECEBE AUX
        }

    }
    return novo;
}

nodo **heap(nodo **novo, int tam){ //USAMOS O NODO E O TAMANHO ATUAL DO 'VETOR'
    int i;
    for(i = (tam/2);i>=0;i--){
        novo = heapfy(novo, i, tam); //FUNCÇAO QUE FARA AS ALTERAÇOES DE POSIÇOES PARA FORMAR UMA MIN-HEAP
    }
    return novo;
}

nodo *criaPai(nodo *aux1, nodo *aux2, nodo *pai){
    int j;
    aux1->pai = pai; //O PAI DOS DOIS AUX VAI SER O NODO PAI
    aux2->pai = pai;
    aux1->chave = 0; //AQUI COLOQUEI COMO PADRAO QUE O FILHO ESQUERDO SEJA O 0 E O DIREITO O 1
    aux2->chave = 1;
    pai->filhoEsquerdo = aux1; //DEFININDO OS FILHOS DO NODO PAI
    pai->filhoDireito = aux2;
    pai->chave = 0; //VALOR INICIAL DA CAHVE
    pai->letra = '+'; //ALTERANDO A LETRA DO PAI
    pai->frequencia = aux1->frequencia + aux2->frequencia;
    for(j=0;j<alpha;j++){ //DEFININDO O VETOR CAMINHO
        if(aux1->caminho[j] == 1 || aux2->caminho[j] == 1){
            pai->caminho[j] = 1;
        }
        else pai->caminho[j] = 0;
    }
    return pai;
}

void imprimeBits(nodo *raiz, int i){
    printf("%d ", raiz->caminho[i]);
    if(raiz->filhoEsquerdo == NULL && raiz->filhoDireito == NULL){
        printf(".");
        return;
    }

    if(raiz->filhoEsquerdo->caminho[i]==1 && raiz->filhoEsquerdo != NULL){
        printf("CONTINUA");
        imprimeBits(i, raiz->filhoEsquerdo);
    }
    else{
        if(raiz->filhoDireito->caminho[i]==1 && raiz->filhoDireito != NULL){
            printf("CONTINUA");
            imprimeBits(i, raiz->filhoDireito);
        }
    }
    return;
}

int main(){
    char *palavra = (char*)malloc(10*sizeof(char)); //LIBERANDO ESPAÇO PRA PALAVRA, NA VERSAO FINAL USAREMOS UM ARQUIVO TXT
    palavra = "dezletrass"; //PALAVRA ALEATORIA
    int i; //VARIAVEL DE ITERAÇAO
    int *frequencia = (int*)malloc(alpha*sizeof(int)); //FREQUENCIA DE CADA LETRA NA STRING PALAVRA
    for(i=0;i<alpha;i++){
        frequencia[i] = 0; //DEFININDO CADA POSIÇAO DO VETOR PARA 0
    }

    nodo **novo = (nodo**)malloc(alpha*sizeof(nodo*)); //CRIANDO A ESTRUTURA DAS FOLHAS
    frequencia = contaLetra(frequencia, palavra, 0); //CHAMANDO A FUNÇAO QUE IMPLEMENTA A FREQUENCIA
    for(i=0;i<alpha;i++){ //IMPRIMINDO A TABELA DE FREQUENCIA E CHAMANDO A FUNÇAO QUE CRIA O NODO
        if(i==28){
            novo[i] = criaNodo(frequencia[i], '\0', i);
            printf("EOF: %d\n", frequencia[i]);
        }
        else if(i==27){
            novo[i] = criaNodo(frequencia[i], '\n', i);
            printf("\\n: %d\n", frequencia[i]);
        }
        else if(i==26){
            novo[i] = criaNodo(frequencia[i], ' ', i);
            printf("  : %d\n", frequencia[i]);
        }
        else{
            novo[i] = criaNodo(frequencia[i], i+97, i);
            printf("%c: %d\n", i+97, frequencia[i]);
        }
    }//FIM DA IMPRESSAO DA TABELA DE FREQUENCIA, FOLHAS JA FORAM CRIADAS

    int aux = alpha;
    while(aux>1){ //AQUI FAREMOS O HEAPFY E TAMBEM A TRIE
        novo = heap(novo, aux); //CHAMA A FUNÇAO QUE MUDA AS POSIÇOES DAS FOLHAS, DE MODO A DEIXA-LAS EM UMA MIN-HEAP
        nodo *aux1;
        if(novo[0]->letra == '\0'){
            aux1 = criaNodo(novo[0]->frequencia, novo[0]->letra, 28); //GARATIMOS QUE O NOVO[0] É O MENOR NODO, ENTÃO CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
        }
        else if(novo[0]->letra == '\0'){
            aux1 = criaNodo(novo[0]->frequencia, novo[0]->letra, 27);
        }
        else if(novo[0]->letra == ' '){
            aux1 = criaNodo(novo[0]->frequencia, novo[0]->letra, 26);
        }
        else{
            aux1 = criaNodo(novo[0]->frequencia, novo[0]->letra, novo[0]->letra-97);
        }
        if(aux1->letra == '\0') printf("EOL ");
        else if(aux1->letra == '\n') printf("\\n ");
        else if(aux1->letra == ' ') printf("ESPACO ");
        else printf("%c, %d", aux1->letra, aux1->frequencia);
        printf("\n");
        novo[0] = novo[aux-1]; //SUBSTITUINDO O VALOR JA UTILIZADO POR OUTRO QUE ESTAVA NO FIM DO VETOR
        aux--; //AGR FAZ EXATAMENTE A MESMA COISA PRA PEGAR A 2 FOLHA

        novo = heap(novo, aux); //CHAMA A FUNÇAO QUE MUDA AS POSIÇOES DAS FOLHAS, DE MODO A DEIXA-LAS EM UMA MIN-HEAP
        nodo *aux2;
        if(novo[0]->letra == '\0'){
            aux2 = criaNodo(novo[0]->frequencia, novo[0]->letra, 28); //GARATIMOS QUE O NOVO[0] É O MENOR NODO, ENTÃO CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
        }
        else if(novo[0]->letra == '\0'){
            aux2 = criaNodo(novo[0]->frequencia, novo[0]->letra, 27);
        }
        else if(novo[0]->letra == ' '){
            aux2 = criaNodo(novo[0]->frequencia, novo[0]->letra, 26);
        }
        else{
            aux2 = criaNodo(novo[0]->frequencia, novo[0]->letra, novo[0]->letra-97);
        }
        if(aux2->letra == '\0') printf("EOL ");
        else if(aux2->letra == '\n') printf("\\n ");
        else if(aux2->letra == ' ') printf("ESPACO ");
        else printf("%c, %d", aux2->letra, aux2->frequencia);
        printf("\n");
        novo[0] = novo[aux-1]; //SUBSTITUINDO O VALOR JA UTILIZADO POR OUTRO QUE ESTAVA NO FIM DO VETOR
        aux--;
        novo[aux] = criaPai(aux1, aux2, novo[aux]); //CRIANDO O PAI
        aux++;
    }
    for(i=0;i<alpha;i++){

    }
    return 0;
}
