#include "conjunto.h"
#include "fila.h"

#define t_inicio 0
#define t_fim_do_mundo 525600
#define n_tamanho_mundo 20000
#define n_habilidades 10
#define n_herois n_habilidades * 5
#define n_bases n_herois / 5
#define n_missoes t_fim_do_mundo / 100

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
};

struct base_t
{
    int id;
    int lotação; //ask
    struct cjto_t *pres;
    struct fila_t *espera;
    struct coord_t *local;
};

struct mission_t
{
    int id;
    struct cjto_t *hab;
    int perigo; //ask
    struct coord_t *local;
};

struct ev_t{
    int tempo;
    struct heroi_t;
    struct base_t;
};

struct mundo_t
{
    int nHerois;
    int Herois[n_herois];
    int nBases;
    int Bases[n_bases];
    int nMissoes;
    int Missoes[n_missoes];
    int nHabilidades;
    struct coord_t *tam_mundo;
    int relogio;
};
