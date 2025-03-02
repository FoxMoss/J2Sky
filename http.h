#pragma once

#include "java/io.h"
#include <cibyl.h>
#include <java/lang.h>
#include <javax/microedition/io.h>
#include <javax/microedition/lcdui.h>
#include <javax/microedition/lcdui/game.h>
#include <resource-manager.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

char *get(char *url);
char *getjwt(char *url, char *jwt);
char *post(char *url, char *content, size_t content_len);
