#include "conjunto.h"

#ifndef ENTI
#define ENTI 

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100

struct coord_t
{
    int x;
    int y;
};

struct heroi_t
{
    int id;
    struct cjto_t *hab;
    int paciencia;
    int veloc;
    int xp;
    int base;
    int status;
};

struct base_t
{
    int id;
    int max; //ask
    struct cjto_t *pres;
    struct lista_t *espera;
    struct coord_t *local;
};

struct mission_t
{
    int id;
    struct cjto_t *hab;
    int perigo; //ask
    struct coord_t *local;
    int status;
};

struct ev_t{
    struct heroi_t *heroi;
    struct base_t *base;
    struct mission_t *missao;
};

struct mundo_t{
    int nHerois;
    struct heroi_t *Herois[N_HEROIS];
    int nBases;
    struct base_t *Bases[N_BASES];
    int nMissoes;
    struct mission_t *Missoes[N_MISSOES];
    int nHabilidades;
    struct coord_t *tam_mundo;
    int relogio;
};

#endif
