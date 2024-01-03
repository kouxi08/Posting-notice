#include <Arduino.h>
#include <HTTPClient.h>


#define WIFI_SSID "SSID"
#define WIFI_PWD "PASSWD"
#define USE_DISCORD
#define DISCORD_WEBHOOK "xxxxx"
#define PIN_SW 3

int flag = 0;

#ifdef USE_DISCORD
bool DiscordMessage(const char *text)
{
  bool ret = false;
  HTTPClient http;
  if(http.begin(DISCORD_WEBHOOK) == false) {
    return false;
  }
  http.addHeader("Content-Type", "application/json; charset=utf-8");
  int httpCode = http.POST(String("{\"content\": \"")+text+"\"}");
  if( (httpCode == 200) || (httpCode == 204) ){
    ret = true;
  }
  else {
    Serial.printf("Error %d\n", httpCode);
  }
  http.end();
  return ret;
}
#endif


void setup() {
    Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PWD);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
}

void loop() {
  int sensorValue = analogRead(PIN_SW);
  Serial.println(sensorValue);
  if (sensorValue <= 3000){
    if (flag == 0){
      #ifdef USE_DISCORD
      DiscordMessage("荷物が投函されました");
      #endif
      flag = 1;
    }
  }else if (sensorValue == 4095 && flag == 1)
  {
    flag = 0;
    Serial.println("reset");
  }

  delay(10000);

//WIFI接続が途切れた際にリセットする処理
  static int ncTimer = 0;
  if(WiFi.status() != WL_CONNECTED){
    ncTimer++;
    if (ncTimer > 60){
      ESP.restart();
    }
    delay(1000);
  }
  else{
    ncTimer = 0;
  }
  delay(10);
}

