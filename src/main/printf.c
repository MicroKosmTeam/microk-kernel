#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <string.h>
#include <printf.h>

void putc(char c);
void puts(const char *s);

// Custom printf-like function using putc
void printk(const char* format, ...) {
    va_list args;
    va_start(args, format);

    while (*format) {
        if (*format == '%') {
            format++; // Move past the '%' character

            switch (*format) {
                case 'd':
                    {
                        int num = va_arg(args, int);
                        char str[12]; // Buffer to hold the integer string
                        itoa(num, str);
                        puts(str); // Output using puts
                        break;
                    }
		case 'x':
                    {
                        int num = va_arg(args, int);
                        char str[9]; // Buffer to hold the hexadecimal string (max 8 digits + null)
                        itoh(num, str);
                        puts(str); // Output using puts
                        break;
		    }
		case 'l':
                    if (*(format + 1) == 'd') {
                        format++; // Move past the 'l'
                        long num = va_arg(args, long);
                        char str[21]; // Buffer to hold the long integer string (max 20 digits + null)
                        ltoa(num, str);
                        puts(str); // Output using puts
                        break;
                    } else if (*(format + 1) == 'x') {
                        format++; // Move past the 'l'
                        long num = va_arg(args, long);
                        char str[17]; // Buffer to hold the long integer string (max 20 digits + null)
                        ltoh(num, str);
                        puts(str); // Output using puts
                        break;
                    }
		    break;
                case 's':
                    {
                        const char* str = va_arg(args, const char*);
                        puts(str); // Output using puts
                        break;
                    }
                default:
                    putc(*format); // Output the '%' if not a valid format specifier
                    break;
            }
        } else {
            putc(*format);
        }

        format++;
    }

    va_end(args);
}
