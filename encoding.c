#include "encoding.h"
#include <stdio.h>

static char url_encoded_chars[] = "!#$&'()*+,/:;=?@[]";
size_t url_encoded_chars_len = 18;

static char hex_key[] = "0123456789ABCDEF";
char *char_to_hex(uint8_t c) {
  char *ret = (char *)malloc(3);

  ret[0] = '%';
  ret[1] = hex_key[(c & 0xF0) >> 4];
  ret[2] = hex_key[c & 0x0F];
  return ret;
}

char *url_encode(char *str, size_t str_len) {
  size_t new_size = str_len;

  for (size_t i = 0; i < str_len; i++) {
    for (size_t char_i = 0; char_i < url_encoded_chars_len; char_i++) {
      if (url_encoded_chars[char_i] == str[i]) {
        new_size += 2;
        break;
      }
    }
  }

  char *new_buf = (char *)calloc(new_size + 1, 1);

  size_t cursor = 0;
  for (size_t i = 0; i < str_len; i++) {
    for (size_t char_i = 0; char_i < url_encoded_chars_len; char_i++) {
      if (url_encoded_chars[char_i] == str[i]) {
        char *percent_encode = char_to_hex(str[i]);
        memcpy(new_buf + cursor, percent_encode, 3);
        free(percent_encode);

        cursor += 2;
        goto break_for;
      }
    }
    new_buf[cursor] = str[i];
  break_for:
    cursor++;
  }
  return new_buf;
}
