#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <stddef.h>

// Estrutura para gerenciamento de buffer dinâmico
typedef struct {
    char *dados;
    size_t tamanho;
    size_t capacidade;
} BufferDinamico;

// Funções de gerenciamento de buffer dinâmico
BufferDinamico* criar_buffer_dinamico(size_t capacidade_inicial);
void liberar_buffer_dinamico(BufferDinamico *buffer);
int adicionar_ao_buffer(BufferDinamico *buffer, const char *dados, size_t tamanho);
int expandir_buffer(BufferDinamico *buffer, size_t nova_capacidade);
void limpar_buffer_dinamico(BufferDinamico *buffer);

// Funções utilitárias de memória
void* alocar_memoria(size_t tamanho);
void* realocar_memoria(void *ptr, size_t novo_tamanho);
void liberar_memoria(void *ptr);
void* alocar_memoria_zerada(size_t num_elementos, size_t tamanho_elemento);

#endif
