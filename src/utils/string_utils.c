#include "string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Alocar buffer com tamanho específico
char* alocar_buffer(size_t tamanho) {
    if (tamanho == 0) {
        return NULL;
    }
    
    char *buffer = (char*)malloc(tamanho * sizeof(char));
    if (!buffer) {
        fprintf(stderr, "Erro ao alocar memória para buffer\n");
        return NULL;
    }
    
    memset(buffer, 0, tamanho);
    return buffer;
}

// Realocar buffer com novo tamanho
char* realocar_buffer(char *buffer, size_t novo_tamanho) {
    if (novo_tamanho == 0) {
        free(buffer);
        return NULL;
    }
    
    char *novo_buffer = (char*)realloc(buffer, novo_tamanho * sizeof(char));
    if (!novo_buffer) {
        fprintf(stderr, "Erro ao realocar memória para buffer\n");
        return buffer; // Retorna o buffer original em caso de erro
    }
    
    return novo_buffer;
}

// Liberar buffer alocado dinamicamente
void liberar_buffer(char *buffer) {
    if (buffer) {
        free(buffer);
    }
}

// Limpar buffer de entrada (stdin)
void limpar_buffer_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Ler uma linha do stdin
char* ler_linha(size_t max_size) {
    char *buffer = alocar_buffer(max_size);
    if (!buffer) {
        return NULL;
    }
    
    if (fgets(buffer, max_size, stdin) != NULL) {
        // Remover o \n do final, se houver
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        return buffer;
    }
    
    liberar_buffer(buffer);
    return NULL;
}

// Ler string com prompt
char* ler_string(const char *prompt, size_t max_size) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    
    return ler_linha(max_size);
}

// Remover espaços em branco do início e fim da string
char* trim_string(char *str) {
    if (!str) {
        return NULL;
    }
    
    // Remover espaços do início
    char *inicio = str;
    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }
    
    // Se a string é toda espaços
    if (*inicio == '\0') {
        *str = '\0';
        return str;
    }
    
    // Remover espaços do final
    char *fim = inicio + strlen(inicio) - 1;
    while (fim > inicio && isspace((unsigned char)*fim)) {
        fim--;
    }
    *(fim + 1) = '\0';
    
    // Mover conteúdo para o início se necessário
    if (inicio != str) {
        memmove(str, inicio, strlen(inicio) + 1);
    }
    
    return str;
}

// Copiar string alocando memória dinamicamente
char* copiar_string(const char *src) {
    if (!src) {
        return NULL;
    }
    
    size_t tamanho = strlen(src) + 1;
    char *copia = alocar_buffer(tamanho);
    
    if (copia) {
        strncpy(copia, src, tamanho);
        copia[tamanho - 1] = '\0';
    }
    
    return copia;
}

// Verificar se string está vazia
int string_vazia(const char *str) {
    if (!str) {
        return 1;
    }
    
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    
    return 1;
}
