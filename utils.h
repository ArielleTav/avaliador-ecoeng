#ifndef UTILS_H
#define UTILS_H

#include "estruturas.h"

void iniciarAleatorio();
int geraIdUnico(int tipo);
bool existeIdAvaliador(int id);
bool existeIdArtigo(int id);
bool existeIdAvaliacao(int id);
void limparBuffer();

int validarNome(const char *nome);
void lerNome(char *destino, int tamanho, const char *prompt);
int validarTitulo(const char *titulo);
void lerTitulo(char *destino, int tamanho, const char *prompt);
int validarCPF(const char *cpf);
void lerCPF(char *cpf);
void formatarCPF(const char *cpf, char *cpfFormatado);
bool existeCPF(const char *cpf);

float lerNota(const char *prompt);

int validarArea(const char *area);
void lerArea(char *destino, int tamanho, const char *prompt);

#endif