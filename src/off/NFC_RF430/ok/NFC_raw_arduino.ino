/* AddText
 * Example NFC application which posts a long sentence to the RF430CL330H
 * NFC Tag controller
 *
 * Written by Eric Brundick 2/10/2015
 */


#include <Wire.h>
#include "RF430CL.h"
#include "NDEF.h"
#include "NDEF_TXT.h"

#define RF430CL330H_BOOSTERPACK_RESET_PIN  14 // ---> 8
#define RF430CL330H_BOOSTERPACK_IRQ_PIN    13 // ---> 12

// Пин ESP-12
// SDA (Data)	GPIO4 (D2) ---> 15
// SCL (Clock)	GPIO5 (D1) ---> 14
/*
 1       3.3V
 2       -
 3       -
 4       -
 5       -
 6       -
 7       SCK
 8       RST
 9       - отрезан MISO / SCL
 10      - отрезан MOSI / SDA

 11      3.3V
 12      INT0
 13      отключен GND
 14      MISO / SCL
 15      MOSI / SDA
 16      -
 17      -
 18      -
 19      -
 20      GND
*/

RF430 nfc(RF430CL330H_BOOSTERPACK_RESET_PIN, RF430CL330H_BOOSTERPACK_IRQ_PIN);

NDEF_TXT msg;
uint8_t msgbuf[512];


void setup() {
  Serial.begin(115200);
  Serial.println("\n\n[+] Initializing I2C");
    Wire.begin();
  Serial.println("[+] Initializing NFC Tag");
    nfc.begin();
  Serial.println("[+] Declaring Text object with 128-byte buffer");
  NDEF_TXT t("en");  // NDEF_TXT object language = English
  uint8_t textbuf[129];
  t.setPayloadBuffer(textbuf, 128); // <-- Payload buffer contents always get NUL-terminated, so be sure
                                    // your allocated buffer is 1 byte longer than the stated maximum size.

  Serial.println("[+] Setting & appending values to buffer");

  // t.setText("Mary had a little lamb.\r\n");  // Could have used .println() here, it'd work the same!
  
  t.setText("Mary had a little lamb.\r\nLittle lamb, little lamb!\r\nMary had a little lamb,\r\nhis face was white as snow!\r\n");
// Could have used .println() here, it'd work the same! ДА ВОТ БЛЯТЬ НИ ХУЯ
  /*
  t.println("Little lamb, little lamb!");
  t.println("Mary had a little lamb,");
  t.println("his face was white as snow!");
  */
  // ^ NDEF_TXT implements write(), and the base NDEF class extends Print, which means you
  // may use .print(), .println() et al natively on it!  But only if .setPayloadBuffer() has been
  // run to give the object a general-purpose buffer to store written text.

  Serial.println("Writing Text object to NFC transceiver");
  int ndef_size = t.sendTo(nfc);  // Export Text NDEF object to RF430's SRAM

  Serial.println(String("Configuring NDEF message size: " + String(ndef_size) + " bytes"));
  nfc.setDataLength(ndef_size);

//  Serial.println("SRAM dump-");
//  DumpSRAM();

  Serial.println("Activating NFC transceiver-");
  nfc.enable();
  // NDEF Text object is now live and available over the air!

  Serial.println(String("Printing Text to Serial port: ["+String( t.getText() )+"]"));

  msg.setPayloadBuffer(msgbuf, 512);  // Set allocation buffer used to store incoming NDEF data

}

void loop() {
  if(nfc.loop()) {

    // If nfc.loop() returns true, nfc.disable() is implicitly run...
    if(nfc.isError()) {
      Serial.println("NFC transceiver reports its SRAM contents do not contain valid NDEF data.");
      DumpSRAM();
    }

    if(nfc.wasRead()) {
      Serial.println("NDEF tag was read!");
    }

    if(nfc.available()) {
      uint16_t len = nfc.getDataLength();
      Serial.println(String("NFC master has written a new tag: "+String(len)+" bytes"));
      Serial.println("NDEF TEXT:");
      int ret = msg.import(nfc);
      if(ret < 0) {
        Serial.println("ERROR importing NDEF TEXT data.  SRAM dump:");
        DumpSRAM();
      } else {
        Serial.println("Success!");
        Serial.print(String("Language code: "+String(msg.getLanguage())+" Text block:\n---------------------------------\n"));
        msgbuf[len]=0;
        Serial.print(msg.getText());
        Serial.print("\n---------------------------------\n");
      }
      nfc.flush();
    }

    nfc.enable();
  }

  // delay(1000);
  // Serial.println("NFC master");
}

void DumpSRAM() {
  uint8_t sram[128];

  nfc.readSRAM(0x0000, sram, 128);
  for (int i=0; i < 128; i++) {
    if (sram[i] < 0x10)
      Serial.print('0');
    Serial.print(sram[i], HEX);
    Serial.print(' ');
    if (i % 9 == 8)
      Serial.println();
  }
  Serial.println();
}
