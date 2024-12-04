#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "eventos.h"

void inicia_mundo(struct mundo_t *W)
{
  W->nHerois = N_HEROIS;
  for (int i = 0; i < N_HEROIS; i++)
    W->Herois[i] = NULL;
  W->nBases = N_BASES;
  for (int i = 0; i < N_BASES; i++)
    W->Bases[i] = NULL;
  W->nMissoes = N_MISSOES;
  W->mCumpridas = 0;
  for (int i = 0; i < N_MISSOES; i++)
    W->Missoes[i] = NULL;
  W->nHabilidades = N_HABILIDADES;
  W->tam_mundo.x = N_TAMANHO_MUNDO;
  W->tam_mundo.y = N_TAMANHO_MUNDO;
  W->relogio = T_INICIO;
}

void inicia_herois(struct mundo_t *W)
{
  int i;
  for (i=0; i<N_HEROIS; i++)
  {
    int habilidades = rand () % (3 - 1 + 1) + 1;
    struct heroi_t *heroi = malloc (sizeof(struct heroi_t));
    if (!heroi)
      return;
    struct cjto_t *habi = cjto_cria(10);
    if (!habi)
      return;
    
    heroi->id = i;
    heroi->xp = 0;
    heroi->status = 1;
    heroi->paciencia = rand() % (101); 
    heroi->veloc = rand () % (5000 - 50 + 1) + 50;  //int habilidades onde é melhor declarar
    for (int j = 0; j<habilidades; j++)
    {
      int h = rand () % (N_HABILIDADES);
      if (cjto_pertence(habi, h))
        j--;
      else 
        cjto_insere(habi,h);
    }
    heroi->hab = habi;
    W->Herois[i] = heroi;
  }
}

void inicia_bases(struct mundo_t *W)
{
  int i;
  for(i=0; i<N_BASES; i++)
  {
    struct base_t *base = malloc (sizeof(struct base_t));
    struct cjto_t *heros = cjto_cria(N_HEROIS); 
    struct lista_t *esp = lista_cria();
    if (!base || !heros || !esp)
      return;
    
    base->id = i; 
    base->local.x = rand() % (N_TAMANHO_MUNDO); 
    base->local.y = rand() % (N_TAMANHO_MUNDO); 
    base->max = rand() % (10 - 3 + 1) + 3; 
    base->pres = heros; 
    base->espera = esp;
    base->fila_max = 0;
    base->nMissoes = 0; 
    W->Bases[i] = base;  
  }
}

void inicia_missoes(struct mundo_t *W)
{
  int i;
  for(i=0; i<N_MISSOES; i++)
  {
    int habis;
    struct mission_t *missao = malloc(sizeof(struct mission_t));
    if (!missao)
      return;
    struct cjto_t *habi_rq = cjto_cria(N_HABILIDADES);
    if (!habi_rq)
      return;
    
    missao->id = i; 
    missao->local.x = rand() % (N_TAMANHO_MUNDO); 
    missao->local.y = rand() % (N_TAMANHO_MUNDO);
    habis = rand() % (10 - 6 + 1) + 6;
    for (int j = 0; j<habis; j++)
    {
      int h = rand () % (N_HABILIDADES);
      if (cjto_pertence(habi_rq, h))
        j--;
      else 
        cjto_insere(habi_rq,h);
    }
    missao->hab = habi_rq; 
    missao->perigo = rand () % (101); 
    missao->tentativas = 0;
    W->Missoes[i] = missao;
  }
}

void inicia_eventos(struct mundo_t *W, struct fprio_t *LEF)
{
  for(int i=0; i<N_HEROIS; i++)
  { 
    struct heroi_t *H = W->Herois[i];
    int base = rand () % (N_BASES); 
    int tempo = rand() % (4321);  
    struct ev_t *chega = malloc(sizeof(struct ev_t));  
    if (!chega)
      return;
    chega->heroi = H;
    chega->base = W->Bases[base];
    fprio_insere(LEF,chega,0,tempo);
  }

  for(int i=0; i<N_MISSOES; i++)
  { 
    int tempo = rand() % (T_FIM_DO_MUNDO); //T_FIM_DO MUNDO + 1?
    struct ev_t *mission = malloc(sizeof(struct ev_t)); 
    if (!mission)
      return;
    mission->missao = W->Missoes[i]; 
    fprio_insere(LEF,mission,8,tempo);
  }

  int tempo = T_FIM_DO_MUNDO; 
  struct ev_t *fim = malloc(sizeof(struct ev_t));
  if (!fim)
    return;
  fprio_insere(LEF,fim,9,tempo);
}

void destroi_mundo(struct mundo_t *W)
{
  if(!W)
    return;

  for (int i=0; i<N_HEROIS; i++)
  {
    cjto_destroi(W->Herois[i]->hab);
    free(W->Herois[i]);
  }
  for (int i=0; i<N_BASES; i++)
  {
    cjto_destroi(W->Bases[i]->pres);
    lista_destroi(W->Bases[i]->espera);
    free(W->Bases[i]);
  }
  for (int i=0; i<N_MISSOES; i++)
  {
    cjto_destroi(W->Missoes[i]->hab);
    free(W->Missoes[i]);
  }

  free(W);
}
