#include "sys.h"

const char* ssid = "artng";
const char* password = "20022002";
int  wifi_count = 0;
/* setup function */
void setup(void) {

  Serial.begin(9600);
  Serial2.begin(9600);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Web_Setup();
}

void loop(void) {
  Web_Run();
  if(WiFi.status() != WL_CONNECTED) {
    wifi_count++;
    if(wifi_count == 10000)
    {
      ESP.restart();
      wifi_count = 0;
    }
  }
  delay(1);
}





