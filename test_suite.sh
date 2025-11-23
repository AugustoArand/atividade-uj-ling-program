#!/bin/bash

# Script de testes automatizados do sistema

echo "=== TESTES AUTOMATIZADOS DO SISTEMA ==="
echo ""

# Limpar dados anteriores
rm -f data/test_*.bin data/test_*.csv

# Compilar
echo "1. Compilando projeto..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Compilação bem-sucedida"
else
    echo "❌ Erro na compilação"
    exit 1
fi
echo ""

# Teste 1: Adicionar 3 contatos
echo "2. Teste: Adicionar 3 contatos"
./contatos adicionar "Teste 1" "11-1111-1111" "teste1@test.com" > /dev/null
./contatos adicionar "Teste Ana" "22-2222-2222" "ana@test.com" > /dev/null
./contatos adicionar "Teste 3" "33-3333-3333" "teste3@test.com" > /dev/null

TOTAL=$(./contatos listar | grep "Total:" | grep -oE '[0-9]+')
if [ "$TOTAL" -eq "3" ]; then
    echo "✅ 3 contatos adicionados com sucesso"
else
    echo "❌ Esperado 3, encontrado $TOTAL"
fi

# Verificar arquivo cresceu
SIZE=$(stat -f%z data/contatos.bin 2>/dev/null || stat -c%s data/contatos.bin 2>/dev/null)
if [ "$SIZE" -gt "0" ]; then
    echo "✅ Arquivo binário criado ($SIZE bytes)"
else
    echo "❌ Arquivo binário não foi criado"
fi
echo ""

# Teste 2: Buscar por substring
echo "3. Teste: Buscar por substring 'ana'"
FOUND=$(./contatos buscar "ana" | grep "Total:" | grep -oE '[0-9]+')
if [ "$FOUND" -eq "1" ]; then
    echo "✅ Busca encontrou 1 contato com 'ana'"
else
    echo "❌ Busca esperava 1, encontrou $FOUND"
fi
echo ""

# Teste 3: Remover por ID
echo "4. Teste: Remover contato ID 2"
./contatos excluir 2 > /dev/null 2>&1
AFTER=$(./contatos listar | grep "Total:" | grep -oE '[0-9]+')
if [ "$AFTER" -eq "2" ]; then
    echo "✅ Contato removido, restam 2 contatos"
else
    echo "❌ Após remoção esperado 2, encontrado $AFTER"
fi

# Verificar que ID 2 não está mais listado
if ./contatos listar | grep -q "Teste Ana"; then
    echo "❌ Contato excluído ainda aparece na lista"
else
    echo "✅ Contato excluído não aparece na lista"
fi
echo ""

# Teste 4: Exportar CSV
echo "5. Teste: Exportar para CSV"
./contatos exportar test_export.csv > /dev/null 2>&1
if [ -f "data/test_export.csv" ]; then
    LINES=$(wc -l < data/test_export.csv)
    if [ "$LINES" -eq "3" ]; then  # Header + 2 contatos
        echo "✅ CSV exportado com cabeçalho e 2 contatos"
        
        # Verificar colunas
        if head -1 data/test_export.csv | grep -q "ID,Nome,Telefone,Email,Status"; then
            echo "✅ Cabeçalho CSV correto"
        else
            echo "❌ Cabeçalho CSV incorreto"
        fi
    else
        echo "❌ CSV esperado 3 linhas, encontrado $LINES"
    fi
else
    echo "❌ Arquivo CSV não foi criado"
fi
echo ""

# Teste 5: Teste de stress (pequeno)
echo "6. Teste: Gerar 100 contatos automaticamente"
rm -f data/contatos.bin

START=$(date +%s%N 2>/dev/null || date +%s)
for i in {1..100}; do
    ./contatos adicionar "Contato$i" "11-9999-$(printf '%04d' $i)" "c$i@test.com" > /dev/null 2>&1
done
END=$(date +%s%N 2>/dev/null || date +%s)

TOTAL_FINAL=$(./contatos listar | grep "Total:" | grep -oE '[0-9]+')
if [ "$TOTAL_FINAL" -eq "100" ]; then
    echo "✅ 100 contatos gerados com sucesso"
else
    echo "❌ Esperado 100, encontrado $TOTAL_FINAL"
fi

# Análise de memória
echo ""
echo "7. Análise de memória com 100 contatos:"
./contatos analisar

# Verificar arquivo final
FINAL_SIZE=$(stat -f%z data/contatos.bin 2>/dev/null || stat -c%s data/contatos.bin 2>/dev/null)
echo ""
echo "Tamanho do arquivo binário: $FINAL_SIZE bytes ($(echo "scale=2; $FINAL_SIZE/1024" | bc) KB)"

# Limpar dados de teste
echo ""
echo "Limpando dados de teste..."
rm -f data/test_*.csv data/contatos.bin

echo ""
echo "=== TESTES CONCLUÍDOS ==="
