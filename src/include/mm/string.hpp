#include <cstdint.hpp>
#include <cdefs.h>

usize Strnlen(const char *str, usize maxLength);
char *Strncpy(char *dest, const char *src, usize maxLength);
int Strncmp(const char *first, const char *second, usize maxLength);

usize Strcspn(const char *s1, const char *s2);
usize Strspn(const char *s1, const char *s2);
char *Strtok(char *string, const char *delim, char **savePtr);


char *Strtok(char *s, char *delim);

void Itoa (char *buf, int base, long long int d);
long long int Atoi(char *str);
