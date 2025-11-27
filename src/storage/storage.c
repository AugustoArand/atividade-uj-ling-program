#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "contact.h"

#define INITIAL_CAPACITY 10

// Salvar contatos no arquivo binário
int save_contacts(ContactList *list, const char *filename) {
    if (!list || !filename) {
        return 0;
    }
    
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error opening file for writing: %s\n", filename);
        return 0;
    }
    
    // Write number of contacts
    if (fwrite(&list->count, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "Error writing contact count\n");
        fclose(fp);
        return 0;
    }
    
    // Write contacts array
    if (list->count > 0) {
        if (fwrite(list->contacts, sizeof(Contact), list->count, fp) != (size_t)list->count) {
            fprintf(stderr, "Error writing contacts\n");
            fclose(fp);
            return 0;
        }
    }
    
    fclose(fp);
    return 1;
}

// Carregar contatos do arquivo binário usando fseek/ftell
ContactList* load_contacts(const char *filename) {
    if (!filename) {
        return create_list();
    }
    
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        // File doesn't exist, return empty list
        return create_list();
    }
    
    // Get file size
    if (fseek(fp, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error seeking to end of file\n");
        fclose(fp);
        return create_list();
    }
    
    long file_size = ftell(fp);
    if (file_size < 0) {
        fprintf(stderr, "Error getting file size\n");
        fclose(fp);
        return create_list();
    }
    
    if (fseek(fp, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error seeking to start of file\n");
        fclose(fp);
        return create_list();
    }
    
    ContactList *list = (ContactList*)malloc(sizeof(ContactList));
    if (!list) {
        fprintf(stderr, "Error allocating memory for list\n");
        fclose(fp);
        return NULL;
    }
    
    // Read number of contacts
    if (fread(&list->count, sizeof(int), 1, fp) != 1) {
        fprintf(stderr, "Error reading contact count\n");
        free(list);
        fclose(fp);
        return create_list();
    }
    
    if (list->count == 0) {
        list->contacts = (Contact*)malloc(INITIAL_CAPACITY * sizeof(Contact));
        if (!list->contacts) {
            fprintf(stderr, "Error allocating initial memory\n");
            free(list);
            fclose(fp);
            return NULL;
        }
        list->capacity = INITIAL_CAPACITY;
        fclose(fp);
        return list;
    }
    
    // Verify file size consistency
    size_t expected_size = sizeof(int) + (list->count * sizeof(Contact));
    if ((size_t)file_size != expected_size) {
        fprintf(stderr, "Warning: Inconsistent file size\n");
    }
    
    // Allocate exact memory for contacts
    list->capacity = list->count > INITIAL_CAPACITY ? list->count : INITIAL_CAPACITY;
    list->contacts = (Contact*)malloc(list->capacity * sizeof(Contact));
    if (!list->contacts) {
        fprintf(stderr, "Error allocating memory for %d contacts\n", list->count);
        free(list);
        fclose(fp);
        return NULL;
    }
    
    // Read contacts array
    size_t read = fread(list->contacts, sizeof(Contact), list->count, fp);
    if (read != (size_t)list->count) {
        fprintf(stderr, "Error: expected %d contacts, read %zu\n", list->count, read);
        free(list->contacts);
        free(list);
        fclose(fp);
        return create_list();
    }
    
    fclose(fp);
    return list;
}

// Exportar contatos para arquivo CSV
int export_csv(ContactList *list, const char *filename) {
    if (!list || !filename) {
        return 0;
    }
    
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error creating CSV file: %s\n", filename);
        return 0;
    }
    
    // Write header
    fprintf(fp, "ID,Name,Phone,Email,Status\n");
    
    // Write data
    for (int i = 0; i < list->count; i++) {
        if (list->contacts[i].active) {
            fprintf(fp, "%d,\"%s\",\"%s\",\"%s\",Active\n",
                    list->contacts[i].id,
                    list->contacts[i].name,
                    list->contacts[i].phone,
                    list->contacts[i].email);
        }
    }
    
    fclose(fp);
    return 1;
}
