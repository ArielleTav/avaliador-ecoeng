#ifndef AVALIADORES_H
#define AVALIADORES_H

#include "estruturas.h"

/* Gerenciamento de memória */
void garantirCapacidadeAvaliadores();
void garantirCapacidadeArtigosAvaliadosParaAvaliador(Avaliador *av);
void liberarMemoriaAvaliadores();

/* CRUD */
void cadastrarAvaliador();
void listarAvaliadores();
Avaliador* encontrarAvaliadorPorId(int id);
void excluirAvaliador();
void editarAvaliador();

/* Login */
Avaliador* loginAvaliador();
Avaliador* encontrarAvaliadorPorCPF(const char *cpf);
Avaliador* loginAvaliadorFlexivel();
#endif