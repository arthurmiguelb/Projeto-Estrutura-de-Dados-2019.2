#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
unsigned char asterisco = '*' ;
typedef struct queue queue;
typedef struct node node;
typedef struct hash hash;
typedef struct node_hash node_hash; //tirar dps

struct queue
{
	node *head;
} ;

struct node // estrutura onde alocamos o nó para a criação da fila e logo em seguida para a árvore
{
	unsigned char  byte;
	long long int freq; // a frequencia em lld, pois em casos de foto e videos a frequencias do caracteres será bem grande
	node *next;
	node *left; // ponteiro para esquerda para a criação da arvore
	node *right; // ponteiro para a direita para a criação da arvore

};

struct node_hash
{
    int bit;
    node_hash *next;
    node_hash *previous;
} ; //tirar dps

struct hash
{
    unsigned char compactado[256];
    int profundidade[256];
    node_hash *head_hash[256]; //tirar dps
} ;

void para_listas(node_hash *head){
    node_hash *ok=head;
    printf("lista: ");
    while(ok!=NULL){
        printf("%d ", ok->bit);
        ok=ok->next;
    }
    printf("\n");
} //tirar dps
void printando(node *head){ //para arvores
    if(head!=NULL){
        printf("%c", head->byte);
        printando(head->left);
        printando(head->right);
    }
} //tirar dps
void enqueue(queue *fila, unsigned char byte, long long int f, node *left, node *right){ // nesse caso, tivemos que passar os ponteiros da criação da arvore 
	                                                                                     //para conseguirmos manipular os pais, e dar dequeue e enqueue
    node *new_node=(node*)malloc(sizeof(node));
    //printf("%hhu\n", byte);
    new_node->freq=f;
    new_node->byte=byte;
    new_node->left=left;
    new_node->right=right;
    if(fila->head==NULL || f <= fila->head->freq){ //verificamos se a fila é nula e logo em seguida verificamos se o valor da frequencia é maior que a prioridade maxima
        new_node->next =fila->head; // colocamos o novo nó com maior frequencia na cabeça da fila
        fila->head=new_node; // pazemos a cabeça apontar para o novo nó que é a maior frequencia
    }
    else{ //caso o valor da frequencia seja menor que o valor da frequencia da cabeça caimos nesse else
        node *current=fila->head; // criamos o nó do tipo 'node' que aponta para a cabeça da fila
        while((current->next !=NULL) && (current->next->freq < f)){ //andaremos por toda a fila ate encontrar o local em que devemos por o nosso nó com prioridade 'x'
            current=current->next; //percorrendo toda a fila
        }
        new_node->next=current->next; //colocando o novo nó na lista
        current->next=new_node;
    }
}
void ler_arquivo(FILE *pont, queue *fila){ //ler o arquivo em byte e cria a frequencia dos bytes em uma fila
    unsigned char byte;
    long long int frequencia[256]; 
    long long int total=0;
    int i;
    fila->head=NULL;
    for(i=0;i<256;i++){
        frequencia[i]=0; //colocamos todas os indices igual a zero para fazer a contagem das frequencias
    }
    i=0;
    while(fscanf(pont,"%c", &byte)!=EOF){ //lendo os bytes em caracteres
        frequencia[byte]++; // dando ++ na frenquecia caso em sua posição
        //total++;
    }
    for(i=0;i<256;i++){
        if(frequencia[i]!=0){
            if(i==asterisco){
                enqueue(fila, 92, frequencia[i], NULL, NULL); // '92' numero decimal da tabela ascii equivalente a '\'
            }else
                enqueue(fila, i, frequencia[i], NULL, NULL);
        }
    } //fila criada

    if(fila->head==NULL){
        printf("ERRO NA FORMAÇÃO DA FILA!\n");
    }
    node *aux=fila->head;
    /*while(aux!=NULL){ // percorrendo a fila 
        printf("%c %lld\n",aux->byte, aux->freq); //printando o caractere e a sua frequencia
        aux=aux->next;
    }*/
    //printf("meu irmaaao\n");
    //printf("total de bytes: %lld\n", total);
}
void criar_arvore(queue *fila){ //na função cria a arvore e faz os ponteiro esq e dir apontar para os filhos
    node *aux=fila->head; // ponteiros auxiliares para guardar o no e dar enqueue da fila 
    node *aux2=aux->next; // ponteiros auxiliares para guardar o no e dar enqueue da fila 
    if(aux==NULL){ //a fila está vazia
        printf("A FILA ESTA VAZIA\n");
        return;
    }
    if(aux2==NULL){ //a arvore esta pronta
        return;
    }
    fila->head=aux2->next; //desenfileirando os dois menores nós
    enqueue(fila,'*', (aux->freq)+(aux2->freq), aux, aux2); // no lugar do carectere byte passmos '*' para representar o pai 
             												// e a frequencia sera a soma da frequencia dos filhos
    criar_arvore(fila);// recursivamente passa a nova fila ja com o no pai enfileirado
}
/*node_hash *adicionando_bit(node_hash *ajuda, node_hash *fim, int bit){
    node_hash *novo_bit=(node_hash*)malloc(sizeof(node_hash));
    if(ajuda==NULL){
        ajuda=novo_bit;
        novo_bit->previous=NULL;
        fim=novo_bit;
    } else {
        fim->next=novo_bit;
        novo_bit->previous=fim;
    }
    novo_bit->bit=bit;
    novo_bit->next=NULL;
    return novo_bit; //atualizando o fim
}*/ //tirar dps
node_hash *salvando_caminho(node_hash *ajuda, node_hash *head){
    node_hash *aux=ajuda;
    while(aux!=NULL){
        node_hash *lista_indice=(node_hash*)malloc(sizeof(node_hash));
        lista_indice->bit=ajuda->bit;
        lista_indice->next=head;
        head=lista_indice;
        aux=aux->next;
    }
    printf("socorro\n");
    return head;
}//tirar dps
void dicionario(node *head, hash *new_hash, unsigned char byte, int profundidade) {
    if(head!=NULL) {
        if (head->left==NULL && head->right==NULL) {
            new_hash->compactado[head->byte] = byte;
            new_hash->profundidade[head->byte]=profundidade;

        }
        profundidade++;
        byte = byte << 1;
        dicionario(head->left, new_hash, byte, profundidade);
        byte++;
       dicionario(head->right, new_hash, byte, profundidade);
    }
}
/*void dicionario(node *head, hash *new_hash, node_hash *ajuda, node_hash *fim){
    printf("%c\n", head->byte);
    if(head->byte==asterisco){
        fim=adicionando_bit(ajuda, fim, 0);
        if(ajuda==NULL){
            ajuda=fim;
        }

        printf("antes esquerda=");
        para_listas(ajuda);

        dicionario(head->left, new_hash, ajuda, fim);

        printf("depois esquerda=");
        para_listas(ajuda);

        fim=adicionando_bit(ajuda, fim, 1);
        if(ajuda==NULL){
            ajuda=fim;
        }

       printf("antes direita=");
        para_listas(ajuda);

        dicionario(head->right, new_hash, ajuda, fim);
        printf("depois direita=");
        para_listas(ajuda);
         //no fim desse if, devemos apagar da lista ajuda o ultimo nó
        

        printf("apgando o ultimo no=");
        para_listas(ajuda);
        //apagar um no
    }
    else{ //caso seja um byte a ser salvo no dicionario
        new_hash->head_hash[head->byte]=salvando_caminho(ajuda, new_hash->head_hash[head->byte]);
        node_hash *teste=new_hash->head_hash[head->byte];
        printf("lista pronta: ");
        para_listas(teste);
       
        
    }

    if(fim->previous==NULL){ //estamos eliminando o primeiro nó
        ajuda=NULL;
        fim=NULL;
    }else{
        fim=fim->previous;
        fim->next=NULL;
    }
}*/ //tirar dps
void binario(int num){
   int bin[8]; // <---------------
   int aux;

   for (aux = 7; aux >= 0; aux--)
   {
      if (num % 2 == 0)
         bin[aux] = 0;
      else
         bin[aux] = 1;
      num = num / 2;
   }

   for (aux = 0; aux < 8; aux++)
       printf("%d", bin[aux]);

   printf("\n");
} //tirar dps

unsigned char empurrando(unsigned char output, unsigned char aux){
    output = output << 1;
    return output | aux;
}

void compactar(hash *new_hash, char arquivo[]){
    unsigned char input, output=0, aux;
    int bits, posicao=0; //equivale a profundidade da hash e em que bit estamos no byte
    FILE *pont2=fopen(arquivo, "rb");
    FILE *salvar= fopen("compactadah.huff", "w+b"); //criando arquivo em binário

    while(fscanf(pont2,"%c", &input)!=EOF){
        printf("lendo '%c'\n", input);
        printf("aqui\n");
        bits= new_hash->profundidade[input] - 1;
        while(bits>=0){
            if(bits>(7-posicao)){ //condicao de nao caber
                while(posicao<7){ //condição de parada é até onde cabe
                    aux = new_hash->compactado[input] >> bits;
                    output=empurrando(output, aux);
                    posicao++;
                    bits--;
                }
                binario(output);
                fwrite(&output, sizeof(unsigned char), 1, salvar);
                output=0;
                posicao=0;
            }
            bits--;
        }
        while(bits>=0){ //condição de parada é ate zerar os bits
            aux = new_hash->compactado[input] >> bits;
            output=empurrando(output, aux);
            bits--;
        }
        if(posicao==7){ //para o caso de completar o byte

            binario(output);
            fwrite(&output, sizeof(unsigned char), 1, salvar);
            output=0;
            posicao=0;
        }
        posicao++;
    }
    fclose(salvar);
    fclose(pont2);
}
void converte(node *head, char arquivo[]){
    hash *new_hash = (hash*)malloc(sizeof(hash));
    int i;
    for(i=0;i<256;i++){
        new_hash->head_hash[i]=NULL;
        new_hash->profundidade[i]=0;
    }
    node_hash *ajuda=NULL;
    node_hash *fim=NULL;
    node *aux=head;
    node *aux2=head;
    printando(aux2);
    printf("\n");
    dicionario(aux, new_hash,0, 0);
    for(i=0;i<256;i++){
        if(new_hash->profundidade[i]){
            printf("'%c' %d ", i, new_hash->profundidade[i]);
            binario(new_hash->compactado[i]);
        }
    }
    compactar(new_hash, arquivo);
}

void main(){
    char arquivo[100];
    unsigned char bytes;
    printf("Insira o nome do arquivo:\n");
    scanf("%[^\n]s", arquivo);// lê o nome do arquivo ate a quebra de linha
    FILE *pont=fopen(arquivo, "rb"); //abrindo o arquivo com o nome dado em binário
    if(pont == NULL)
    {
        printf("Esse arquivo não existe!\n");
        exit(1); 
    }
    printf("Digite:\n");
    printf("1. Compactar arquivo\n2. Descompactar arquivo\n3. Sair\n");
    int opcao;
    scanf("%d", &opcao);
    if(opcao==1){
        queue *fila=(queue*)malloc(sizeof(queue));
        ler_arquivo(pont, fila);
        criar_arvore(fila);
        fclose(pont);
        converte(fila->head, arquivo);
        //printando(fila->head);
        printf("\n");

    }
    else if(opcao==2){

    }
    else {
        exit(1);
    }
}