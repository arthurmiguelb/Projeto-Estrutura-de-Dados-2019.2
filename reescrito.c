#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct queue queue;
typedef struct node node;
typedef struct hash hash;

struct queue
{
	node *head;
};

struct node{
    void *item;
    int numero;
    node *next;
    node *previous;
    node *left;
    node *right;
};

struct hash{
    unsigned char compactado[256];
    int profundidade[256];
};

void para_listas(node *head){
    node *ok=head;
    printf("lista: ");
    
    while(ok!=NULL){
        printf("%c", *((char*)ok->item));
        ok=ok->next;
    }
    printf("\n");
}
void enqueue(queue *fila_de_frequencia, unsigned char *byte, long long int frequencia_byte, node *left, node *right){ 
    node *new_node=(node*)malloc(sizeof(node));
    new_node->item=byte;
    new_node->numero=frequencia_byte;
    new_node->left=left;
    new_node->right=right;
    if(fila_de_frequencia->head==NULL || frequencia_byte <= fila_de_frequencia->head->numero){
        new_node->next=fila_de_frequencia->head;
        fila_de_frequencia->head=new_node;
    } else {
        node *current=fila_de_frequencia->head;
        while((current->next!=NULL) && (current->next->numero < frequencia_byte)){
            current=current->next;
        }
        new_node->next=current->next;
        current->next=new_node;
    }
    para_listas(fila_de_frequencia->head);

}
void ler_arquivo(FILE *pont, queue *fila_de_frequencia){ 
    unsigned char byte;
    long long int frequencia_dos_bytes[256]; 
    int i;
    fila_de_frequencia->head=NULL;
    for(i=0;i<256;i++){
        frequencia_dos_bytes[i]=0; 
    }
    while(fscanf(pont,"%c", &byte)!=EOF){ 
        frequencia_dos_bytes[byte]++; 
    }
    for(i=0;i<256;i++){
        if(frequencia_dos_bytes[i]){
            enqueue(fila_de_frequencia, &i, frequencia_dos_bytes[i], NULL, NULL);
        }
    } 
   // para_listas(fila_de_frequencia->head);
    if(fila_de_frequencia->head==NULL){
        printf("ERRO NA FORMAÇÃO DA FILA!\n");
    }
}

void main(){
    char arquivo[100];
    printf("Insira o nome do arquivo:\n");
    scanf("%[^\n]s", arquivo);
    FILE *pont=fopen(arquivo, "rb"); 
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
        queue *fila_de_frequencia=(queue*)malloc(sizeof(queue));
        ler_arquivo(pont, fila_de_frequencia);
        //criar_arvore(fila_de_frequencia);
        fclose(pont);
        //converte(fila_de_frequencia->head, arquivo);
        printf("Arquivo compactado!\n");
    }
    else if(opcao==2){
        //lendo_cabecalho(pont);
        fclose(pont);


        printf("Arquivo descompactado!\n");
    }
    else {
        exit(1);
    }
}