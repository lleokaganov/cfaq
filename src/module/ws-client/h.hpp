#pragma once
#include "Arduino.h"

#include "module/crypto25519/crypto25519.h"
#include "ws-client.h"
#include "keys.h"
#include "sys/url.h"

extern LSocketyee ws;

void ws_onMessageText_cb(const String& s);
void ws_onMessageBin_cb(uint8_t cmd, uint32_t from, uint16_t msg_id, const String& body);
void ws_onConnect_cb();
void ws_onDisconnect_cb();
