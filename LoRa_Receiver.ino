#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h"

SSD1306  display(0x3c, 4, 15);

// OLED pins to ESP32 GPIOs via this connection:
// OLED_SDA -- GPIO4
// OLED_SCL -- GPIO15
// OLED_RST -- GPIO16

// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ  (Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6

void setup() {
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);
  delay(50); 
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
  display.drawString(5,5,"LoRa Receiver"); 
  display.display();
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    display.drawString(5,25,"Starting LoRa failed!");
    while (1);
  }
  display.drawString(5,25,"LoRa Initializing OK!");
  display.display();
}
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Received packet:");
    display.display();
    while (LoRa.available()) {
    String data = LoRa.readString();
    display.drawString(0,10, data);
    display.display();
    }
    Serial.print(" with RSSI ");
    Serial.println(LoRa.packetRssi());
    display.drawString(0, 20, "RSSI:  " + (String)LoRa.packetRssi());
    display.display();
  }
}
