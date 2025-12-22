#pragma once
#include <Arduino.h>
#include "h.hpp"

struct UserKey {
    uint32_t user_id;
    uint8_t x[32];
    uint8_t ed[32];
};

extern UserKey* keys;
extern size_t keys_count;

bool key_add(uint32_t id, const uint8_t* x, const uint8_t* ed);
UserKey* key_find(uint32_t id);

bool import_my_keys(const String& s);
bool import_key(const String& s);

String bin2hex(const uint8_t* data, size_t length);
void hex2bin(uint8_t *out, const String &hex);

extern uint8_t x_secret[32];
extern uint8_t x_public[32];
extern uint8_t ed_secret[64];
extern uint8_t ed_public[32];