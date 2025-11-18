#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "avaliadores.h"
#include "artigos.h"
#include "utils.h"

//CSV = Comma-Separated Values 
void exportarRelatorioCSV() {
    FILE *file = fopen("relatorio_estatistico.csv", "w");
    if (!file) {
        printf("❌ Erro ao criar arquivo de relatório.\n");
        return;
    }
    
    // Cabeçalho do CSV
    fprintf(file, "Categoria,Valor\n");
    
    // Estatísticas básicas
    fprintf(file, "Total Artigos,%d\n", countArtigos);
    fprintf(file, "Total Avaliadores,%d\n", countAvaliadores);
    
    // Contagem por status
    int pendentes = 0, emAvaliacao = 0, avaliados = 0, aprovados = 0, reprovados = 0;
    
    for (int i = 0; i < countArtigos; i++) {
        if (strcmp(artigos[i].status, "Pendente") == 0) pendentes++;
        else if (strcmp(artigos[i].status, "Em Avaliação") == 0) emAvaliacao++;
        else if (strcmp(artigos[i].status, "Avaliado") == 0) avaliados++;
        else if (strcmp(artigos[i].status, "Aprovado para TCC") == 0) aprovados++;
        else if (strcmp(artigos[i].status, "Reprovado") == 0) reprovados++;
    }
    
    fprintf(file, "Artigos Pendentes,%d\n", pendentes);
    fprintf(file, "Artigos Em Avaliacao,%d\n", emAvaliacao);
    fprintf(file, "Artigos Avaliados,%d\n", avaliados);
    fprintf(file, "Artigos Aprovados,%d\n", aprovados);
    fprintf(file, "Artigos Reprovados,%d\n", reprovados);
    
    // Agendamentos
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
    
    fprintf(file, "Salas Ocupados,%d\n", salasOcupadas);
    fprintf(file, "Salas Disponiveis,%d\n", totalSalas - salasOcupadas);
    fprintf(file, "Total Salas,%d\n", totalSalas);
    
    fclose(file);
    printf("✅ Relatório exportado para 'relatorio_estatistico.csv'\n");
}


void salvarAvaliadores() {
    FILE *file = fopen("avaliadores.txt", "w");
    if (!file) {
        printf("^^^^Erro ao salvar avaliadores.\n");
        return;
    }

    for (int i = 0; i < countAvaliadores; i++) {
        fprintf(file, "ID:%d|Nome:%s|CPF:%s|Area:%s|Senha:%s|Avaliacoes:%d\n",
                avaliadores[i].idAvaliador,
                avaliadores[i].nomeAvaliador,
                avaliadores[i].cpf,
                avaliadores[i].areaAtuacao,
                avaliadores[i].senha,
                avaliadores[i].artigosAvaliadosCount);
        
        // Salvar artigos avaliados
        if (avaliadores[i].artigosAvaliadosCount > 0) {
            fprintf(file, "ArtigosAvaliados:");
            for (int j = 0; j < avaliadores[i].artigosAvaliadosCount; j++) {
                fprintf(file, "%d", avaliadores[i].artigosAvaliados[j]);
                if (j < avaliadores[i].artigosAvaliadosCount - 1) {
                    fprintf(file, ",");
                }
            }
            fprintf(file, "\n");
        }
        fprintf(file, "---\n");
    }

    fclose(file);
}

void carregarAvaliadores() {
    FILE *file = fopen("avaliadores.txt", "r");
    if (!file) {
        printf("Arquivo de avaliadores não encontrado. Iniciando com dados vazios.\n");
        return;
    }

    char linha[512];
    Avaliador temp;

    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\n")] = 0;

        if (strncmp(linha, "ID:", 3) == 0) {
            // Novo avaliador
            memset(&temp, 0, sizeof(Avaliador));
            sscanf(linha, "ID:%d|Nome:%[^|]|CPF:%[^|]|Area:%[^|]|Senha:%[^|]|Avaliacoes:%d",
                &temp.idAvaliador, temp.nomeAvaliador, temp.cpf, temp.areaAtuacao, 
                temp.senha, &temp.artigosAvaliadosCount);
            
            temp.artigosAvaliados = NULL;
            temp.artigosAvaliadosCount = 0;
            temp.artigosAvaliadosCapacity = 0;

        } else if (strncmp(linha, "ArtigosAvaliados:", 17) == 0) {
            // Carregar artigos avaliados
            char *token = strtok(linha + 17, ",");
            while (token) {
                garantirCapacidadeArtigosAvaliadosParaAvaliador(&temp);
                temp.artigosAvaliados[temp.artigosAvaliadosCount++] = atoi(token);
                token = strtok(NULL, ",");
            }

        } else if (strcmp(linha, "---") == 0) {
            // Final do avaliador - adicionar ao vetor
            garantirCapacidadeAvaliadores();
            avaliadores[countAvaliadores++] = temp;
        }
    }

    fclose(file);
    printf("Carregados %d avaliadores.\n", countAvaliadores);
}

void salvarArtigos() {
    FILE *file = fopen("artigos.txt", "w");
    if (!file) {
        printf("Erro ao salvar artigos.\n");
        return;
    }

    for (int i = 0; i < countArtigos; i++) {
        fprintf(file, "ID:%d|Titulo:%s|Autor:%s|Curso:%s|Area:%s|"
                      "Sala:%s|Data:%s|Horario:%s|MediaFinal:%.2f|Status:%s|Avaliacoes:%d\n",
                artigos[i].idArtigo,
                artigos[i].tituloArtigo,
                artigos[i].autorArtigo,
                artigos[i].cursoAutor,
                artigos[i].areaDescricao,
                artigos[i].sala,
                artigos[i].dataDefesa,
                artigos[i].horarioDefesa,
                artigos[i].mediaFinal,
                artigos[i].status,
                artigos[i].avaliacoesCount);
        fprintf(file, "---\n");
    }

    fclose(file);
}

void carregarArtigos() {
    FILE *file = fopen("artigos.txt", "r");
    if (!file) {
        printf("Arquivo de artigos não encontrado. Iniciando com dados vazios.\n");
        return;
    }

    char linha[512];
    Artigo temp;

    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\n")] = 0;

        if (strncmp(linha, "ID:", 3) == 0) {
            // Novo artigo
            memset(&temp, 0, sizeof(Artigo));
            int avaliacoesCount;
            sscanf(linha, "ID:%d|Titulo:%[^|]|Autor:%[^|]|Curso:%[^|]|Area:%[^|]|"
                          "Sala:%[^|]|Data:%[^|]|Horario:%[^|]|MediaFinal:%f|Status:%[^|]|Avaliacoes:%d",
                   &temp.idArtigo, temp.tituloArtigo, temp.autorArtigo, 
                   temp.cursoAutor, temp.areaDescricao, temp.sala, temp.dataDefesa,
                   temp.horarioDefesa, &temp.mediaFinal, temp.status, &avaliacoesCount);
            
            temp.avaliacoes = NULL;
            temp.avaliacoesCount = 0;
            temp.avaliacoesCapacity = 0;

        } else if (strcmp(linha, "---") == 0) {
            // Final do artigo - adicionar ao vetor
            garantirCapacidadeArtigos();
            artigos[countArtigos++] = temp;
        }
    }

    fclose(file);
    printf("Carregados %d artigos.\n", countArtigos);
}

void salvarAvaliacoes() {
    FILE *file = fopen("avaliacoes.txt", "w");
    if (!file) {
        printf("Erro ao salvar avaliações.\n");
        return;
    }

    for (int i = 0; i < countArtigos; i++) {
        for (int j = 0; j < artigos[i].avaliacoesCount; j++) {
            Avaliacao *av = &artigos[i].avaliacoes[j];
            fprintf(file, "AvaliacaoID:%d|ArtigoID:%d|AvaliadorID:%d|"
                          "NotaEscrita:%.2f|NotaDefesa:%.2f|Media:%.2f|Comentario:%s\n",
                    av->idAvaliacao, av->idArtigo, av->idAvaliador,
                    av->mediaNotaEscrita, av->mediaNotaDefesa, av->mediaAvaliador,
                    av->comentario);
        }
    }

    fclose(file);
}

void carregarAvaliacoes() {
    FILE *file = fopen("avaliacoes.txt", "r");
    if (!file) {
        printf("Arquivo de avaliações não encontrado.\n");
        return;
    }

    char linha[512];
    int avaliacoesCarregadas = 0;

    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\n")] = 0;

        Avaliacao av;
        int artigoID;
        
        sscanf(linha, "AvaliacaoID:%d|ArtigoID:%d|AvaliadorID:%d|"
                      "NotaEscrita:%f|NotaDefesa:%f|Media:%f|Comentario:%[^\n]",
               &av.idAvaliacao, &artigoID, &av.idAvaliador,
               &av.mediaNotaEscrita, &av.mediaNotaDefesa, &av.mediaAvaliador,
               av.comentario);

        // Encontrar o artigo correspondente
        Artigo *artigo = encontrarArtigoPorId(artigoID);
        if (artigo) {
            garantirCapacidadeAvaliacoesParaArtigo(artigo);
            artigo->avaliacoes[artigo->avaliacoesCount++] = av;
            avaliacoesCarregadas++;
            
            // Atualizar status do artigo
            atualizarMediaFinalSeNecessario(artigo);
        }
    }

    fclose(file);
    printf("Carregadas %d avaliações.\n", avaliacoesCarregadas);
}

void salvarAgendamentos() {
    FILE *file = fopen("agendamentos.txt", "w");
    if (!file) {
        printf("Erro ao salvar agendamentos.\n");
        return;
    }

    for (int room = 0; room < N_SALAS; room++) {
        for (int day = 0; day < N_DIAS; day++) {
            for (int slot = 0; slot < N_HORARIOS; slot++) {
                fprintf(file, "Sala:%s|Dia:%s|Horario:%s|Ocupado:%d\n",
                        NOMES_SALA[room], DIA_STRINGS[day], 
                        HORARIO_STRINGS[slot], disponibilidade[room][day][slot]);
            }
        }
    }

    fclose(file);
}

void carregarAgendamentos() {
    FILE *file = fopen("agendamentos.txt", "r");
    if (!file) {
        printf("Arquivo de agendamentos não encontrado. Iniciando com agenda vazia.\n");
        return;
    }

    char linha[256];
    char sala[8], data[32], horario[32];
    int ocupado;

    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\n")] = 0;
        
        sscanf(linha, "Sala:%[^|]|Dia:%[^|]|Horario:%[^|]|Ocupado:%d",
               sala, data, horario, &ocupado);

        // Encontrar índices correspondentes
        for (int room = 0; room < N_SALAS; room++) {
            for (int day = 0; day < N_DIAS; day++) {
                for (int slot = 0; slot < N_HORARIOS; slot++) {
                    if (strcmp(sala, NOMES_SALA[room]) == 0 &&
                        strcmp(data, DIA_STRINGS[day]) == 0 &&
                        strcmp(horario, HORARIO_STRINGS[slot]) == 0) {
                        disponibilidade[room][day][slot] = ocupado;
                    }
                }
            }
        }
    }

    fclose(file);
    printf("Agendamentos carregados.\n");
}

void salvarDados() {
    printf("Salvando dados...\n");
    salvarAvaliadores();
    salvarArtigos();
    salvarAvaliacoes();
    salvarAgendamentos();
    printf("Dados salvos com sucesso!\n");
}

void carregarDados() {
    printf("Carregando dados...\n");
    carregarAvaliadores();
    carregarArtigos();
    carregarAvaliacoes();
    carregarAgendamentos();
    printf("Dados carregados com sucesso!\n");
}