#ifndef CONTACT_H
#define CONTACT_H

#define MAX_NAME 100
#define MAX_PHONE 20
#define MAX_EMAIL 100

typedef struct {
    int id;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
    int active; // 1 = active, 0 = deleted (soft delete)
} Contact;

typedef struct {
    Contact *contacts;
    int count;
    int capacity;
} ContactList;

// List management functions
ContactList* create_list();
void free_list(ContactList *list);
int add_contact(ContactList *list, const char *name, const char *phone, const char *email);
int edit_contact(ContactList *list, int id, const char *name, const char *phone, const char *email);
int delete_contact(ContactList *list, int id);
Contact* find_contact_by_id(ContactList *list, int id);
void list_contacts(ContactList *list);
void search_contacts(ContactList *list, const char *term);

// Test and analysis functions
void generate_test_contacts(ContactList *list, int count);
void analyze_memory(ContactList *list);

#endif
