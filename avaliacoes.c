#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avaliacoes.h"
#include "artigos.h"
#include "avaliadores.h"
#include "utils.h"

bool avaliadorJaAvaliou(Artigo *a, int idAvaliador) {
    for (int i = 0; i < a->avaliacoesCount; i++) 
        if (a->avaliacoes[i].idAvaliador == idAvaliador) 
            return true;
    return false;
}

void registrarAvaliacao(Avaliador *logado) {
    printf("\n| REGISTRAR AVALIAÇÃO |\n");
    printf("Digite ID do artigo a avaliar: ");
    int idArt;
    
    if (scanf("%d",&idArt) != 1) { 
        limparBuffer(); 
        printf("Entrada inválida.\n"); 
        return; 
    }
    limparBuffer();
    
    Artigo *a = encontrarArtigoPorId(idArt);
    if (!a) { 
        printf("Artigo não encontrado.\n"); 
        return; 
    }

    if (a->avaliacoesCount >= 3) { 
        printf("Artigo já possui 3 avaliações. BLOQUEADO.\n"); 
        return; 
    }
    
    if (avaliadorJaAvaliou(a, logado->idAvaliador)) { 
        printf("Você já avaliou este artigo.\n"); 
        return; 
    }

    Avaliacao av;
    av.idAvaliacao = geraIdUnico(3);
    av.idArtigo = a->idArtigo;
    av.idAvaliador = logado->idAvaliador;

    av.mediaNotaEscrita = lerNota("INSIRA MÉDIA (0-10) DOS CRITÉRIOS DA PARTE ESCRITA: ");
    av.mediaNotaDefesa = lerNota("INSIRA MÉDIA (0-10) DOS CRITÉRIOS DA DEFESA: ");
  
    
    if (av.mediaNotaEscrita < 0 || av.mediaNotaEscrita > 10 || 
        av.mediaNotaDefesa < 0 || av.mediaNotaDefesa > 10) {
        printf("As notas devem estar entre 0 e 10.\n");
        return;
    }
    
    av.mediaAvaliador = (av.mediaNotaEscrita + av.mediaNotaDefesa) / 2.0f;

    printf("Deseja adicionar comentário (s/n)? ");
    char op = getchar();
    limparBuffer();
    
    if (op == 's' || op == 'S') {
        printf("Comentário (linha): ");
        fgets(av.comentario, sizeof(av.comentario), stdin);
        av.comentario[strcspn(av.comentario, "\n")] = 0;
    } else {
        av.comentario[0] = '\0';
    }

    /* Adiciona avaliação ao artigo */
    garantirCapacidadeAvaliacoesParaArtigo(a);
    a->avaliacoes[a->avaliacoesCount++] = av;

    /* Marca artigo no avaliador */
    garantirCapacidadeArtigosAvaliadosParaAvaliador(logado);
    logado->artigosAvaliados[logado->artigosAvaliadosCount++] = a->idArtigo;

    /* Atualiza status */
    atualizarMediaFinalSeNecessario(a);

    printf("Avaliação registrada: idAvaliacao=%d | mediaAvaliador=%.2f\n", 
           av.idAvaliacao, av.mediaAvaliador);
    
    if (a->avaliacoesCount == 3) {
        printf("Artigo possui 3 avaliações. Média final = %.2f. Status: %s\n", 
               a->mediaFinal, a->status);
    } else {
        printf("Artigo agora tem %d avaliações; faltam %d para fechar.\n", 
               a->avaliacoesCount, 3 - a->avaliacoesCount);
    }
}

void consultarNotas(Avaliador *logado) {
    printf("\n--- Consultar Notas ---\n");
    printf("1) Ver minhas avaliações\n2) Ver avaliações por artigo (ID)\nEscolha: ");
    
    int op;
    if (scanf("%d",&op) != 1) { 
        limparBuffer(); 
        printf("Entrada inválida.\n"); 
        return; 
    }
    limparBuffer();
    
    if (op == 1) {
        if (logado->artigosAvaliadosCount == 0) { 
            printf("Você ainda não avaliou artigos.\n"); 
            return; 
        }
        
        printf("Avaliações feitas por %s:\n", logado->nomeAvaliador);
        for (int i = 0; i < logado->artigosAvaliadosCount; i++) {
            int id = logado->artigosAvaliados[i];
            Artigo *a = encontrarArtigoPorId(id);
            if (!a) continue;
            
            printf(" - Artigo ID %d | %s\n", a->idArtigo, a->tituloArtigo);
            for (int j = 0; j < a->avaliacoesCount; j++) {
                Avaliacao *av = &a->avaliacoes[j];
                if (av->idAvaliador == logado->idAvaliador) {
                    printf("    AvaliaçãoID %d | escrita=%.2f | defesa=%.2f | media=%.2f\n",
                           av->idAvaliacao, av->mediaNotaEscrita, av->mediaNotaDefesa, av->mediaAvaliador);
                    if (strlen(av->comentario) > 0) 
                        printf("      Comentário: %s\n", av->comentario);
                }
            }
        }
    } else if (op == 2) {
        consultarArtigoDetalhado();
    } else {
        printf("Opção inválida.\n");
    }
}
 
void listarArtigosAprovados() {
    printf("\n--- Artigos Aprovados para TCC ---\n");
    int found = 0;
    for (int i = 0; i < countArtigos; i++) {
        if (strcmp(artigos[i].status, "Aprovado para TCC") == 0) {
            printf("ID %d | %s | Média final: %.2f | Sala: %s | %s | %s\n",
                   artigos[i].idArtigo, artigos[i].tituloArtigo, artigos[i].mediaFinal,
                   artigos[i].sala, artigos[i].dataDefesa, artigos[i].horarioDefesa);
            found = 1;
        }
    }
    if (!found) printf("Nenhum artigo aprovado para TCC.\n");
}