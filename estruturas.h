#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H


#include <stdbool.h>

/* ---------- Configurações / Limites ---------- */
#define MAX_LOGIN 20
#define INITIAL_CAPACITY 8
#define ID_MIN 1000
#define ID_MAX 9999

/* Salas e horários */
#define N_SALAS 3
#define N_DIAS 2
#define N_HORARIOS 2

extern const char *NOMES_SALA[];
extern const char *HORARIO_STRINGS[];
extern const char *DIA_STRINGS[];

/* ---------- Tipos / Estruturas ---------- */

/* Avaliação feita por um avaliador para um artigo */
typedef struct {
    int idAvaliacao;
    int idArtigo;
    int idAvaliador;
    float mediaNotaEscrita;
    float mediaNotaDefesa;
    float mediaAvaliador;
    char comentario[256];
} Avaliacao;

/* Artigo submetido ao evento */
typedef struct {
    int idArtigo;
    char cursoAutor[80];
    char tituloArtigo[200];
    char autorArtigo[120];
    char areaDescricao[100];
    char sala[8];
    char horarioDefesa[32];
    char dataDefesa[32];
    Avaliacao *avaliacoes;
    int avaliacoesCount;
    int avaliacoesCapacity;
    float mediaFinal;
    char status[40];
} Artigo;

/* Avaliador */
typedef struct {
    int idAvaliador;
    char nomeAvaliador[120];
    char areaAtuacao[80];
    char cpf[12];
    char senha[64];
    int *artigosAvaliados;
    int artigosAvaliadosCount;
    int artigosAvaliadosCapacity;
} Avaliador;

/* Bancos de dados em memória (dinâmicos) */
extern Avaliador *avaliadores;
extern int countAvaliadores;
extern int capAvaliadores;

extern Artigo *artigos;
extern int countArtigos;
extern int capArtigos;

extern int disponibilidade[N_SALAS][N_DIAS][N_HORARIOS];

#endif