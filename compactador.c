#include <stdio.h> //BIBLIOTECA IN E OUT
#include <stdlib.h> //BIBLIOTECA PADR�O
#include <math.h>
#define alpha 30 //NOSSO ALFABETO TERA 29 ELEMENTOS, DE a � z, ' ', '\n', 'EOF'. 1 INDEXADO
#define max 1000000

typedef struct Nodo{
    int frequencia, chave; //FREQUENCIA E CHAVE: NUMERO DE VEZES QUE A LETRA APARECE E SE VAI SER 1 OU 0 NA TRIE
    int caminho[alpha]; //CAMINHO A SER PERCORRIDO NA TRIE PARA ENCONTRAR UMA LETRA
    char letra; //A LETRA
    struct Nodo *pai, *filhoEsquerdo, *filhoDireito; //CADA NODO TEM UM PAI, UM FILHO ESQUERDO E UM FILHO DIREITO
}nodo;

nodo *criaNodo(int f, char c, int i){
    int j; //VARIAVEL DE ITERA�AO
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
    novo->pai = NULL; //PAI E FILHOS INICIALMENTE VAZIOS, NAS FOLHAS OS FILHOS SEMPRE SER�O VAZIOS
    novo->filhoEsquerdo = NULL;
    novo->filhoDireito = NULL;
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
        if(aux+1<tam && novo[aux]->frequencia > novo[aux + 1]->frequencia){ //SE AUX + 1 FOR MAIOR QUE O TAMANHO ESTAREMOS VERIFICANDO UMA POSI�AO INEXISTENTE
            aux = aux + 1;
        }
        if(novo[j]->frequencia <= novo[aux]->frequencia){//SE O ELEMENTO EM J FOR O MENOR, TROQUE J POR TAM E ACABE A ITERA�AO
            j = tam;
        }
        else{
            nodo *temp;
            if(novo[j]->letra == '\0'){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 29); //GARATIMOS QUE O NOVO[0] � O MENOR NODO, ENT�O CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
                temp->filhoDireito = novo[j]->filhoDireito;
                temp->filhoEsquerdo = novo[j]->filhoEsquerdo;
            }
            else if(novo[j]->letra == '\n'){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 28);
                temp->pai = novo[j]->pai;
                temp->filhoDireito = novo[j]->filhoDireito;
                temp->filhoEsquerdo = novo[j]->filhoEsquerdo;
            }
            else if(novo[j]->letra == ' '){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 27);
                temp->pai = novo[j]->pai;
                temp->filhoDireito = novo[j]->filhoDireito;
                temp->filhoEsquerdo = novo[j]->filhoEsquerdo;
            }
            else if(novo[j]->letra == '+'){
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, 0);
                for(i=1;i<alpha;i++) temp->caminho[i] = novo[j]->caminho[i];
                temp->pai = novo[j]->pai;
                temp->filhoDireito = novo[j]->filhoDireito;
                temp->filhoEsquerdo = novo[j]->filhoEsquerdo;
            }
            else{
                temp = criaNodo(novo[j]->frequencia, novo[j]->letra, novo[j]->letra-96);
                temp->pai = novo[j]->pai;
                temp->filhoDireito = novo[j]->filhoDireito;
                temp->filhoEsquerdo = novo[j]->filhoEsquerdo;
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
        novo = heapfy(novo, i, tam); //FUN�AO QUE FARA AS ALTERA�OES DE POSI�OES PARA FORMAR UMA MIN-HEAP
    }
    return novo;
}

int imprimeBits(nodo *raiz, int i, int tam, int *Vt){
    if(raiz->filhoEsquerdo == NULL && raiz->filhoDireito == NULL){
        Vt[i] = tam;
        return tam;
    }
    if(raiz->filhoEsquerdo->caminho[i] == 1){
        printf("%d", raiz->filhoEsquerdo->chave);
        return imprimeBits(raiz->filhoEsquerdo, i, tam+1, Vt);
    }
    else{
        printf("%d", raiz->filhoDireito->chave);
        return imprimeBits(raiz->filhoDireito, i, tam+1, Vt);
    }
}

int *escreveCaminhos(nodo *raiz, int i, int *V, int j){
    while(raiz->filhoDireito!=NULL && raiz->filhoEsquerdo!=NULL){
        if(raiz->filhoDireito->caminho[i] == 1){
            V[j] = raiz->filhoDireito->chave;
            raiz = raiz->filhoDireito;
        }
        else if(raiz->filhoEsquerdo->caminho[i] == 1){
            V[j] = raiz->filhoEsquerdo->chave;
            raiz = raiz->filhoEsquerdo;
        }
        j++;
    }
    return V;
}

void escreveHzip(unsigned char hex, char *argv2){
    FILE *e;
    e = fopen(argv2, "ab");
    fwrite(&hex, sizeof(unsigned char), 1, e); //ESCREVENDO O BINARIO NO ARQUIVO
    fclose(e);
}

int *escreveLetras(char *palavra, nodo *raiz, int indice, int *V, char *argv2, int *tam){
    int binario = 0, j = 0, k = 0;
    int codigo = 0;
    int i = 0;
    int soma = 0;
    int *buffer = (int*)malloc(8*sizeof(int));
    for(i=0;i<8;i++){
        buffer[i]=0;
    }
    if(V[7]>0 && V[7]<8){
        soma = V[7];
        for(i=0;i<soma;i++){
            buffer[i] = V[i];
        }
    }
    int tamG;
    while(palavra[indice] != '\0' && palavra[indice] != '\n'){
        if(palavra[indice] == ' '){
            tamG = tam[27];
            V = (int*)malloc(tam[27]*sizeof(int));
            for(j=0;j<tam[27];j++){
                V[j] = 0;
            }
            V = escreveCaminhos(raiz, 27, V, 0);
            soma += tam[27];
        }
        else{
            tamG = tam[palavra[indice]-96];
            V = (int*)malloc(tamG*sizeof(int));
            for(j=0;j<tam[palavra[indice]-96];j++){
                V[j] = 0;
            }
            V = escreveCaminhos(raiz, palavra[indice]-96, V, 0);
            soma += tam[palavra[indice]-96];
        }
        if(soma==8){
            k = 0;
            for(j=soma-tamG;j<8;j++){
                buffer[j] = V[k];
                k++;
            }
            for(j=0;j<8;j++){
                codigo += buffer[j]*pow(10, soma-j-1); //ESCREVER O CODIGO COMO 1 INT
            }
            for (int k=0; k<8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            unsigned char hex = binario;
            escreveHzip(hex, argv2);
            soma = 0;
            codigo = 0;
            binario = 0;
        }
        else if(soma<=7){
            k = 0;
            for(j=soma-tamG;j<soma;j++){
                buffer[j] = V[k];
                k++;
            }
        }

        else if(soma>=9){
            k = 0;
            while(soma>=9){
                for(j=soma-tamG;j<8;j++){
                    buffer[j] = V[k];
                    k++;
                }
                for(j=0;j<8;j++){
                    codigo += buffer[j]*pow(10, 8-j-1); //ESCREVER O CODIGO COMO 1 INT
                }
                for (int k=0; k<8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
                    int resto = codigo - ((codigo / 10)*10);
                    codigo = codigo / 10;
                    binario += resto * pow(2,k);
                }
                unsigned char hex = binario;
                escreveHzip(hex, argv2);
                soma = soma - 8;
                codigo = 0;
                binario = 0;
            }
            for(j=0;j<soma;j++){
                buffer[j] = V[k];
                k++;
            }
        }
        indice++;
    }
    if(palavra[indice] == '\n'){
        tamG = tam[28];
        V = (int*)malloc(tam[28]*sizeof(int));
        for(j=0;j<tam[28];j++){
            V[j] = 0;
        }
        V = escreveCaminhos(raiz, 28, V, 0);
        soma += tam[28];
        if(soma==8){
            k=0;
            for(j=soma-tamG;j<8;j++){
                buffer[j] = V[k];
                k++;
            }
            for(j=0;j<8;j++){
                codigo += buffer[j]*pow(10, soma-j-1); //ESCREVER O CODIGO COMO 1 INT
            }
            for (int k=0; k<8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            unsigned char hex = binario;
            escreveHzip(hex, argv2);
            for(i=0;i<8;i++){
                buffer[i] = 0;
            }
            soma = 0;
            codigo = 0;
            binario = 0;
        }
        else if(soma<=7){
            k = 0;
            for(j=soma-tamG;j<soma;j++){
                buffer[j] = V[k];
                k++;
            }
        }

        else if(soma>=9){
            k = 0;
            int vez = 0;
        while(soma>=9){
            if(vez == 0){
                for(j=soma-tamG;j<8;j++){
                    buffer[j] = V[k];
                    k++;
                }
                vez++;
            }
            else{
                for(j=0;j<8;j++){
                    buffer[j] = V[k];
                    k++;
                }
            }
            for(j=0;j<8;j++){
                codigo += buffer[j]*pow(10, 8-j-1); //ESCREVER O CODIGO COMO 1 INT
            }
            for (int k=0; k<8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            int hex = binario;
            escreveHzip(hex, argv2);
            soma = soma - 8;
            codigo = 0;
            binario = 0;
        }
            for(j=0;j<soma;j++){
                buffer[j] = V[k];
                k++;
            }
        }
    }
    buffer[7] = soma;
    return buffer;
}

int *contaLetra(int *Vetor, char *palavra, int indice){
    if(palavra[indice] == '\0'){ //SE FOR O FIM DO ARQUIVO
        Vetor[29]++; //INCREMENTA A FREQUENCIA
        return Vetor;
    }
     else if(palavra[indice] == '\n'){
        Vetor[28]++; //INCREMENTA A FREQUENCIA
        return Vetor;
    }
    else if(palavra[indice] == ' '){
        Vetor[27]++; //INCREMENTA A FREQUENCIA
        return contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }
    else{
        Vetor[palavra[indice]-96]++; //INCREMENTA A FREQUENCIA
        return contaLetra(Vetor, palavra, indice+1); //CHAMA RECURSIVA PRA PROIMA LETRA
    }

}

void freeTrie(nodo *raiz){
    if(raiz != NULL){
        freeTrie(raiz->filhoDireito);
        freeTrie(raiz->filhoEsquerdo);
        free(raiz);
        raiz = NULL;
    }
}

int main(int argc, char *argv[]){
    char palavra[max]; //AQUI SERA ARMAZENADO NOSSO VETOR DE CARACTERES DO ARQUIVO TXT

    FILE *file; //DECLARA�AO PADRAO DO FILE
    file = fopen(argv[1], "r"); //ABRINDO NO MODO LEITURA O TEXTO DO USUARIO
    if(file == NULL) return 0; //TRATAMENTO DE ERRO
    int i; //VARIAVEL DE ITERACAO
    int *frequencia = (int*)malloc(alpha*sizeof(int)); //FREQUENCIA DE CADA LETRA NA STRING PALAVRA
    for(i=1;i<alpha;i++){
        frequencia[i]=0; //INICIALMENTE 0
    }

    while(fgets(palavra, max, file) != NULL){
        frequencia = contaLetra(frequencia, palavra, 0);
    }
    fclose(file);
    nodo **novo = (nodo**)malloc(alpha*sizeof(nodo*)); //CRIANDO A ESTRUTURA DAS FOLHAS
    for(i=1;i<alpha;i++){ //IMPRIMINDO A TABELA DE FREQUENCIA E CRIANDO AS FOLHAS DA TRIE BINARIA
        if(i==29){
            if(frequencia[i] == 0) frequencia[i]++;
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
        novo = heap(novo, aux); //CHAMA A FUN�AO QUE MUDA AS POSI�OES DAS FOLHAS, DE MODO A DEIXA-LAS EM UMA MIN-HEAP ----FUN�AO HEAPFY RETORNA UMA MIN HEAP----
        nodo *menor1; //CRIANDO O NODO QUE RECEBERA O MENOR VALOR DA HEAP
        if(novo[1]->letra == '\0'){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 29); //GARATIMOS QUE O NOVO[0] � O MENOR NODO, ENT�O CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
            menor1->pai = novo[1]->pai;
            menor1->filhoDireito = novo[1]->filhoDireito;
            menor1->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == '\n'){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 28);
            menor1->pai = novo[1]->pai;
            menor1->filhoDireito = novo[1]->filhoDireito;
            menor1->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == ' '){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 27);
            menor1->pai = novo[1]->pai;
            menor1->filhoDireito = novo[1]->filhoDireito;
            menor1->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == '+'){
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, 0);
            for(i=1;i<alpha;i++) menor1->caminho[i] = novo[1]->caminho[i];
            menor1->pai = novo[1]->pai;
            menor1->filhoDireito = novo[1]->filhoDireito;
            menor1->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else{
            menor1 = criaNodo(novo[1]->frequencia, novo[1]->letra, novo[1]->letra-96);
            menor1->pai = novo[1]->pai;
            menor1->filhoDireito = novo[1]->filhoDireito;
            menor1->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        aux--;
        novo[1] = novo[aux]; //SUBSTITUINDO O VALOR JA UTILIZADO POR OUTRO QUE ESTAVA NO FIM DO VETOR
        //AGR FAZ EXATAMENTE A MESMA COISA PRA PEGAR A 2 FOLHA

        novo = heap(novo, aux); //CHAMA A FUN�AO QUE MUDA AS POSI�OES DAS FOLHAS, DE MODO A DEIXA-LAS EM UMA MIN-HEAP ----FUN�AO HEAPFY RETORNA UMA MIN HEAP----
        nodo *menor2; //CRIANDO O NODO QUE RECEBERA O MENOR VALOR DA HEAP
        if(novo[1]->letra == '\0'){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 29); //GARATIMOS QUE O NOVO[0] � O MENOR NODO, ENT�O CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
            menor2->pai = novo[1]->pai;
            menor2->filhoDireito = novo[1]->filhoDireito;
            menor2->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == '\n'){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 28);
            menor2->pai = novo[1]->pai;
            menor2->filhoDireito = novo[1]->filhoDireito;
            menor2->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == ' '){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 27);
            menor2->pai = novo[1]->pai;
            menor2->filhoDireito = novo[1]->filhoDireito;
            menor2->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == '+'){
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, 0);
            for(i=1;i<alpha;i++) menor2->caminho[i] = novo[1]->caminho[i];
            menor2->pai = novo[1]->pai;
            menor2->filhoDireito = novo[1]->filhoDireito;
            menor2->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else{
            menor2 = criaNodo(novo[1]->frequencia, novo[1]->letra, novo[1]->letra-96);
            menor2->pai = novo[1]->pai;
            menor2->filhoDireito = novo[1]->filhoDireito;
            menor2->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        aux--;
        novo[1] = novo[aux]; //SUBSTITUINDO O VALOR JA UTILIZADO POR OUTRO QUE ESTAVA NO FIM DO VETOR
        nodo *temp;
        if(novo[1]->letra == '\0'){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 29); //GARATIMOS QUE O NOVO[0] � O MENOR NODO, ENT�O CRIAMOS UM NODO AUXILIAR COM SUAS CARACTERISTICAS
            temp->pai = novo[1]->pai;
            temp->filhoDireito = novo[1]->filhoDireito;
            temp->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == '\n'){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 28);
            temp->pai = novo[1]->pai;
            temp->filhoDireito = novo[1]->filhoDireito;
            temp->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == ' '){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 27);
            temp->pai = novo[1]->pai;
            temp->filhoDireito = novo[1]->filhoDireito;
            temp->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else if(novo[1]->letra == '+'){
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, 0);
            for(i=1;i<alpha;i++) temp->caminho[i] = novo[1]->caminho[i];
            temp->pai = novo[1]->pai;
            temp->filhoDireito = novo[1]->filhoDireito;
            temp->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        else{
            temp = criaNodo(novo[1]->frequencia, novo[1]->letra, novo[1]->letra-96);
            temp->pai = novo[1]->pai;
            temp->filhoDireito = novo[1]->filhoDireito;
            temp->filhoEsquerdo = novo[1]->filhoEsquerdo;
        }
        novo[1] = temp;
        novo[aux] = criaPai(menor1, menor2, novo[aux]); //CRIANDO O PAI
        aux++;
    } //TRIE CRIADA, TUDO CERTO ATE AQUI
    printf("---\n");
    int *tam = (int*)malloc(alpha*sizeof(int));
    for(i=1;i<alpha;i++){ //IMPRIMINDO O CODIGO DE HUFFMAN
        if(i==29) printf("EOF: ");
        else if(i==28) printf("\\n: ");
        else if(i==27) printf("  : ");
        else printf("%c: ", i+96);
        imprimeBits(novo[1], i, 0, tam);
        printf("\n");
    }//FIM DA IMPRESSAO

    FILE *fp;
    fp = fopen(argv[2], "wb");
    unsigned char hex = 0;
    for(i=1;i<30;i++){ //COME�ANDO A ARMAZENAR O CABE�ALHO
        int j=0;
        int codigo = 0;
        hex = tam[i];
        fp = fopen(argv[2], "ab");
        fwrite(&hex, 1, sizeof(unsigned char), fp);
        fclose(fp);
        hex = 0;
        int *V = (int*)malloc(tam[i]*sizeof(int));
        for(int j=0;j<tam[i];j++){
            V[j] = 0;
        }
        V = escreveCaminhos(novo[1], i, V, 0);
        if(tam[i]>0 && tam[i]<=8){ // CASO O "TAMANHO" DO VETOR SEJA DE NO MAXIMO 1 BYTE
            for(j=0;j<tam[i];j++){
                codigo += V[j]*pow(10, tam[i]-j-1); //ESCREVER O CODIGO COMO 1 INT
            }
            while(j!=8){ //AJUSTANDO O TAMNHO PARA 1 BYTE
                codigo *= 10;
                j++;
            }
            int binario = 0;
            for (int k = 0; k < 8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp); //ESCREVENDO O BINARIO NO ARQUIVO
            fclose(fp); //FECHA ARQUIVO
            for(j=0;j<3;j++){ //ESCREVENDO A SEQUENCIA DE 0
                fp = fopen(argv[2], "ab");
                hex = 0;
                fwrite(&hex, 1, sizeof(unsigned char), fp);
                fclose(fp);
            }

        }
        else if(tam[i]>8 && tam[i]<=16){ // CASO O "TAMANHO" SEJA DE NO MAXIMO 2 BYTES
            for(j=0;j<8;j++){
                codigo += V[j]*pow(10, 8-j-1);
            }
            int binario = 0;
            for(int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            codigo = 0;
            binario = 0;
            for(j=8;j<tam[i];j++){
                codigo += V[j]*pow(10, tam[i]-j-1);
            }
            while(j!=16){
                codigo *=10;
                j++;
            }
            for (int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);

            for(j=0;j<2;j++){
                fp = fopen(argv[2], "ab");
                hex = 0;
                fwrite(&hex, 1, sizeof(unsigned char), fp);
                fclose(fp);
            }

        }
        else if(tam[i]>16 && tam[i]<=24){ //CASO O "TAMANHO" SEJA DE NO MAXIMO 3 BYTES
            for(j=0;j<8;j++){
                codigo += V[j]*pow(10, 8-j-1);
            }
            int binario = 0;
            for (int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            binario = 0;
            codigo = 0;

            for(j=8;j<16;j++){
                codigo += V[j]*pow(10, 16-j-1);
            }
            for (int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            binario = 0;
            codigo = 0;

            for(j=16;j<tam[i];j++){
                codigo += V[j]*pow(10, tam[i]-j-1);
            }
            while(j!=24){
                codigo *= 10;
                j++;
            }
            for (int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "a");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            binario = 0;

            for(j=0;j<1;j++){
                fp = fopen(argv[2], "ab");
                hex = 0;
                fwrite(&hex, 1, sizeof(unsigned char), fp);
                fclose(fp);
            }

        }
        else if(tam[i]>24 && tam[i]<=32){ //CASO O "TAMANHO" SEJA DE NO MAXIMO 4 BYTES
            for(j=0;j<8;j++){
                codigo += V[j]*pow(10, 8-j-1);
            }
            int binario = 0;
            for (int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            binario = 0;
            codigo = 0;

            for(j=8;j<16;j++){
                codigo += V[j]*pow(10, 16-j-1);
            }
            for (int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            binario = 0;
            codigo = 0;

            for(j=16;j<24;j++){
                codigo += V[j]*pow(10, 24-j-1);
            }
            for (int k = 0; k < 8; k++) {
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            binario = 0;
            codigo =0;
            for(j=24; j<tam[i];j++){
                codigo += V[j]*pow(10, tam[i]-j-1);
            }
            while(j!=32){
                codigo *= 10;
                j++;
            }
            for(int k=0; k<8;k++){
                int resto = codigo -((codigo/10)*10);
                codigo = codigo/10;
                binario += resto * pow(2,k);
            }
            hex = binario;
            fp = fopen(argv[2], "ab");
            fwrite(&hex, 1, sizeof(unsigned char), fp);
            fclose(fp);
            binario = 0;
            codigo =0;
        }
        free(V);
    } //FIM DO CABECALHO

    FILE *leitura;
    leitura = fopen(argv[1], "r");
    int *Vetor = (int*)malloc(8*sizeof(int));
    for(i=0;i<8;i++) Vetor[i] = 0;
    int j=0;
    while(fgets(palavra, max, leitura) != NULL){ //ARAMAZENANDO O ARQUIVO COMPRIMIDO
        Vetor = escreveLetras(palavra, novo[1], 0, Vetor, argv[2], tam);
    }
    int *buffer = (int*)malloc(8*sizeof(int));
    for(i=0;i<8;i++){
        buffer[i] = 0;
    }
    int k = 0, soma = 0, codigo = 0, binario = 0;
    if(Vetor[7]>0 && Vetor[7]<8){
        soma = Vetor[7];
        for(i=0;i<soma;i++){
            buffer[i] = Vetor[i];
        }
    }
    int tamG = tam[29];
    free(Vetor);
    Vetor = (int*)malloc(tam[29]*sizeof(int));
    for(j=0;j<tam[29];j++){
        Vetor[j] = 0;
    }
    Vetor = escreveCaminhos(novo[1], 29, Vetor, 0);
    soma += tamG;
    if(soma<=7){
        k = 0;
        for(j=soma-tamG;j<soma;j++){
            buffer[j] = Vetor[k];
            k++;
        }
        for(j=soma;j<8;j++){
            buffer[j] = 0;
        }
        soma = 8;
    }
    else if(soma>=9){
        k = 0;
        int vez = 0;
        while(soma>=9){
            if(vez == 0){
                for(j=soma-tamG;j<8;j++){
                    buffer[j] = Vetor[k];
                    k++;
                }
                vez++;
            }
            else{
                for(j=0;j<8;j++){
                    buffer[j] = Vetor[k];
                    k++;
                }
            }
            for(j=0;j<8;j++){
                codigo += buffer[j]*pow(10, 8-j-1); //ESCREVER O CODIGO COMO 1 INT
            }
            for (int k=0; k<8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            int hex = binario;
            escreveHzip(hex, argv[2]);
            soma = soma - 8;
            codigo = 0;
            binario = 0;
        }
        for(j=0;j<soma;j++){
            buffer[j] = Vetor[k];
            k++;
        }
        for(j=soma;j<8;j++){
            buffer[j] = 0;
        }
        for(j=0;j<8;j++){
                codigo += buffer[j]*pow(10, 8-j-1); //ESCREVER O CODIGO COMO 1 INT
            }
            for (int k=0; k<8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
                int resto = codigo - ((codigo / 10)*10);
                codigo = codigo / 10;
                binario += resto * pow(2,k);
            }
            int hex = binario;
            escreveHzip(hex, argv[2]);
            soma = soma - 8;
            codigo = 0;
            binario = 0;
    }
    if(soma==8){
        k=0;
        for(j=soma-tamG;j<8;j++){
            buffer[j] = Vetor[k];
            k++;
        }
        for(j=0;j<8;j++){
            codigo += buffer[j]*pow(10, soma-j-1); //ESCREVER O CODIGO COMO 1 INT
        }
        for (int k=0; k<8; k++) {//TRANSFORMANDO O CODIGO DE UM INT PARA BINARIO.
            int resto = codigo - ((codigo / 10)*10);
            codigo = codigo / 10;
            binario += resto * pow(2,k);
        }
        int hex = binario;
        escreveHzip(hex, argv[2]);
        soma = 0;
        codigo = 0;
        binario = 0;
    }
    fclose(leitura);
    free(frequencia);
    free(tam);
    free(Vetor);
    free(buffer);
    freeTrie(novo[1]);
    free(novo);
    return 0;
}
