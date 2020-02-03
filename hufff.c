#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

unsigned char asterisco = '*';
typedef struct queue queue;
typedef struct node_arvore node_arvore;
typedef struct hash hash;
typedef struct node_lista node_lista; //tirar dps
int tamanho_da_arvore=0, lixo, pre=0;


struct queue
{
	node_arvore *head;
};

struct node_arvore // estrutura onde alocamos o nó para a criação da fila e logo em seguida para a árvore
{
	unsigned char  byte;
	long long int freq; // a frequencia em lld, pois em casos de foto e videos a frequencias do caracteres será bem grande
	node_arvore *next;
	node_arvore *left; // ponteiro para esquerda para a criação da arvore
	node_arvore *right; // ponteiro para a direita para a criação da arvore

};

struct node_lista
{
    int bit;
    unsigned char byte_comp;
    node_lista *next;
    node_lista *previous;
}; //tirar dps

struct hash
{
    unsigned char compactado[256];
    int profundidade[256];
    node_lista *head_hash[256]; //tirar dps
};
int contando_bits(int num){
    int bits=1;
    while(num/2 != 0)
    {
        num = num/2;
        bits++;
    }
    return bits;
}
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

   printf(" ");
} //tirar dps

void para_listas(node_lista *head){
    node_lista *ok=head;
    printf("lista: ");
    
    while(ok!=NULL){
       binario(ok->byte_comp);
        ok=ok->next;
    }
    printf("\n");
} //tirar dps
node_lista *cria_lista_invertida(node_lista *head, node_lista *fim, unsigned char byte){
    node_lista *novo_byte=(node_lista*)malloc(sizeof(node_lista));
    if(head==NULL){
        head=novo_byte;
        novo_byte->previous=NULL;
    } else {
        fim->next=novo_byte;
        novo_byte->previous=fim;
    }
    novo_byte->byte_comp=byte;
    novo_byte->next=NULL;
    return novo_byte; //atualizando o fim
}
void contando(node_arvore *head){ //para arvores
    if(head!=NULL){
        if((head->byte=='*'&&head->left==NULL&&head->right==NULL) || head->byte==92){
            tamanho_da_arvore++;
        }
        tamanho_da_arvore++;
        contando(head->left);
        contando(head->right);
    }
}
void para_arvores(node_arvore *head){ //para arvores
    if(head!=NULL){
        printf("%c", head->byte);
        para_arvores(head->left);
        para_arvores(head->right);
    }
}
void escreve_arvore(node_arvore *head, FILE *salvar){
    unsigned char barra= 92;
    if(head!=NULL){
        if(head->byte==92){
            fwrite(&barra, sizeof(unsigned char), 1, salvar);
        }
        else if(head->byte=='*' && head->left==NULL && head->right==NULL){
            fwrite(&barra, sizeof(unsigned char), 1, salvar);
        }
        fwrite(&head->byte, sizeof(char), 1, salvar);
        escreve_arvore(head->left, salvar);
        escreve_arvore(head->right, salvar);
    }
}
void enqueue(queue *fila, unsigned char byte, long long int f, node_arvore *left, node_arvore *right){ // nesse caso, tivemos que passar os ponteiros da criação da arvore 
	                                                                                     //para conseguirmos manipular os pais, e dar dequeue e enqueue
    node_arvore *new_node=(node_arvore*)malloc(sizeof(node_arvore));
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
        node_arvore *current=fila->head; // criamos o nó do tipo 'node_arvore' que aponta para a cabeça da fila
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
            //if(i==asterisco){
            //    enqueue(fila, 92, frequencia[i], NULL, NULL); // '92' numero decimal da tabela ascii equivalente a '\'
            //}else
                enqueue(fila, i, frequencia[i], NULL, NULL);
        }
    } //fila criada

    if(fila->head==NULL){
        printf("ERRO NA FORMAÇÃO DA FILA!\n");
    }
    node_arvore *aux=fila->head;

}
void criar_arvore(queue *fila){ //na função cria a arvore e faz os ponteiro esq e dir apontar para os filhos
    node_arvore *aux=fila->head; // ponteiros auxiliares para guardar o no e dar enqueue da fila 
    node_arvore *aux2=aux->next; // ponteiros auxiliares para guardar o no e dar enqueue da fila 
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
void dicionario(node_arvore *head, hash *new_hash, unsigned char byte, int profundidade) {
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
void escreve(node_lista *head, node_arvore *cabeca_arvore){
    FILE *salvar= fopen("compactadah.huff", "wb"); //criando arquivo em binário

    unsigned char cabecalho1, cabecalho2, aux;
    int i = 13 - tamanho_da_arvore;
    cabecalho1 = lixo << 5;
    int tam_arv_bin = contando_bits(tamanho_da_arvore);
    if(tam_arv_bin < 8){
        cabecalho2 = tamanho_da_arvore;
    } 
    else {
        aux = tamanho_da_arvore >> 8;
        cabecalho1 = (cabecalho1 | aux);
        cabecalho2 = tamanho_da_arvore << (13 - tam_arv_bin);
   }
    fwrite(&cabecalho1, sizeof(char), 1, salvar);
    fwrite(&cabecalho2, sizeof(char), 1, salvar);
    escreve_arvore(cabeca_arvore, salvar);
    while(head != NULL)
    {
        fwrite(&head->byte_comp, sizeof(char), 1, salvar);
        head = head->next;
    }
    fclose(salvar);
}
unsigned char empurrando(unsigned char output, unsigned char aux){
    output = output << 1;
    return output | aux;
}
void compactar(hash *new_hash, char arquivo[], node_arvore *cabeca_arvore){
    unsigned char input, output=0, aux;
    node_lista *head=NULL, *fim=NULL;
    int bits, posicao=0; //equivale a profundidade da hash e em que bit estamos no byte
    FILE *pont2=fopen(arquivo, "rb");

    while(fscanf(pont2,"%c", &input)!=EOF){
        bits= new_hash->profundidade[input] - 1;
        while(bits>=0){
            if(posicao==8){
               // printf("output:");
               // binario(output);
                fim=cria_lista_invertida(head,fim,output);
                if(head==NULL){
                    head = fim;
                }
                //para_listas(head);
                posicao=0;
                output=0;
            }
            aux = new_hash->compactado[input] >> bits;
            //printf("aux:");
           // binario(aux);
            output=empurrando(output, aux);
            posicao++;
            bits--;
        }
    }
    //if(posicao<7){
        lixo=8-posicao;
        output=output<<lixo;
        fim=cria_lista_invertida(head,fim,output);
    //}
    escreve(head, cabeca_arvore);
    //para_listas(head);
    fclose(pont2);

}
void converte(node_arvore *head, char arquivo[]){
    hash *new_hash = (hash*)malloc(sizeof(hash));
    int i;
    for(i=0;i<256;i++){
        new_hash->head_hash[i]=NULL;
        new_hash->profundidade[i]=0;
    }
    node_lista *ajuda=NULL;
    node_lista *fim=NULL;
    node_arvore *aux=head;
    node_arvore *aux2=head;
    contando(aux2);
    //printf("eaaaa%d\n", tamanho_da_arvore);
    //printf("\n");
    dicionario(aux, new_hash,0, 0);

    for(i=0;i<256;i++){
        if(new_hash->profundidade[i]){
            //printf("'%c' %d ", i, new_hash->profundidade[i]);
            //binario(new_hash->compactado[i]);
        }
    }
    compactar(new_hash, arquivo, head);
}

node_arvore *cria_arv_pre(unsigned char arv[], int tam_arvore, node_arvore *head_descomp){ //tirar o tam_arvore
    node_arvore *novo_no = (node_arvore*)malloc(sizeof(node_arvore));
    if(head_descomp==NULL){
        head_descomp = novo_no;
    }
    if(arv[pre]==asterisco){
        novo_no->byte = asterisco;
        novo_no->left = NULL;
        novo_no->right = NULL;
        pre++;
        novo_no->left = cria_arv_pre(arv, tam_arvore, head_descomp);
        novo_no->right = cria_arv_pre(arv, tam_arvore, head_descomp);
    }
    else if(arv[pre]==92){
        pre++;
        novo_no->byte = arv[pre];
        novo_no->left = NULL;
        novo_no->right = NULL;
        pre++;
    }
    else{
        novo_no->byte = arv[pre];
        novo_no->left = NULL;
        novo_no->right = NULL;
        pre++;

    }
    return novo_no;
} 

void descompactar(node_arvore *head_arv, node_lista *head_list, node_lista *fim_list, int descartar, FILE *pont){
    int i=7, folha=0;
    unsigned char bit;
    //para_listas(head_list);
    //printf("\n");
    FILE *descomp=fopen("descompactadah.dat", "wb");
    node_arvore *aux=head_arv;
    printf("descartar %d\n", descartar);
    while(head_list!=NULL){
        i=7;
        if(head_list==fim_list){ //chegou no ultimo byte
            while(i>=descartar){
                if(folha==1){
                    folha=0;
                    aux=head_arv;
                }
                //binario(head_list->byte_comp);
                bit=head_list->byte_comp >> i;
                printf("'%d' ", bit);

                if(bit%2 != 0){ //para caso o bit seja 1
                    aux=aux->right;
                    if(aux->left==NULL && aux->right==NULL){
                        folha=1;
                        //printf("aqui1\n");
                        fwrite(&aux->byte,sizeof(unsigned char), 1, descomp);
                    }
                }else{ 
                    aux=aux->left;
                    if(aux->left==NULL && aux->right==NULL){
                        //printf("aqui\n");
                        folha=1;
                        fwrite(&aux->byte,sizeof(unsigned char), 1, descomp);
                    }
                }
                i--;
            }
        }
        else{
            while(i>=0){
                if(folha==1){
                    folha=0;
                    aux=head_arv;
                }
                //binario(head_list->byte_comp);
                bit=head_list->byte_comp >> i;
                //printf("'%d' ", bit);
                if(bit%2 != 0){ //para caso o bit seja 1
                    aux=aux->right;
                    if(aux->left==NULL && aux->right==NULL){
                        folha=1;
                        //printf("aqui3\n");
                        fwrite(&aux->byte,sizeof(unsigned char), 1, descomp);
                    }
                }else{ 
                    aux=aux->left;
                    if(aux->left==NULL && aux->right==NULL){
                        folha=1;
                        //printf("aqui4\n");
                        fwrite(&aux->byte,sizeof(unsigned char), 1, descomp);
                    }
                }
                i--;
            }
        }
        
        head_list=head_list->next;
    }
    fclose(descomp);
}

void lendo_cabecalho(FILE *pont){
    unsigned char cabecalho1, cabecalho2;
    int descartar, tam_arvore, i=31, j;
    fscanf(pont,"%c", &cabecalho1);
    binario(cabecalho1);
    fscanf(pont,"%c", &cabecalho2);
    binario(cabecalho2);

    descartar = cabecalho1 >> 5;
    tam_arvore = ((cabecalho1 & i)*pow(2,8)) + (cabecalho2);
    unsigned char arv[tam_arvore];
    //printf("arvore %d\n", tam_arvore);
    for(j=0; j<tam_arvore; j++){
        fscanf(pont,"%c", &arv[j]);
        //printf("%c", arv[j]);    
    }
    node_arvore *head_descomp=NULL;
    head_descomp = cria_arv_pre(arv, tam_arvore, head_descomp);  
    node_arvore *aff=head_descomp;
    //para_arvores(aff);
    unsigned char lendo;
    node_lista *arq_comp=NULL, *fim_comp=NULL;
    while(fscanf(pont,"%c", &lendo)!= EOF){
        fim_comp=cria_lista_invertida(arq_comp,fim_comp,lendo);
        if(arq_comp==NULL){
            arq_comp=fim_comp;
        }
    }
    //para_listas(arq_comp);
    //printf("\n");
    descompactar(head_descomp, arq_comp, fim_comp, descartar, pont);

    //para_listas(arq_comp);
    //para_arvores(head_descomp);
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
        //printf("arvore %d\n", tamanho_da_arvore);

        printf("Arquivo compactado!\n");
    }
    else if(opcao==2){
        lendo_cabecalho(pont);
        fclose(pont);


        printf("Arquivo descompactado!\n");
    }
    else {
        exit(1);
    }
}