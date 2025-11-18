#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <signal.h>
#include "estruturas.h"
#include "utils.h"
#include "menu.h"
#include "avaliadores.h"
#include "artigos.h"
#include "persistencia.h"
#include <windows.h>

Avaliador *avaliadores = NULL;
int countAvaliadores = 0;
int capAvaliadores = 0;

Artigo *artigos = NULL;
int countArtigos = 0;
int capArtigos = 0;

int disponibilidade[N_SALAS][N_DIAS][N_HORARIOS] = {0};

// Declarações das funções
void liberarMemoria();
void salvarDados();
void mostrarEstatisticas();

/* Função para lidar com encerramento gracioso */
void handle_sigint() {
    printf("\n\nRecebido sinal de interrupção. Salvando dados...\n");
    salvarDados();
    liberarMemoria();
    exit(0);
}

void liberarMemoria() {
    liberarMemoriaAvaliadores();
    liberarMemoriaArtigos();
}

void mostrarEstatisticas() {
    printf("\n~~~~ ESTATÍSTICAS DO SISTEMA ~~~~\n");
    printf("Avaliadores cadastrados: %d\n", countAvaliadores);
    printf("Artigos submetidos: %d\n", countArtigos);
    
    int artigosAvaliados = 0;
    int artigosAprovados = 0;
    
    for (int i = 0; i < countArtigos; i++) {
        if (artigos[i].avaliacoesCount == 3) {
            artigosAvaliados++;
            if (strcmp(artigos[i].status, "Aprovado para TCC") == 0) {
                artigosAprovados++;
            }
        }
    }
    
    printf("Artigos completamente avaliados: %d\n", artigosAvaliados);
    printf("Artigos aprovados para TCC: %d\n", artigosAprovados);
    
    // Estatísticas de disponibilidade
    int salasOcupadas = 0;
    int totalSalas = N_SALAS * N_DIAS * N_HORARIOS;
    
    for (int room = 0; room < N_SALAS; room++) {
        for (int day = 0; day < N_DIAS; day++) {
            for (int slot = 0; slot < N_HORARIOS; slot++) {
                if (disponibilidade[room][day][slot]) {
                    salasOcupadas++;
                }
            }
        }
    }
    
    printf("Salas de defesa ocupados: %d/%d\n", salasOcupadas, totalSalas);
    printf("Salas disponíveis: %d\n", totalSalas - salasOcupadas);
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_UTF8);
    
    // Configurar handler para Ctrl+C
    signal(SIGINT, handle_sigint);

    iniciarAleatorio();
    carregarDados();
    
    printf("/-/-/ Bem-vindo ao Sistema de Avaliação EncoEng! \\-\\-\\\n");
    printf("  Sistema de Avaliação para Artigos Acadêmicos  \n");
    
    int opc;
    do {
        printf("\n--- | MENU PRINCIPAL | ---\n");
        printf("\nESCOLHA UMA OPÇÃO\n");
        printf("1) MÓDULO AVALIADOR (Login / Cadastro)\n");
        printf("2) MÓDULO ARTIGO (Cadastrar / Consultar)\n");
        printf("3) MÓDULO NOTAS/AVALIAÇÃO (Relatórios)\n");
        printf("4) ESTATÍSTICAS DO SISTEMA\n");
        printf("5) SALVAR DADOS\n");
        printf("6) MODIFICAR OU EXCLUIR CADASTRO\n");
        printf("0) SAIR\n");
        
        
        if (scanf("%d",&opc) != 1) { 
            limparBuffer(); 
            printf("Entrada inválida.\n"); 
            continue; 
        }
        limparBuffer();
        
        switch (opc) {
            case 1: moduloAvaliador(); break;
            case 2: moduloArtigo(); break;
            case 3: moduloNotas(); break;
            case 4: mostrarEstatisticas(); break;
            case 5: salvarDados(); break;
            case 6: moduloGerenciamento(); break;
            case 0: 
                printf("Tem certeza que deseja sair? (s/n): ");
                char confirmacao = getchar();
                limparBuffer();
                if (confirmacao == 's' || confirmacao == 'S') {
                    printf("Encerrando sistema...\n");
                    salvarDados();
                    opc = 0;
                } else {
                    opc = -1;
                }
                break;
                
        }
    } while (opc != 0);

    liberarMemoria();
    printf("Sistema encerrado. Dados salvos.\n");
    return 0;
}