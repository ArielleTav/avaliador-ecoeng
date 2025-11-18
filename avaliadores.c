#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avaliadores.h"
#include "utils.h"

void garantirCapacidadeAvaliadores() {
    if (capAvaliadores == 0) {
        capAvaliadores = INITIAL_CAPACITY;
        avaliadores = (Avaliador*) malloc(capAvaliadores * sizeof(Avaliador));
    } else if (countAvaliadores >= capAvaliadores) {
        capAvaliadores *= 2;
        avaliadores = (Avaliador*) realloc(avaliadores, capAvaliadores * sizeof(Avaliador));
    }
}

void garantirCapacidadeArtigosAvaliadosParaAvaliador(Avaliador *av) {
    if (av->artigosAvaliadosCapacity == 0) {
        av->artigosAvaliadosCapacity = INITIAL_CAPACITY;
        av->artigosAvaliados = (int*) malloc(av->artigosAvaliadosCapacity * sizeof(int));
    } else if (av->artigosAvaliadosCount >= av->artigosAvaliadosCapacity) {
        av->artigosAvaliadosCapacity *= 2;
        av->artigosAvaliados = (int*) realloc(av->artigosAvaliados, av->artigosAvaliadosCapacity * sizeof(int));
    }
}

void cadastrarAvaliador() {
    garantirCapacidadeAvaliadores();
    Avaliador novo;
    novo.idAvaliador = geraIdUnico(1);
    
    printf("\n--- Cadastrar Avaliador ---\n");
    printf("ID gerado automaticamente: %d\n", novo.idAvaliador);
    
    lerNome(novo.nomeAvaliador, sizeof(novo.nomeAvaliador), "Nome completo: ");
    
    //lerCPF(novo.cpf);  // Validação de CPF
    
    lerNome(novo.areaAtuacao, sizeof(novo.areaAtuacao), "Área de atuação: ");
    
      //  VALIDAÇÃO DE CPF DUPLICADO 
    char cpfTemp[12];
    do {
        lerCPF(cpfTemp);  // Valida formato do CPF
        
        if (existeCPF(cpfTemp)) {
            printf("❌ Este CPF já está cadastrado no sistema!\n");
            printf("   Por favor, digite um CPF diferente.\n");
        } else {
            strcpy(novo.cpf, cpfTemp);
            break; // CPF válido e único
        }
    } while (1);
    
    do {
        printf("Senha (mínimo 4 caracteres): ");
        fgets(novo.senha, sizeof(novo.senha), stdin);
        novo.senha[strcspn(novo.senha, "\n")] = 0;
        
        if (strlen(novo.senha) < 4) {
            printf("❌ A senha deve ter pelo menos 4 caracteres!\n");
        } else {
            break;
        }
    } while (1);
    
    novo.artigosAvaliados = NULL;
    novo.artigosAvaliadosCount = 0;
    novo.artigosAvaliadosCapacity = 0;

    avaliadores[countAvaliadores++] = novo;
    printf("✅ Avaliador cadastrado com sucesso!\n");
}


void listarAvaliadores() {
    printf("\n--- Lista de Avaliadores (%d) ---\n", countAvaliadores);
     
    for (int i = 0; i < countAvaliadores; i++) {
        char cpfFormatado[15];
       formatarCPF(avaliadores[i].cpf, cpfFormatado);
        
        printf("ID: %d | Nome: %s | CPF: %s | Área: %s | Avaliações: %d\n",
               avaliadores[i].idAvaliador,
               avaliadores[i].nomeAvaliador,
               cpfFormatado,  // CPF formatado
               avaliadores[i].areaAtuacao,
               avaliadores[i].artigosAvaliadosCount);
    }
}

Avaliador* encontrarAvaliadorPorId(int id) {
    for (int i = 0; i < countAvaliadores; i++) 
        if (avaliadores[i].idAvaliador == id) 
            return &avaliadores[i];
    return NULL;
}

Avaliador* encontrarAvaliadorPorCPF(const char *cpf) {
    for (int i = 0; i < countAvaliadores; i++) {
        if (strcmp(avaliadores[i].cpf, cpf) == 0) {
            return &avaliadores[i];
        }
    }
    return NULL;
}

Avaliador* loginAvaliadorFlexivel() {
    printf("\n| LOGIN AVALIADOR |\n");
    printf("Digite seu ID (4 dígitos) ou CPF (11 dígitos): ");
    
    char entrada[50];  // Buffer maior
    if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
        printf("❌ Erro na leitura da entrada.\n");
        return NULL;
    }
    entrada[strcspn(entrada, "\n")] = 0;
    
    // Limpa a entrada - remove tudo que não é dígito
    char entradaLimpa[50] = {0};
    int j = 0;
    for (int i = 0; entrada[i] != '\0' && j < 49; i++) {
        if (isdigit((unsigned char)entrada[i])) {
            entradaLimpa[j++] = entrada[i];
        }
    }
    entradaLimpa[j] = '\0';
    
    // Validação da entrada limpa
    int tamanho = strlen(entradaLimpa);
    if (tamanho == 0) {
        printf("❌ Nenhum número válido encontrado.\n");
        return NULL;
    }
    
    if (tamanho != 4 && tamanho != 11) {
        printf("❌ Entrada deve ter 4 dígitos (ID) ou 11 dígitos (CPF). Você digitou %d dígitos.\n", tamanho);
        return NULL;
    }
    
    Avaliador *av = NULL;
    
    // Busca por ID (4 dígitos)
    if (tamanho == 4) {
        int id = atoi(entradaLimpa);
        av = encontrarAvaliadorPorId(id);
        if (av) {
            printf("✅ Login por ID: %d\n", id);
        } else {
            printf("❌ Nenhum avaliador encontrado com ID: %d\n", id);
        }
    }
    
    // Busca por CPF (11 dígitos)
    if (tamanho == 11) {
        // Valida formato do CPF
        if (!validarCPF(entradaLimpa)) {
            printf("❌ CPF inválido.\n");
            return NULL;
        }
        
        av = encontrarAvaliadorPorCPF(entradaLimpa);
        if (av) {
            char cpfFormatado[15];
            formatarCPF(entradaLimpa, cpfFormatado);
            printf("✅ Login por CPF: %s\n", cpfFormatado);
        } else {
            printf("❌ Nenhum avaliador encontrado com este CPF.\n");
        }
    }
    
    if (!av) {
        return NULL;
    }
    
    // Verificação de senha
    printf("Senha: ");
    char senha[64];
    if (fgets(senha, sizeof(senha), stdin) == NULL) {
        printf("❌ Erro na leitura da senha.\n");
        return NULL;
    }
    senha[strcspn(senha, "\n")] = 0;
    
    if (strcmp(av->senha, senha) != 0) {
        printf("❌ Senha incorreta.\n");
        return NULL;
    }
    
    printf("🎉 Login efetuado: %s\n", av->nomeAvaliador);
    return av;
}

void excluirAvaliador() {
    if (countAvaliadores == 0) {
        printf("❌ Não há avaliadores para excluir.\n");
        return;
    }
    
    printf("\n--- Excluir Avaliador ---\n");
    listarAvaliadores();
    
    printf("\nDigite o ID do avaliador a excluir: ");
    int id;
    if (scanf("%d", &id) != 1) {
        limparBuffer();
        printf("❌ ID inválido!\n");
        return;
    }
    limparBuffer();
    
    Avaliador *av = encontrarAvaliadorPorId(id);
    if (!av) {
        printf("❌ Avaliador não encontrado!\n");
        return;
    }
    
    // Verifica se o avaliador tem artigos avaliados
    if (av->artigosAvaliadosCount > 0) {
        printf("❌ Este avaliador já realizou %d avaliações. Não pode ser excluído.\n", 
               av->artigosAvaliadosCount);
        return;
    }
    
    // Confirmação
    printf("Tem certeza que deseja excluir o avaliador '%s'? (s/n): ", 
           av->nomeAvaliador);
    char confirma = getchar();
    limparBuffer();
    
    if (confirma != 's' && confirma != 'S') {
        printf("Operação cancelada.\n");
        return;
    }
    
    // Encontra o índice do avaliador
    int idx = -1;
    for (int i = 0; i < countAvaliadores; i++) {
        if (avaliadores[i].idAvaliador == id) {
            idx = i;
            break;
        }
    }
    
    if (idx == -1) {
        printf("❌ Erro interno: avaliador não encontrado no array.\n");
        return;
    }
    
    // Libera memória dos artigos avaliados
    if (avaliadores[idx].artigosAvaliados) {
        free(avaliadores[idx].artigosAvaliados);
    }
    
    // Move avaliadores para frente
    for (int i = idx; i < countAvaliadores - 1; i++) {
        avaliadores[i] = avaliadores[i + 1];
    }
    
    countAvaliadores--;
    printf("✅ Avaliador excluído com sucesso!\n");
}

void editarAvaliador() {
    if (countAvaliadores == 0) {
        printf("❌ Não há avaliadores para editar.\n");
        return;
    }
    
    printf("\n--- Editar Avaliador ---\n");
    listarAvaliadores();
    
    printf("\nDigite o ID do avaliador a editar: ");
    int id;
    if (scanf("%d", &id) != 1) {
        limparBuffer();
        printf("❌ ID inválido!\n");
        return;
    }
    limparBuffer();
    
    Avaliador *av = encontrarAvaliadorPorId(id);
    if (!av) {
        printf("❌ Avaliador não encontrado!\n");
        return;
    }
    
    printf("\nEditando avaliador: %s\n", av->nomeAvaliador);
    printf("Deixe em branco para manter o valor atual.\n");
    
    // Editar nome
    printf("Novo nome [atual: %s]: ", av->nomeAvaliador);
    char novoNome[120];
    fgets(novoNome, sizeof(novoNome), stdin);
    novoNome[strcspn(novoNome, "\n")] = 0;
    if (strlen(novoNome) > 0) {
        strcpy(av->nomeAvaliador, novoNome);
    }
    
    // Editar área
    printf("Nova área [atual: %s]: ", av->areaAtuacao);
    char novaArea[80];
    fgets(novaArea, sizeof(novaArea), stdin);
    novaArea[strcspn(novaArea, "\n")] = 0;
    if (strlen(novaArea) > 0) {
        strcpy(av->areaAtuacao, novaArea);
    }
    
    printf("✅ Avaliador editado com sucesso!\n");

    printf("Nova senha [deixe em branco para manter]: ");
    char novaSenha[64];
    fgets(novaSenha, sizeof(novaSenha), stdin);
    novaSenha[strcspn(novaSenha, "\n")] = 0;
    if (strlen(novaSenha) > 0) {
        strcpy(av->senha, novaSenha);
    }
    
    printf("✅ Avaliador editado com sucesso!\n");

}


void liberarMemoriaAvaliadores() {
    if (avaliadores) {
        for (int i = 0; i < countAvaliadores; i++) {
            if (avaliadores[i].artigosAvaliados) 
                free(avaliadores[i].artigosAvaliados);
        }
        free(avaliadores);
    }
}