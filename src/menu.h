#ifndef MENU_H
#define MENU_H

#include "contato.h"

// Funções do menu interativo
void exibir_menu_principal();
void executar_menu_interativo();
void aguardar_enter();
void limpar_tela();

// Operações interativas
void menu_adicionar_contato(ListaContatos *lista);
void menu_listar_contatos(ListaContatos *lista);
void menu_buscar_contatos(ListaContatos *lista);
void menu_editar_contato(ListaContatos *lista);
void menu_excluir_contato(ListaContatos *lista);
void menu_exportar_csv(ListaContatos *lista);
void menu_analisar_memoria(ListaContatos *lista);
void menu_teste_stress(ListaContatos *lista);

#endif
