#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"

#define INITIAL_CAPACITY 10

// Criação de uma lista vazia
ContactList* create_list() {
    ContactList *list = (ContactList*)malloc(sizeof(ContactList));
    if (!list) {
        fprintf(stderr, "Erro ao alocar memória para a lista\n");
        return NULL;
    }
    
    list->contacts = (Contact*)malloc(INITIAL_CAPACITY * sizeof(Contact));
    if (!list->contacts) {
        fprintf(stderr, "Erro ao alocar memória para os contatos\n");
        free(list);
        return NULL;
    }
    
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

// Função para liberar memória
void free_list(ContactList *list) {
    if (list) {
        if (list->contacts) {
            free(list->contacts);
        }
        free(list);
    }
}

// Função de expandir capacidade quando chamada
static int expand_list(ContactList *list) {
    int new_capacity = list->capacity * 2;
    Contact *new_contacts = (Contact*)realloc(list->contacts, new_capacity * sizeof(Contact));
    
    if (!new_contacts) {
        fprintf(stderr, "Erro ao expandir a lista de contatos\n");
        return 0;
    }
    
    list->contacts = new_contacts;
    list->capacity = new_capacity;
    return 1;
}

// Geração do próximo ID disponível
static int generate_id(ContactList *list) {
    int max_id = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->contacts[i].id > max_id) {
            max_id = list->contacts[i].id;
        }
    }
    return max_id + 1;
}

// CRUD - Adicionar novo contato
int add_contact(ContactList *list, const char *name, const char *phone, const char *email) {
    // Verificar se a lista precisa ser expandida
    if (list->count >= list->capacity) {
        if (!expand_list(list)) {
            return -1;  // Erro ao expandir
        }
    }
    
    Contact *new_contact = &list->contacts[list->count];
    new_contact->id = generate_id(list);
    strncpy(new_contact->name, name, MAX_NAME - 1);
    new_contact->name[MAX_NAME - 1] = '\0';
    strncpy(new_contact->phone, phone, MAX_PHONE - 1);
    new_contact->phone[MAX_PHONE - 1] = '\0';
    strncpy(new_contact->email, email, MAX_EMAIL - 1);
    new_contact->email[MAX_EMAIL - 1] = '\0';
    new_contact->active = 1;
    
    list->count++;
    return new_contact->id;
}

// CRUD - Buscar contato por ID
Contact* find_contact_by_id(ContactList *list, int id) {
    if (!list) {
        printf("Erro ao encontrar contato: lista inválida\n");
        return NULL;
    }
    
    for (int i = 0; i < list->count; i++) {
        if (list->contacts[i].id == id && list->contacts[i].active) {
            return &list->contacts[i];
        }
    }
    return NULL;
}

// CRUD - Editar contato existente
int edit_contact(ContactList *list, int id, const char *name, const char *phone, const char *email) {
    Contact *contact = find_contact_by_id(list, id);
    if (!contact) {
        printf("Contato com ID %d não encontrado para edição.\n", id);
        return 0;
    }
    
    if (name && strlen(name) > 0) {
        strncpy(contact->name, name, MAX_NAME - 1);
        contact->name[MAX_NAME - 1] = '\0';
    }
    
    if (phone && strlen(phone) > 0) {
        strncpy(contact->phone, phone, MAX_PHONE - 1);
        contact->phone[MAX_PHONE - 1] = '\0';
    }
    
    if (email && strlen(email) > 0) {
        strncpy(contact->email, email, MAX_EMAIL - 1);
        contact->email[MAX_EMAIL - 1] = '\0';
    }
    
    return 1;
}

// CRUD - Remover contato (remoção física com compactação)
int delete_contact(ContactList *list, int id) {
    if (!list) {
        printf("Erro ao deletar contato: lista inválida\n");
        return 0;
    }
    
    // Find contact index
    int index = -1;
    for (int i = 0; i < list->count; i++) {
        if (list->contacts[i].id == id && list->contacts[i].active) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        return 0;
    }
    
    // Shift elements using memmove to maintain compact order
    if (index < list->count - 1) {
        memmove(&list->contacts[index], 
                &list->contacts[index + 1],
                (list->count - index - 1) * sizeof(Contact));
    }
    
    list->count--;
    
    // Reallocate to free space if needed (when mostly empty)
    if (list->count > 0 && list->capacity > INITIAL_CAPACITY * 2 
        && list->count < list->capacity / 2) {
        int new_capacity = list->capacity / 2;
        Contact *new_array = (Contact*)realloc(list->contacts, new_capacity * sizeof(Contact));
        if (new_array) {
            list->contacts = new_array;
            list->capacity = new_capacity;
        }
    }
    
    return 1;
}

// CRUD - Listar todo os contato
void list_contacts(ContactList *list) {
    if (!list || list->count == 0) {
        printf("Nenhum contato registrado.\n");
        return;
    }
    
    int count = 0;
    printf("\n%-5s %-30s %-20s %-30s\n", "ID", "Nome", "Telefone", "Email");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < list->count; i++) {
        if (list->contacts[i].active) {
            printf("%-5d %-30s %-20s %-30s\n",
                   list->contacts[i].id,
                   list->contacts[i].name,
                   list->contacts[i].phone,
                   list->contacts[i].email);
            count++;
        }
    }
    
    if (count == 0) {
        printf("Nenhum contato ativo encontrado.\n");
    } else {
        printf("\nTotal: %d contato(s)\n", count);
    }
}

// CRUD - Buscar contato por nome, email, telefone
void search_contacts(ContactList *list, const char *term) {
    if (!list || !term) {
        printf("Nenhum contato encontrado.\n");
        return;
    }
    
    int count = 0;
    printf("\n%-5s %-30s %-20s %-30s\n", "ID", "Nome", "Telefone", "Email");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < list->count; i++) {
        if (list->contacts[i].active) {
            if (strstr(list->contacts[i].name, term) ||
                strstr(list->contacts[i].phone, term) ||
                strstr(list->contacts[i].email, term)) {
                printf("%-5d %-30s %-20s %-30s\n",
                       list->contacts[i].id,
                       list->contacts[i].name,
                       list->contacts[i].phone,
                       list->contacts[i].email);
                count++;
            }
        }
    }
    
    if (count == 0) {
        printf("Nenhum contato encontrado com o termo '%s'.\n", term);
    } else {
        printf("\nTotal: %d contato(s) encontrado(s)\n", count);
    }
}

// Gerar contatos de teste automaticamente
void generate_test_contacts(ContactList *list, int count) {
    if (!list || count <= 0) {
        return;
    }
    
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
    
    for (int i = 0; i < count; i++) {
        snprintf(name, MAX_NAME, "Contato de Teste %d", i + 1);
        snprintf(phone, MAX_PHONE, "11-9%04d-%04d", i / 10000, i % 10000);
        snprintf(email, MAX_EMAIL, "contato%d@test.com", i + 1);
        
        add_contact(list, name, phone, email);
    }
}

// Analisar uso de memória da lista
void analyze_memory(ContactList *list) {
    if (!list) {
        printf("Lista inválida\n");
        return;
    }
    
    size_t list_memory = sizeof(ContactList);
    size_t array_memory = list->capacity * sizeof(Contact);
    size_t used_memory = list->count * sizeof(Contact);
    size_t total_memory = list_memory + array_memory;
    
    printf("\n=== Análise de Memória ===\n");
    printf("Contatos ativos:       %d\n", list->count);
    printf("Capacidade alocada:    %d\n", list->capacity);
    printf("Taxa de ocupação:      %.1f%%\n", 
           list->capacity > 0 ? (list->count * 100.0 / list->capacity) : 0);
    printf("\n");
    printf("Memória da estrutura:  %zu bytes\n", list_memory);
    printf("Memória do array:      %zu bytes (%.2f KB)\n", 
           array_memory, array_memory / 1024.0);
    printf("Memória em uso:        %zu bytes (%.2f KB)\n", 
           used_memory, used_memory / 1024.0);
    printf("Memória desperdiçada:  %zu bytes (%.2f KB)\n", 
           array_memory - used_memory, (array_memory - used_memory) / 1024.0);
    printf("Memória total:         %zu bytes (%.2f KB)\n", 
           total_memory, total_memory / 1024.0);
    printf("\n");
}
