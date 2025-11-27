#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "contact.h"
#include "storage.h"
#include "utils.h"

#define DATA_FILE "data/contacts.bin"

// Menu principal
void display_main_menu() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║   CONTACT MANAGEMENT SYSTEM                   ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║  1. Adicionar Contato                         ║\n");
    printf("║  2. Listar Todos os Contatos                  ║\n");
    printf("║  3. Buscar Contato                            ║\n");
    printf("║  4. Editar Contato                            ║\n");
    printf("║  5. Excluir Contato                           ║\n");
    printf("║  6. Exportar para CSV                         ║\n");
    printf("║  7. Análise de Memória                        ║\n");
    printf("║  8. Teste de Estresse                         ║\n");
    printf("║  0. Sair                                     ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    printf("\nEscolha uma opção: ");
}

// Menu: Adicionar contato
void menu_add_contact(ContactList *list) {
    clear_screen();
    printf("\n=== ADICIONAR NOVO CONTATO ===\n\n");
    
    char *name = read_string("Nome: ", 100);
    if (!name || is_empty_string(name)) {
        printf("❌ O nome não pode estar vazio!\n");
        if (name) free_buffer(name);
        wait_for_enter();
        return;
    }
    trim_string(name);
    
    char *phone = read_string("Phone: ", 20);
    if (!phone || is_empty_string(phone)) {
        printf("❌ O telefone não pode estar vazio!\n");
        free_buffer(name);
        if (phone) free_buffer(phone);
        wait_for_enter();
        return;
    }
    trim_string(phone);
    
    char *email = read_string("Email: ", 100);
    if (!email || is_empty_string(email)) {
        printf("❌ O email não pode estar vazio!\n");
        free_buffer(name);
        free_buffer(phone);
        if (email) free_buffer(email);
        wait_for_enter();
        return;
    }
    trim_string(email);
    
    int id = add_contact(list, name, phone, email);
    
    if (id > 0) {
        printf("\n✅ Contato adicionado com sucesso! ID: %d\n", id);
        if (save_contacts(list, DATA_FILE)) {
            printf("✅ Dados salvos com sucesso.\n");
        } else {
            printf("⚠️  Aviso: Erro ao salvar os dados.\n");
        }
    } else {
        printf("❌ Erro ao adicionar contato.\n");
    }
    
    free_buffer(name);
    free_buffer(phone);
    free_buffer(email);
    
    wait_for_enter();
}

// Menu: Listar contato
void menu_list_contacts(ContactList *list) {
    clear_screen();
    printf("\n=== LISTAR CONTATOS ===\n");
    list_contacts(list);
    wait_for_enter();
}

// Menu: Buscar contatos
void menu_search_contacts(ContactList *list) {
    clear_screen();
    printf("\n=== BUSCAR CONTATOS ===\n\n");
    
    char *term = read_string("Digite o termo de busca (nome, telefone ou email): ", 100);
    if (!term || is_empty_string(term)) {
        printf("❌ O termo de busca não pode estar vazio!\n");
        if (term) free_buffer(term);
        wait_for_enter();
        return;
    }
    trim_string(term);
    
    printf("\n");
    search_contacts(list, term);
    
    free_buffer(term);
    wait_for_enter();
}

// Menu: Editar contato
void menu_edit_contact(ContactList *list) {
    clear_screen();
    printf("\n=== EDITAR CONTATO ===\n\n");
    
    // List contacts for reference
    list_contacts(list);
    
    char *id_str = read_string("\nDigite o ID do contato para editar (0 para cancelar): ", 20);
    if (!id_str || is_empty_string(id_str)) {
        if (id_str) free_buffer(id_str);
        return;
    }
    
    int id = atoi(id_str);
    free_buffer(id_str);
    
    if (id == 0) {
        printf("Operação cancelada.\n");
        wait_for_enter();
        return;
    }
    
    Contact *contact = find_contact_by_id(list, id);
    if (!contact) {
        printf("❌ Contato com ID %d não encontrado.\n", id);
        wait_for_enter();
        return;
    }
    
    printf("\n--- Contato Atual ---\n");
    printf("Nome: %s\n", contact->name);
    printf("Telefone: %s\n", contact->phone);
    printf("Email: %s\n", contact->email);
    
    printf("\n--- Novos Dados (deixe em branco para manter) ---\n");
    
    char *new_name = read_string("Novo nome: ", 100);
    char *new_phone = read_string("Novo telefone: ", 20);
    char *new_email = read_string("Novo email: ", 100);
    
    // Apply trim
    if (new_name && !is_empty_string(new_name)) trim_string(new_name);
    if (new_phone && !is_empty_string(new_phone)) trim_string(new_phone);
    if (new_email && !is_empty_string(new_email)) trim_string(new_email);
    
    if (edit_contact(list, id, 
                     (new_name && !is_empty_string(new_name)) ? new_name : "",
                     (new_phone && !is_empty_string(new_phone)) ? new_phone : "",
                     (new_email && !is_empty_string(new_email)) ? new_email : "")) {
        printf("\n✅ Contato %d editado com sucesso!\n", id);
        if (save_contacts(list, DATA_FILE)) {
            printf("✅ Dados salvos com sucesso.\n");
        } else {
            printf("⚠️  Aviso: Erro ao salvar os dados.\n");
        }
    } else {
        printf("❌ Erro ao editar contato.\n");
    }
    
    if (new_name) free_buffer(new_name);
    if (new_phone) free_buffer(new_phone);
    if (new_email) free_buffer(new_email);
    
    wait_for_enter();
}

// Menu: Deletar contato
void menu_delete_contact(ContactList *list) {
    clear_screen();
    printf("\n=== DELETAR CONTATO ===\n\n");
    
    // List contacts for reference
    list_contacts(list);
    
    char *id_str = read_string("\nDigite o ID do contato para deletar (0 para cancelar): ", 20);
    if (!id_str || is_empty_string(id_str)) {
        if (id_str) free_buffer(id_str);
        return;
    }
    
    int id = atoi(id_str);
    free_buffer(id_str);
    
    if (id == 0) {
        printf("Operação cancelada.\n");
        wait_for_enter();
        return;
    }
    
    Contact *contact = find_contact_by_id(list, id);
    if (!contact) {
        printf("❌ Contato com ID %d não encontrado.\n", id);
        wait_for_enter();
        return;
    }
    
    printf("\n--- Contato a ser deletado ---\n");
    printf("Nome: %s\n", contact->name);
    printf("Telefone: %s\n", contact->phone);
    printf("Email: %s\n", contact->email);
    
    char *confirm = read_string("\nTem certeza? (s/n): ", 10);
    if (confirm && (confirm[0] == 's' || confirm[0] == 'S')) {
        if (delete_contact(list, id)) {
            printf("\n✅ Contato %d deletado com sucesso!\n", id);
            if (save_contacts(list, DATA_FILE)) {
                printf("✅ Dados salvos com sucesso.\n");
            } else {
                printf("⚠️  Aviso: Erro ao salvar os dados.\n");
            }
        } else {
            printf("❌ Erro ao deletar contato.\n");
        }
    } else {
        printf("Operação cancelada.\n");
    }
    
    if (confirm) free_buffer(confirm);
    wait_for_enter();
}

// Funcionalidade: Exportar para CSV
void menu_export_csv(ContactList *list) {
    clear_screen();
    printf("\n=== EXPORTAR PARA CSV ===\n\n");
    
    char *filename = read_string("Nome do arquivo (ex: contatos.csv): ", 100);
    if (!filename || is_empty_string(filename)) {
        printf("❌ O nome do arquivo não pode estar vazio!\n");
        if (filename) free_buffer(filename);
        wait_for_enter();
        return;
    }
    trim_string(filename);
    
    // Add .csv extension if not present
    char full_filename[120];
    if (strstr(filename, ".csv") == NULL) {
        snprintf(full_filename, sizeof(full_filename), "data/%s.csv", filename);
    } else {
        snprintf(full_filename, sizeof(full_filename), "data/%s", filename);
    }
    
    if (export_csv(list, full_filename)) {
        printf("\n✅ Contatos exportados com sucesso para: %s\n", full_filename);
        printf("Total de contatos exportados: %d\n", list->count);
    } else {
        printf("❌ Erro ao exportar contatos.\n");
    }
    
    free_buffer(filename);
    wait_for_enter();
}

// Funcionalidade: Analisar uso de memória
void menu_analyze_memory(ContactList *list) {
    clear_screen();
    analyze_memory(list);
    wait_for_enter();
}

// Funcionalidade: Teste de estresse
void menu_stress_test(ContactList *list) {
    clear_screen();
    printf("\n=== TESTE DE ESTRESSE ===\n\n");
    
    char *qty_str = read_string("Quantos contatos gerar? (recomendado: 10-100): ", 20);
    if (!qty_str || is_empty_string(qty_str)) {
        printf("❌ Quantidade inválida!\n");
        if (qty_str) free_buffer(qty_str);
        wait_for_enter();
        return;
    }
    
    int quantity = atoi(qty_str);
    free_buffer(qty_str);
    
    if (quantity <= 0 || quantity > 100) {
        printf("❌ A quantidade deve estar entre 1 e 100!\n");
        wait_for_enter();
        return;
    }
    
    printf("\nGerando %d contatos...\n", quantity);
    
    // Measure time
    clock_t start = clock();
    generate_test_contacts(list, quantity);
    clock_t end = clock();
    
    double generation_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("✅ %d contatos gerados em %.3f segundos\n", quantity, generation_time);
    
    // Save
    printf("\nSalvando no arquivo...\n");
    start = clock();
    if (save_contacts(list, DATA_FILE)) {
        end = clock();
        double save_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("✅ Dados salvos em %.3f segundos\n", save_time);
    } else {
        printf("❌ Erro ao salvar os dados\n");
    }
    
    // Memory analysis
    printf("\n");
    analyze_memory(list);
    
    wait_for_enter();
}

// Funcionalidade: Looping de menu
void run_interactive_menu() {
    ContactList *list = load_contacts(DATA_FILE);
    
    if (!list) {
        fprintf(stderr, "Erro ao carregar lista de contatos\n");
        return;
    }
    
    int option = -1;
    
    while (option != 0) {
        clear_screen();
        display_main_menu();
        
        char *option_str = read_line(10);
        if (option_str) {
            option = atoi(option_str);
            free_buffer(option_str);
        } else {
            option = -1;
        }
        
        switch (option) {
            case 1:
                menu_add_contact(list);
                break;
            case 2:
                menu_list_contacts(list);
                break;
            case 3:
                menu_search_contacts(list);
                break;
            case 4:
                menu_edit_contact(list);
                break;
            case 5:
                menu_delete_contact(list);
                break;
            case 6:
                menu_export_csv(list);
                break;
            case 7:
                menu_analyze_memory(list);
                break;
            case 8:
                menu_stress_test(list);
                break;
            case 0:
                clear_screen();
                printf("\n✅ Saindo do sistema...\n");
                printf("Obrigado por usar o Sistema de Gerenciamento de Contatos!\n\n");
                break;
            default:
                printf("❌ Opção inválida! Tente novamente.\n");
                wait_for_enter();
                break;
        }
    }
    
    free_list(list);
}

int main() {
    run_interactive_menu();
    return 0;
}
