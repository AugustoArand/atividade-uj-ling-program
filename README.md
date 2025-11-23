# Sistema de Gerenciamento de Contatos

Sistema completo de gerenciamento de contatos desenvolvido em C com persistência em arquivo binário.

## Funcionalidades

- ✅ Adicionar contatos
- ✅ Listar todos os contatos
- ✅ Buscar contatos por nome, telefone ou email
- ✅ Editar contatos existentes
- ✅ Excluir contatos (soft delete)
- ✅ Persistência em arquivo binário
- ✅ Alocação dinâmica de memória
- ✅ Parsing de linha de comando
- ✅ Menu interativo de navegação
- ✅ Exportação para CSV
- ✅ Análise de uso de memória
- ✅ Teste de stress automatizado
- ✅ Remoção física com compactação de memória

## Compilação

```bash
make
```

Para limpar os arquivos compilados:
```bash
make clean
```

## Uso

O sistema possui **dois modos de operação**:

### Modo 1: Menu Interativo (Recomendado)
```bash
./contatos
```
Inicia um menu interativo com navegação visual:
- **1** - Adicionar Contato
- **2** - Listar Todos os Contatos
- **3** - Buscar Contato
- **4** - Editar Contato
- **5** - Excluir Contato
- **6** - Exportar para CSV
- **7** - Análise de Memória
- **8** - Teste de Stress
- **0** - Sair

### Modo 2: Linha de Comando (Scripts/Automação)

#### Adicionar um contato
```bash
./contatos adicionar "João Silva" "11-98765-4321" "joao@email.com"
```

#### Listar todos os contatos
```bash
./contatos listar
```

#### Buscar contatos
```bash
./contatos buscar Silva
```

#### Editar um contato
```bash
# Editar todos os campos
./contatos editar 1 "João Santos" "11-99999-9999" "joao.santos@email.com"

# Editar apenas o nome
./contatos editar 1 "João Santos" "" ""

# Editar nome e telefone
./contatos editar 1 "João Santos" "11-99999-9999" ""
```

#### Excluir um contato
```bash
./contatos excluir 1
```

#### Exportar para CSV
```bash
./contatos exportar contatos.csv
```

#### Análise de memória
```bash
./contatos analisar
```

#### Ajuda
```bash
./contatos ajuda
```

## Estrutura do Projeto

```
.
├── Makefile              - Script de compilação
├── README.md             - Documentação
├── contatos              - Executável
├── src/                  - Código fonte
│   ├── contato.h         - Definições de estruturas e protótipos
│   ├── contato.c         - Implementação das operações CRUD e persistência
│   ├── menu.h            - Interface do menu interativo
│   ├── menu.c            - Implementação do menu interativo
│   ├── main.c            - Programa principal
│   └── utils/            - Funções utilitárias
│       ├── string_utils.h/.c  - Manipulação de strings
│       └── memory_utils.h/.c  - Gerenciamento de memória
└── data/                 - Arquivos de dados
    └── contatos.bin      - Arquivo binário de contatos (gerado automaticamente)
```

## Características Técnicas

### Gerenciamento de Memória
- **Alocação Dinâmica**: Usa `malloc`, `realloc` e `free` para gerenciar memória dinamicamente
- **Expansão Automática**: A lista cresce automaticamente quando necessário
- **Compactação de Memória**: Usa `memmove` para deslocar elementos após remoção
- **Liberação de Espaço**: Realoca automaticamente quando ocupação < 50%
- **Detecção de Vazamentos**: Verificação de ponteiros nulos após alocações

### Persistência e I/O
- **fseek/ftell**: Descobre tamanho do arquivo antes de alocar memória
- **Validação de Leitura**: Verifica retorno de `fread` para garantir integridade
- **Modo Binário**: Usa "rb", "wb" para portabilidade entre plataformas
- **Cálculo de Tamanho**: `n = size / sizeof(Contato)` para alocação precisa
- **Exportação CSV**: Gera relatórios em formato texto estruturado

### Interface
- **Parsing de CLI**: Interface de linha de comando com validação de argumentos
- **Menu Interativo**: Navegação visual com validação de entrada
- **Análise de Memória**: Exibe uso detalhado de recursos

## Exemplos de Uso

### Exemplo 1: Modo Interativo
```bash
# Compilar o programa
make

# Iniciar modo interativo
./contatos

# Seguir as instruções do menu:
# 1. Escolher opção 1 (Adicionar)
# 2. Digitar nome, telefone e email
# 3. Escolher opção 2 (Listar) para ver o contato
# 4. Escolher opção 0 (Sair)
```

### Exemplo 2: Linha de Comando
```bash
# Adicionar alguns contatos
./contatos adicionar "Maria Silva" "11-98765-1234" "maria@email.com"
./contatos adicionar "José Santos" "21-99876-5432" "jose@email.com"
./contatos adicionar "Ana Costa" "31-91234-5678" "ana@email.com"

# Listar todos
./contatos listar

# Buscar por nome
./contatos buscar Silva

# Editar um contato
./contatos editar 1 "Maria Silva Santos" "" ""

# Excluir um contato
./contatos excluir 2

# Ver resultado final
./contatos listar

# Exportar para CSV
./contatos exportar meus_contatos.csv

# Análise de memória
./contatos analisar
```

### Exemplo 3: Testes Automatizados
```bash
# Executar suite completa de testes
./test_suite.sh
```

## Testes e Casos de Uso

### Teste 1: Adicionar e Verificar Crescimento
```bash
./contatos adicionar "João" "11-1111" "j@test.com"
./contatos adicionar "Maria" "22-2222" "m@test.com"
./contatos adicionar "Pedro" "33-3333" "p@test.com"
ls -lh data/contatos.bin  # Verificar arquivo cresceu
./contatos listar          # Verificar todos aparecem
```

### Teste 2: Busca por Substring
```bash
./contatos buscar "ana"    # Retorna nomes com "ana"
./contatos buscar "@test"  # Busca por domínio de email
```

### Teste 3: Remoção e Compactação
```bash
./contatos analisar        # Ver memória antes
./contatos excluir 2       # Remover contato
./contatos listar          # Verificar não aparece mais
./contatos analisar        # Ver memória após compactação
```

### Teste 4: Exportação CSV
```bash
./contatos exportar relatorio.csv
cat data/relatorio.csv     # Conferir colunas: ID,Nome,Telefone,Email,Status
```

### Teste 5: Stress Test (10.000 contatos)
```bash
# Via menu interativo:
./contatos
# Escolher opção 8 e digitar 10000

# Medir tempo de carregamento:
time ./contatos listar

# Verificar uso de memória:
./contatos analisar
```



