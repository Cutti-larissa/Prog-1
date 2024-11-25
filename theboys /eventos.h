//verificar erro
#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "fila_espera.h"
#include "fprio.h"

void chega(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF); //tipo 0

void espera(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF); //tipo 1

void desiste(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W); //tipo 2

void avisa(int t, struct base_t *B, struct fprio_t *LEF); //tipo 3

void entra(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF); //tipo 4

void sai(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W); //tipo 5

void viaja(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF, struct mundo_t *W); //tipo 6

void morre(int t, struct heroi_t *H, struct base_t *B, struct fprio_t *LEF); //tipo 7

void missao(int t, struct mission_t *M, struct fprio_t *LEF, struct mundo_t *W); //tipo 8

void fim(int t);  //tipo 9
