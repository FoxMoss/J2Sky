#include "http.h"
#include "cibyl.h"
#include "java/io.h"
#include "javax/microedition/io.h"
#include "main.h"
#include "polyfill/cibylmem.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOPH_custom_try(callback, arg)                                         \
  do {                                                                         \
    asm volatile("");                                                          \
    __NOPH_try(callback, arg);                                                 \
  } while (0);                                                                 \
  if (*(int *)0 != 0)

// not thread safe, this thing have threads?
void error_callback(NOPH_Exception_t exception, int *response_code) {
  NOPH_CibylMem_printException(exception);
  error("Error happened!");
}
char *get(char *url) {
  char *buffer = NULL;
  NOPH_try(error_callback, NULL) //

      NOPH_HttpConnection_t connection =
          NOPH_Connector_open(url); // TODO: port tls 1.3
  int response_code = NOPH_HttpConnection_getResponseCode(connection);
  NOPH_InputStream_t istream = NOPH_HttpConnection_openInputStream(connection);

  int size = NOPH_InputStream_available(istream);
  char *buffer = (char *)calloc(size + 1, 1);

  short eof;
  NOPH_InputStream_read_into(istream, buffer, size, &eof);
  NOPH_HttpConnection_close(connection);
  NOPH_catch();
  return buffer;
}
char *getjwt(char *url, char *jwt) {
  char *buffer = NULL;
  char *bearer = "Bearer ";
  char *auth = (char *)calloc(strlen(bearer) + strlen(jwt) + 1, 1);
  strcat(auth, bearer);
  strcat(auth, jwt);
  NOPH_try(error_callback, NULL) //

      NOPH_HttpConnection_t connection =
          NOPH_Connector_open(url); // TODO: port tls 1.3

  NOPH_HttpConnection_setRequestProperty(connection, "authorization", auth);

  int response_code = NOPH_HttpConnection_getResponseCode(connection);
  NOPH_InputStream_t istream = NOPH_HttpConnection_openInputStream(connection);

  int size = NOPH_InputStream_available(istream);
  buffer = (char *)calloc(size + 1, 1);
  short eof;
  NOPH_InputStream_read_into(istream, buffer, size, &eof);

  NOPH_HttpConnection_close(connection);
  NOPH_catch();
  free(auth);
  return buffer;
}

char *post(char *url, char *content, size_t content_len) {
  char *buffer = NULL;
  NOPH_try(error_callback, NULL) NOPH_HttpConnection_t connection =
      NOPH_Connector_open(url); // TODO: port tls 1.3
  NOPH_HttpConnection_setRequestMethod(connection, "POST");
  NOPH_OutputStream_t stream = NOPH_HttpConnection_openOutputStream(connection);
  char size_str[256];
  snprintf(size_str, 256, "%i", content_len);
  for (size_t i; i < content_len; i++) {
    NOPH_OutputStream_write(stream,
                            content[i]); // i hate this more then you do
  }
  NOPH_OutputStream_flush(stream);

  int response_code;
  response_code = NOPH_HttpConnection_getResponseCode(connection);
  NOPH_InputStream_t istream = NOPH_HttpConnection_openInputStream(connection);

  int size = NOPH_InputStream_available(istream);
  buffer = (char *)calloc(size + 1, 1);
  short eof;
  NOPH_InputStream_read_into(istream, buffer, size, &eof);
  NOPH_HttpConnection_close(connection);
  NOPH_catch();
  return buffer;
}

char *postjwt(char *url, char *content, size_t content_len, char *jwt) {
  char *buffer = NULL;
  char *bearer = "Bearer ";
  char *auth = (char *)calloc(strlen(bearer) + strlen(jwt) + 1, 1);
  strcat(auth, bearer);
  strcat(auth, jwt);

  NOPH_try(error_callback, NULL) NOPH_HttpConnection_t connection =
      NOPH_Connector_open(url); // TODO: port tls 1.3
  NOPH_HttpConnection_setRequestMethod(connection, "POST");
  NOPH_HttpConnection_setRequestProperty(connection, "authorization", auth);

  NOPH_OutputStream_t stream = NOPH_HttpConnection_openOutputStream(connection);

  char size_str[256];
  snprintf(size_str, 256, "%i", content_len);
  for (size_t i; i < content_len; i++) {
    NOPH_OutputStream_write(stream,
                            content[i]); // i hate this more then you do
  }
  NOPH_OutputStream_flush(stream);

  int response_code;
  response_code = NOPH_HttpConnection_getResponseCode(connection);
  NOPH_InputStream_t istream = NOPH_HttpConnection_openInputStream(connection);

  int size = NOPH_InputStream_available(istream);
  buffer = (char *)calloc(size + 1, 1);
  short eof;
  NOPH_InputStream_read_into(istream, buffer, size, &eof);
  NOPH_HttpConnection_close(connection);
  NOPH_catch();
  return buffer;
}
