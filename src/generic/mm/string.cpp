#include <mm/string.hpp>
#include <cstdint.hpp>

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
char *Strtok(char *s, char *delim) {
	static char *p; // start of the next search
	if(!s) s = p;
	if(!s) return NULL;

	// handle beginning of the string containing delims
	while(true) {
		if(IsDelim(*s, delim)) {
			s++;
			continue;
		}

		if(*s == '\0') {
			return NULL; // we've reached the end of the string
		}

		// now, we've hit a regular character. Let's exit the
		// loop, and we'd need to give the caller a string
		// that starts here.
		//
		break;
	}


	char *ret = s;

	while(true) {
		if(*s == '\0') {
			p = s; // next exec will return NULL
			return ret;
		}

		if(IsDelim(*s, delim)) {
			*s = '\0';
			p = s + 1;
			return ret;
		}

		s++;
	}
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
        // Initialize result
        long long int res = 0;

        // Iterate through all characters
        // of input string and update result
        // take ASCII character of corresponding digit and
        // subtract the code from '0' to get numerical
        // value and multiply res by 10 to shuffle
        // digits left to update running total
        for (int i = 0; str[i] != '\0'; ++i)
                res = res * 10 + str[i] - '0';

        // return result.
        return res;
}
