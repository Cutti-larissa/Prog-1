// Tipo abstrato de dado "fila de inteiros"
// Prof. Carlos Maziero - DINF/UFPR, Out 2024
//
// Este TAD implementa uma fila de números inteiros com capacidade
// para até CAP elementos, definida no momento de sua criação.
//
// Implementação com lista encadeada;
#include <stdio.h>
#include <stdlib.h>
#include "fila_espera.h"

struct fila_t *fila_cria (int capacidade){
    struct fila_t *f;
    f->prim = NULL;
    f->ult = NULL;
    f->capacidade = capacidade;
    return(f);
}

int fila_insere (struct fila_t *f, int elem){
    struct nodo_t *heroi = malloc(sizeof(struct nodo_t));
    if (!heroi)
        return(-1);
    heroi->hero->id = elem;
    heroi->prox = NULL;
    if(!f->prim)
    {
        f->prim = heroi;
        f->ult = heroi;
    } else {
        f->ult->prox = heroi;
        f->ult = heroi;
    }
    f->qtde++;
    return(f->qtde);
}

// Retira o elemento do inicio da fila (politica FIFO) e o devolve;
// Retorna o número de elementos na fila após a operação
// ou -1 em caso de erro
int fila_retira (struct fila_t *f, int *elem){
    if (!f || !elem || f->prim)
        return(-1);
    *elem = f->prim->hero->id;
    struct nodo_t *aux;
    aux = malloc(sizeof(struct nodo_t));
    if (!aux)
        return(-1);
    aux = f->prim;
    f->prim = f->prim->prox;
    free(aux);
    f->qtde--;
    return(f->qtde);
}

// Devolve o primeiro da fila, sem removê-lo
// Retorna o número de elementos na fila ou -1 em caso de erro
int fila_primeiro (struct fila_t *f, int *elem){
    if(!f->prim)
        return(-1);
    *elem=f->prim->hero->id;
    return(f->qtde);
}

// Retorna o tamanho da fila (número de elementos presentes)
int fila_tamanho (struct fila_t *f){
    return(f->qtde);
}

// Imprime o conteúdo da fila do início ao fim, no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois e sem \n no fim
void fila_imprime (struct fila_t *f){
    int i;
    struct nodo_t *aux = f->prim;
    for (i=1;i<=f->qtde;i++)
    {
        printf("%d", aux->hero->id);
        if(i!=f->qtde)
        printf(" ");
        aux = aux->prox;
    }
}

// Remove todos os elementos da fila, libera memória e retorna NULL
struct fila_t *fila_destroi (struct fila_t *f)
{
  if(!f)
    return(NULL);
  struct fpnodo_t *aux = f->prim;
  while(f->prim!=NULL)
  {
    f->prim = f->prim->prox;
    free(aux);
    aux = f->prim;
  }
  free(f);
  return(NULL); 
}
