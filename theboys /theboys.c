// programa principal do projeto "The Boys - 2024/2"
// Autor: Larissa Schaldach Cutti, GRR 20240590

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
  W->nBases = N_BASES;
  W->nMissoes = N_MISSOES;
  W->nHabilidades = N_HABILIDADES;
  W->tam_mundo->x = N_TAMANHO_MUNDO;
  W->tam_mundo->y = N_TAMANHO_MUNDO;
  W->relogio = T_INICIO;
  printf("Mundo criado\n");
}

void inicia_herois(struct mundo_t *W)
{
  int i;
  for (i=0; i<N_HEROIS; i++)
  {
    int habilidades = rand () % (3 - 1 + 1) + 1;
    struct heroi_t *heroi = malloc (sizeof(struct heroi_t));
    struct cjto_t *habi = malloc (sizeof(struct cjto_t));
    
    heroi->id = i; //id = número sequencial [0...N_HEROIS-1]
    heroi->xp = 0; //experiência = 0
    heroi->status = 1; //heroi vivo
    heroi->paciencia = rand() % (101); //paciência = número aleatório [0...100]
    heroi->veloc = rand () % (5000 - 50 + 1) + 50; //velocidade  = número aleatório [50...5000] (em metros/minuto = 3 Km/h a 300 Km/h)
    habi = cjto_cria(10);
    for (int j = 0; j<habilidades; j++){
      int h = rand () % (N_HABILIDADES);
      if (cjto_pertence(habi, h))
        j--;
      else 
        cjto_insere(habi,h);
    }
    heroi->hab = habi; //habilidades = conjunto com tamanho aleatório [1...3] de habilidades aleatórias
    W->Herois[i] = heroi;
  }
  printf("Herois criados\n");
}

void inicia_bases(struct mundo_t *W)
{
  int i;
  for(i=0; i<N_BASES; i++)
  {
    struct base_t *base = malloc (sizeof(struct base_t));
    struct cjto_t *heros = malloc(sizeof(struct cjto_t));
    struct lista_t *esp = malloc(sizeof(struct lista_t));
    
    base->id = i; //id = número sequencial [0...N_BASES-1]
    base->local->x = rand() % (N_TAMANHO_MUNDO); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
    base->local->y = rand() % (N_TAMANHO_MUNDO); // ou usar W->tam_mundo
    base->max = rand() % (10 - 3 + 1) + 1; //lotação = número aleatório [3...10]
    heros = cjto_cria(N_HEROIS); //conjunto que pode armazenar valores de 0 até N_HEROIS - 1
    base->pres = heros; // presentes = conjunto vazio (com capacidade para armazenar IDs de heróis até a lotação da base) (?)
    esp = lista_cria();
    base->espera = esp;     
    W->Bases[i] = base;  
  }
  printf("Bases criadas\n");
}

void inicia_missoes(struct mundo_t *W)
{
  int i;
  for(i=0; i<N_MISSOES; i++){
    int habis;
    struct mission_t *missao = malloc(sizeof(struct mission_t));
    struct cjto_t *habi_rq = malloc(sizeof(struct cjto_t));
    
    missao->id = i; //id = número sequencial [0...N_MISSOES-1]
    missao->local->x = rand() % (N_TAMANHO_MUNDO); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
    missao->local->y = rand() % (N_TAMANHO_MUNDO);
    habis = rand() % (10 - 6 + 1) + 1;
    habi_rq = cjto_cria(N_HABILIDADES);
    for (int j = 0; j<habis; j++){
      int h = rand () % (N_HABILIDADES);
      if (cjto_pertence(habi_rq, h))
        j--;
      else 
        cjto_insere(habi_rq,h);
    }
    missao->hab = habi_rq; //habilidades = conjunto com tamanho aleatório [6...10] de habilidades aleatórias
    missao->perigo = rand () % (101); //perigo = número aleatório [0...100]
    missao->status = 0;
    W->Missoes[i] = missao;
  }
  printf("Criou missões\n");
}

void inicia_eventos(struct mundo_t *W, struct fprio_t *LEF)
{
  for(int i=0; i<N_HEROIS; i++){ //para cada herói H:
    struct heroi_t *H = W->Herois[i];
    int base = rand () % (N_BASES); //base  = número aleatório [0...N_BASES-1]
    int tempo = rand() % (4321);  //tempo = número aleatório [0...4320]  // 4320 = 60*24*3 = 3 dias
    struct ev_t *chega = malloc(sizeof(struct ev_t));  //criar e inserir na LEF o evento CHEGA (tempo, H, base)
      chega->heroi = H;
      chega->base = W->Bases[base];
      fprio_insere(LEF,chega,0,tempo);
  }

  for(int i=0; i<N_MISSOES; i++){ //para cada missão M:
    int tempo = rand() % (T_FIM_DO_MUNDO); //tempo = número aleatório [0...T_FIM_DO_MUNDO]
    struct ev_t *mission = malloc(sizeof(struct ev_t)); // criar e inserir na LEF o evento MISSÃO (tempo, M)
    mission->missao = W->Bases[i]; //observar
    fprio_insere(LEF,mission,8,tempo);
  }

  int tempo = T_FIM_DO_MUNDO; //tempo = T_FIM_DO_MUNDO
  struct ev_t *fim = malloc(sizeof(struct ev_t));
  fprio_insere(LEF,fim,9,tempo); //criar e inserir na LEF o evento FIM (tempo)
  printf("Eventos iniciados\n");
}

void destroi_mundo(struct mundo_t *W)
{
  if(!W)
    return;
  for (int i=0; i<N_HEROIS; i++){
    cjto_destroi(W->Herois[i]->hab);
    free(W->Herois[i]);
  }
  for (int i=0; i<N_BASES; i++){
    cjto_destroi(W->Bases[i]->pres);
    lista_destroi(W->Bases[i]->espera);
    free(W->Bases[i]);
  }
  for (int i=0; i<N_MISSOES; i++){
    cjto_destroi(W->Missoes[i]->hab);
    free(W->Missoes[i]);
  }
  free(W);
}

// programa principal
int main ()
{
  int tipo_ev, tempo, evt_trat;
  struct ev_t *ev;
  struct mundo_t *W = malloc(sizeof(struct mundo_t)); //iniciar as entidades e atributos do mundo
  if (!W)
    return -1;
  struct fprio_t *LEF = malloc(sizeof(struct fprio_t));
    if (!LEF)
    {
      free(W);
      return -1;
    }

  inicia_mundo(W);
  inicia_herois(W);
  inicia_bases(W);
  inicia_missoes(W);
  LEF = fprio_cria(); // criar a lista de eventos futuros
  inicia_eventos(W,LEF); //criar os eventos iniciais

  while (W->relogio< T_FIM_DO_MUNDO){ //repetir até o fim da simulação //laço da simulação
    ev = fprio_retira(LEF, &tipo_ev, &tempo); //ev = 1º evento da lista de eventos futuros
    evt_trat++;
    W->relogio = tempo; // relógio = tempo (ev)
    switch (tipo_ev) //caso tipo (ev) seja:
    {
      case 0:
        chega(tempo, ev->heroi, ev->base, LEF, W);
        break;
      case 1:
        espera(tempo, ev->heroi, ev->base, LEF, W);
        break;
      case 2:
        desiste(tempo, ev->heroi, ev->base, LEF, W);
        break;
      case 3:
        avisa(tempo, ev->base, LEF, W);
        break;
      case 4:
        entra(tempo, ev->heroi, ev->base, LEF, W);
        break;
      case 5:
        sai(tempo, ev->heroi, ev->base, LEF, W);
        break;
      case 6:
        viaja(tempo, ev->heroi, ev->base, LEF, W);
        break;
      case 7:
        morre(tempo, ev->heroi, ev->base, ev->mission->id, LEF, W);
        break;
      case 8:
        missao(tempo, ev->missao, LEF, W);
        break;
      case 9:
        fim(W, evt_trat); //estatisticas criar struct statistc
        break;
      default:
        break;
    }
  }
  fprio_destroi(LEF);
  destroi_mundo(W); // destruir o mundo

  return (0) ;
}
