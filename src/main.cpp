#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <freertos/FreeRTOS.h>
#include <Preferences.h>
#include "KeyInput.h"

// put function declarations here:

Preferences pref;


// Global variables
unsigned long globalmilisbuff_start;
unsigned long globalmilisbuff_end;
bool wifi_available;
const int connection_time_limit = 10000;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct WifiInfo {
  String ssid;
  int32_t rssi;
  wifi_auth_mode_t enc;
};


void setup() {

  entrypoint();

  auto networks = network_scan();

  // 1. for every network, try to login with prev credential
  if(!log_with_prev_credential(&networks)){
    // 2. prompt user for available networks and then require the user to input the credentials
    choose_network(&networks);
  }

}

void entrypoint(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
}

std::vector<WifiInfo> network_scan(){
  int amount_of_network = WiFi.scanNetworks();
  std::vector<WifiInfo> wifi_arr; 
  struct WifiInfo buffer;

  if(amount_of_network == 0){
    Serial.println("No networks found");
    wifi_available = false;
  } else {
    wifi_available = true;
  
    for(uint16_t i = 0; i < amount_of_network; i++){
      buffer.ssid = WiFi.SSID(i);
      buffer.rssi = WiFi.RSSI(i);
      buffer.enc = WiFi.encryptionType(i);
      
      wifi_arr.push_back(buffer);
    }
  }

  return wifi_arr;
}

bool log_with_prev_credential(std::vector<WifiInfo>* networks){
  if(!pref.begin("wifi-credentials", true)) {  // true = read-only
    Serial.println("Failed to open preferences");
    return false;
  }
  
  String prev_ssid = pref.getString("ssid", "");
  String prev_pass = pref.getString("password", "");
  pref.end();
  
  if(prev_ssid.length() == 0) {
    return false;
  }
  
  for(const WifiInfo& network : *networks){
    if(network.ssid.equals(prev_ssid)){
      Serial.println("Found saved network: " + prev_ssid);
      
      if(needpass(network.enc)){
        WiFi.begin(network.ssid, prev_pass);
      } else {
        WiFi.begin(network.ssid);  
      }
      
      // Wait for connection
      unsigned long start_time = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - start_time < connection_time_limit){
        delay(500);
        Serial.print(".");
      }
      
      if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to saved network!");
        return true;
      } else {
        Serial.println("\nFailed to connect to saved network");
        WiFi.disconnect();
        return false;
      }
    }
  }
  
  return false;  // Saved network not found in scan
}

void choose_network(std::vector<WifiInfo> *networks){
  // display networks, maybe per page, one wifi each page, then buttons for next and previous wifi selection
  // the page shows wifi name, network strength, locked by password or not
  
}


// helper
uint8_t needpass(wifi_auth_mode_t enc){
  switch(enc) {
    case WIFI_AUTH_OPEN:
        // Open network - no password needed
        return 0;
        break;
        
    case WIFI_AUTH_WEP:
    case WIFI_AUTH_WPA_PSK:
    case WIFI_AUTH_WPA2_PSK:
    case WIFI_AUTH_WPA_WPA2_PSK:
    case WIFI_AUTH_WPA3_PSK:
        return 1;
        break;
        
    case WIFI_AUTH_WPA2_ENTERPRISE:
        Serial.println("Enterprise networks not supported with simple WiFi.begin()");
        return 2;
        break;
        
    default:
        Serial.println("Unknown encryption type");
        return 100;
        break;
  }
}



void main(){

}