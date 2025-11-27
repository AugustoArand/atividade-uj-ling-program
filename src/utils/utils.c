#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Alocação de buffer com tamanho especificado
char* allocate_buffer(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    char *buffer = (char*)malloc(size * sizeof(char));
    if (!buffer) {
        fprintf(stderr, "Erro ao alocar memória para o buffer\n");
        return NULL;
    }
    
    memset(buffer, 0, size);
    return buffer;
}

// Realocar buffer com novo tamanho
char* reallocate_buffer(char *buffer, size_t new_size) {
    if (new_size == 0) {
        free(buffer);
        return NULL;
    }
    
    char *new_buffer = (char*)realloc(buffer, new_size * sizeof(char));
    if (!new_buffer) {
        fprintf(stderr, "Erro ao realocar memória para o buffer\n");
        return buffer; // Return original buffer on error
    }
    
    return new_buffer;
}

// Liberar buffer alocado dinamicamente
void free_buffer(char *buffer) {
    if (buffer) {
        free(buffer);
    }
}

// Limpar buffer de entrada (stdin)
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Ler uma linha do stdin
char* read_line(size_t max_size) {
    char *buffer = allocate_buffer(max_size);
    if (!buffer) {
        return NULL;
    }
    
    if (fgets(buffer, max_size, stdin) != NULL) {
        // Remove trailing \n if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        return buffer;
    }
    
    free_buffer(buffer);
    return NULL;
}

// Ler string com prompt
char* read_string(const char *prompt, size_t max_size) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    
    return read_line(max_size);
}

// Remover espaços em branco à esquerda e à direita da string
char* trim_string(char *str) {
    if (!str) {
        return NULL;
    }
    
    // Remove leading whitespace
    char *start = str;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    
    // If string is all whitespace
    if (*start == '\0') {
        *str = '\0';
        return str;
    }
    
    // Remove trailing whitespace
    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    
    // Move content to start if needed
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    return str;
}

// Copiar string com alocação dinâmica de memória
char* copy_string(const char *src) {
    if (!src) {
        return NULL;
    }
    
    size_t size = strlen(src) + 1;
    char *copy = allocate_buffer(size);
    
    if (copy) {
        strncpy(copy, src, size);
        copy[size - 1] = '\0';
    }
    
    return copy;
}

// Verificar se a string está vazia
int is_empty_string(const char *str) {
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

// Limpar tela
void clear_screen() {
    #ifdef _WIN32
        int ret = system("cls");
    #else
        int ret = system("clear");
    #endif
    (void)ret; // Suppress warning
}

// Aguardar o usuário pressionar ENTER
void wait_for_enter() {
    printf("\nPressione ENTER para continuar...");
    clear_input_buffer();
}
