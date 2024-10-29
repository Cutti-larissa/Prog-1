// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h> //conferir
#include "lista.h"

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria (){
    struct lista_t *lst; //cria lst
    lst = malloc(sizeof(struct lista_t));
    if (!lst)
        return(NULL);
    lst->prim=NULL;
    lst->ult=NULL;
    lst->tamanho=0;
    return(lst);
}

// Nas operações insere/retira/consulta/procura, a lista inicia na
// posição 0 (primeiro item) e termina na posição TAM-1 (último item).

int lista_insere (struct lista_t *lst, int item, int pos){
    int i;
    struct item_t *itm; //cria item_t
    itm = malloc(sizeof(struct item_t));
    if (!itm)
        return(-1); //erro de alocação de memória
    itm->valor = item;
    if (pos > lst->tamanho || pos == -1){
        itm->ant=lst->ult; // se não tiver itens recebe NULL se não recebe o ponteiro do ultimo
        itm->prox=NULL; //Como vai ser o ultimo o proximo é null
        if (lst->tamanho == 0)
            lst->prim=itm;
        if(lst->ult)
            lst->ult->prox=itm;
        lst->ult = itm;
    }else{ 
        struct item_t *aux;
        aux = malloc(sizeof(struct item_t)); //cria aux
        if (!aux)
            return(-1); //erro de alocação de memória
        aux=lst->prim;
        for(i=1;i<=pos;i++)
            aux=aux->prox;
        itm->ant=aux->ant;
        itm->prox=aux;
        aux->ant=itm;
        free(aux); //libera aux
    }
    if(pos==0)
        lst->prim=itm;
    lst->tamanho++;
    return(lst->tamanho); //retorna o tamanho da lista
}

// Retira o item da lista da posição indicada.
// se a posição for -1, retira do fim
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira (struct lista_t *lst, int *item, int pos){
    int i;
    struct item_t *aux; //cria aux
    aux = malloc(sizeof(struct item_t));
        if (!aux)
            return(-1); //erro de alocação de memória
    if (pos == -1){
        *item = lst->ult->valor;
        lst->ult->ant->prox = NULL;
        aux = lst->ult;
        lst->ult=lst->ult->ant;
    }else{
        aux=lst->prim;
        for (i=1;i<=pos;i++){
            aux=aux->prox;
        }
        *item = aux->valor;
        aux->ant->prox=aux->prox;
        aux->prox->ant=aux->ant;
    }
    free(aux); //libera aux
    lst->tamanho--;
    return(lst->tamanho);
}

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for -1, consulta do fim.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta (struct lista_t *lst, int *item, int pos){
    int i;
    struct item_t *aux; //cria aux
    aux = malloc(sizeof(struct item_t));
        if (!aux)
            return(-1); //erro de alocação de memória
    if (pos>lst->tamanho)
        return(-1); //erro de consulta de posição inexistente
    if(pos == -1)
        *item=lst->ult->valor;
    aux=lst->prim;
    for (i=1;i<=pos;i++)
        aux=aux->prox;
    *item=aux->valor;
    free(aux); //libera aux
    return(lst->tamanho);
}

// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura (struct lista_t *lst, int valor){
    int i;
    struct item_t *aux; //cria aux
    aux = malloc(sizeof(struct item_t));
        if (!aux)
            return(-1); //erro de alocação de memória
    aux=lst->prim;
    for(i=1;i<=lst->tamanho;i++){
        if(aux->valor==valor)
            return(i-1); // achou e devolve a posição
        aux=aux->prox;
    }
    free(aux); //libera aux
    return(-1); //Não achou
}

// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho (struct lista_t *lst){
    if(lst->tamanho == 0)
        return(-1);
    return(lst->tamanho);
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime (struct lista_t *lst){
    int i;
    struct item_t *aux; //cria aux
    aux = malloc(sizeof(struct item_t));
    aux=lst->prim;
    for (i=1;i<lst->tamanho;i++){
        printf("%d",aux->valor);
        aux=aux->prox;
    }
    free(aux); //libera aux
}

// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst){
    int i;
    struct item_t *aux; //cria aux
    aux = malloc(sizeof(struct item_t));
        if (!aux)
            return(NULL); //erro de alocaçãode memória
    aux=lst->prim;
    for(i=1;i<=lst->tamanho;i++){
        aux=aux->prox;
        free(aux->ant); //libera cada elemento
    }
    free(lst);
    lst=NULL;
    free(aux); //libera aux
    return(NULL);
}
