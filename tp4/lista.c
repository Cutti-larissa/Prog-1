#include <stdio.h>
#include <stdlib.h> 
#include "lista.h"

struct lista_t* lista_cria (){
  struct lista_t* lst = malloc(sizeof(struct lista_t));
  if (!lst)
    return(NULL);
  
  lst->prim=NULL;
  lst->ult=NULL;
  lst->tamanho=0;
  
  return(lst);
}

int lista_insere (struct lista_t* lst, int item, int pos){
  if(!lst)
    return(-1);
  
  struct item_t* itm = malloc(sizeof(struct item_t));
  if (!itm || pos < -1)
    return(-1);
  
  itm->valor = item;
  
  if (pos > lst->tamanho || pos == -1){
    itm->ant = lst->ult;
    itm->prox = NULL;
    if (lst->tamanho == 0)
      lst->prim=itm;
    if(lst->ult)
      lst->ult->prox = itm;
    lst->ult = itm;
  }else{
    if(lst->tamanho==0){
      itm->ant = NULL;
      itm->prox = NULL;
      lst->ult = itm;
    }else{
      struct item_t* aux;
      aux = lst->prim;
      for(int i = 0; i < pos; i++)
        aux = aux->prox;
      itm->ant = aux->ant;
      itm->prox = aux;
      if(pos != 0)
        aux->ant->prox = itm;
      aux->ant = itm;
    }
  }
  if(pos == 0)
    lst->prim = itm;
  lst->tamanho++;
  
  return(lst->tamanho);
}

int lista_retira (struct lista_t* lst, int* item, int pos){
  if(!lst)
    return(-1);
  
  if (!item || lst->tamanho == 0 || pos<-1 || pos>lst->tamanho)
    return(-1);

  struct item_t* aux = NULL;
  if (pos == -1 || pos == lst->tamanho - 1)
  {
    *item = lst->ult->valor;
    aux = lst->ult;
    if (lst->tamanho == 1){
      lst->prim = NULL;
      lst->ult = NULL;
    }else{
      lst->ult->ant->prox = NULL;
      lst->ult = lst->ult->ant;
    }
  }else
  {
    aux = lst->prim;
    for (int i = 0; i < pos; i++)
      aux = aux->prox;
    *item = aux->valor;
    if(pos == 0){
      aux->prox->ant = NULL;
      lst->prim = aux->prox;
    }else{
      aux->ant->prox = aux->prox;
      aux->prox->ant = aux->ant;
    }
  }
  
  free(aux);
  lst->tamanho--;
  
  return(lst->tamanho);
}

int lista_consulta (struct lista_t* lst, int* item, int pos){
  if(!lst)
    return(-1);
  
  if (!item || pos >= lst->tamanho || pos < -1 || lst->tamanho == 0)
    return(-1);
  
  if(pos == -1){
    *item = lst->ult->valor;
    return(lst->tamanho);
  }
  
  struct item_t* aux = lst->prim;
  for (int i = 1; i <= pos; i++)
    aux = aux->prox;
  
  *item = aux->valor;
  
  return(lst->tamanho);
}

int lista_procura (struct lista_t* lst, int valor){
  if(!lst)
    return(-1);
  
  struct item_t* aux = lst->prim;
  for(int i = 1; i <= lst->tamanho; i++){
    if(aux->valor == valor)
      return(i-1);
    aux = aux->prox;
  }
  
  return(-1);
}

int lista_tamanho (struct lista_t* lst){
  if(!lst)
    return(-1);
  return(lst->tamanho);
}

void lista_imprime (struct lista_t* lst){
  struct item_t* aux = lst->prim;
  for (int i = 1; i <= lst->tamanho; i++){
    printf("%d",aux->valor);
    if (i != lst->tamanho)
      printf(" ");
    aux = aux->prox;
  }
}

struct lista_t* lista_destroi (struct lista_t* lst){
  struct item_t* aux = lst->prim;
  while(lst->prim != NULL){
    lst->prim = aux->prox;
    free(aux);
    aux = lst->prim;
  }
  free(lst);
  
  return(NULL);
}
