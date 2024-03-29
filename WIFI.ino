#include<WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//wifi部分
const char* ssid =     "IRIS";
const char* password = "irishk*******";

//wifi初始化
void wifi_init (void)
{
static char en;
WiFi.begin(ssid, password);
Serial.printf("\r\nConnecting to %s\r\n",ssid);
while(en != WL_CONNECTED)
  {
    en=WiFi.status();
    delay(500);
    Serial.printf(".");
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("\r\nConnected to %s\r\n",ssid);
    Serial.printf("\r\nIP address: %s\r\n", WiFi.localIP().toString().c_str());
  }
}


void setup() 
{
  Serial.begin(115200);
  wifi_init();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://api.seniverse.com/v3/weather/now.json?key=SkFDCVetgzk_mLBSc&location=beijing&language=zh-Hans&unit=c");
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);

      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      JsonObject results = doc["results"][0];
//Json解析Location
      JsonObject location = results["location"];
      String cityName = location["name"];
      String country = location["country"];
      Serial.print("City: ");
      Serial.print(cityName);
      Serial.print(", Country: ");
      Serial.println(country);


      JsonObject now = results["now"];
      String weatherText = now["text"];
      String temperature = now["temperature"];
      Serial.print("Weather: ");
      Serial.print(weatherText);
      Serial.print(", Temperature: ");
      Serial.println(temperature);

      String lastUpdate = results["last_update"];
      Serial.print("Last Update: ");
      Serial.println(lastUpdate);
    }
    http.end();
  }
  delay(10000); // 每分钟更新一次天气数据
}