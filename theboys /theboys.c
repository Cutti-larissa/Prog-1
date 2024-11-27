// programa principal do projeto "The Boys - 2024/2"
// Autor: Larissa Schaldach Cutti, GRR 20240590

#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "eventos.h"
#include "entidades.h"
#include "fila_espera.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100

void inicia_mundo(struct mundo_t *W)
{
    W->nHerois = N_HEROIS;
    W->nBases = N_BASES;
    W->nMissoes = N_MISSOES;
    W->nHabilidades = N_HABILIDADES;
    W->tam_mundo->x = N_TAMANHO_MUNDO;
    W->tam_mundo->y = N_TAMANHO_MUNDO;
    W->relogio = 0;
}

void inicia_herois(struct mundo_t *W)
{
  int i;
  for (i=0; i<N_HEROIS; i++)
  {
    struct heroi_t *heroi = malloc (sizeof(struct heroi_t));
    struct cjto_t *habi = malloc (sizeof(struct cjto_t));
    
    int habilidades = rand () % (3 - 1 + 1) + 1;
    
    heroi->id = i; //id = número sequencial [0...N_HEROIS-1]
    heroi->xp = 0; //experiência = 0
    heroi->status = 1; //heroi vivo
    heroi->paciencia = rand() % (101); //paciência = número aleatório [0...100]
    heroi->veloc = rand () % (5000 - 50 + 1) + 50; //velocidade  = número aleatório [50...5000] (em metros/minuto = 3 Km/h a 300 Km/h)
    habi = cjto_cria(habilidades);
    int j;
    for (j=0; j<habilidades; j++){
        int h;
        h = rand () % (10);
        if (cjto_pertence(habi, h))
            j--;
        else 
            cjto_insere(habi,h);
    }
    heroi->hab = habi; //habilidades = conjunto com tamanho aleatório [1...3] de habilidades aleatórias
    W->Herois[i] = heroi;
  }
}

void inicia_bases(struct mundo_t *W)
{
    int i;
    for(i=0; i<N_BASES; i++)
    {
        struct base_t *base = malloc (sizeof(struct base_t));
        struct cjto_t *heros = malloc(sizeof(struct cjto_t));
        struct fila_t *esp = malloc(sizeof(struct fila_t));

        base->id = i; //id = número sequencial [0...N_BASES-1]
        base->local->x = rand() % (20000); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
        base->local->y = rand() % (20000);
        base->max = rand() % (10 - 3 + 1) + 1; //lotação = número aleatório [3...10]
        heros = cjto_cria(base->max);
        base->pres = heros; // presentes = conjunto vazio (com capacidade para armazenar IDs de heróis até a lotação da base)
        esp = fila_cria(base->max);
        base->espera = esp;     
        W->Bases[i] = base;  
    }
}

void inicia_missoes(struct mundo_t *W)
{
    int i;
    for(i=0; i<N_MISSOES; i++){
        struct mission_t *missao = malloc(sizeof(struct mission_t));
        struct cjto_t *habi_rq = malloc(sizeof(struct cjto_t));

        missao->id = i; //id = número sequencial [0...N_MISSOES-1]
        missao->local->x = rand() % (20000); //local = par de números aleatórios [0...N_TAMANHO_MUNDO-1]
        missao->local->y = rand() % (20000);
        int habis = rand() % (10 - 6 + 1) + 1;
        habi_rq = cjto_cria(habis);
        int j;
        for (j=0; j<habis; j++){
            int h;
            h = rand () % (10);
            if (cjto_pertence(habi_rq, h))
                j--;
            else 
                cjto_insere(habi_rq,h);
        }
        missao->hab = habi_rq; //habilidades = conjunto com tamanho aleatório [6...10] de habilidades aleatórias
        missao->perigo = rand () % (101); //perigo      = número aleatório [0...100]
        W->Missoes[i] = missao;
    }
}

void inicia_eventos(struct mundo_t *W, struct fprio_t *LEF)
{
    for(int i=0; i<N_HEROIS; i++){ //para cada herói H:
        struct heroi_t *H = W->Herois[i];
        H->base = rand () % (N_BASES); //base  = número aleatório [0...N_BASES-1]
        int tempo = rand() % (4321);  //tempo = número aleatório [0...4320]  // 4320 = 60*24*3 = 3 dias
        struct ev_t *chega = malloc(sizeof(struct ev_t));  //criar e inserir na LEF o evento CHEGA (tempo, H, base)
        chega->tempo = tempo;
        chega->heroi = H;
        chega->base = W->Bases[H->base];
        fprio_insere(LEF,chega,0,chega->tempo);
    }

    for(int i=0; i<N_MISSOES; i++){ //para cada missão M:
        int tempo = rand() % (T_FIM_DO_MUNDO); //tempo = número aleatório [0...T_FIM_DO_MUNDO]
        struct ev_t *mission = malloc(sizeof(struct ev_t)); // criar e inserir na LEF o evento MISSÃO (tempo, M)
        mission->tempo = tempo;
        fprio_insere(LEF,mission,8,mission->tempo);
    }

    int tempo = T_FIM_DO_MUNDO; //tempo = T_FIM_DO_MUNDO
    struct ev_t *fim = malloc(sizeof(struct ev_t));
    fprio_insere(LEF,fim,9,tempo); //criar e inserir na LEF o evento FIM (tempo)
}

// programa principal
int main ()
{
    int ev,prio,t;
    struct heroi_t *H;
    struct base_t *B;
    struct mundo_t *W = malloc(sizeof(struct mundo_t)); //iniciar as entidades e atributos do mundo
    inicia_mundo(W);
    inicia_herois(W);
    inicia_bases(W);
    inicia_missoes(W);

    struct fprio_t *LEF;
    LEF = fprio_cria(); // criar a lista de eventos futuros

    inicia_eventos(W,LEF); //criar os eventos iniciais
    W->relogio = T_INICIO; //relógio = 0

    while (W->relogio< T_FIM_DO_MUNDO){ //repetir até o fim da simulação
        fprio_retira(LEF, &ev, &prio, &t, H, B); //ev = 1º evento da lista de eventos futuros
        W->relogio = prio; // relógio = tempo (ev)
        switch (ev) //caso tipo (ev) seja:
        {
            case 0:
                chega(t, H, B, LEF);
                break;
            case 1:
                espera(t, H, B, LEF);
                break;
            case 2:
                desiste(t, H, B, LEF, W);
                break;
            case 3:
                avisa(t, B, LEF);
                break;
            case 4:
                entra(t, H, B, LEF);
                break;
            case 5:
                sai(t, H, B, LEF, W);
                break;
            case 6:
                viaja(t, H, B, LEF, W);
                break;
            case 7:
                morre(t, H, B, LEF);
                break;
            case 8:
                //missao(t, M, LEF, W);
        }
    }
/* 
  
apresentar resultados*/

  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}
