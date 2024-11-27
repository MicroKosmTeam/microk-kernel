#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// Function to convert integer to string using itoa
char* itoa(int num, char* str) {
	int i = 0;
	int isNegative = num < 0;

	// Handle negative numbers by converting them to positive and remembering the sign
	if (isNegative)
		num = -num;

	// Convert each digit into characters and store in reverse order
	do {
		str[i++] = (char)('0' + (num % 10));
		num /= 10;
	} while (num > 0);

	// Add the negative sign for negative numbers
	if (isNegative)
		str[i++] = '-';

	// Null-terminate the string
	str[i] = '\0';

	// Reverse the string to get the correct order of digits
	int start = 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		++start;
		--end;
	}

	return str;
}

// Function to convert long integer to string
char* ltoa(long num, char* str) {
	int i = 0;
	int isNegative = num < 0;

	// Handle negative numbers by converting them to positive and remembering the sign
	if (isNegative)
		num = -num;

	// Convert each digit into characters and store in reverse order
	do {
		str[i++] = (char)('0' + (num % 10));
		num /= 10;
	} while (num > 0);

	// Add the negative sign for negative numbers
	if (isNegative)
		str[i++] = '-';

	// Null-terminate the string
	str[i] = '\0';

	// Reverse the string to get the correct order of digits
	int start = 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		++start;
		--end;
	}

	return str;
}

// Function to convert integer to hexadecimal string
char* itoh(int num, char* str) {
	int i = 0;
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// Handle negative numbers by converting them to positive and remembering the sign
	while (num > 0) {
		char remainder = num % 16;
		if (remainder < 10)
			str[i++] = '0' + remainder;
		else
			str[i++] = 'A' + (remainder - 10);
		num /= 16;
	}

	// Null-terminate the string
	str[i] = '\0';

	// Reverse the string to get the correct order of digits
	int start = 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		++start;
		--end;
	}

	return str;
}

// Function to convert long integer to hexadecimal string
char* ltoh(long num, char* str) {
	int i = 0;
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}

	// Handle negative numbers by converting them to positive and remembering the sign
	while (num > 0) {
		char remainder = num % 16;
		if (remainder < 10)
			str[i++] = '0' + remainder;
		else
			str[i++] = 'A' + (remainder - 10);
		num /= 16;
	}

	// Null-terminate the string
	str[i] = '\0';

	// Reverse the string to get the correct order of digits
	int start = 0;
	int end = i - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		++start;
		--end;
	}

	return str;
}

void *memcpy(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;

	for (size_t i = 0; i < n; i++) {
		pdest[i] = psrc[i];
	}

	return dest;
}

void *memset(void *s, int c, size_t n) {
	uint8_t *p = (uint8_t *)s;

	for (size_t i = 0; i < n; i++) {
		p[i] = (uint8_t)c;
	}

	return s;
}

void *memclr(void *s, size_t n) {
	return memset(s, 0, n);
}

void *memmove(void *dest, const void *src, size_t n) {
	uint8_t *pdest = (uint8_t *)dest;
	const uint8_t *psrc = (const uint8_t *)src;

	if (src > dest) {
		for (size_t i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	} else if (src < dest) {
		for (size_t i = n; i > 0; i--) {
			pdest[i-1] = psrc[i-1];
		}
	}

	return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const uint8_t *p1 = (const uint8_t *)s1;
	const uint8_t *p2 = (const uint8_t *)s2;

	for (size_t i = 0; i < n; i++) {
		if (p1[i] != p2[i]) {
			return p1[i] < p2[i] ? -1 : 1;
		}
	}

	return 0;
}
