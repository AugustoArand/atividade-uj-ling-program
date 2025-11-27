#!/bin/bash

# Stress Test for Contact Management System
# Tests system under heavy load with large datasets

echo "========================================"
echo "Contact Management - Stress Test Suite"
echo "========================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0
START_TIME=$(date +%s)

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

# Function to measure execution time
measure_time() {
    local start=$1
    local end=$(date +%s)
    echo $((end - start))
}

# Create test data directory
mkdir -p data
TEST_BIN="data/stress_test_contacts.bin"
TEST_CSV="data/stress_test_export.csv"

# Clean previous test data
rm -f "$TEST_BIN" "$TEST_CSV"

echo "Preparing environment..."
# Compile the project
make clean > /dev/null 2>&1
make > /dev/null 2>&1
if [ $? -ne 0 ]; then
    echo -e "${RED}Failed to compile project${NC}"
    exit 1
fi
echo -e "${GREEN}✓${NC} Project compiled successfully"
echo ""

# Create a test program for stress testing
cat > /tmp/stress_test.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "contact.h"
#include "storage.h"
#include "utils.h"

void stress_test_add(int count) {
    printf("Adding %d contacts...\n", count);
    ContactList *list = create_list();
    if (!list) {
        printf("Failed to create list\n");
        exit(1);
    }
    
    clock_t start = clock();
    for (int i = 0; i < count; i++) {
        char name[100], phone[20], email[100];
        sprintf(name, "Contact %d", i);
        sprintf(phone, "1234567%04d", i % 10000);
        sprintf(email, "contact%d@test.com", i);
        
        if (add_contact(list, name, phone, email) < 0) {
            printf("Failed to add contact %d\n", i);
            free_list(list);
            exit(1);
        }
        
        if ((i + 1) % 1000 == 0) {
            printf("  Added %d contacts...\n", i + 1);
        }
    }
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Successfully added %d contacts in %.2f seconds\n", count, time_spent);
    printf("Average time per contact: %.6f seconds\n", time_spent / count);
    
    free_list(list);
}

void stress_test_save_load(int count) {
    printf("\nTesting save/load with %d contacts...\n", count);
    ContactList *list = create_list();
    if (!list) {
        printf("Failed to create list\n");
        exit(1);
    }
    
    // Add contacts
    for (int i = 0; i < count; i++) {
        char name[100], phone[20], email[100];
        sprintf(name, "SaveTest %d", i);
        sprintf(phone, "9999%06d", i % 1000000);
        sprintf(email, "save%d@test.com", i);
        add_contact(list, name, phone, email);
    }
    
    // Save to file
    clock_t start = clock();
    int result = save_contacts(list, "data/stress_test_contacts.bin");
    if (result == 0) {
        printf("Failed to save contacts\n");
        free_list(list);
        exit(1);
    }
    clock_t end = clock();
    double save_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Saved %d contacts in %.2f seconds\n", count, save_time);
    
    free_list(list);
    
    // Load from file
    start = clock();
    ContactList *loaded = load_contacts("data/stress_test_contacts.bin");
    if (!loaded) {
        printf("Failed to load contacts\n");
        exit(1);
    }
    end = clock();
    double load_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Loaded %d contacts in %.2f seconds\n", loaded->count, load_time);
    
    if (loaded->count != count) {
        printf("Error: Expected %d contacts, got %d\n", count, loaded->count);
        free_list(loaded);
        exit(1);
    }
    
    free_list(loaded);
}

void stress_test_search(int count) {
    printf("\nTesting search performance with %d contacts...\n", count);
    ContactList *list = create_list();
    if (!list) {
        printf("Failed to create list\n");
        exit(1);
    }
    
    // Add contacts
    for (int i = 0; i < count; i++) {
        char name[100], phone[20], email[100];
        sprintf(name, "SearchTest %d", i);
        sprintf(phone, "8888%06d", i % 1000000);
        sprintf(email, "search%d@test.com", i);
        add_contact(list, name, phone, email);
    }
    
    // Perform searches
    int search_count = 100;
    clock_t start = clock();
    for (int i = 0; i < search_count; i++) {
        int target = rand() % count;
        Contact *found = find_contact_by_id(list, target + 1);
    }
    clock_t end = clock();
    double search_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Performed %d searches in %.2f seconds\n", search_count, search_time);
    printf("Average search time: %.6f seconds\n", search_time / search_count);
    
    free_list(list);
}

void stress_test_delete(int count) {
    printf("\nTesting delete performance with %d contacts...\n", count);
    ContactList *list = create_list();
    if (!list) {
        printf("Failed to create list\n");
        exit(1);
    }
    
    // Add contacts
    int *ids = malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) {
        char name[100], phone[20], email[100];
        sprintf(name, "DeleteTest %d", i);
        sprintf(phone, "7777%06d", i % 1000000);
        sprintf(email, "delete%d@test.com", i);
        ids[i] = add_contact(list, name, phone, email);
    }
    
    // Delete half the contacts (delete from the end to avoid shifting issues)
    int delete_count = count / 2;
    clock_t start = clock();
    int deleted = 0;
    for (int i = count - 1; i >= count - delete_count; i--) {
        if (delete_contact(list, ids[i]) == 0) {
            printf("Failed to delete contact %d\n", ids[i]);
        } else {
            deleted++;
        }
    }
    clock_t end = clock();
    double delete_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Deleted %d contacts in %.2f seconds\n", deleted, delete_time);
    printf("Average delete time: %.6f seconds\n", delete_time / deleted);
    
    free(ids);
    free_list(list);
}

void stress_test_export(int count) {
    printf("\nTesting CSV export with %d contacts...\n", count);
    ContactList *list = create_list();
    if (!list) {
        printf("Failed to create list\n");
        exit(1);
    }
    
    // Add contacts
    for (int i = 0; i < count; i++) {
        char name[100], phone[20], email[100];
        sprintf(name, "ExportTest %d", i);
        sprintf(phone, "6666%06d", i % 1000000);
        sprintf(email, "export%d@test.com", i);
        add_contact(list, name, phone, email);
    }
    
    // Export to CSV
    clock_t start = clock();
    int result = export_csv(list, "data/stress_test_export.csv");
    if (result == 0) {
        printf("Failed to export contacts\n");
        free_list(list);
        exit(1);
    }
    clock_t end = clock();
    double export_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("Exported %d contacts in %.2f seconds\n", count, export_time);
    
    free_list(list);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <test_type> <count>\n", argv[0]);
        printf("test_type: add, save_load, search, delete, export\n");
        return 1;
    }
    
    srand(time(NULL));
    int count = atoi(argv[2]);
    
    if (strcmp(argv[1], "add") == 0) {
        stress_test_add(count);
    } else if (strcmp(argv[1], "save_load") == 0) {
        stress_test_save_load(count);
    } else if (strcmp(argv[1], "search") == 0) {
        stress_test_search(count);
    } else if (strcmp(argv[1], "delete") == 0) {
        stress_test_delete(count);
    } else if (strcmp(argv[1], "export") == 0) {
        stress_test_export(count);
    } else {
        printf("Unknown test type: %s\n", argv[1]);
        return 1;
    }
    
    return 0;
}
EOF

# Compile stress test program
echo "Compiling stress test program..."
gcc -o /tmp/stress_test /tmp/stress_test.c \
    src/contact/contact.c \
    src/storage/storage.c \
    src/utils/utils.c \
    -Isrc/contact -Isrc/storage -Isrc/utils -lm 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Failed to compile stress test program${NC}"
    exit 1
fi
echo -e "${GREEN}✓${NC} Stress test program compiled"
echo ""

# Test 1: Add many contacts (small batch)
echo -e "${BLUE}Test 1: Adding 1,000 contacts${NC}"
/tmp/stress_test add 1000
if [ $? -eq 0 ]; then
    print_result 0 "Add 1,000 contacts"
else
    print_result 1 "Add 1,000 contacts"
fi
echo ""

# Test 2: Add many contacts (medium batch)
echo -e "${BLUE}Test 2: Adding 5,000 contacts${NC}"
/tmp/stress_test add 5000
if [ $? -eq 0 ]; then
    print_result 0 "Add 5,000 contacts"
else
    print_result 1 "Add 5,000 contacts"
fi
echo ""

# Test 3: Save and Load (small batch)
echo -e "${BLUE}Test 3: Save/Load 1,000 contacts${NC}"
/tmp/stress_test save_load 1000
if [ $? -eq 0 ]; then
    print_result 0 "Save/Load 1,000 contacts"
else
    print_result 1 "Save/Load 1,000 contacts"
fi
echo ""

# Test 4: Save and Load (medium batch)
echo -e "${BLUE}Test 4: Save/Load 5,000 contacts${NC}"
/tmp/stress_test save_load 5000
if [ $? -eq 0 ]; then
    print_result 0 "Save/Load 5,000 contacts"
else
    print_result 1 "Save/Load 5,000 contacts"
fi
echo ""

# Test 5: Search performance
echo -e "${BLUE}Test 5: Search performance with 5,000 contacts${NC}"
/tmp/stress_test search 5000
if [ $? -eq 0 ]; then
    print_result 0 "Search performance"
else
    print_result 1 "Search performance"
fi
echo ""

# Test 6: Delete performance
echo -e "${BLUE}Test 6: Delete performance with 2,000 contacts${NC}"
/tmp/stress_test delete 2000
if [ $? -eq 0 ]; then
    print_result 0 "Delete performance"
else
    print_result 1 "Delete performance"
fi
echo ""

# Test 7: Export CSV
echo -e "${BLUE}Test 7: CSV export with 3,000 contacts${NC}"
/tmp/stress_test export 3000
if [ $? -eq 0 ]; then
    print_result 0 "CSV export"
else
    print_result 1 "CSV export"
fi
echo ""

# Verify exported file
if [ -f "$TEST_CSV" ]; then
    LINES=$(wc -l < "$TEST_CSV")
    if [ $LINES -eq 3001 ]; then  # 3000 contacts + 1 header
        print_result 0 "CSV file has correct number of lines"
    else
        print_result 1 "CSV file has incorrect number of lines (expected 3001, got $LINES)"
    fi
else
    print_result 1 "CSV file was not created"
fi
echo ""

# Test 8: Memory test with large dataset
echo -e "${BLUE}Test 8: Memory test with 10,000 contacts${NC}"
/tmp/stress_test add 10000 > /dev/null 2>&1
if [ $? -eq 0 ]; then
    print_result 0 "Handle 10,000 contacts without crash"
else
    print_result 1 "Handle 10,000 contacts without crash"
fi
echo ""

# Clean up
rm -f "$TEST_BIN" "$TEST_CSV" /tmp/stress_test /tmp/stress_test.c

END_TIME=$(date +%s)
TOTAL_TIME=$((END_TIME - START_TIME))

echo "========================================"
echo "Stress Test Summary"
echo "========================================"
echo -e "Tests Passed: ${GREEN}${TESTS_PASSED}${NC}"
echo -e "Tests Failed: ${RED}${TESTS_FAILED}${NC}"
echo -e "Total Time: ${BLUE}${TOTAL_TIME}s${NC}"
echo "========================================"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All stress tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some stress tests failed.${NC}"
    exit 1
fi
