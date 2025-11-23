#ifndef CONTATO_H
#define CONTATO_H

#define MAX_NOME 100
#define MAX_TELEFONE 20
#define MAX_EMAIL 100

typedef struct {
    int id;
    char nome[MAX_NOME];
    char telefone[MAX_TELEFONE];
    char email[MAX_EMAIL];
    int ativo; // 1 = ativo, 0 = excluído (soft delete)
} Contato;

typedef struct {
    Contato *contatos;
    int quantidade;
    int capacidade;
} ListaContatos;

// Funções de gerenciamento da lista
ListaContatos* criar_lista();
void liberar_lista(ListaContatos *lista);
int adicionar_contato(ListaContatos *lista, const char *nome, const char *telefone, const char *email);
int editar_contato(ListaContatos *lista, int id, const char *nome, const char *telefone, const char *email);
int excluir_contato(ListaContatos *lista, int id);
Contato* buscar_contato_por_id(ListaContatos *lista, int id);
void listar_contatos(ListaContatos *lista);
void buscar_contatos(ListaContatos *lista, const char *termo);

// Funções de persistência
int salvar_contatos(ListaContatos *lista, const char *arquivo);
ListaContatos* carregar_contatos(const char *arquivo);

// Funções de exportação
int exportar_csv(ListaContatos *lista, const char *arquivo);

// Funções de teste e análise
void gerar_contatos_teste(ListaContatos *lista, int quantidade);
void analisar_memoria(ListaContatos *lista);

#endif
