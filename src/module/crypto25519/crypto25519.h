#pragma once
#include "Arduino.h"

#include "sys/timer.h" // для UnixTime

#include <memory>

extern "C" {
  #include "monocypher.h"
  #include "monocypher-ed25519.h"
}

// принты
extern String bin2hex(const uint8_t* data, size_t length = 32);
extern void hex2bin(uint8_t *out, const String &hex);


// String generate_secret();
// String get_public(const String &secret);
// String encrypt_message(
//     const String &s, // сообщение
//     const String &public_to, // публичный кому
//     const String &secret_from, // секретный от кого
//     uint64_t nonce // nonce
// );

// String decrypt_message(
//     const String &s, // зашифрованное сообщение
//     const String &public_from, // публичный кому
//     const String &secret_to, // секретный от кого
//     uint64_t nonce
// );

// extern String bin2hex(const uint8_t* data, size_t length);
// extern void hex2bin(uint8_t *out, const String &hex);

// // -------------------------------

// String sign_message(const String &msg, const String &seed_ed);

// // ==========================================================================


#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
uint64_t get_unixtime();
// extern uint64_t UnixTime;
// extern uint64_t unixtime_remote;
// extern unsigned long unixtime_mark;

String crypto25519_seed(void);
void crypto25519_x_secret(String seed, uint8_t out_secret[32]);
void crypto25519_x_public(const uint8_t secret[32], uint8_t out_public[32]);
void crypto25519_ed_secret(String seed, uint8_t out_secret_ed[64]);
void crypto25519_ed_public(String seed, uint8_t out_public_ed[32]);
void crypto25519_nonce_from_u64(uint64_t n, uint8_t out_nonce[24]);
// void x25519_shared_key(const uint8_t my_secret[32], const uint8_t their_public[32], uint8_t out_shared[32]);
// void ed25519_sign(const uint8_t secret_ed[64], const uint8_t *msg, size_t msg_len, uint8_t signature[64]);
// int ed25519_verify(const uint8_t public_ed[32], const uint8_t *msg, size_t msg_len, const uint8_t signature[64]);

// encrypted = XChaCha20Poly1305(shared, nonce).encrypt(plaintext)
// out: ciphertext||tag (len + 16)
// return 1 = OK, 0 = fail
void crypto25519_encrypt(const uint8_t their_public[32], const uint8_t my_secret[32],
                    const uint8_t *data, size_t len, uint64_t nonce, uint8_t *encrypted); // len+16

bool crypto25519_decrypt(const uint8_t their_public[32], const uint8_t my_secret[32],
                    const uint8_t *encrypted, size_t len, uint64_t nonce, uint8_t *data);

// [ 8 bytes nonce LE ] [ ciphertext (len + 16) ] [ 64-byte signature ]
void crypto25519_encrypt_and_sign(
    const uint8_t *data, size_t len,
    const uint8_t x_my_secret[32],
    const uint8_t ed_my_secret[64],
    const uint8_t x_he_public[32],
    uint8_t *packet
);

// decrypt and verify packet
bool crypto25519_verify_and_decrypt(
    const uint8_t *packet, size_t len,
    const uint8_t x_my_secret[32],
    const uint8_t x_he_public[32],
    const uint8_t ed_he_public[32],
    uint64_t now,
    uint64_t max_nonce_skew,
    uint8_t *data
);

#ifdef __cplusplus
}
#endif
