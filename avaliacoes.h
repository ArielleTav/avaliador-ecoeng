#ifndef AVALIACOES_H
#define AVALIACOES_H

#include "estruturas.h"

/* Avaliações */
bool avaliadorJaAvaliou(Artigo *a, int idAvaliador);
void registrarAvaliacao(Avaliador *logado);
void consultarNotas(Avaliador *logado);

/* Relatórios */
void listarArtigosAprovados();

#endif