#include <stdio.h>
#include <stdlib.h> 
#include "lista.h"

struct lista_t *lista_cria (){
    struct lista_t *lst malloc(sizeof(struct lista_t)); //cria lst
    if (!lst)
        return(NULL);
    lst->prim=NULL;
    lst->ult=NULL;
    lst->tamanho=0;
    return(lst);
}

int lista_insere (struct lista_t *lst, int item, int pos){
    int i;
    struct item_t *itm = malloc(sizeof(struct item_t)); //cria item_t
    if (!itm || pos<-1)
        return(-1); //erro de alocação de memória ou posição indefinida
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
        aux=lst->prim;
        for(i=1;i<=pos;i++)
            aux=aux->prox;
        itm->ant=aux->ant;
        itm->prox=aux;
        aux->ant=itm;
    }
    if(pos==0)
        lst->prim=itm;
    lst->tamanho++;
    return(lst->tamanho); //retorna o tamanho da lista
}

int lista_retira (struct lista_t *lst, int *item, int pos){
    int i;
    struct item_t *aux;
    if (lst->tamanho == 0 || pos<-1 || pos>lst->tamanho)
        return(-1); //Erro de acesso de posição indefinida
    if (pos == -1 || pos == lst->tamanho-1){
        *item = lst->ult->valor;
        aux = lst->ult;
        if (lst->tamanho==1){
            lst->prim=NULL;
            lst->ult=NULL;
        }else{
            lst->ult->ant->prox = NULL;
            lst->ult=lst->ult->ant;
        }
    }else{
        aux=lst->prim;
        for (i=1;i<=pos;i++)
                aux=aux->prox;
        *item = aux->valor;
        if(pos==0){
            aux->prox->ant=NULL;
            lst->prim=aux->prox;
        }else{
            aux->ant->prox=aux->prox;
            aux->prox->ant=aux->ant;
        }
    }
    free(aux);
    lst->tamanho--;
    return(lst->tamanho);
}

int lista_consulta (struct lista_t *lst, int *item, int pos){
    int i;
    struct item_t *aux; //cria aux
    if (pos>=lst->tamanho || pos <-1 || lst->tamanho == 0)
        return(-1); //erro de consulta de posição inexistente
    if(pos == -1){
        *item=lst->ult->valor;
        return(lst->tamanho);
    }
    aux=lst->prim;
    for (i=1;i<=pos;i++)
        aux=aux->prox;
    *item=aux->valor;
    return(lst->tamanho);
}

int lista_procura (struct lista_t *lst, int valor){
    int i;
    struct item_t *aux; 
    aux=lst->prim;
    for(i=1;i<=lst->tamanho;i++){
        if(aux->valor==valor)
            return(i-1); // achou e devolve a posição
        aux=aux->prox;
    }
    return(-1); //Não achou ou acessou posição inválida
}

int lista_tamanho (struct lista_t *lst){
    if(!lst)
        return(-1);
    return(lst->tamanho);
}

void lista_imprime (struct lista_t *lst){
    int i;
    struct item_t *aux;
    aux=lst->prim;
    for (i=1;i<=lst->tamanho;i++){
        printf("%d",aux->valor);
        if (i!=lst->tamanho)
            printf(" ");
        aux=aux->prox;
    }
}

struct lista_t *lista_destroi (struct lista_t *lst){
    int i;
    struct item_t *aux, *aux2;
    aux=lst->prim;
    for(i=1;i<=lst->tamanho;i++){
        aux2=aux->prox;
        free(aux); //libera cada elemento
        aux=aux2;
    }
    free(lst);
    return(NULL);
}
