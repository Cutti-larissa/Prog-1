#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria ()
{
    struct fprio_t *f = malloc(sizeof(struct fprio_t));
    if (!f)
        return(NULL);
    f->num = 0;
    f->prim = NULL;
    return(f);
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro. (perguntar)
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio)
{
    if (!f || !item)
        return(-1);
    struct fpnodo_t *nodo = malloc (sizeof(struct fpnodo_t));
    nodo->item = item;
    nodo->prio = prio;
    nodo->tipo = tipo;
    if(!f->prim)
    {
        nodo->prox = NULL;
        f->prim = nodo;
    }
    else
    {
        int i;
        struct fpnodo_t *aux = f->prim;
        while (aux->prox && nodo->prio>=aux->prox->prio)
            aux = aux->prox;
        nodo->prox = aux->prox;
        aux->prox = nodo;   
    }
    f->num++;
    return(f->num);
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio)
{
    if(!f || !f->prim)
        return(NULL);
    struct fpnodo_t *aux = f->prim; 
    void *item;
    tipo = f->prim->tipo;
    prio = f->prim->prio;
    f->prim = aux->prox;
    item = aux->item;
    free(aux->item);
    free(aux);
    return(item);
}

int fprio_tamanho (struct fprio_t *f)
{
    if (!f)
        return(-1);
    return(f->num);
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f)
{
    int i;
    struct fpnodo_t *aux = f->prim;
    for (i=1;i<=f->num;i++){
        printf("(%d %d)", aux->tipo, aux->prio);
        if(i!=f->num)
            printf(" ");
        aux = aux->prox;
    }
}

struct fprio_t *fprio_destroi (struct fprio_t *f)
{
    if(!f)
        return(NULL);
    struct fpnodo_t *aux = f->prim;
    while(f->prim!=NULL){
        f->prim=f->prim->prox;
        free(aux->item);
        free(aux);
        aux=f->prim;
    }
    free(f);
    return(NULL); 
}
