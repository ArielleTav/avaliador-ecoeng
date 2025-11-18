#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artigos.h"
#include "avaliadores.h"
#include "utils.h"

const char *NOMES_SALA[] = {"C1","C2","C3"};
const char *HORARIO_STRINGS[] = {"19:00-20:00","20:30-21:30"};
const char *DIA_STRINGS[] = {"Dia 18","Dia 19"};

void garantirCapacidadeArtigos() {
    if (capArtigos == 0) {
        capArtigos = INITIAL_CAPACITY;
        artigos = (Artigo*) malloc(capArtigos * sizeof(Artigo));
    } else if (countArtigos >= capArtigos) {
        capArtigos *= 2;
        artigos = (Artigo*) realloc(artigos, capArtigos * sizeof(Artigo));
    }
}

void garantirCapacidadeAvaliacoesParaArtigo(Artigo *a) {
    if (a->avaliacoesCapacity == 0) {
        a->avaliacoesCapacity = 3;
        a->avaliacoes = (Avaliacao*) malloc(a->avaliacoesCapacity * sizeof(Avaliacao));
    } else if (a->avaliacoesCount >= a->avaliacoesCapacity) {
        a->avaliacoesCapacity *= 2;
        a->avaliacoes = (Avaliacao*) realloc(a->avaliacoes, a->avaliacoesCapacity * sizeof(Avaliacao));
    }
}

/* FUNÇÃO RECURSIVA - Requisito do trabalho */
int buscarArtigoRecursivo(int idx, int idProcurado) {
    if (idx >= countArtigos) return -1;
    if (artigos[idx].idArtigo == idProcurado) return idx;
    return buscarArtigoRecursivo(idx + 1, idProcurado);
}

Artigo* encontrarArtigoPorId(int id) {
    int idx = buscarArtigoRecursivo(0, id);
    if (idx == -1) return NULL;
    return &artigos[idx];
}


bool agendarDefesa(char *salaStr, char *dataStr, char *horarioStr) {
    for (int day = 0; day < N_DIAS; day++) {
        for (int slot = 0; slot < N_HORARIOS; slot++) {
            for (int room = 0; room < N_SALAS; room++) {
                if (!disponibilidade[room][day][slot]) {
                    disponibilidade[room][day][slot] = 1;
                    strcpy(salaStr, NOMES_SALA[room]);
                    strcpy(dataStr, DIA_STRINGS[day]);
                    strcpy(horarioStr, HORARIO_STRINGS[slot]);
                    return true;
                }
            }
        }
    }
    return false;
}

int* preSelecionaAvaliadores(int *nSelecionados) {
    *nSelecionados = 0;
    if (countAvaliadores == 0) return NULL;
    
    int need = 3;
    if (countAvaliadores < 3) need = countAvaliadores;

    int *selecionados = (int*) malloc(need * sizeof(int));
    int selectedCount = 0;
    
    while (selectedCount < need) {
        int idx = rand() % countAvaliadores;
        int id = avaliadores[idx].idAvaliador;
        bool ja = false;
        
        for (int i = 0; i < selectedCount; i++) 
            if (selecionados[i] == id) { 
                ja = true; 
                break; 
            }
        
        if (!ja) selecionados[selectedCount++] = id;
    }
    
    *nSelecionados = selectedCount;
    return selecionados;
}

void excluirArtigo() {
    if (countArtigos == 0) {
        printf("❌ Não há artigos para excluir.\n");
        return;
    }
    
    printf("\n--- Excluir Artigo ---\n");
    listarArtigos();
    
    printf("\nDigite o ID do artigo a excluir: ");
    int id;
    if (scanf("%d", &id) != 1) {
        limparBuffer();
        printf("❌ ID inválido!\n");
        return;
    }
    limparBuffer();
    
    int idx = buscarArtigoRecursivo(0, id);
    if (idx == -1) {
        printf("❌ Artigo não encontrado!\n");
        return;
    }
    
    // Confirmação
    printf("Tem certeza que deseja excluir o artigo '%s'? (s/n): ", 
           artigos[idx].tituloArtigo);
    char confirma = getchar();
    limparBuffer();
    
    if (confirma != 's' && confirma != 'S') {
        printf("Operação cancelada.\n");
        return;
    }
    
    // Libera memória das avaliações
    if (artigos[idx].avaliacoes) {
        free(artigos[idx].avaliacoes);
    }
    
    // Move artigos para frente
    for (int i = idx; i < countArtigos - 1; i++) {
        artigos[i] = artigos[i + 1];
    }
    
    countArtigos--;
    printf("✅ Artigo excluído com sucesso!\n");
}

void cadastrarArtigo() {
    garantirCapacidadeArtigos();
    Artigo novo;
    novo.idArtigo = geraIdUnico(2);
    
    printf("\n| CADASTRAR ARTIGO |\n");
    printf("ID gerado automaticamente: %d\n", novo.idArtigo);


    lerNome(novo.autorArtigo, sizeof(novo.autorArtigo), "Autor (nome principal / equipe): ");
    lerNome(novo.cursoAutor, sizeof(novo.cursoAutor), "Curso do autor: ");
    lerTitulo(novo.tituloArtigo, sizeof(novo.tituloArtigo), "Titulo do artigo: ");
    lerArea(novo.areaDescricao, sizeof(novo.areaDescricao), "Area tematica: ");

    /* Agendamento automático */
    char salaTmp[8] = "";
    char dataTmp[32] = "";
    char horarioTmp[32] = "";
    
    if (agendarDefesa(salaTmp, dataTmp, horarioTmp)) {
        strcpy(novo.sala, salaTmp);
        strcpy(novo.dataDefesa, dataTmp);
        strcpy(novo.horarioDefesa, horarioTmp);
        printf("Defesa agendada: Sala %s | %s | %s\n", novo.sala, novo.dataDefesa, novo.horarioDefesa);
    } else {
        strcpy(novo.sala, "N/A");
        strcpy(novo.dataDefesa, "N/D");
        strcpy(novo.horarioDefesa, "N/D");
        printf("ATENÇÃO: Não há disponibilidade para agendamento de defesa.\n");
    }

    /* Inicializa avaliações */
    novo.avaliacoes = NULL;
    novo.avaliacoesCount = 0;
    novo.avaliacoesCapacity = 0;
    novo.mediaFinal = 0.0f;
    strcpy(novo.status, "Pendente");

    /* Pré-seleção de avaliadores */
    int nSel = 0;
    int *sel = preSelecionaAvaliadores(&nSel);
    
    if (nSel > 0) {
        printf("Pré-seleção de avaliadores para este artigo (%d):\n", nSel);
        for (int i = 0; i < nSel; i++) {
            Avaliador *av = encontrarAvaliadorPorId(sel[i]);
            if (av) printf(" - ID %d | %s | %s\n", av->idAvaliador, av->nomeAvaliador, av->areaAtuacao);
        }
    } else {
        printf("Nenhum avaliador cadastrado para pré-seleção.\n");
    }
    
    if (sel) free(sel);

    artigos[countArtigos++] = novo;
    printf("✅ Artigo cadastrado com sucesso!\n");
}

void buscarArtigosPorTitulo() {
    if (countArtigos == 0) {
        printf("❌ Não há artigos cadastrados.\n");
        return;
    }
    
    printf("\n| Buscar Artigos por Título |\n");
    printf("Digite parte do título para buscar: ");
    char termo[100];
    fgets(termo, sizeof(termo), stdin);
    termo[strcspn(termo, "\n")] = 0;
    
    int encontrados = 0;
    printf("\n📋 Artigos encontrados:\n");
    
    for (int i = 0; i < countArtigos; i++) {
        if (strstr(artigos[i].tituloArtigo, termo) != NULL) {
            printf("ID: %d | Título: %s | Autor: %s | Status: %s\n",
                   artigos[i].idArtigo, artigos[i].tituloArtigo,
                   artigos[i].autorArtigo, artigos[i].status);
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf("❌ Nenhum artigo encontrado com o termo '%s'\n", termo);
    } else {
        printf("✅ Encontrados %d artigo(s)\n", encontrados);
    }
}

void buscarArtigosPorArea() {
    if (countArtigos == 0) {
        printf("❌ Não há artigos cadastrados.\n");
        return;
    }
    
    printf("\n| Buscar Artigos por Área |\n");
    printf("Digite a área temática: ");
    char area[100];
    fgets(area, sizeof(area), stdin);
    area[strcspn(area, "\n")] = 0;
    
    int encontrados = 0;
    printf("\n📋 Artigos da área '%s':\n", area);
    
    for (int i = 0; i < countArtigos; i++) {
        if (strstr(artigos[i].areaDescricao, area) != NULL) {
            printf("ID: %d | Título: %s | Área: %s | Status: %s\n",
                   artigos[i].idArtigo, artigos[i].tituloArtigo,
                   artigos[i].areaDescricao, artigos[i].status);
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf("❌ Nenhum artigo encontrado na área '%s'\n", area);
    } else {
        printf("✅ Encontrados %d artigo(s)\n", encontrados);
    }
}

void relatorioEstatisticoCompleto() {
    printf("\n=== RELATÓRIO ESTATÍSTICO COMPLETO ===\n");
    
    // Estatísticas básicas
    printf("📊 ESTATÍSTICAS GERAIS:\n");
    printf("   • TOTAL DE ARTIGOS: %d\n", countArtigos);
    printf("   • TOTAL DE AVALIADORES: %d\n", countAvaliadores);
    
    // Estatísticas de status
    int pendentes = 0, emAvaliacao = 0, avaliados = 0, aprovados = 0, reprovados = 0;
    
    for (int i = 0; i < countArtigos; i++) {
        if (strcmp(artigos[i].status, "Pendente") == 0) pendentes++;
        else if (strcmp(artigos[i].status, "Em Avaliação") == 0) emAvaliacao++;
        else if (strcmp(artigos[i].status, "Avaliado") == 0) avaliados++;
        else if (strcmp(artigos[i].status, "Aprovado para TCC") == 0) aprovados++;
        else if (strcmp(artigos[i].status, "Reprovado") == 0) reprovados++;
    }
    
    printf("\n📈 STATUS DOS ARTIGOS:\n");
    printf("   • PENDENTES: %d\n", pendentes);
    printf("   • EM AVALIAÇÃO: %d\n", emAvaliacao);
    printf("   • AVALIADOS: %d\n", avaliados);
    printf("   • APROVADOS PARA TCC: %d\n", aprovados);
    printf("   • REPROVADOS: %d\n", reprovados);
    
    // Taxa de aprovação
    float taxaAprovacao = 0;
    if (avaliados > 0) {
        taxaAprovacao = ((float)aprovados / avaliados) * 100;
    }
    printf("   • Taxa de aprovação: %.1f%%\n", taxaAprovacao);
    
    // Médias
    float somaMedias = 0;
    int artigosComMedia = 0;
    
    for (int i = 0; i < countArtigos; i++) {
        if (artigos[i].avaliacoesCount == 3) { // Completamente avaliados
            somaMedias += artigos[i].mediaFinal;
            artigosComMedia++;
        }
    }
    
    printf("\n⭐ MÉDIAS:\n");
    if (artigosComMedia > 0) {
        printf("   • Média geral: %.2f\n", somaMedias / artigosComMedia);
        printf("   • Artigos com avaliação completa: %d\n", artigosComMedia);
    } else {
        printf("   • Nenhum artigo completamente avaliado\n");
    }
    
    // Distribuição por área (simplificado)
    printf("\n DISTRIBUIÇÃO POR ÁREA:\n");
    char areas[50][100];
    int contagemAreas[50] = {0};
    int totalAreas = 0;
    
    for (int i = 0; i < countArtigos; i++) {
        int encontrada = 0;
        for (int j = 0; j < totalAreas; j++) {
            if (strcmp(areas[j], artigos[i].areaDescricao) == 0) {
                contagemAreas[j]++;
                encontrada = 1;
                break;
            }
        }
        if (!encontrada && totalAreas < 50) {
            strcpy(areas[totalAreas], artigos[i].areaDescricao);
            contagemAreas[totalAreas] = 1;
            totalAreas++;
        }
    }
    
    for (int i = 0; i < totalAreas; i++) {
        printf("   • %s: %d artigos\n", areas[i], contagemAreas[i]);
    }
    
    // Estatísticas de agendamento
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
    
    printf("\n🏫 AGENDAMENTOS:\n");
    printf("   • Salas ocupados: %d/%d\n", salasOcupadas, totalSalas);
    printf("   • Salas disponíveis: %d\n", totalSalas - salasOcupadas);
    printf("   • Taxa de ocupação: %.1f%%\n", ((float)salasOcupadas / totalSalas) * 100);
}


void listarArtigos() {
    printf("\n--- Lista de Artigos (%d) ---\n", countArtigos);
    for (int i = 0; i < countArtigos; i++) {
        Artigo *a = &artigos[i];
        printf("ID: %d | Título: %s | Autor: %s | Status: %s | Avaliações: %d | Sala: %s | %s | %s\n",
               a->idArtigo, a->tituloArtigo, a->autorArtigo, a->status,
               a->avaliacoesCount, a->sala, a->dataDefesa, a->horarioDefesa);
    }
}

void consultarArtigoDetalhado() {
    printf("\nDigite ID do artigo: ");
    int id;
    if (scanf("%d",&id) != 1) { 
        limparBuffer(); 
        printf("Entrada inválida.\n"); 
        return; 
    }
    limparBuffer();
    
    Artigo *a = encontrarArtigoPorId(id);
    if (!a) { 
        printf("Artigo não encontrado.\n"); 
        return; 
    }
    
    printf("\n--- Artigo ID %d ---\n", a->idArtigo);
    printf("Título: %s\n", a->tituloArtigo);
    printf("Autor: %s\n", a->autorArtigo);
    printf("Curso do autor: %s\n", a->cursoAutor);
    printf("Área: %s\n", a->areaDescricao);
    printf("Sala: %s | %s | %s\n", a->sala, a->dataDefesa, a->horarioDefesa);
    printf("Status: %s\n", a->status);
    printf("Média final: %.2f\n", a->mediaFinal);
    printf("Avaliações (%d):\n", a->avaliacoesCount);
    
    for (int j = 0; j < a->avaliacoesCount; j++) {
        Avaliacao *av = &a->avaliacoes[j];
        printf("  Avaliação ID %d | Avaliador %d | notaEscrita=%.2f | notaDefesa=%.2f | media=%.2f\n",
               av->idAvaliacao, av->idAvaliador, av->mediaNotaEscrita, av->mediaNotaDefesa, av->mediaAvaliador);
        if (strlen(av->comentario) > 0) 
            printf("   Comentário: %s\n", av->comentario);
    }
}

void atualizarMediaFinalSeNecessario(Artigo *a) {
    if (a->avaliacoesCount < 1) {
        strcpy(a->status, "Pendente");
        a->mediaFinal = 0.0f;
        return;
    } else if (a->avaliacoesCount < 3) {
        strcpy(a->status, "Em Avaliação");
        a->mediaFinal = 0.0f;
        return;
    } else {
        float soma = 0.0f;
        for (int i = 0; i < a->avaliacoesCount; i++) 
            soma += a->avaliacoes[i].mediaAvaliador;
        
        a->mediaFinal = soma / (float) a->avaliacoesCount;
        strcpy(a->status, "Avaliado");
        
        if (a->mediaFinal >= 7.5f) 
            strcpy(a->status, "Aprovado para TCC");
        else 
            strcpy(a->status, "Reprovado");
    }
}

void editarArtigo() {
    if (countArtigos == 0) {
        printf("❌ Não há artigos para editar.\n");
        return;
    }
    
    printf("\n--- Editar Artigo ---\n");
    listarArtigos();
    
    printf("\nDigite o ID do artigo a editar: ");
    int id;
    if (scanf("%d", &id) != 1) {
        limparBuffer();
        printf("❌ ID inválido!\n");
        return;
    }
    limparBuffer();
    
    Artigo *artigo = encontrarArtigoPorId(id);
    if (!artigo) {
        printf("❌ Artigo não encontrado!\n");
        return;
    }
    
    printf("\nEditando artigo: %s\n", artigo->tituloArtigo);
    printf("Deixe em branco para manter o valor atual.\n");
    
    // Editar título
    printf("Novo título [atual: %s]: ", artigo->tituloArtigo);
    char novoTitulo[200];
    fgets(novoTitulo, sizeof(novoTitulo), stdin);
    novoTitulo[strcspn(novoTitulo, "\n")] = 0;
    if (strlen(novoTitulo) > 0) {
        strcpy(artigo->tituloArtigo, novoTitulo);
    }
    
    // Editar autor
    printf("Novo autor [atual: %s]: ", artigo->autorArtigo);
    char novoAutor[120];
    fgets(novoAutor, sizeof(novoAutor), stdin);
    novoAutor[strcspn(novoAutor, "\n")] = 0;
    if (strlen(novoAutor) > 0) {
        strcpy(artigo->autorArtigo, novoAutor);
    }
    
    printf("✅ Artigo editado com sucesso!\n");

    // Editar curso
    printf("Novo curso [atual: %s]: ", artigo->cursoAutor);
    char novoCurso[80];
    fgets(novoCurso, sizeof(novoCurso), stdin);
    novoCurso[strcspn(novoCurso, "\n")] = 0;
    if (strlen(novoCurso) > 0) {
        strcpy(artigo->cursoAutor, novoCurso);
    }

    // Editar área
    printf("Nova área [atual: %s]: ", artigo->areaDescricao);
    char novaArea[100];
    fgets(novaArea, sizeof(novaArea), stdin);
    novaArea[strcspn(novaArea, "\n")] = 0;
    if (strlen(novaArea) > 0) {
        strcpy(artigo->areaDescricao, novaArea);
    }
    
    printf("✅ Artigo editado com sucesso!\n");

}

void liberarMemoriaArtigos() {
    if (artigos) {
        for (int i = 0; i < countArtigos; i++) {
            if (artigos[i].avaliacoes) 
                free(artigos[i].avaliacoes);
        }
        free(artigos);
    }
}