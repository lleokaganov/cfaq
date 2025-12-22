#include "keys.h"
#include "Arduino.h"
#include "module/crypto25519/crypto25519.h"

UserKey* keys = nullptr;
size_t keys_count = 0;

// My own keys
uint8_t x_secret[32];
uint8_t x_public[32];
uint8_t ed_secret[64];
uint8_t ed_public[32];

auto is_hex = [](char c){ return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'); };

bool import_my_keys(const String& s) {
    int pos = s.indexOf('.');
    if (pos < 0) return false;
    String seed_x  = s.substring(0, pos);
    String seed_ed = s.substring(pos + 1);
    for (int i = 0; i < 64; i++) { if (!is_hex(seed_x[i]) || !is_hex(seed_ed[i])) return false; }
    crypto25519_x_secret(seed_x, x_secret);
    crypto25519_x_public(x_secret, x_public);
    crypto25519_ed_secret(seed_ed, ed_secret);
    crypto25519_ed_public(seed_ed, ed_public);
    return true;
}

bool import_key(const String& s) {
  int pos1 = s.indexOf('-');
  int pos2 = s.indexOf('-', pos1 + 1);
  if (pos1 < 0 || pos2 < 0) return false;
  uint32_t id = (uint32_t)strtoul(s.substring(0, pos1).c_str(), nullptr, 10);
  String key_x  = s.substring(pos1 + 1, pos2);
  String key_ed = s.substring(pos2 + 1);
  if (key_x.length() != 64 || key_ed.length() != 64) return false;
  for (int i = 0; i < 64; i++) { if (!is_hex(key_x[i]) || !is_hex(key_ed[i])) return false; }
  uint8_t x[32];
  uint8_t ed[32];
  hex2bin(x, key_x);
  hex2bin(ed, key_ed);
  key_add(id, x, ed);
  return true;
}

String bin2hex(const uint8_t* data, size_t length) {
  String s; char buf[3];
  for (size_t i = 0; i < length; i++) { sprintf(buf, "%02X", data[i]); s += buf; }
  return s;
}

void hex2bin(uint8_t *out, const String &hex) {
  size_t len = hex.length() / 2;
  for (size_t i = 0; i < len; i++) {
    char buf[3] = { hex[2 * i], hex[2 * i + 1], 0 };
    out[i] = strtoul(buf, nullptr, 16);
  }
}

UserKey* key_find(uint32_t id) {
    for (size_t i = 0; i < keys_count; i++) {
        if (keys[i].user_id == id) return &keys[i];
    }
    return nullptr;
}

bool key_add(uint32_t id, const uint8_t* x, const uint8_t* ed) {
    // обновление
    UserKey* k = key_find(id);
    if (k) {
        memcpy(k->x, x, 32);
        memcpy(k->ed, ed, 32);
        return true;
    }

    // новый — увеличиваем массив
    UserKey* new_ptr = (UserKey*)realloc(keys, (keys_count + 1) * sizeof(UserKey));
    if (!new_ptr) return false; // не повезло, ESP32 устала

    keys = new_ptr;
    keys[keys_count].user_id = id;
    memcpy(keys[keys_count].x, x, 32);
    memcpy(keys[keys_count].ed, ed, 32);
    keys_count++;
    return true;
}
