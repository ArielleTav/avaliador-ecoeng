#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "menu.h"
#include "avaliadores.h"
#include "artigos.h"
#include "avaliacoes.h"
#include "utils.h"
#include "persistencia.h"

void listarArtigosPendentes();
void relatorioCompletoArtigos();
void listarArtigosAprovados();

void menuAvaliador(Avaliador *logado) {
    int opc;
    do {
        printf("\n|  MENU AVALIADOR (%s)  |\n", logado->nomeAvaliador);
        printf("1) LISTAR ARTIGOS RESUMIDOS\n");
        printf("2) INICIAR AVALIAÇÃO DE ARTIGO\n");
        printf("3) CONSULTAR NOTAS (MINHAS / POR ARTIGO)\n");
        printf("0) SAIR\n");
        printf("ESCOLHA: ");
        
        if (scanf("%d",&opc) != 1) { 
            limparBuffer(); 
            printf("Entrada inválida.\n"); 
            continue; 
        }
        limparBuffer();
        
        switch (opc) {
            case 1: listarArtigos(); break;
            case 2: registrarAvaliacao(logado); break;
            case 3: consultarNotas(logado); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opc != 0);
}

void moduloAvaliador() {
    int op;
    printf("\n| MÓDULO AVALIADOR |\n");
    printf("1) LOGIN (ID OU CPF)\n");
    printf("2) CADASTRAR AVALIADOR\n");
    printf("3) LISTAR AVALIADORES\n");
    printf("0) VOLTAR\n");
    printf("ESCOLHA: ");  

    if (scanf("%d",&op) != 1) { 
        limparBuffer(); 
        printf("Entrada inválida.\n"); 
        return; 
    }
    limparBuffer();
    
    if (op == 1) {
        Avaliador *logado = loginAvaliadorFlexivel();
        if (logado) menuAvaliador(logado);
    } else if (op == 2) {
        cadastrarAvaliador();
    } else if (op == 3) {
        listarAvaliadores();
    } else if (op == 0) {
        return;
    } else {
        printf("Opção inválida.\n");
    }
}

void moduloArtigo() {
    int op;
    printf("\n|  MÓDULO ARTIGO  |\n");
    printf("1) CADASTRAR ARTIGO\n");
    printf("2) CONSULTAR ARTIGO (DETALHADO)\n");
    printf("3) LISTAR ARTIGOS (RESUMIDO)\n");
    printf("4) BUSCAR POR TÍTULO\n");        
    printf("5) BUSCAR POR ÁREA\n");          
    printf("0) VOLTAR\nESCOLHA: ");

    if (scanf("%d",&op) != 1) { 
        limparBuffer(); 
        printf("Entrada inválida.\n"); 
        return; 
    }
    limparBuffer();
    
    switch (op) {
        case 1: cadastrarArtigo(); break;
        case 2: consultarArtigoDetalhado(); break;
        case 3: listarArtigos(); break;
        case 4: buscarArtigosPorTitulo(); break;  
        case 5: buscarArtigosPorArea(); break;   
        case 0: return;
        default: printf("Opção inválida.\n");
    }
}

void moduloNotas() {
    int op;
        printf("\n|  MÓDULO NOTAS/AVALIAÇÃO  |\n");
        printf("1) CONSULTAR AVALIAÇÕES DE UM ARTIGO (POR ID)\n");
        printf("2) LISTAR ARTIGOS APROVADOS PARA TCC\n");
        printf("3) LISTAR ARTIGOS PENDENTES DE AVALIAÇÃO\n");
        printf("4) RELATÓRIO COMPLETO DE ARTIGOS\n");
        printf("5) RELATÓRIO ESTATÍSTICO COMPLETO\n");
        printf("6) EXPORTAR RELATÓRIO (CSV)\n");
        printf("0) VOLTAR\n");
        printf("ESCOLHA: ");
            
    if (scanf("%d",&op) != 1) { 
        limparBuffer(); 
        printf("Entrada inválida.\n"); 
        return; 
    }
    limparBuffer();
    
    switch (op) {
        case 1: 
            consultarArtigoDetalhado(); 
            break;
        case 2: 
            listarArtigosAprovados(); 
            break;
        case 3:
            listarArtigosPendentes();
            break;
        case 4:
            relatorioCompletoArtigos();
            break;
        case 5: relatorioEstatisticoCompleto(); 
            break; 
        case 6: exportarRelatorioCSV(); 
            break;
        case 0: 
            return;
        default: 
            printf("Opção inválida.\n");
    }
}

// Nova função para listar artigos pendentes
void listarArtigosPendentes() {
    printf("\n|  ARTIGOS PENDENTES DE AVALIAÇÃO  |\n");
    int found = 0;
    for (int i = 0; i < countArtigos; i++) {
        if (artigos[i].avaliacoesCount < 3) {
            printf("ID: %d | Título: %s | Avaliações: %d/3 | Faltam: %d\n",
                   artigos[i].idArtigo, artigos[i].tituloArtigo,
                   artigos[i].avaliacoesCount, 3 - artigos[i].avaliacoesCount);
            found = 1;
        }
    }
    if (!found) printf("Todos os artigos estão completamente avaliados.\n");
}

// Nova função para relatório completo
void relatorioCompletoArtigos() {
    printf("\n--- Relatório Completo de Artigos ---\n");
    for (int i = 0; i < countArtigos; i++) {
        Artigo *a = &artigos[i];
        printf("\nID: %d\n", a->idArtigo);
        printf("Título: %s\n", a->tituloArtigo);
        printf("Autor: %s | Curso: %s\n", a->autorArtigo, a->cursoAutor);
        printf("Área: %s\n", a->areaDescricao);
        printf("Defesa: Sala %s | %s | %s\n", a->sala, a->dataDefesa, a->horarioDefesa);
        printf("Status: %s | Média Final: %.2f\n", a->status, a->mediaFinal);
        printf("Avaliações: %d/3\n", a->avaliacoesCount);
        
        for (int j = 0; j < a->avaliacoesCount; j++) {
            Avaliacao *av = &a->avaliacoes[j];
            Avaliador *aval = encontrarAvaliadorPorId(av->idAvaliador);
            printf("  Avaliador: %s | Escrita: %.2f | Defesa: %.2f | Média: %.2f\n",
                   aval ? aval->nomeAvaliador : "N/A",
                   av->mediaNotaEscrita, av->mediaNotaDefesa, av->mediaAvaliador);
        }
        printf("---\n");
    }
}

void moduloGerenciamento() {
    int op;
    printf("\n---| MODIFICAR OU EXCLUIR CADASTRO |---\n");
    
    printf("1) EDITAR ARTIGO\n");
    printf("2) EXCLUIR ARTIGO\n");
    printf("3) EDITAR AVALIADOR\n");
    printf("4) EXCLUIR AVALIADOR\n");
    printf("0) VOLTAR\n");
    
    printf("\nESCOLHA UMA OPÇÃO: \n");
    
     if (scanf("%d", &op) != 1) { 
        limparBuffer(); 
        printf("Entrada inválida.\n"); 
        return; 
    }

    limparBuffer();
    
    switch (op) {
        case 1: editarArtigo(); break;
        case 2: excluirArtigo(); break;
        case 3: editarAvaliador(); break;
        case 4: excluirAvaliador(); break;
        case 0: return;
        default: printf("Opção inválida.\n");
    }
}
