 #include <stdio.h> //BIBLIOTECA IN E OUT
#include <stdlib.h> //BIBLIOTECA PADRÃO
#define alpha 30 //NOSSO ALFABETO TERA 29 ELEMENTOS, DE a À z, ' ', '\n', 'EOF'. 1 INDEXADO

typedef struct Nodo{
    int frequencia, chave; //FREQUENCIA E CHAVE: NUMERO DE VEZES QUE A LETRA APARECE E SE VAI SER 1 OU 0 NA TRIE
    int caminho[alpha]; //CAMINHO A SER PERCORRIDO NA TRIE PARA ENCONTRAR UMA LETRA
    char letra; //A LETRA
    struct Nodo *pai, *filhoEsquerdo, *filhoDireito; //CADA NODO TEM UM PAI, UM FILHO ESQUERDO E UM FILHO DIREITO
}nodo;

nodo *criaNodo(int f, char c, int i){
    int j; //VARIAVEL DE ITERAÇAO
    nodo *novo = (nodo*)malloc(sizeof(nodo)); //LIBERANDO A MEMORIA PRO NODO
    novo->frequencia = f; //DEFININDO A FREQUENCIA DO NODO
    novo->chave = 0; //DEFININDO A CHAVE DO NODO
    for(j=1;j<alpha;j++){
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
    for(j=1;j<alpha;j++){ //DEFININDO O VETOR CAMINHO
        if(aux1->caminho[j] == 1 || aux2->caminho[j] == 1){
            pai->caminho[j] = 1;
        }
        else pai->caminho[j] = 0;
    }
    return pai;
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
            nodo *temp;
            if(novo[j]->letra == '\0'){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 29); //GARATIMOS QUE O NOVO[0] É O MENOR NODO, ENTÃO CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
            }
            else if(novo[j]->letra == '\n'){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 28);
            }
            else if(novo[j]->letra == ' '){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 27);
            }
            else if(novo[j]->letra == '+'){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 0);
                temp = novo[j];
            }
            else{
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, novo[j]->letra-96);
            }

            novo[j] = novo[aux];
            novo[aux] = temp;

            j = aux; //J RECEBE AUX
        }

    }
    return novo;
}

nodo **heap(nodo **novo, int tam){ //USAMOS O NODO E O TAMANHO ATUAL DO 'VETOR'
    int i;
    for(i = (tam/2);i>=1;i--){
        novo = heapfy(novo, i, tam); //FUNÇAO QUE FARA AS ALTERAÇOES DE POSIÇOES PARA FORMAR UMA MIN-HEAP
    }
    return novo;
}

int *contaLetra(int *Vetor, char *palavra, int indice){
    if(palavra[indice] == '\0'){ //SE FOR O FIM DO ARQUIVO
        Vetor[29]++; //INCREMENTA A FREQUENCIA
        return Vetor;
    }
     else if(palavra[indice] == '\n'){
        Vetor[28]++; //INCREMENTA A FREQUENCIA
        contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }
    else if(palavra[indice] == ' '){
        Vetor[27]++; //INCREMENTA A FREQUENCIA
        contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }
    else{
        Vetor[palavra[indice]-96]++; //INCREMENTA A FREQUENCIA
        contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }

}

void imprimeBits(nodo *raiz, int i){
    if(raiz->filhoEsquerdo == NULL && raiz->filhoDireito == NULL){
        printf(".");
        return;
    }
    if(raiz->filhoEsquerdo->caminho[i] == 1){
        printf("%d", raiz->filhoEsquerdo->chave);
        imprimeBits(raiz->filhoEsquerdo, i);
    }
    if(raiz->filhoDireito->caminho[i] == 1){
        printf("%d", raiz->filhoDireito->chave);
        imprimeBits(raiz->filhoDireito, i);
    }

}

int main(){
    int FTotal = 0;
    char *palavra = (char*)malloc(10*sizeof(char)); //LIBERANDO ESPAÇO PRA PALAVRA, NA VERSAO FINAL USAREMOS UM ARQUIVO TXT
    palavra = "uma frase aleatoria"; //PALAVRA ALEATORIA
    int i; //VARIAVEL DE ITERAÇAO
    int *frequencia = (int*)malloc(alpha*sizeof(int)); //FREQUENCIA DE CADA LETRA NA STRING PALAVRA
    for(i=1;i<alpha;i++){
        frequencia[i]=0; //INICIALMENTE 0
    }
    frequencia = contaLetra(frequencia, palavra, 0); //CHAMANDO A FUNÇAO QUE IMPLEMENTA A FREQUENCIA -----ATE AQUI TUDO CERTO----


    nodo **novo = (nodo**)malloc(alpha*sizeof(nodo*)); //CRIANDO A ESTRUTURA DAS FOLHAS
    for(i=1;i<alpha;i++){ //IMPRIMINDO A TABELA DE FREQUENCIA E CRIANDO AS FOLHAS DA TRIE BINARIA
        if(i==29){
            novo[i] = criaNodo(frequencia[i], '\0', i);
            printf("EOF: %d\n", frequencia[i]);
        }
        else if(i==28){
            novo[i] = criaNodo(frequencia[i], '\n', i);
            printf("\\n: %d\n", frequencia[i]);
        }
        else if(i==27){
            novo[i] = criaNodo(frequencia[i], ' ', i);
            printf("  : %d\n", frequencia[i]);
        }
        else{
            novo[i] = criaNodo(frequencia[i], i+96, i);
            printf("%c: %d\n", i+96, frequencia[i]);
        }
    } //FIM DA TABELA, FOLHAS CRIADAS  ----TUDO CERTO ATE AQUI----
    int aux = alpha;
    while(aux>2){ //AQUI FAREMOS O HEAPFY, TAMBEM MONTAREMOS A TRIE COMPLETA
        novo = heap(novo, aux); //CHAMA A FUNÇAO QUE MUDA AS POSIÇOES DAS FOLHAS, DE MODO A DEIXA-LAS EM UMA MIN-HEAP ----FUNÇAO HEAPFY RETORNA UMA MIN HEAP----
        nodo *menor1; //CRIANDO O NODO QUE RECEBERA O MENOR VALOR DA HEAP
        if(novo[1]->letra == '\0'){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 29); //GARATIMOS QUE O NOVO[0] É O MENOR NODO, ENTÃO CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
        }
        else if(novo[1]->letra == '\n'){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 28);
        }
        else if(novo[1]->letra == ' '){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 27);
        }
        else if(novo[1]->letra == '+'){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 0);
            for(i=1;i<alpha;i++) menor1->caminho[i] = novo[1]->caminho[i];
            menor1->chave = novo[1]->chave;
        }
        else{
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, novo[1]->letra-96);
        }
        aux--;
        novo[1] = novo[aux]; //SUBSTITUINDO O VALOR JA UTILIZADO POR OUTRO QUE ESTAVA NO FIM DO VETOR
        //AGR FAZ EXATAMENTE A MESMA COISA PRA PEGAR A 2 FOLHA

        novo = heap(novo, aux); //CHAMA A FUNÇAO QUE MUDA AS POSIÇOES DAS FOLHAS, DE MODO A DEIXA-LAS EM UMA MIN-HEAP ----FUNÇAO HEAPFY RETORNA UMA MIN HEAP----
        nodo *menor2; //CRIANDO O NODO QUE RECEBERA O MENOR VALOR DA HEAP
        if(novo[1]->letra == '\0'){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 29); //GARATIMOS QUE O NOVO[0] É O MENOR NODO, ENTÃO CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
        }
        else if(novo[1]->letra == '\n'){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 28);
        }
        else if(novo[1]->letra == ' '){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 27);
        }
        else if(novo[1]->letra == '+'){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 0);
            for(i=1;i<alpha;i++) menor2->caminho[i] = novo[1]->caminho[i];
            menor2->chave = novo[1]->chave;
        }
        else{
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, novo[1]->letra-96);
        }
        aux--;
        novo[1] = novo[aux]; //SUBSTITUINDO O VALOR JA UTILIZADO POR OUTRO QUE ESTAVA NO FIM DO VETOR
        nodo *temp;
        if(novo[1]->letra == '\0'){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 29); //GARATIMOS QUE O NOVO[0] É O MENOR NODO, ENTÃO CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
        }
        else if(novo[1]->letra == '\n'){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 28);
        }
        else if(novo[1]->letra == ' '){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 27);
        }
        else if(novo[1]->letra == '+'){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 0);
            for(i=1;i<alpha;i++) temp->caminho[i] = novo[1]->caminho[i];
            temp->chave = novo[1]->chave;
        }
        else{
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, novo[1]->letra-96);
        }
        novo[1] = temp;
        novo[aux] = criaPai(menor1, menor2, novo[aux]); //CRIANDO O PAI
        aux++;
    } //TRIE CRIADA, TUDO CERTO ATE AQUI
    printf("---\n");
    for(i=1;i<alpha;i++){
        if(novo[1]->filhoDireito->caminho[i]==1) printf("%d ", i);

    }
    return 0;
}
