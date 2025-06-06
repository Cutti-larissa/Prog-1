#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t* fprio_cria (){
  struct fprio_t* f = (struct fprio_t*) malloc(sizeof(struct fprio_t));
  if (!f)
    return(NULL);
  
  f->num = 0;
  f->prim = NULL;
  
  return(f);
}

int fprio_insere (struct fprio_t* f, void* item, int tipo, int prio){
  if (!f || !item)
    return(-1);
  
  struct fpnodo_t* aux = f->prim;
  for(int i = 0; i < f->num; i++)
    if (aux->item == item)
      return(-1);
  
  struct fpnodo_t* nodo = (struct fpnodo_t*) malloc (sizeof(struct fpnodo_t));
  nodo->item = item;
  nodo->prio = prio;
  nodo->tipo = tipo;
  
  if(!f->prim){
    nodo->prox = NULL;
    f->prim = nodo;
  }else{
    if(f->prim->prio > nodo->prio){
      nodo->prox = f->prim;
      f->prim = nodo;
    }else{
      struct fpnodo_t *aux = f->prim;
      while (aux->prox && nodo->prio >= aux->prox->prio)
        aux = aux->prox;
      nodo->prox = aux->prox;
      aux->prox = nodo; 
    }
  }
  f->num++;
  
  return(f->num);
}

void* fprio_retira (struct fprio_t* f, int* tipo, int* prio){
  if(!f || !f->prim || !tipo || !prio)
    return(NULL);
  
  struct fpnodo_t* aux = f->prim; 
  *tipo = f->prim->tipo;
  *prio = f->prim->prio;
  void* item = aux->item;
  f->prim = aux->prox;
  f->num--;
  free(aux);
  
  return(item);
}

int fprio_tamanho (struct fprio_t* f){
  if (!f)
    return(-1);
  return(f->num);
}

void fprio_imprime (struct fprio_t* f){
  struct fpnodo_t* aux = f->prim;
  for (int i = 1; i <= f->num; i++){
    printf("(%d %d)", aux->tipo, aux->prio);
    if(i != f->num)
      printf(" ");
    aux = aux->prox;
  }
}

struct fprio_t* fprio_destroi (struct fprio_t* f){
  if(!f)
    return(NULL);
  struct fpnodo_t* aux = f->prim;
  while(f->prim!=NULL){
    f->prim = f->prim->prox;
    free(aux->item);
    free(aux);
    aux = f->prim;
  }
  free(f);
  return(NULL); 
}
