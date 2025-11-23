#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stddef.h>

// Funções de leitura de strings
char* ler_string(const char *prompt, size_t max_size);
char* ler_linha(size_t max_size);
void limpar_buffer_entrada();

// Funções de alocação dinâmica de buffer
char* alocar_buffer(size_t tamanho);
char* realocar_buffer(char *buffer, size_t novo_tamanho);
void liberar_buffer(char *buffer);

// Funções de manipulação de strings
char* trim_string(char *str);
char* copiar_string(const char *src);
int string_vazia(const char *str);

#endif
