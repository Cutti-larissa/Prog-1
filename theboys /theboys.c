// programa principal do projeto "The Boys - 2024/2"
// Autor: Larissa Schaldach Cutti, GRR 20240590

#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fprio.h"
#include "lista.h"
#include "eventos.h"
#include "fila_espera.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100

struct ev_t{
    int tempo;
    struct heroi_t;
    struct base_t;
};

struct coord_t{
    int x;
    int y;
};

struct heroi_t{
    int id;
    struct cjto_t *hab;
    int paciencia;
    int veloc;
    int xp;
    int base;
};

struct base_t{
    int id;
    int lotação; //ask
    struct cjto_t *pres;
    struct fila_t *espera;
    struct coord_t *local;
};

struct mission_t{
    int id;
    struct cjto_t *hab;
    int perigo; //ask
    struct coord_t *local;
};

struct mundo_t{
    int nHerois;
    int Herois[N_HEROIS];
    int nBases;
    int Bases[N_BASES];
    int nMissoes;
    int Missoes[N_MISSOES];
    int nHabilidades;
    struct coord_t *tam_mundo;
    int relogio;
};

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
    struct cjto_t *hab_heroi = cjto_cria(habilidades);
    
    heroi->id = i; //id = número sequencial [0...N_HEROIS-1]
    heroi->xp = 0; //experiência = 0
    heroi->paciencia = rand() % (101); //paciência = número aleatório [0...100]
    heroi->veloc = rand () % (5000 - 50 + 1) + 50; //velocidade  = número aleatório [50...5000] (em metros/minuto = 3 Km/h a 300 Km/h)
    habi = cjto_cria(rand()% (3 - 1 + 1) + 1);
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
        base->lotação = rand() % (10 - 3 + 1) + 1; //lotação = número aleatório [3...10]
        heros = cjto_cria(base->lotação);
        base->pres = heros; // presentes = conjunto vazio (com capacidade para armazenar IDs de heróis até a lotação da base)
        esp = fila_cria(base->lotação);
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
        habi_rq = cjto_cria(rand() % (10 - 6 + 1) + 1);
        int j;
        for (j=0; j<habi_rq; j++){
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

// programa principal
int main ()
{
    struct mundo_t *W = malloc(sizeof(struct mundo_t)); //iniciar as entidades e atributos do mundo
    inicia_mundo(W);
    inicia_herois(W);
    inicia_bases(W);
    inicia_missoes(W);

    struct fprio_t *LEF;
    LEF = fprio_cria(); // criar a lista de eventos futuros

/*
criar os eventos iniciais
  
relógio = 0
repetir
    ev = 1º evento da lista de eventos futuros
    relógio = tempo (ev)
    caso tipo (ev) seja:
        EV1: executa evento 1
        EV2: executa evento 2
        EV3: executa evento 3
        ...
    fim
até o fim da simulação
  
apresentar resultados*/

  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}
