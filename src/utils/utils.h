#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// String reading functions
char* read_string(const char *prompt, size_t max_size);
char* read_line(size_t max_size);
void clear_input_buffer();

// Dynamic buffer allocation functions
char* allocate_buffer(size_t size);
char* reallocate_buffer(char *buffer, size_t new_size);
void free_buffer(char *buffer);

// String manipulation functions
char* trim_string(char *str);
char* copy_string(const char *src);
int is_empty_string(const char *str);

// Screen control functions
void clear_screen();
void wait_for_enter();

#endif
