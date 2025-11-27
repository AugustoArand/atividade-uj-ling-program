# Contact Management System

Complete contact management system developed in C with binary file persistence. Fully modularized architecture following software engineering best practices.

## Features

- ✅ Add contacts
- ✅ List all contacts
- ✅ Search contacts by name, phone or email
- ✅ Edit existing contacts
- ✅ Delete contacts (physical removal with compaction)
- ✅ Binary file persistence
- ✅ Dynamic memory allocation
- ✅ Interactive menu navigation
- ✅ CSV export
- ✅ Memory usage analysis
- ✅ Automated stress testing
- ✅ Modular architecture

## Project Structure

```
.
├── Makefile                    # Build automation
├── README.md                   # Project documentation
├── contatos                    # Compiled executable
├── src/
│   ├── contact/               # Contact module
│   │   ├── contact.h          # Contact struct and CRUD operations (header)
│   │   └── contact.c          # Contact CRUD implementation
│   ├── storage/               # Storage module
│   │   ├── storage.h          # Persistence functions (header)
│   │   └── storage.c          # Load/save binary file operations
│   ├── utils/                 # Utils module
│   │   ├── utils.h            # Utility functions (header)
│   │   └── utils.c            # String handling, memory, screen control
│   └── main/                  # Main module
│       └── main.c             # Command parsing, main loop and integration
├── data/
│   └── contacts.bin           # Binary data file (runtime generated)
└── tests/
    ├── test_suite.sh          # Automated test suite
    └── README.md              # Test documentation
```

## Architecture

The project follows a modular architecture with clear separation of concerns. Each module is organized in its own directory:

### Core Modules

**src/contact/** - Contact management module
- `Contact` struct definition
- `ContactList` management
- CRUD operations (Create, Read, Update, Delete)
- Search and listing functions
- Memory analysis and test data generation

**src/storage/** - Persistence module
- Binary file persistence (`contacts.bin`)
- Save contact list to file
- Load contact list from file
- CSV export functionality

**src/utils/** - Utilities module
- String input/output utilities
- Dynamic buffer allocation
- String manipulation (trim, copy, validation)
- Screen control (clear, wait)

**src/main/** - Application entry point
- Interactive menu system
- User interface
- Command routing
- Main application loop

## Compilation

Build the project:
```bash
make
```

Clean compiled files:
```bash
make clean
```

Build and run:
```bash
make run
```

## Usage

### Interactive Menu Mode
```bash
./contatos
```

Menu options:
- **1** - Add Contact
- **2** - List All Contacts
- **3** - Search Contact
- **4** - Edit Contact
- **5** - Delete Contact
- **6** - Export to CSV
- **7** - Memory Analysis
- **8** - Stress Test
- **0** - Exit

## Testing

Run the automated test suite:
```bash
chmod +x tests/test_suite.sh
./tests/test_suite.sh
```

The test suite verifies:
- Project compilation
- File structure integrity
- Header guards
- Memory management
- All required source files

## Module Organization

Each module is self-contained in its own directory:

### 📂 src/contact/
Contains all contact management logic:
- **contact.h**: Public interface (Contact struct, ContactList, function declarations)
- **contact.c**: Implementation of CRUD operations, search, memory management

### 📂 src/storage/
Handles all data persistence:
- **storage.h**: Public interface (save, load, export functions)
- **storage.c**: Binary file I/O, CSV export implementation

### 📂 src/utils/
Provides utility functions:
- **utils.h**: Public interface (string, memory, screen utilities)
- **utils.c**: Helper function implementations

### 📂 src/main/
Application entry point:
- **main.c**: Interactive menu, user interface, command routing

This structure provides:
- ✅ **Clear boundaries** between modules
- ✅ **Easy navigation** - one concern per directory
- ✅ **Scalability** - easy to add new modules
- ✅ **Maintainability** - isolated changes
- ✅ **Testability** - modules can be tested independently

## Project Structure Details

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



