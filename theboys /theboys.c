// programa principal do projeto "The Boys - 2024/2"
// Autor: Larissa Schaldach Cutti, GRR 20240590

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "entidades.h"
#include "eventos.h"
#include "fila.h"

#define t_inicio 0
#define t_fim_do_mundo 525600
#define n_tamanho_mundo 20000
#define n_habilidades 10
#define n_herois n_habilidades * 5
#define n_bases n_herois / 5
#define n_missoes t_fim_do_mundo / 100

// programa principal
int main ()
{
  int i;
  struct heroi_t *heroi;
  for (i=0; i<n_herois; i++)
  {
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

