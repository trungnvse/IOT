#define BLYNK_TEMPLATE_ID "TMPL68vcVhXqO"
#define BLYNK_TEMPLATE_NAME "Check with DHT11"
#define BLYNK_AUTH_TOKEN "QYb0gDSkL7N5IyUqi2NzKChateHM-fPE"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <DHT.h>

#define DHTPIN  15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "FPT 4G";
char pass[] = "21012024";

float hum = 0;
float temp = 0;

BlynkTimer timer;

void get_data_from_sensor()
{
  hum  = dht.readHumidity();
  temp = dht.readTemperature();
}

void Send_data_to_blynk()
{
  get_data_from_sensor();

  Blynk.virtualWrite(V0, hum);
  Blynk.virtualWrite(V1, temp);
}

void setup() {
  // put your setup code here, to run once:
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  timer.setInterval(2000L, Send_data_to_blynk);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}
