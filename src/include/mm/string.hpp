#include <stddef.h>
#include <cdefs.h>

size_t Strnlen(const char *str, size_t maxLength);
char *Strncpy(char *dest, const char *src, size_t maxLength);
int Strncmp(const char *first, const char *second, size_t maxLength);

char *Strtok(char *s, char *delim);

void Itoa (char *buf, int base, long long int d);
long long int Atoi(char *str);
