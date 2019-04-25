#include <Arduino.h>
#include <ESP8266WiFi.h>

#define CMMC_USE_ALIAS CMMC

#include <CMMC_Utils.h>
#include <CMMC_ESPNow.h>

uint8_t self_mac[6];
uint8_t master_mac[6];
CMMC_ESPNow espNow;
String macStr = "240AC4004E42"; // nano32
// String macStr = "B4E62D8E5FF9";  // TTGO OLED
// uint8_t macUint = B4E62D8E5FF9;
String deviceName = "superman";
uint16_t data = 0;

#define LED 13

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  uint8_t *slave_addr = CMMC::getESPNowSlaveMacAddress();
  memcpy(self_mac, slave_addr, 6);

  Serial.printf("Loaded mac %s, name=%s\r\n", macStr.c_str(), deviceName.c_str());
  uint8_t mac[6];
  CMMC::convertMacStringToUint8(macStr.c_str(), mac);
  memcpy(master_mac, mac, 6);

  espNow.init(NOW_MODE_SLAVE);
  espNow.enable_retries(true);

  espNow.on_message_sent([](uint8_t *macaddr, u8 status) {

  });

  espNow.on_message_recv([](uint8_t *macaddr, uint8_t *data, uint8_t len) {
    Serial.printf("RECV: len = %u byte, sleepTime = %lu at(%lu ms)\r\n", len, data[0], millis());
  });
}

void loop()
{
  data++;

  espNow.send(master_mac, (u8 *)&data, sizeof(data), [&]() {
    // _go_sleep(_defaultDeepSleep_m);
  },
              1000);

  digitalWrite(LED, !digitalRead(LED));
  delay(200);
  digitalWrite(LED, !digitalRead(LED));

  Serial.print("data: ");
  Serial.println(data);
  delay(1000);
}