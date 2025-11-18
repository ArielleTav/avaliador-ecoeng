#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "estruturas.h"
#include <ctype.h>

// Função para validar título
int validarTitulo(const char *titulo) {
    // Verifica se está vazio
    if (strlen(titulo) == 0) {
        printf("❌ O título não pode estar vazio!\n");
        return 0;
    }
    
    // Verifica tamanho mínimo
    if (strlen(titulo) < 5) {
        printf("❌ O título deve ter pelo menos 5 caracteres!\n");
        return 0;
    }
    
    // Verifica se não são apenas espaços
    int apenasEspacos = 1;
    for (int i = 0; titulo[i] != '\0'; i++) {
        if (!isspace(titulo[i])) {
            apenasEspacos = 0;
            break;
        }
    }
    if (apenasEspacos) {
        printf("❌ O título não pode conter apenas espaços!\n");
        return 0;
    }
    
    return 1; // Título válido
}

// Função para ler título com validação
void lerTitulo(char *destino, int tamanho, const char *prompt) {
    do {
        printf("%s", prompt);
        fgets(destino, tamanho, stdin);
        destino[strcspn(destino, "\n")] = 0;
        
        if (!validarTitulo(destino)) {
            continue;
        }
        
        break;
    } while (1);
}


//Validar area
int validarArea(const char *area) {
    // Verifica se está vazio
    if (strlen(area) == 0) {
        printf("❌ O campo não pode estar vazio!\n");
        return 0;
    }
    
    // Verifica tamanho mínimo
    if (strlen(area) < 2) {
        printf("❌ A área deve ter pelo menos 2 caracteres!\n");
        return 0;
    }
    
    // Verifica se não são apenas espaços
    int apenasEspacos = 1;
    for (int i = 0; area[i] != '\0'; i++) {
        if (!isspace(area[i])) {
            apenasEspacos = 0;
            break;
        }
    }
    if (apenasEspacos) {
        printf("❌ A área não pode conter apenas espaços!\n");
        return 0;
    }
    
    // Verifica caracteres válidos (mais flexível - permite números)
    for (int i = 0; area[i] != '\0'; i++) {
        unsigned char c = area[i];
        // Permite: letras, números, espaços, ponto, vírgula, hífen, etc.
        if (!isalnum(c) && c != ' ' && c != '.' && c != ',' && c != '-' && 
            c != '&' && c != '(' && c != ')' && c != '\'' && c != '"' && c != '/') {
            printf("❌ Caractere inválido encontrado: '%c'\n", c);
            printf("   Use apenas letras, números, espaços e os caracteres: . , - & ( ) ' \" /\n");
            return 0;
        }
    }
    
    return 1; // Área válida
}

// Função para ler área com validação
void lerArea(char *destino, int tamanho, const char *prompt) {
    do {
        printf("%s", prompt);
        fgets(destino, tamanho, stdin);
        destino[strcspn(destino, "\n")] = 0; // Remove \n
        
        // Remove espaços extras do início e fim
        char temp[256];
        strcpy(temp, destino);
        
        // Remove espaços do início
        int inicio = 0;
        while (isspace(temp[inicio])) inicio++;
        
        // Remove espaços do fim
        int fim = strlen(temp) - 1;
        while (fim >= 0 && isspace(temp[fim])) fim--;
        
        if (fim < inicio) {
            // String vazia ou só espaços
            printf("❌ O campo não pode estar vazio!\n");
            continue;
        }
        
        // Copia a string limpa
        strncpy(destino, temp + inicio, fim - inicio + 1);
        destino[fim - inicio + 1] = '\0';
        
        if (!validarArea(destino)) {
            continue;
        }
        
        break; // Área válida
    } while (1);
}

// Função para ler uma nota com validação
float lerNota(const char *prompt) {
    float nota;
    do {
        printf("%s", prompt);
        if (scanf("%f", &nota) != 1) {
            limparBuffer();
            printf("❌ Por favor, digite um número válido!\n");
            continue;
        }
        limparBuffer();
        
        if (nota < 0 || nota > 10) {
            printf("❌ A nota deve estar entre 0 e 10!\n");
        } else {
            break;
        }
    } while (1);
    
    return nota;
}


//************ SOBRE CPF ****************
 
#include <ctype.h>
#include <string.h>

// Função para validar CPF
int validarCPF(const char *cpf) {
    int i, j, digito1 = 0, digito2 = 0;
    
    // Verifica se tem 11 caracteres
    if (strlen(cpf) != 11) return 0;
    
    // Verifica se todos são dígitos
    for (i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) return 0;
    }
    
    // Verifica CPFs inválidos conhecidos (todos dígitos iguais)
    int todosIguais = 1;
    for (i = 1; i < 11; i++) {
        if (cpf[i] != cpf[0]) {
            todosIguais = 0;
            break;
        }
    }
    if (todosIguais) return 0;
    
    // Calcula primeiro dígito verificador
    for (i = 0, j = 10; i < 9; i++, j--) {
        digito1 += (cpf[i] - '0') * j;
    }
    digito1 = (digito1 * 10) % 11;
    if (digito1 == 10) digito1 = 0;
    
    // Verifica primeiro dígito
    if (digito1 != (cpf[9] - '0')) return 0;
    
    // Calcula segundo dígito verificador
    for (i = 0, j = 11; i < 10; i++, j--) {
        digito2 += (cpf[i] - '0') * j;
    }
    digito2 = (digito2 * 10) % 11;
    if (digito2 == 10) digito2 = 0;
    
    // Verifica segundo dígito
    if (digito2 != (cpf[10] - '0')) return 0;
    
    return 1; // CPF válido
}

// Função para ler e validar CPF
void lerCPF(char *cpf) {
    char cpfTemp[20];
    
    do {
        printf("CPF (apenas números): ");
        fgets(cpfTemp, sizeof(cpfTemp), stdin);
        cpfTemp[strcspn(cpfTemp, "\n")] = 0; // Remove \n
        
        // Remove pontos e traços se o usuário digitou
        char cpfLimpo[12];
        int j = 0;
        for (int i = 0; cpfTemp[i] != '\0' && j < 11; i++) {
            if (isdigit(cpfTemp[i])) {
                cpfLimpo[j++] = cpfTemp[i];
            }
        }
        cpfLimpo[j] = '\0';
        
        if (!validarCPF(cpfLimpo)) {
            printf("❌ CPF inválido! Digite um CPF válido (11 dígitos).\n");
        } else {
            strcpy(cpf, cpfLimpo);
            break;
        }
    } while (1);
}

// Função para formatar CPF na exibição
void formatarCPF(const char *cpf, char *cpfFormatado) {
    sprintf(cpfFormatado, "%.3s.%.3s.%.3s-%.2s", 
            cpf, cpf + 3, cpf + 6, cpf + 9);
}

// Função para verificar se CPF já existe
bool existeCPF(const char *cpf) {
    for (int i = 0; i < countAvaliadores; i++) {
        if (strcmp(avaliadores[i].cpf, cpf) == 0) {
            return true; // CPF já existe
        }
    }
    return false; // CPF não existe
}


//*********** VALIDAR NOME *******************
int validarNome(const char *nome) {
    // Verifica tamanho mínimo (pelo menos 3 caracteres)
    if (strlen(nome) < 3) {
        printf("❌ O campo deve ter pelo menos 3 caracteres!\n");
        return 0;
    }
    
    // Verifica se não são apenas espaços
    int apenasEspacos = 1;
    for (int i = 0; nome[i] != '\0'; i++) {
        if (!isspace(nome[i])) {
            apenasEspacos = 0;
            break;
        }
    }
    if (apenasEspacos) {
        printf("❌ O campo não pode conter apenas espaços!\n");
        return 0;
    }
    
    // Verifica caracteres válidos
    for (int i = 0; nome[i] != '\0'; i++) {
        // Permite letras, espaços, acentos e alguns caracteres especiais
        if (!isalpha(nome[i]) && nome[i] != ' ' && nome[i] != '.' && 
            nome[i] != ',' && nome[i] != '-' && nome[i] != '&' &&
            nome[i] != '(' && nome[i] != ')') {
            printf("❌ Caractere inválido encontrado: '%c'\n", nome[i]);
            printf("   Use apenas letras, espaços e . , - & ( )\n");
            return 0;
        }
    }
    return 1; // Nome válido
}

void lerNome(char *destino, int tamanho, const char *prompt) {
      do {
        printf("%s", prompt);
        fgets(destino, tamanho, stdin);
        destino[strcspn(destino, "\n")] = 0; // Remove \n
        
        // Remove espaços extras no início e fim
        char temp[256];
        strcpy(temp, destino);
        
        // Remove espaços do início
        int inicio = 0;
        while (isspace(temp[inicio])) inicio++;
        
        // Remove espaços do fim
        int fim = strlen(temp) - 1;
        while (fim >= 0 && isspace(temp[fim])) fim--;
        
        if (fim < inicio) {
            // String vazia ou só espaços
            printf("❌ O campo não pode estar vazio!\n");
            continue;
        }
        
        // Copia a string limpa
        strncpy(destino, temp + inicio, fim - inicio + 1);
        destino[fim - inicio + 1] = '\0';
        
        if (!validarNome(destino)) {
            // A mensagem de erro já foi mostrada pela validarNome
            continue;
        }
        
        break; // Nome válido
    } while (1);
}

//*************************************************************/
void iniciarAleatorio() {
    srand((unsigned int) time(NULL));
}

bool existeIdAvaliador(int id) {
    for (int i = 0; i < countAvaliadores; i++) 
        if (avaliadores[i].idAvaliador == id) 
            return true;
    return false;
}

bool existeIdArtigo(int id) {
    for (int i = 0; i < countArtigos; i++) 
        if (artigos[i].idArtigo == id) 
            return true;
    return false;
}

bool existeIdAvaliacao(int id) {
    for (int i = 0; i < countArtigos; i++) {
        for (int j = 0; j < artigos[i].avaliacoesCount; j++) {
            if (artigos[i].avaliacoes[j].idAvaliacao == id) 
                return true;
        }
    }
    return false;
}

int geraIdUnico(int tipo) {
    int id;
    bool repetido;
    do {
        id = rand() % (ID_MAX - ID_MIN + 1) + ID_MIN;
        repetido = false;
        if (tipo == 1 && existeIdAvaliador(id)) repetido = true;
        else if (tipo == 2 && existeIdArtigo(id)) repetido = true;
        else if (tipo == 3 && existeIdAvaliacao(id)) repetido = true;
    } while (repetido);
    return id;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}