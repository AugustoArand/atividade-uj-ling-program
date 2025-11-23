#include "memory_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Alocar memória com verificação de erro
void* alocar_memoria(size_t tamanho) {
    if (tamanho == 0) {
        return NULL;
    }
    
    void *ptr = malloc(tamanho);
    if (!ptr) {
        fprintf(stderr, "Erro crítico: Falha ao alocar %zu bytes de memória\n", tamanho);
        return NULL;
    }
    
    return ptr;
}

// Realocar memória com verificação de erro
void* realocar_memoria(void *ptr, size_t novo_tamanho) {
    if (novo_tamanho == 0) {
        free(ptr);
        return NULL;
    }
    
    void *novo_ptr = realloc(ptr, novo_tamanho);
    if (!novo_ptr) {
        fprintf(stderr, "Erro crítico: Falha ao realocar para %zu bytes de memória\n", novo_tamanho);
        return ptr; // Retorna ponteiro original em caso de erro
    }
    
    return novo_ptr;
}

// Liberar memória
void liberar_memoria(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}

// Alocar memória zerada (calloc wrapper)
void* alocar_memoria_zerada(size_t num_elementos, size_t tamanho_elemento) {
    if (num_elementos == 0 || tamanho_elemento == 0) {
        return NULL;
    }
    
    void *ptr = calloc(num_elementos, tamanho_elemento);
    if (!ptr) {
        fprintf(stderr, "Erro crítico: Falha ao alocar %zu elementos de %zu bytes\n", 
                num_elementos, tamanho_elemento);
        return NULL;
    }
    
    return ptr;
}

// Criar buffer dinâmico
BufferDinamico* criar_buffer_dinamico(size_t capacidade_inicial) {
    if (capacidade_inicial == 0) {
        capacidade_inicial = 256; // Capacidade padrão
    }
    
    BufferDinamico *buffer = (BufferDinamico*)alocar_memoria(sizeof(BufferDinamico));
    if (!buffer) {
        return NULL;
    }
    
    buffer->dados = (char*)alocar_memoria(capacidade_inicial);
    if (!buffer->dados) {
        liberar_memoria(buffer);
        return NULL;
    }
    
    buffer->tamanho = 0;
    buffer->capacidade = capacidade_inicial;
    buffer->dados[0] = '\0';
    
    return buffer;
}

// Liberar buffer dinâmico
void liberar_buffer_dinamico(BufferDinamico *buffer) {
    if (buffer) {
        if (buffer->dados) {
            liberar_memoria(buffer->dados);
        }
        liberar_memoria(buffer);
    }
}

// Expandir capacidade do buffer
int expandir_buffer(BufferDinamico *buffer, size_t nova_capacidade) {
    if (!buffer) {
        return 0;
    }
    
    if (nova_capacidade <= buffer->capacidade) {
        return 1; // Já tem capacidade suficiente
    }
    
    char *novos_dados = (char*)realocar_memoria(buffer->dados, nova_capacidade);
    if (!novos_dados) {
        return 0;
    }
    
    buffer->dados = novos_dados;
    buffer->capacidade = nova_capacidade;
    
    return 1;
}

// Adicionar dados ao buffer
int adicionar_ao_buffer(BufferDinamico *buffer, const char *dados, size_t tamanho) {
    if (!buffer || !dados || tamanho == 0) {
        return 0;
    }
    
    size_t novo_tamanho = buffer->tamanho + tamanho;
    
    // Expandir se necessário
    if (novo_tamanho >= buffer->capacidade) {
        size_t nova_capacidade = buffer->capacidade * 2;
        while (nova_capacidade <= novo_tamanho) {
            nova_capacidade *= 2;
        }
        
        if (!expandir_buffer(buffer, nova_capacidade)) {
            return 0;
        }
    }
    
    memcpy(buffer->dados + buffer->tamanho, dados, tamanho);
    buffer->tamanho = novo_tamanho;
    buffer->dados[buffer->tamanho] = '\0';
    
    return 1;
}

// Limpar conteúdo do buffer
void limpar_buffer_dinamico(BufferDinamico *buffer) {
    if (buffer && buffer->dados) {
        buffer->tamanho = 0;
        buffer->dados[0] = '\0';
    }
}
