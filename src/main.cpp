#include <Arduino.h>
#include <Wire.h>
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

void setup() {
  Serial.begin(115200);
  pref.begin("wifi-credentials", false);
  WiFi.mode(WIFI_STA);

  int amount_of_network = WiFi.scanNetworks();
  if(amount_of_network == 0){
    Serial.println("No networks found");
    wifi_available = false;
  } else {
    wifi_available = true;
    wifi_process(amount_of_network);
  }
  
  std::vector<std::tuple<const char *, uint32_t, wifi_auth_mode_t>> wifi_arr; 

  for(uint16_t i= 0; i < amount_of_network; i++){
    wifi_arr.push_back(std::make_tuple(WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.encryptionType(i)));
  }


  // THIS IS wifi_process function
  // trying to connect for each of the ssid available
  // for each ssid we try to find the password for it
  // then we try to connect to the wifi with the ssid and password pair
  // if connected we break the loop and finish the wifi finding process
  // if not connected, continue until we find the correct ssid and password pair
  // if we reach the end of the loop, and none of the ssid has the correct password pair-
  // we will loop again for each ssid that's 
  char ssidsave[52] = "";
  char passwordsave[52] = "";
  bool flagged = false;

  // looping every available wifi, if saved then connect with the available credential
  for (const auto& [ssid, rssi, enc] : wifi_arr){
    const char *truepass = pref.getString(ssid, "").c_str();
    if (strlen(truepass) == 0){
      continue;
    } else {
      strcpy(ssidsave, ssid);
      WiFi.begin(ssid, truepass);
      globalmilisbuff_start = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - globalmilisbuff_start < connection_time_limit){
        delay(1000);
        Serial.print(".");
      }
      globalmilisbuff_start = 0;
      
      if (WiFi.status() == WL_CONNECTED){
        // kalo ketemu dan benar break
        flagged = true;
        break;
      } else{
        // kalo salah coba ke yg lain dulu
        continue;
      }
    }
  }

  if(flagged == false){
    // prompt user untuk milih wifi mana yang mau di pake dan masukin credentialsnya

    // display untuk milih wifi

    // setelah pilih masukin password dengan button events untuk milih huruf per huruf
    const char* input = prompt_keyboard();
  }
  pref.putString("ssid", ssid);
  pref.putString("password", truepass);
  
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  xTaskCreate(main, "main", 10000, NULL, 1, NULL);
}

void main(){

}