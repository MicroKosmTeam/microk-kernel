#include <cstdint.hpp>
#include <cdefs.h>

usize Strnlen(const char *str, usize maxLength);
char *Strncpy(char *dest, const char *src, usize maxLength);
int Strncmp(const char *first, const char *second, usize maxLength);

char *Strtok(char *s, char *delim);

void Itoa (char *buf, int base, long long int d);
long long int Atoi(char *str);
