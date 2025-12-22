#include "crypto25519.h"

// // Генерация ключей
// String generate_secret() {
//   uint8_t sk[32]; uint32_t *p = (uint32_t *)sk; // интерпретируем массив как 8 слов по 4 байта
//   for (int i = 0; i < 8; i++) p[i] = esp_random();
//   return bin2hex(sk, 32);
// }

// String get_public(const String &secret) {
//   uint8_t sk[32]; hex2bin(sk, secret);
//   uint8_t pk[32]; crypto_x25519_public_key(pk, sk);
//   return bin2hex(pk, 32);
// }

// void make_nonce_buf(uint8_t buf[24], uint64_t nonce) {
//   memcpy(buf,      &nonce, 8);
//   memcpy(buf + 8,  &nonce, 8);
//   memcpy(buf + 16, &nonce, 8);
// }

// String encrypt_message(
//     const String &s, // сообщение
//     const String &public_to, // публичный кому
//     const String &secret_from, // секретный от кого
//     uint64_t nonce // nonce
// ) {
//   uint8_t nonce_buf[24]; make_nonce_buf(nonce_buf, nonce);
//   uint8_t public_buf[32]; hex2bin(public_buf, public_to);
//   uint8_t secret_buf[32]; hex2bin(secret_buf, secret_from);
//   uint8_t shared_key[32]; crypto_x25519(shared_key, secret_buf, public_buf);

//   size_t len = s.length();
//   std::unique_ptr<uint8_t[]> data(new uint8_t[len]);
//   uint8_t mac[16];
//   crypto_aead_lock(data.get(), mac, shared_key, nonce_buf, NULL, 0, (const uint8_t *)s.c_str(), len);
//   return bin2hex(mac,16)+bin2hex(data.get(), len);
// }

// String decrypt_message(
//     const String &s, // зашифрованное сообщение
//     const String &public_from, // публичный кому
//     const String &secret_to, // секретный от кого
//     uint64_t nonce
// ) {
//   uint8_t nonce_buf[24]; make_nonce_buf(nonce_buf, nonce);
//   uint8_t public_buf[32]; hex2bin(public_buf, public_from);
//   uint8_t secret_buf[32]; hex2bin(secret_buf, secret_to);
//   uint8_t shared_key[32]; crypto_x25519(shared_key, secret_buf, public_buf);
//   // разделение mac и data
//   size_t n = s.length() / 2;
//   std::unique_ptr<uint8_t[]> all(new uint8_t[n]); hex2bin(all.get(), s);
//   uint8_t *mac  = all.get();         // первые 16 байт
//   uint8_t *data = all.get() + 16;    // всё остальное

//   // расшифровка
//   std::unique_ptr<uint8_t[]> decrypted(new uint8_t[n-16 + 1]); decrypted[n-16] = 0;
//   int ok = crypto_aead_unlock(decrypted.get(), mac, shared_key, nonce_buf, NULL, 0, data, n-16);

//   if (ok != 0) return "ERROR";
//   return String((char *)decrypted.get());
// }


// ======================================================

// uint64_t unixtime_remote = 0;
// unsigned long unixtime_mark = 0;

uint64_t get_unixtime() { 
  return UnixTime;
  // return (uint64_t)unixtime_remote + (millis() - unixtime_mark)/1000;
};

String crypto25519_seed(void) {
  uint8_t buf[32];
  // for (int i = 0; i < 8; i++) p[i] = esp_random();
  for (int i = 0; i < 8; i++) ((uint32_t*)buf)[i] = esp_random();
  return bin2hex(buf, 32);
}

void crypto25519_x_secret(String seed, uint8_t out_secret[32]) {
  uint8_t sk[32];
  hex2bin(sk, seed);
  sk[0]  &= 248;
  sk[31] &= 127;
  sk[31] |= 64;
  memcpy(out_secret, sk, 32);
}

void crypto25519_x_public(const uint8_t secret[32], uint8_t out_public[32]) {
    crypto_x25519_public_key(out_public, secret);
}

void crypto25519_ed_secret(String seed, uint8_t out_secret_ed[64]) {
    uint8_t seed_bin[32];
    uint8_t pk[32];
    hex2bin(seed_bin, seed);
    crypto_ed25519_key_pair(out_secret_ed, pk, seed_bin);
}

void crypto25519_ed_public(String seed, uint8_t out_public_ed[32]) {
    uint8_t seed_bin[32];
    uint8_t sk[64];
    hex2bin(seed_bin, seed);
    crypto_ed25519_key_pair(sk, out_public_ed, seed_bin);
}

void crypto25519_nonce_from_u64(uint64_t n, uint8_t out_nonce[24]) {
  memcpy(out_nonce,      &n, 8);
  memcpy(out_nonce + 8,  &n, 8);
  memcpy(out_nonce + 16, &n, 8);
}

void crypto25519_encrypt(const uint8_t their_public[32], const uint8_t my_secret[32],
  const uint8_t *data, size_t len, uint64_t nonce, uint8_t *encrypted // len + 16
) {
    uint8_t shared[32]; crypto_x25519(shared, my_secret, their_public);
    uint8_t nonce_buf[24]; crypto25519_nonce_from_u64(nonce, nonce_buf);   
    uint8_t mac[16]; crypto_aead_lock(encrypted, mac, shared, nonce_buf, NULL, 0, data, len);
    memcpy(encrypted + len, mac, 16);
}

bool crypto25519_decrypt(const uint8_t their_public[32], const uint8_t my_secret[32],
  const uint8_t *encrypted, size_t len, uint64_t nonce, uint8_t *data // len - 16
) {
    if (len < 16) return 0;
    uint8_t shared[32]; crypto_x25519(shared, my_secret, their_public);
    uint8_t nonce_buf[24]; crypto25519_nonce_from_u64(nonce, nonce_buf);
    const uint8_t *cipher = encrypted;
    const uint8_t *mac    = encrypted + (len - 16);
    int ok = crypto_aead_unlock( data, mac, shared, nonce_buf, NULL, 0, cipher, len - 16 ); // 0 OK, -1 error
    return (ok == 0);
}

void crypto25519_encrypt_and_sign( const uint8_t *data, size_t len,
    const uint8_t x_my_secret[32],
    const uint8_t ed_my_secret[64],
    const uint8_t x_he_public[32],
    uint8_t *packet // 8 + len + 16 + 64
){
    uint64_t nonce = get_unixtime();
    memcpy(packet, &nonce, 8);
    uint8_t *cipher = packet + 8;
    crypto25519_encrypt(x_he_public, x_my_secret, data, len, nonce, cipher);
    size_t signed_len = 8 + (len + 16);
    uint8_t *sig = packet + signed_len;
    crypto_ed25519_sign(sig, ed_my_secret, packet, signed_len);
}

bool crypto25519_verify_and_decrypt(
    const uint8_t *packet, size_t len,
    const uint8_t x_my_secret[32],
    const uint8_t x_he_public[32],
    const uint8_t ed_he_public[32],
    uint64_t now, uint64_t max_nonce_skew,
    uint8_t *data // длина: len - 8 - 16 - 64
){
    if (len < 8 + 16 + 64) return false;
    uint64_t nonce; memcpy(&nonce, packet, 8);
    if (max_nonce_skew > 0 && ( (now > nonce) ? (now - nonce) : (nonce - now) ) > max_nonce_skew ) return false;
    size_t cipher_with_tag_len = len - 8 - 64;
    const uint8_t *cipher_with_tag = packet + 8;
    const uint8_t *signature = packet + (len - 64);
    size_t signed_len = len - 64;
    if (crypto_ed25519_check(signature, ed_he_public, packet, signed_len) != 0) return false;
    size_t plaintext_len = cipher_with_tag_len - 16;
    return crypto25519_decrypt(x_he_public, x_my_secret, cipher_with_tag, cipher_with_tag_len, nonce, data);
}
