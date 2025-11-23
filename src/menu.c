#include "menu.h"
#include "utils/string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARQUIVO_DADOS "data/contatos.bin"

void limpar_tela() {
    #ifdef _WIN32
        int ret = system("cls");
    #else
        int ret = system("clear");
    #endif
    (void)ret; // Suprimir warning
}

void aguardar_enter() {
    printf("\nPressione ENTER para continuar...");
    limpar_buffer_entrada();
}

void exibir_menu_principal() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════╗\n");
    printf("║   SISTEMA DE GERENCIAMENTO DE CONTATOS       ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║  1. Adicionar Contato                         ║\n");
    printf("║  2. Listar Todos os Contatos                  ║\n");
    printf("║  3. Buscar Contato                            ║\n");
    printf("║  4. Editar Contato                            ║\n");
    printf("║  5. Excluir Contato                           ║\n");
    printf("║  6. Exportar para CSV                         ║\n");
    printf("║  7. Análise de Memória                        ║\n");
    printf("║  8. Teste de Stress                           ║\n");
    printf("║  0. Sair                                      ║\n");
    printf("╚═══════════════════════════════════════════════╝\n");
    printf("\nEscolha uma opção: ");
}

void menu_adicionar_contato(ListaContatos *lista) {
    limpar_tela();
    printf("\n=== ADICIONAR NOVO CONTATO ===\n\n");
    
    char *nome = ler_string("Nome: ", 100);
    if (!nome || string_vazia(nome)) {
        printf("❌ Nome não pode ser vazio!\n");
        if (nome) liberar_buffer(nome);
        aguardar_enter();
        return;
    }
    trim_string(nome);
    
    char *telefone = ler_string("Telefone: ", 20);
    if (!telefone || string_vazia(telefone)) {
        printf("❌ Telefone não pode ser vazio!\n");
        liberar_buffer(nome);
        if (telefone) liberar_buffer(telefone);
        aguardar_enter();
        return;
    }
    trim_string(telefone);
    
    char *email = ler_string("Email: ", 100);
    if (!email || string_vazia(email)) {
        printf("❌ Email não pode ser vazio!\n");
        liberar_buffer(nome);
        liberar_buffer(telefone);
        if (email) liberar_buffer(email);
        aguardar_enter();
        return;
    }
    trim_string(email);
    
    int id = adicionar_contato(lista, nome, telefone, email);
    
    if (id > 0) {
        printf("\n✅ Contato adicionado com sucesso! ID: %d\n", id);
        if (salvar_contatos(lista, ARQUIVO_DADOS)) {
            printf("✅ Dados salvos com sucesso.\n");
        } else {
            printf("⚠️  Aviso: Erro ao salvar dados.\n");
        }
    } else {
        printf("❌ Erro ao adicionar contato.\n");
    }
    
    liberar_buffer(nome);
    liberar_buffer(telefone);
    liberar_buffer(email);
    
    aguardar_enter();
}

void menu_listar_contatos(ListaContatos *lista) {
    limpar_tela();
    printf("\n=== LISTA DE CONTATOS ===\n");
    listar_contatos(lista);
    aguardar_enter();
}

void menu_buscar_contatos(ListaContatos *lista) {
    limpar_tela();
    printf("\n=== BUSCAR CONTATOS ===\n\n");
    
    char *termo = ler_string("Digite o termo de busca (nome, telefone ou email): ", 100);
    if (!termo || string_vazia(termo)) {
        printf("❌ Termo de busca não pode ser vazio!\n");
        if (termo) liberar_buffer(termo);
        aguardar_enter();
        return;
    }
    trim_string(termo);
    
    printf("\n");
    buscar_contatos(lista, termo);
    
    liberar_buffer(termo);
    aguardar_enter();
}

void menu_editar_contato(ListaContatos *lista) {
    limpar_tela();
    printf("\n=== EDITAR CONTATO ===\n\n");
    
    // Listar contatos para referência
    listar_contatos(lista);
    
    char *id_str = ler_string("\nDigite o ID do contato a editar (0 para cancelar): ", 20);
    if (!id_str || string_vazia(id_str)) {
        if (id_str) liberar_buffer(id_str);
        return;
    }
    
    int id = atoi(id_str);
    liberar_buffer(id_str);
    
    if (id == 0) {
        printf("Operação cancelada.\n");
        aguardar_enter();
        return;
    }
    
    Contato *contato = buscar_contato_por_id(lista, id);
    if (!contato) {
        printf("❌ Contato com ID %d não encontrado.\n", id);
        aguardar_enter();
        return;
    }
    
    printf("\n--- Contato Atual ---\n");
    printf("Nome: %s\n", contato->nome);
    printf("Telefone: %s\n", contato->telefone);
    printf("Email: %s\n", contato->email);
    
    printf("\n--- Novos Dados (deixe em branco para manter) ---\n");
    
    char *novo_nome = ler_string("Novo nome: ", 100);
    char *novo_telefone = ler_string("Novo telefone: ", 20);
    char *novo_email = ler_string("Novo email: ", 100);
    
    // Aplicar trim
    if (novo_nome && !string_vazia(novo_nome)) trim_string(novo_nome);
    if (novo_telefone && !string_vazia(novo_telefone)) trim_string(novo_telefone);
    if (novo_email && !string_vazia(novo_email)) trim_string(novo_email);
    
    if (editar_contato(lista, id, 
                       (novo_nome && !string_vazia(novo_nome)) ? novo_nome : "",
                       (novo_telefone && !string_vazia(novo_telefone)) ? novo_telefone : "",
                       (novo_email && !string_vazia(novo_email)) ? novo_email : "")) {
        printf("\n✅ Contato %d editado com sucesso!\n", id);
        if (salvar_contatos(lista, ARQUIVO_DADOS)) {
            printf("✅ Dados salvos com sucesso.\n");
        } else {
            printf("⚠️  Aviso: Erro ao salvar dados.\n");
        }
    } else {
        printf("❌ Erro ao editar contato.\n");
    }
    
    if (novo_nome) liberar_buffer(novo_nome);
    if (novo_telefone) liberar_buffer(novo_telefone);
    if (novo_email) liberar_buffer(novo_email);
    
    aguardar_enter();
}

void menu_excluir_contato(ListaContatos *lista) {
    limpar_tela();
    printf("\n=== EXCLUIR CONTATO ===\n\n");
    
    // Listar contatos para referência
    listar_contatos(lista);
    
    char *id_str = ler_string("\nDigite o ID do contato a excluir (0 para cancelar): ", 20);
    if (!id_str || string_vazia(id_str)) {
        if (id_str) liberar_buffer(id_str);
        return;
    }
    
    int id = atoi(id_str);
    liberar_buffer(id_str);
    
    if (id == 0) {
        printf("Operação cancelada.\n");
        aguardar_enter();
        return;
    }
    
    Contato *contato = buscar_contato_por_id(lista, id);
    if (!contato) {
        printf("❌ Contato com ID %d não encontrado.\n", id);
        aguardar_enter();
        return;
    }
    
    printf("\n--- Contato a ser excluído ---\n");
    printf("Nome: %s\n", contato->nome);
    printf("Telefone: %s\n", contato->telefone);
    printf("Email: %s\n", contato->email);
    
    char *confirma = ler_string("\nTem certeza? (s/n): ", 10);
    if (confirma && (confirma[0] == 's' || confirma[0] == 'S')) {
        if (excluir_contato(lista, id)) {
            printf("\n✅ Contato %d excluído com sucesso!\n", id);
            if (salvar_contatos(lista, ARQUIVO_DADOS)) {
                printf("✅ Dados salvos com sucesso.\n");
            } else {
                printf("⚠️  Aviso: Erro ao salvar dados.\n");
            }
        } else {
            printf("❌ Erro ao excluir contato.\n");
        }
    } else {
        printf("Operação cancelada.\n");
    }
    
    if (confirma) liberar_buffer(confirma);
    aguardar_enter();
}

void menu_exportar_csv(ListaContatos *lista) {
    limpar_tela();
    printf("\n=== EXPORTAR PARA CSV ===\n\n");
    
    char *arquivo = ler_string("Nome do arquivo (ex: contatos.csv): ", 100);
    if (!arquivo || string_vazia(arquivo)) {
        printf("❌ Nome do arquivo não pode ser vazio!\n");
        if (arquivo) liberar_buffer(arquivo);
        aguardar_enter();
        return;
    }
    trim_string(arquivo);
    
    // Adicionar extensão .csv se não tiver
    char arquivo_completo[120];
    if (strstr(arquivo, ".csv") == NULL) {
        snprintf(arquivo_completo, sizeof(arquivo_completo), "data/%s.csv", arquivo);
    } else {
        snprintf(arquivo_completo, sizeof(arquivo_completo), "data/%s", arquivo);
    }
    
    if (exportar_csv(lista, arquivo_completo)) {
        printf("\n✅ Contatos exportados com sucesso para: %s\n", arquivo_completo);
        printf("Total de contatos exportados: %d\n", lista->quantidade);
    } else {
        printf("❌ Erro ao exportar contatos.\n");
    }
    
    liberar_buffer(arquivo);
    aguardar_enter();
}

void menu_analisar_memoria(ListaContatos *lista) {
    limpar_tela();
    analisar_memoria(lista);
    aguardar_enter();
}

void menu_teste_stress(ListaContatos *lista) {
    limpar_tela();
    printf("\n=== TESTE DE STRESS ===\n\n");
    
    char *qtd_str = ler_string("Quantos contatos gerar? (recomendado: 1000-10000): ", 20);
    if (!qtd_str || string_vazia(qtd_str)) {
        printf("❌ Quantidade inválida!\n");
        if (qtd_str) liberar_buffer(qtd_str);
        aguardar_enter();
        return;
    }
    
    int quantidade = atoi(qtd_str);
    liberar_buffer(qtd_str);
    
    if (quantidade <= 0 || quantidade > 100000) {
        printf("❌ Quantidade deve estar entre 1 e 100000!\n");
        aguardar_enter();
        return;
    }
    
    printf("\nGerando %d contatos...\n", quantidade);
    
    // Medir tempo
    clock_t inicio = clock();
    gerar_contatos_teste(lista, quantidade);
    clock_t fim = clock();
    
    double tempo_geracao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("✅ %d contatos gerados em %.3f segundos\n", quantidade, tempo_geracao);
    
    // Salvar
    printf("\nSalvando em arquivo...\n");
    inicio = clock();
    if (salvar_contatos(lista, ARQUIVO_DADOS)) {
        fim = clock();
        double tempo_salvamento = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
        printf("✅ Dados salvos em %.3f segundos\n", tempo_salvamento);
    } else {
        printf("❌ Erro ao salvar dados\n");
    }
    
    // Análise de memória
    printf("\n");
    analisar_memoria(lista);
    
    aguardar_enter();
}

void executar_menu_interativo() {
    ListaContatos *lista = carregar_contatos(ARQUIVO_DADOS);
    
    if (!lista) {
        fprintf(stderr, "Erro ao carregar lista de contatos\n");
        return;
    }
    
    int opcao = -1;
    
    while (opcao != 0) {
        limpar_tela();
        exibir_menu_principal();
        
        char *opcao_str = ler_linha(10);
        if (opcao_str) {
            opcao = atoi(opcao_str);
            liberar_buffer(opcao_str);
        } else {
            opcao = -1;
        }
        
        switch (opcao) {
            case 1:
                menu_adicionar_contato(lista);
                break;
            case 2:
                menu_listar_contatos(lista);
                break;
            case 3:
                menu_buscar_contatos(lista);
                break;
            case 4:
                menu_editar_contato(lista);
                break;
            case 5:
                menu_excluir_contato(lista);
                break;
            case 6:
                menu_exportar_csv(lista);
                break;
            case 7:
                menu_analisar_memoria(lista);
                break;
            case 8:
                menu_teste_stress(lista);
                break;
            case 0:
                limpar_tela();
                printf("\n✅ Saindo do sistema...\n");
                printf("Obrigado por usar o Sistema de Gerenciamento de Contatos!\n\n");
                break;
            default:
                printf("❌ Opção inválida! Tente novamente.\n");
                aguardar_enter();
                break;
        }
    }
    
    liberar_lista(lista);
}
