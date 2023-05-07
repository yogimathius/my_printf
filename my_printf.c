#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int is_null_terminated(const char *str) {
  return (strchr(str, '\0') != NULL);
}
void reverse(char *str, int len) {
  int i, j;
  for (i = 0, j = len - 1; i < j; i++, j--) {
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
  }
}

char *itoa(int value, char *str, int base) {
  // check for valid base
  if (base < 2 || base > 36) {
    *str = '\0';
    return str;
  }

  // handle negative values
  int is_negative = 0;
  if (value < 0) {
    is_negative = 1;
    value = -value;
  }

  // convert digits to string in reverse order
  int i = 0;
  while (value != 0) {
    int rem = value % base;
    str[i++] = (rem > 9) ? (rem - 10 + 'a') : (rem + '0');
    value /= base;
  }

  // add sign if necessary
  if (is_negative) {
    str[i++] = '-';
  }

  // add null terminator and reverse the string
  str[i] = '\0';
  reverse(str, i);

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
            count+=len;
            break;
        }
        if (is_null_terminated(s)) {
            int len = strlen(s);

            // printf("string found %s\n", s);
            write(1, s, len);
            count+=len;
            break;
        }
      case 'd': /* int */
          d = va_arg(args, int);
          char buffer[20];
          itoa(d, buffer, 10);
          int len1 = strlen(buffer);
          write(1, buffer, len1);
          count += len1;
          break;
      case 'c': /* char */
        /* Note: char is promoted to int. */
        c = va_arg(args, int);
        write(1, &c, sizeof(char));
        count++;
        break;
      }
    }
    i++;
  }
  va_end(args);
  return count;
}

int main(void) {
  // my_printf("Hello %s!\n", "QWASAR.IO");
  // my_printf("Hello %s %s!\n", "Mr", "Gaetan");
  int ret_val = my_printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o');
  printf("return value %d", ret_val);
  // my_printf("%c%c%c%c%c!\n", 'H', 'e', 'l', 'l', 'o');
  return 0;
}