#include <FastLED.h>
#include <WiFi.h>
#include <esp_now.h>
#include "reactive_common.h"

#define READ_PIN 27
#define BUTTON_PIN 14

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

const int checkDelay = 5000;
const int buttonDoubleTapDelay = 200;
const int numOpModes = 3;

unsigned long lastChecked;
unsigned long buttonChecked;
bool buttonClicked = false;
bool queueDouble = false;
bool clickTrigger;
bool doubleTapped;
//WiFiUDP UDP;

struct led_command {
  uint8_t opmode;
  uint32_t data;
};

//bool heartbeats[NUMBER_OF_CLIENTS];

static int opMode = 1;

void setup()
{
  pinMode(READ_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT );

  /* WiFi Part */
  Serial.begin(115200);
  Serial.println();
  //Serial.print("Setting soft-AP ... ");
  //WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  //WiFi.softAP("sound_reactive", "123456789");
  //Serial.print("Soft-AP IP address = ");
  //Serial.println(WiFi.softAPIP());
  //UDP.begin(7171); 
  //resetHeartBeats();
  //waitForConnections();
  lastChecked = millis();
  buttonChecked = 0;

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // register peer
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  // register first peer  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}


void loop()
{
  uint32_t analogRaw;
  //buttonCheck();
    //lastChecked = millis();

  switch (opMode) {
    case 1:
      analogRaw = analogRead(READ_PIN);
      if (analogRaw <= 3)
        break;
      sendLedData(analogRaw, opMode);
      break;
    case 2:
      sendLedData(0, opMode);
      delay(10);
      break;
    case 3:
      sendLedData(0, opMode);
      delay(10);
      break;
  }
  delay(4);
}

void sendLedData(uint32_t data, uint8_t op_mode) 
{
 struct led_command send_data;
 send_data.opmode = op_mode; 
 send_data.data = data; 

 esp_now_send(0, (uint8_t *) &send_data, sizeof(struct led_command));
}

/*
void waitForConnections() {
  while(true) {
      readHeartBeat();
      if (checkHeartBeats()) {
        return;
      }
      delay(checkDelay);
      resetHeartBeats();
  }
}

void resetHeartBeats() {
  for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
   heartbeats[i] = false;
  }
}

void readHeartBeat() {
  struct heartbeat_message hbm;
 while(true) {
  int packetSize = UDP.parsePacket();
  if (!packetSize) {
    break;
  }
  UDP.read((char *)&hbm, sizeof(struct heartbeat_message));
  if (hbm.client_id > NUMBER_OF_CLIENTS) {
    Serial.println("Error: invalid client_id received");
    continue;
  }
  heartbeats[hbm.client_id - 1] = true;
 }
}


bool checkHeartBeats() {
  for (int i = 0; i < NUMBER_OF_CLIENTS; i++) {
   if (!heartbeats[i]) {
    return false;
   }
  }
  resetHeartBeats();
  return true;
}


void buttonCheck()
{
  int but = digitalRead(BUTTON_PIN);
  if (but == 0) {
    if (millis() - buttonChecked < buttonDoubleTapDelay && buttonClicked == false ) {
      doubleClicked();
      doubleTapped = true;
    }
    clickTrigger = true;
    buttonClicked = true; 
    buttonChecked = millis();
  }

  else if (but == 1) {
    if (millis() - buttonChecked > buttonDoubleTapDelay && clickTrigger) {
      if (!doubleTapped) {
        clicked();
      }
      clickTrigger = false;
      doubleTapped = false;
    }
    buttonClicked = false;
  }
}

void clicked() {
  if (opMode == numOpModes)
    opMode = 1;
  else
    opMode++;
  Serial.printf("Setting opmode %d \n", opMode);
}

void doubleClicked() {

}
*/
