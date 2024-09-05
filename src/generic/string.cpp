#include <string.h>

extern "C" {

usize Strnlen(const char *str, usize maxLength) {
        const char *strEnd = str;

        for (usize current = 0;
	     current < maxLength && *strEnd;
	     ++strEnd, ++current);

        return (strEnd - str);
}

char *Strncpy(char *dest, const char *src, usize maxLength){
        if(dest == NULL || src == NULL) return NULL;

        char *destStart = dest;

        for (usize current = 0;
	    current < maxLength && (*dest++ = *src++);
	    ++current);

        return destStart;
}

int Strncmp(const char *first, const char *second, usize maxLength) {
        const u8 *firstCurrent = (const u8*)first;
        const u8 *secondCurrent = (const u8*)second;

        for (usize current = 0;
	     current < maxLength && (*firstCurrent != '\0');
	     ++current) {
                if (*firstCurrent == '\0') return  1;
                if (*secondCurrent > *firstCurrent)   return -1;
                if (*firstCurrent > *secondCurrent)   return  1;

                ++firstCurrent;
                ++secondCurrent;
        }

        if (*secondCurrent != '\0') return -1;

        return 0;
}

bool IsDelim(char c, char *delim) {
	while(*delim != '\0') {
		if(c == *delim)
			return true;
		delim++;
	}

	return false;
}

/* find index of first s1[i] that matches any s2[]. */
usize Strcspn(const char *s1, const char *s2) {
	const char *sc1, *sc2;
	for (sc1 = s1; *sc1 != '\0'; ++sc1) {
		for (sc2 = s2; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (sc1 - s1);
		}
	}
	return (sc1 - s1); // terminating nulls match.
}

/* find index of first s1[i] that matches no s2[]. */
usize Strspn(const char *s1, const char *s2) {
	const char *sc1, *sc2;
	for (sc1 = s1; *sc1 != '\0'; ++sc1) {
		for (sc2 = s2; ; ++sc2) {
			if (*sc2 == '\0') {
				return (sc1 - s1);
			} else if (*sc1 == *sc2) {
				break;
			}
		}
	}   
	return (sc1 - s1);
}

char *Strtok(char *string, const char *delim, char **savePtr) {
	char *end;

	if (string == NULL) {
		string = *savePtr;
	}

	if (*string == '\0') {
		*savePtr = string;
		return NULL;
	}

	/* Scan leading delimiters.  */
	string += Strspn(string, delim);
	if (*string == '\0'){
		*savePtr = string;
		return NULL;
	}

	/* Find the end of the token.  */
	end = string + Strcspn(string, delim);
	if (*end == '\0') {
		*savePtr = end;
		return string;
	}

	/* Terminate the token and make *SAVE_PTR point past it.  */
	*end = '\0';
	*savePtr = end + 1;
	return string;
}


void Itoa (char *buf, int base, long long int d) {
        char *p = buf;
        char *p1, *p2;
        unsigned long long ud = d;
        int divisor = 10;

        /*  If %d is specified and D is minus, put ‘-’ in the head. */
        if (base == 'd' && d < 0) {
                *p++ = '-';
                buf++;
                ud = -d;
        } else if (base == 'x')
                divisor = 16;

        /*  Divide UD by DIVISOR until UD == 0. */
        do {
                int remainder = ud % divisor;
                *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        } while (ud /= divisor);

        /*  Terminate BUF. */
        *p = 0;

        /*  Reverse BUF. */
        p1 = buf;
        p2 = p - 1;

        while (p1 < p2) {
                char tmp = *p1;
                *p1 = *p2;
                *p2 = tmp;
                p1++;
                p2--;
        }
}

long long int Atoi(char *str) {
	int base;

	if (str != NULL &&
	    str[0] != '\0' &&
	    str[0] == '0' &&
	    str[1] != '\0' &&
	    str[1] == 'x') {
		base = 16;
		str += 2;
	} else {
		base = 10;
	}

        // Initialize result
        long long int res = 0;

        // Iterate through all characters
        // of input string and update result
        // take ASCII character of corresponding digit and
        // subtract the code from '0' to get numerical
        // value and multiply res by 10 to shuffle
        // digits left to update running total
        for (int i = 0; str[i] != '\0'; ++i) {
		if(base == 16) {
			if (str[i] == 'A' ||
			    str[i] == 'B' ||
			    str[i] == 'C' ||
			    str[i] == 'D' ||
			    str[i] == 'E' ||
			    str[i] == 'F') {
				res = res * base + str[i] - 'A' + 0xA;
			} else if (str[i] == 'a' ||
			    str[i] == 'b' ||
			    str[i] == 'c' ||
			    str[i] == 'd' ||
			    str[i] == 'e' ||
			    str[i] == 'f') {
				res = res * base + str[i] - 'a' + 0xA;
			} else {
                		res = res * base + str[i] - '0';
			}
		} else {
                	res = res * base + str[i] - '0';

		}
	}

        // return result.
        return res;
}

int memcmp(const void* buf1, const void* buf2, usize count) {
	return Memcmp(buf1, buf2, count);
}

void *memcpy(void *dest, const void *src, usize n) {
	return Memcpy(dest, src, n);
}

void *memmove(void *dest, const void *src, usize n) {
	return Memmove(dest, src, n);
}

void *memset(void *start, u8 value, usize num) {
	return Memset(start, value, num);
}

void *Memclr(void *start, usize num) {
	return Memset(start, 0, num);
}

int Memcmp(const void* buf1, const void* buf2, usize count) {
	if(!count)
		return(0);

	while(--count && *(char*)buf1 == *(char*)buf2 ) {
		buf1 = (char*)buf1 + 1;
		buf2 = (char*)buf2 + 1;
	}

	return(*((unsigned char*)buf1) - *((unsigned char*)buf2));
}

void *Memcpy(void *dest, const void *src, usize n) {
	char *csrc = (char *)src;
	char *cdest = (char *)dest;

	for (usize i=0; i<n; i++) cdest[i] = csrc[i];

	return dest;
}

void *Memmove(void *dest, const void *src, usize n) {
	u8 *pdest = (u8 *)dest;
	const u8 *psrc = (const u8 *)src;

	if (src > dest) {
		for (usize i = 0; i < n; i++) {
			pdest[i] = psrc[i];
		}
	} else if (src < dest) {
		for (usize i = n; i > 0; i--) {
			pdest[i-1] = psrc[i-1];
		}
	}

	return dest;
}

void *Memset(void *start, u8 value, usize num) {
	num /= sizeof(u8);

	for (usize i = 0; i < num; i++) {
		*(u8*)((uptr)start + i) = value;
	}


	return start;
}
}
