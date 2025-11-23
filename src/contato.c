#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contato.h"

#define CAPACIDADE_INICIAL 10
#define ARQUIVO_DADOS "contatos.dat"

// Criar lista vazia
ListaContatos* criar_lista() {
    ListaContatos *lista = (ListaContatos*)malloc(sizeof(ListaContatos));
    if (!lista) {
        fprintf(stderr, "Erro ao alocar memória para a lista\n");
        return NULL;
    }
    // Alocando memória inicial para contatos
    lista->contatos = (Contato*)malloc(CAPACIDADE_INICIAL * sizeof(Contato));
    if (!lista->contatos) {
        fprintf(stderr, "Erro ao alocar memória para contatos\n");
        free(lista);
        return NULL;
    }
    
    lista->quantidade = 0; // Inicia com 0, indicando a lista vazia
    lista->capacidade = CAPACIDADE_INICIAL; // Define a capacidade inicial da lista
    return lista;
}

// Liberar memória da lista
void liberar_lista(ListaContatos *lista) {
    if (lista) {
        if (lista->contatos) {
            free(lista->contatos);
        }
        free(lista);
    }
}

// Expandir capacidade da lista quando necessário
static int expandir_lista(ListaContatos *lista) {
    int nova_capacidade = lista->capacidade * 2;
    Contato *novos_contatos = (Contato*)realloc(lista->contatos, nova_capacidade * sizeof(Contato));
    
    if (!novos_contatos) {
        fprintf(stderr, "Erro ao expandir a lista de contatos\n");
        return 0;
    }
    
    lista->contatos = novos_contatos;
    lista->capacidade = nova_capacidade;
    return 1;
}

// Gerar próximo ID disponível
static int gerar_id(ListaContatos *lista) {
    int max_id = 0;
    for (int i = 0; i < lista->quantidade; i++) {
        if (lista->contatos[i].id > max_id) {
            max_id = lista->contatos[i].id;
        }
    }
    return max_id + 1;
}

// Adicionar novo contato
int adicionar_contato(ListaContatos *lista, const char *nome, const char *telefone, const char *email) {
    if (!lista || !nome || !telefone || !email) {
        return -1;
    }
    
    if (lista->quantidade >= lista->capacidade) {
        if (!expandir_lista(lista)) {
            return -1;
        }
    }
    
    Contato *novo = &lista->contatos[lista->quantidade];
    novo->id = gerar_id(lista);
    strncpy(novo->nome, nome, MAX_NOME - 1);
    novo->nome[MAX_NOME - 1] = '\0';
    strncpy(novo->telefone, telefone, MAX_TELEFONE - 1);
    novo->telefone[MAX_TELEFONE - 1] = '\0';
    strncpy(novo->email, email, MAX_EMAIL - 1);
    novo->email[MAX_EMAIL - 1] = '\0';
    novo->ativo = 1;
    
    lista->quantidade++;
    return novo->id;
}

// Buscar contato por ID
Contato* buscar_contato_por_id(ListaContatos *lista, int id) {
    if (!lista) {
        printf("Erro ao buscar contato: lista inválida\n");
        return NULL;
    }
    
    for (int i = 0; i < lista->quantidade; i++) {
        if (lista->contatos[i].id == id && lista->contatos[i].ativo) {
            return &lista->contatos[i];
        }
    }
    return NULL;
}

// Editar contato existente
int editar_contato(ListaContatos *lista, int id, const char *nome, const char *telefone, const char *email) {
    Contato *contato = buscar_contato_por_id(lista, id);
    if (!contato) {
        printf("Contato com ID %d não encontrado para edição.\n", id);
        return 0;
    }
    
    if (nome && strlen(nome) > 0) { // Se o nome não for NULL ou vazio
        strncpy(contato->nome, nome, MAX_NOME - 1);
        contato->nome[MAX_NOME - 1] = '\0';
    }
    
    if (telefone && strlen(telefone) > 0) { // Se o telefone não for NULL ou vazio
        strncpy(contato->telefone, telefone, MAX_TELEFONE - 1);
        contato->telefone[MAX_TELEFONE - 1] = '\0';
    }
    
    if (email && strlen(email) > 0) { // Se o email não for NULL ou vazio
        strncpy(contato->email, email, MAX_EMAIL - 1);
        contato->email[MAX_EMAIL - 1] = '\0';
    }
    
    return 1; // Edição bem sucedida, retornando o valor 1 para a função 'editar_contato'
}

// Excluir contato (remoção física com compactação)
int excluir_contato(ListaContatos *lista, int id) {
    if (!lista) {
        return 0;
    }
    
    // Encontrar índice do contato
    int indice = -1;
    for (int i = 0; i < lista->quantidade; i++) {
        if (lista->contatos[i].id == id && lista->contatos[i].ativo) {
            indice = i;
            break;
        }
    }
    
    if (indice == -1) {
        return 0;
    }
    
    // Deslocar elementos usando memmove para manter ordem compacta
    if (indice < lista->quantidade - 1) {
        memmove(&lista->contatos[indice], 
                &lista->contatos[indice + 1],
                (lista->quantidade - indice - 1) * sizeof(Contato));
    }
    
    lista->quantidade--;
    
    // Realocar para liberar espaço se necessário (quando muito vazio)
    if (lista->quantidade > 0 && lista->capacidade > CAPACIDADE_INICIAL * 2 
        && lista->quantidade < lista->capacidade / 2) {
        int nova_capacidade = lista->capacidade / 2;
        Contato *novo_array = (Contato*)realloc(lista->contatos, nova_capacidade * sizeof(Contato));
        if (novo_array) {
            lista->contatos = novo_array;
            lista->capacidade = nova_capacidade;
        }
    }
    
    return 1;
}

// Listar todos os contatos ativos
void listar_contatos(ListaContatos *lista) {
    if (!lista || lista->quantidade == 0) {
        printf("Nenhum contato cadastrado.\n");
        return;
    }
    
    int count = 0;
    printf("\n%-5s %-30s %-20s %-30s\n", "ID", "Nome", "Telefone", "Email");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < lista->quantidade; i++) {
        if (lista->contatos[i].ativo) {
            printf("%-5d %-30s %-20s %-30s\n",
                   lista->contatos[i].id,
                   lista->contatos[i].nome,
                   lista->contatos[i].telefone,
                   lista->contatos[i].email);
            count++;
        }
    }
    
    if (count == 0) {
        printf("Nenhum contato ativo encontrado.\n");
    } else {
        printf("\nTotal: %d contato(s)\n", count);
    }
}

// Buscar contatos por termo (nome, telefone ou email)
void buscar_contatos(ListaContatos *lista, const char *termo) {
    if (!lista || !termo) {
        printf("Nenhum contato encontrado.\n");
        return;
    }
    
    int count = 0;
    printf("\n%-5s %-30s %-20s %-30s\n", "ID", "Nome", "Telefone", "Email");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < lista->quantidade; i++) {
        if (lista->contatos[i].ativo) {
            if (strstr(lista->contatos[i].nome, termo) ||
                strstr(lista->contatos[i].telefone, termo) ||
                strstr(lista->contatos[i].email, termo)) {
                printf("%-5d %-30s %-20s %-30s\n",
                       lista->contatos[i].id,
                       lista->contatos[i].nome,
                       lista->contatos[i].telefone,
                       lista->contatos[i].email);
                count++;
            }
        }
    }
    
    if (count == 0) {
        printf("Nenhum contato encontrado com o termo '%s'.\n", termo);
    } else {
        printf("\nTotal: %d contato(s) encontrado(s)\n", count);
    }
}

// Salvar contatos em arquivo binário
int salvar_contatos(ListaContatos *lista, const char *arquivo) {
    if (!lista || !arquivo) {
        return 0;
    }
    
    FILE *fp = fopen(arquivo, "wb");
    if (!fp) {
        fprintf(stderr, "Erro ao abrir arquivo para escrita: %s\n", arquivo);
        return 0;
    }
    
    // Escrever quantidade de contatos
    if (fwrite(&lista->quantidade, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "Erro ao escrever quantidade de contatos\n");
        fclose(fp);
        return 0;
    }
    
    // Escrever array de contatos
    if (lista->quantidade > 0) {
        if (fwrite(lista->contatos, sizeof(Contato), lista->quantidade, fp) != (size_t)lista->quantidade) {
            fprintf(stderr, "Erro ao escrever contatos\n");
            fclose(fp);
            return 0;
        }
    }
    
    fclose(fp);
    return 1;
}

// Carregar contatos de arquivo binário usando fseek/ftell
ListaContatos* carregar_contatos(const char *arquivo) {
    if (!arquivo) {
        return criar_lista();
    }
    
    FILE *fp = fopen(arquivo, "rb");
    if (!fp) {
        // Arquivo não existe, retornar lista vazia
        return criar_lista();
    }
    
    // Descobrir tamanho do arquivo
    if (fseek(fp, 0, SEEK_END) != 0) {
        fprintf(stderr, "Erro ao buscar final do arquivo\n");
        fclose(fp);
        return criar_lista();
    }
    
    long file_size = ftell(fp);
    if (file_size < 0) {
        fprintf(stderr, "Erro ao obter tamanho do arquivo\n");
        fclose(fp);
        return criar_lista();
    }
    
    if (fseek(fp, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Erro ao voltar ao início do arquivo\n");
        fclose(fp);
        return criar_lista();
    }
    
    ListaContatos *lista = (ListaContatos*)malloc(sizeof(ListaContatos));
    if (!lista) {
        fprintf(stderr, "Erro ao alocar memória para lista\n");
        fclose(fp);
        return NULL;
    }
    
    // Ler quantidade de contatos
    if (fread(&lista->quantidade, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "Erro ao ler quantidade de contatos\n");
        free(lista);
        fclose(fp);
        return criar_lista();
    }
    
    if (lista->quantidade == 0) {
        lista->contatos = (Contato*)malloc(CAPACIDADE_INICIAL * sizeof(Contato));
        if (!lista->contatos) {
            fprintf(stderr, "Erro ao alocar memória inicial\n");
            free(lista);
            fclose(fp);
            return NULL;
        }
        lista->capacidade = CAPACIDADE_INICIAL;
        fclose(fp);
        return lista;
    }
    
    // Calcular número de contatos baseado no tamanho do arquivo
    size_t expected_size = sizeof(int) + (lista->quantidade * sizeof(Contato));
    if ((size_t)file_size != expected_size) {
        fprintf(stderr, "Aviso: Tamanho do arquivo inconsistente\n");
    }
    
    // Alocar memória exata para os contatos
    lista->capacidade = lista->quantidade > CAPACIDADE_INICIAL ? lista->quantidade : CAPACIDADE_INICIAL;
    lista->contatos = (Contato*)malloc(lista->capacidade * sizeof(Contato));
    if (!lista->contatos) {
        fprintf(stderr, "Erro ao alocar memória para %d contatos\n", lista->quantidade);
        free(lista);
        fclose(fp);
        return NULL;
    }
    
    // Ler array de contatos
    size_t lidos = fread(lista->contatos, sizeof(Contato), lista->quantidade, fp);
    if (lidos != (size_t)lista->quantidade) {
        fprintf(stderr, "Erro: esperado %d contatos, lidos %zu\n", lista->quantidade, lidos);
        free(lista->contatos);
        free(lista);
        fclose(fp);
        return criar_lista();
    }
    
    fclose(fp);
    return lista;
}

// Exportar contatos para arquivo CSV
int exportar_csv(ListaContatos *lista, const char *arquivo) {
    if (!lista || !arquivo) {
        return 0;
    }
    
    FILE *fp = fopen(arquivo, "w");
    if (!fp) {
        fprintf(stderr, "Erro ao criar arquivo CSV: %s\n", arquivo);
        return 0;
    }
    
    // Escrever cabeçalho
    fprintf(fp, "ID,Nome,Telefone,Email,Status\n");
    
    // Escrever dados
    for (int i = 0; i < lista->quantidade; i++) {
        if (lista->contatos[i].ativo) {
            fprintf(fp, "%d,\"%s\",\"%s\",\"%s\",Ativo\n",
                    lista->contatos[i].id,
                    lista->contatos[i].nome,
                    lista->contatos[i].telefone,
                    lista->contatos[i].email);
        }
    }
    
    fclose(fp);
    return 1;
}

// Gerar contatos de teste automaticamente
void gerar_contatos_teste(ListaContatos *lista, int quantidade) {
    if (!lista || quantidade <= 0) {
        return;
    }
    
    char nome[MAX_NOME];
    char telefone[MAX_TELEFONE];
    char email[MAX_EMAIL];
    
    for (int i = 0; i < quantidade; i++) {
        snprintf(nome, MAX_NOME, "Contato Teste %d", i + 1);
        snprintf(telefone, MAX_TELEFONE, "11-9%04d-%04d", i / 10000, i % 10000);
        snprintf(email, MAX_EMAIL, "contato%d@teste.com", i + 1);
        
        adicionar_contato(lista, nome, telefone, email);
    }
}

// Analisar uso de memória da lista
void analisar_memoria(ListaContatos *lista) {
    if (!lista) {
        printf("Lista inválida\n");
        return;
    }
    
    size_t memoria_lista = sizeof(ListaContatos);
    size_t memoria_array = lista->capacidade * sizeof(Contato);
    size_t memoria_usada = lista->quantidade * sizeof(Contato);
    size_t memoria_total = memoria_lista + memoria_array;
    
    printf("\n=== Análise de Memória ===\n");
    printf("Contatos ativos:       %d\n", lista->quantidade);
    printf("Capacidade alocada:    %d\n", lista->capacidade);
    printf("Taxa de ocupação:      %.1f%%\n", 
           lista->capacidade > 0 ? (lista->quantidade * 100.0 / lista->capacidade) : 0);
    printf("\n");
    printf("Memória da estrutura:  %zu bytes\n", memoria_lista);
    printf("Memória do array:      %zu bytes (%.2f KB)\n", 
           memoria_array, memoria_array / 1024.0);
    printf("Memória em uso:        %zu bytes (%.2f KB)\n", 
           memoria_usada, memoria_usada / 1024.0);
    printf("Memória desperdiçada:  %zu bytes (%.2f KB)\n", 
           memoria_array - memoria_usada, (memoria_array - memoria_usada) / 1024.0);
    printf("Memória total:         %zu bytes (%.2f KB)\n", 
           memoria_total, memoria_total / 1024.0);
    printf("\n");
}
