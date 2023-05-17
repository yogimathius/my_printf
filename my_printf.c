#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int is_null_terminated(const char *str) { return (strchr(str, '\0') != NULL); }

char *intToString(long num, char *str, int base) {
  int i = 0, sign = 0;

  // check if number is negative
  if (num < 0) {
    sign = 1;
    num = -num;
  }

  // Extract digits from the number and convert them to characters
  do {
    // Get the remainder of the number divided by the base
    int digit = num % base;

    // If digit < 10, convert it to a character by adding the ASCII value of '0'.
    // The ASCII value of '0' is 48, if digit = 5, the code adds 5 + 48 to get the ASCII value of '5'.
    if (digit < 10) {
      str[i++] = digit + '0';
    }
    // If digit >= 10, convert it to a character by subtracting 10 from the digit
    // to get the value of the second digit, and then adding the ASCII value of 'a'.
    // The ASCII value of 'a' is 97, if digit = 10, the code subtracts 10 from digit (10 - 10 = 0)
    // and adds 97 to get the ASCII value of 'a'.
    else {
      str[i++] = digit - 10 + 'a';
    }

    // Divide the number by the base to get the next digit
    // Since it's an int, the decimal value is not included
    // eg 12 / 10 = 1.2 = 1
    num /= base;
  } while (num > 0);

  if (sign) {
    str[i++] = '-';
  }

  str[i] = '\0';

  int len = i;
  for (i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
  }
  return str;
}

int write_char(char c) { return (write(1, &c, 1)); }

int my_printf(const char *format, ...) {
  int i = 0, length = 0, count = 0;
  int d, c;
  char *s;
  length = strlen(format);

  va_list args;
  va_start(args, format);

  while (i < length) {
    if (format[i] != '%') {
      write(1, &format[i], sizeof(format[i]));
      count++;
    } else {
      i++;
      switch (format[i]) {
      case 's': /* string */
        s = va_arg(args, char *);
        if (s == NULL) {
          int len = strlen("(null)");
          write(1, "(null)", len);
          count += len;
          break;
        }
        if (is_null_terminated(s)) {
          int len = strlen(s);
          write(1, s, len);
          count += len;
          break;
        }
      case 'd': /* int */
        d = va_arg(args, int);
        char buffer[20];
        intToString(d, buffer, 10);
        int len1 = strlen(buffer);
        write(1, buffer, len1);
        count += len1;
        break;
      case 'c': /* char */
        /* char is promoted to int. */
        c = va_arg(args, int);
        write(1, &c, sizeof(char));
        count++;
        break;
      case 'o': { /* octal */
        int u = va_arg(args, unsigned int);
        char buffer[20];
        intToString(u, buffer, 8);
        int len = strlen(buffer);
        write(1, buffer, len);
        count += len;
        break;
      }
      case 'u': { /* unsigned decimal */
        int u = va_arg(args, unsigned int);
        char buffer[20];
        intToString(u, buffer, 10);
        int len = strlen(buffer);
        write(1, buffer, len);
        count += len;
        break;
      }
      case 'x': { /* unsigned decimal */
        int u = va_arg(args, unsigned int);
        char buffer[20];
        intToString(u, buffer, 16); /* Use base 16 */
        int len = strlen(buffer);
        /* Convert string to uppercase */
        for (int i = 0; i < len; i++) {
          buffer[i] = toupper(buffer[i]);
        }
        write(1, buffer, len);
        count += len;
        break;
      }
      case 'p': { /* pointer */
        void *p = va_arg(args, void *);
        uintptr_t p_as_int = (uintptr_t)p;
        char buffer[20];
        intToString(p_as_int, buffer, 16);
        int len = strlen(buffer);

        /* Prefix with "0x" to indicate memory address */
        char output[22] = "0x";
        strncpy(output + 2, buffer, len);
        output[len + 2] = '\0';

        write(1, output, len + 2);
        count += len + 2;
        break;
      }
      }
    }
    i++;
  }
  va_end(args);
  return count;
}