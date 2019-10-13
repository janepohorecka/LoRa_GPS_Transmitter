#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h"
#include<Arduino.h>

//OLED pins to ESP32 GPIOs via this connection:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

SSD1306  display(0x3c, 4, 15);

// WIFI_LoRa_32 ports:
// GPIO 5  -- SX1278's SCK
// GPIO 19 -- SX1278's MISO
// GPIO 27 -- SX1278's MOSI
// GPIO 18 -- SX1278's CS
// GPIO 14 -- SX1278's RESET
// GPIO 26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6  //915E6 

int counter = 0;

void setup() {
  pinMode(25, OUTPUT); //Send success, LED will bright 1 second
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50);
  digitalWrite(16, HIGH);

  Serial.begin(115200);
  //while (!Serial); //If just the the basic function, must connect to a computer
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(5, 5, "LoRa Sender");
  display.display();

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setSyncWord(0xF3);
  display.drawString(5, 20, "LoRa Initializing OK!");
  display.display();
  delay(2000);
}
void loop() {

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hi Jane!");
  display.drawString(0, 10, "Sending Packet: " + String(counter) );
  display.drawString(0, 20, "That contains:");
  display.drawString(0, 30, "Long:");
  display.drawString(0, 40, "Lat:");
  display.drawString(0, 50, "Elevation:");
  display.display();

  LoRa.beginPacket();
  LoRa.print("Hi! You received packet: ");
  LoRa.print(counter);
  LoRa.print("That contains longitude, latitude and elevation!");
  LoRa.endPacket();


  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // pause for one second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // pause for one second
  delay(3000);
}
