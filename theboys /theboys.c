// programa principal do projeto "The Boys - 2024/2"
// Autor: Larissa Schaldach Cutti, GRR 20240590

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "eventos.h"
#include "fila.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100

// programa principal
int main ()
{
  int i;
  for (i=0; i<n_herois; i++)
  {
    struct heroi_t *heroi;
    int habilidades = rand () % (3 - 1 + 1) + 1;
    struct cjto_t *hab_heroi = cjto_cria(habilidades);
    
    heroi->id = 1;
    heroi->xp = 0;
    heroi->paciencia = rand() % (101);
    heroi->veloc = rand () % (5000 - 50 + 1) + 50;
    heroi->hab = 
  }
/*iniciar as entidades e atributos do mundo
criar a fila de eventos futuros
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

