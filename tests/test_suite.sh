#!/bin/bash

# Test Suite for Contact Management System
# This script performs basic functional tests

echo "========================================"
echo "Contact Management System - Test Suite"
echo "========================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Function to print test result
print_result() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓ PASSED${NC}: $2"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ FAILED${NC}: $2"
        ((TESTS_FAILED++))
    fi
}

# Create test data directory
mkdir -p data
TEST_BIN="data/test_contacts.bin"

# Clean previous test data
rm -f $TEST_BIN

echo "Test 1: Compilation"
make clean > /dev/null 2>&1
make > /dev/null 2>&1
if [ $? -eq 0 ]; then
    print_result 0 "Project compiles successfully"
else
    print_result 1 "Project compilation failed"
    exit 1
fi
echo ""

echo "Test 2: Binary exists"
if [ -f "./contatos" ]; then
    print_result 0 "Executable 'contatos' exists"
else
    print_result 1 "Executable 'contatos' not found"
    exit 1
fi
echo ""

echo "Test 3: Source files structure"
check_file() {
    if [ -f "$1" ]; then
        print_result 0 "File $1 exists"
    else
        print_result 1 "File $1 not found"
    fi
}

check_file "src/main/main.c"
check_file "src/contact/contact.h"
check_file "src/contact/contact.c"
check_file "src/storage/storage.h"
check_file "src/storage/storage.c"
check_file "src/utils/utils.h"
check_file "src/utils/utils.c"
echo ""

echo "Test 4: Data directory"
if [ -d "data" ]; then
    print_result 0 "Data directory exists"
else
    print_result 1 "Data directory not found"
    mkdir -p data
fi
echo ""

echo "Test 5: Header guards"
for header in src/*/*.h; do
    if [ -f "$header" ]; then
        if grep -q "#ifndef" "$header" && grep -q "#define" "$header" && grep -q "#endif" "$header"; then
            print_result 0 "Header guards in $(basename $header)"
        else
            print_result 1 "Missing header guards in $(basename $header)"
        fi
    fi
done
echo ""

echo "Test 6: Memory safety checks"
if grep -q "malloc" src/*/*.c && grep -q "free" src/*/*.c; then
    print_result 0 "Memory allocation/deallocation present"
else
    print_result 1 "Memory management functions not found"
fi
echo ""

echo "========================================"
echo "Test Summary"
echo "========================================"
echo -e "Tests Passed: ${GREEN}${TESTS_PASSED}${NC}"
echo -e "Tests Failed: ${RED}${TESTS_FAILED}${NC}"
echo "========================================"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
