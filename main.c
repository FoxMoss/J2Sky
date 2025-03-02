#include "main.h"
#include "encoding.h"
#include "http.h"
#include "json.h"
#define JSMN_HEADER
#include "jsmn.h"
#include "polyfill/cibylmem.h"
#include "polyfill/command_mgr.h"
#include "stdlib.h"
#include <cibyl.h>
#include <java/lang.h>
#include <javax/microedition/io.h>
#include <javax/microedition/lcdui.h>
#include <javax/microedition/lcdui/game.h>
#include <javax/microedition/midlet.h>
#include <resource-manager.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

NOPH_FormCommandMgr_t command_manager;
NOPH_Item_t identifier_box;
NOPH_Item_t password_box;

enum State {
  STATE_LOGIN,
  STATE_LOADING,
  STATE_MAIN,
  STATE_ERROR,
};
enum State current_state;

struct BskyPost {
  char *handle;
  char *text;
};
struct BskyPostContainer {
  struct BskyPost *posts;
  size_t len;
  char *cursor;
};

struct BskyPostContainer grab_feed(char *jwt) {
  char *section1 = "http://localhost:8000/https://bsky.social/xrpc/"
                   "app.bsky.feed.getFeed?feed=";
  char *decoded =
      "at://did:plc:z72i7hdynmk6r22z27h6tvur/app.bsky.feed.generator/whats-hot";
  char *section2 = url_encode(decoded, strlen(decoded));
  char *section3 = "&limit=10";

  char *combined = (char *)calloc(
      strlen(section1) + strlen(section2) + strlen(section3) + 1, 1);

  strcat(combined, section1);
  strcat(combined, section2);
  free(section2);
  strcat(combined, section3);

  char *return_data = getjwt(combined, jwt);
  printf("%s\n", return_data);

  struct BskyPost *posts;
  size_t posts_len;
  char *cursor;

  struct json_value_s *root = json_parse(return_data, strlen(return_data));
  struct json_object_s *object = (struct json_object_s *)root->payload;
  if (strcmp(object->start->name->string, "feed") == 0) {
    // feed: []
    struct json_array_s *feed =
        (struct json_array_s *)object->start->value->payload;

    posts = (struct BskyPost *)malloc(sizeof(struct BskyPost) * feed->length);
    posts_len = feed->length;

    struct json_array_element_s *object = feed->start;
    for (uint16_t object_index = 0; object_index < feed->length;
         object_index++) {

      struct json_object_s *object_value = object->value->payload;

      struct json_object_s *post = object_value->start->value->payload;

      struct json_object_element_s *key = post->start;
      for (uint16_t key_item = 0; key_item < post->length; key_item++) {
        if (strcmp(key->name->string, "record") == 0) {
          // feed[i]["record"]
          struct json_object_s *record = key->value->payload;
          struct json_object_element_s *record_key = record->start;
          for (uint16_t record_item = 0; record_item < record->length;
               record_item++) {
            if (strcmp(record_key->name->string, "text") == 0) {
              // feed[i]["record"]["text"]
              struct json_string_s *text = record_key->value->payload;
              posts[object_index].text = calloc(text->string_size + 1, 1);
              memcpy(posts[object_index].text, text->string, text->string_size);
            }
            record_key = record_key->next;
          }
        }
        if (strcmp(key->name->string, "author") == 0) {
          // feed[i]["post"]["author"]

          struct json_object_s *author = key->value->payload;
          struct json_object_element_s *author_key = author->start;
          for (uint16_t author_item = 0; author_item < author->length;
               author_item++) {

            if (strcmp(author_key->name->string, "handle") == 0) {
              // feed[i]["post"]["author"]["handle"]
              struct json_string_s *handle = author_key->value->payload;
              posts[object_index].handle = calloc(handle->string_size + 1, 1);
              memcpy(posts[object_index].handle, handle->string,
                     handle->string_size);
            }
            author_key = author_key->next;
          }
        }

        key = key->next;
      }

      object = object->next;
    }
  }
  if (strcmp(object->start->next->name->string, "cursor") == 0) {
    struct json_string_s *cursor_string = object->start->next->value->payload;
    cursor = calloc(cursor_string->string_size + 1, 1);
    memcpy(cursor, cursor_string->string, cursor_string->string_size);
  }

  free(return_data);
  free(root);
  return (struct BskyPostContainer){posts, posts_len, cursor};
}

void main_page(char *identifier, char *jwt) {
  current_state = STATE_MAIN;
  NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());

  NOPH_Form_t form = NOPH_Form_new("JavaSky");

  NOPH_Item_t str_item = NOPH_StringItem_new(identifier, "");
  NOPH_Form_append(form, str_item);

  struct BskyPostContainer container = grab_feed(jwt);

  for (size_t index = 0; index < container.len; index++) {

    NOPH_Item_t handle =
        NOPH_StringItem_new("---", container.posts[index].handle);
    NOPH_Form_append(form, handle);

    NOPH_Item_t post = NOPH_StringItem_new("---", container.posts[index].text);
    NOPH_Form_append(form, post);

    NOPH_Item_t padding = NOPH_StringItem_new("---", "");
    NOPH_Form_append(form, padding);

    free(container.posts[index].handle);
    free(container.posts[index].text);
  }

  NOPH_Display_setCurrent(display, form);
  free(container.posts);

  free(identifier);
  free(jwt);
}

void error(char *error_str) {
  current_state = STATE_ERROR;
  NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());

  NOPH_Form_t form = NOPH_Form_new("JavaSky");

  NOPH_Item_t str_item = NOPH_StringItem_new("Error: ", error_str);
  NOPH_Form_append(form, str_item);
  NOPH_Display_setCurrent(display, form);

  command_manager = NOPH_FormCommandMgr_new(form);
  NOPH_FormCommandMgr_addCommand(command_manager, 0, 1);
}

void loading_ui() {
  current_state = STATE_LOADING;
  NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());

  NOPH_Form_t form = NOPH_Form_new("J2Sky Loading");

  NOPH_Gauge_t gauge = NOPH_Gauge_new("Loading...", 1, 10, 0);
  NOPH_Form_append(form, gauge);
  NOPH_Display_setCurrent(display, form);
}

void login_callback() {
  loading_ui();

  char *create_session = "http://localhost:8000/https://bsky.social/xrpc/"
                         "com.atproto.server.createSession";

  int identifier_size = NOPH_TextField_size(identifier_box) + 1;
  char *identifier = (char *)calloc(identifier_size, 1);
  NOPH_TextField_getCString(identifier_box, identifier, identifier_size);

  int password_size = NOPH_TextField_size(password_box) + 1;
  char *password = (char *)calloc(password_size, 1);
  NOPH_TextField_getCString(password_box, password, password_size);

  char chunk1[] = "{\"identifier\": \"";
  char chunk2[] = "\", \"password\": \"";
  char chunk3[] = "\"}";
  char *data =
      (char *)calloc(strlen(chunk1) + identifier_size + strlen(chunk2) +
                         password_size + strlen(chunk3) + 1,
                     1);
  memcpy(data, chunk1, strlen(chunk1));
  strcat(data, identifier);
  strcat(data, chunk2);
  strcat(data, password);
  strcat(data, chunk3);

  char *return_data = post(create_session, data, strlen(data));

  free(password);
  free(data);

  jsmn_parser parser;
  jsmntok_t json_tokens[128];
  jsmn_init(&parser);
  size_t json_tokens_len =
      jsmn_parse(&parser, return_data, strlen(return_data), json_tokens, 128);
  for (size_t index = 0; index < json_tokens_len; index++) {
    if (jsoneq(return_data, &json_tokens[index], "message") == 0) {
      char *error_message = (char *)calloc(
          json_tokens[index + 1].end - json_tokens[index + 1].start, 1);
      memcpy(error_message, return_data + json_tokens[index + 1].start,
             json_tokens[index + 1].end - json_tokens[index + 1].start);
      error(error_message);

    } else if ((jsoneq(return_data, &json_tokens[index], "accessJwt") == 0)) {
      char *jwt = (char *)calloc(
          json_tokens[index + 1].end - json_tokens[index + 1].start, 1);
      memcpy(jwt, return_data + json_tokens[index + 1].start,
             json_tokens[index + 1].end - json_tokens[index + 1].start);
      main_page(identifier, jwt);
    }
  }

  // free(identifier);
}

void login_ui() {
  current_state = STATE_LOGIN;
  NOPH_Display_t display = NOPH_Display_getDisplay(NOPH_MIDlet_get());

  NOPH_Form_t form = NOPH_Form_new("J2Sky Login");
  command_manager = NOPH_FormCommandMgr_new(form);
  NOPH_FormCommandMgr_addCommand(command_manager, 0, 0);

  identifier_box =
      NOPH_TextField_new("Identifier:", "", 256, NOPH_TextField_ANY);
  password_box =
      NOPH_TextField_new("Password:", "", 256, NOPH_TextField_PASSWORD);

  NOPH_Form_append(form, identifier_box);
  NOPH_Form_append(form, password_box);
  NOPH_Display_setCurrent(display, form);
}

void key_press(int key) {
  printf("%i\n", key);
  if (current_state == STATE_LOADING && key == NOPH_Canvas_FIRE) {
    login_callback();
  }
}

int main() {
  NOPH_FormCommandMgr_Setup(0);
  void *cb_login_callback =
      NOPH_registerCallback("login_callback", login_callback);
  void *cb_login_ui = NOPH_registerCallback("login_ui", login_ui);

  printf("Memory: %i", NOPH_CibylMem_memorySize());

  login_ui();

  return 0;
}
