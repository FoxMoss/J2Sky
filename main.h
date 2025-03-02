#pragma once
#define JSMN_HEADER
#include "jsmn.h"
#include <string.h>

void login_callback();
void login_ui();
void error(char *error_str);

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}
