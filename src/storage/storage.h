#ifndef STORAGE_H
#define STORAGE_H

#include "contact.h"

// Persistence functions
int save_contacts(ContactList *list, const char *filename);
ContactList* load_contacts(const char *filename);

// Export function
int export_csv(ContactList *list, const char *filename);

#endif
