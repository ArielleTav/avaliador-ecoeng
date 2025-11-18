#ifndef ARTIGOS_H
#define ARTIGOS_H

#include "estruturas.h"

/* Gerenciamento de memória */
void garantirCapacidadeArtigos();
void garantirCapacidadeAvaliacoesParaArtigo(Artigo *a);
void liberarMemoriaArtigos();

/* CRUD */
void cadastrarArtigo();
void listarArtigos();
void consultarArtigoDetalhado();
void excluirArtigo();
void editarArtigo();

/* Busca recursiva (requisito do trabalho) */
int buscarArtigoRecursivo(int idx, int idProcurado);
Artigo* encontrarArtigoPorId(int id);

void buscarArtigosPorTitulo();
void buscarArtigosPorArea();

/* Agendamento */
bool agendarDefesa(char *salaStr, char *dataStr, char *horarioStr);
int* preSelecionaAvaliadores(int *nSelecionados);

/* Status e médias */
void atualizarMediaFinalSeNecessario(Artigo *a);

void listarArtigosPendentes();
void relatorioCompletoArtigos();

void relatorioEstatisticoCompleto();

#endif