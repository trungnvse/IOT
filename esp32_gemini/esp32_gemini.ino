/*******************************************************************************
* Include
*******************************************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
/*******************************************************************************
* Global variable
*******************************************************************************/
const char* ssid = "name of wife";
const char* password = "password of wifi";
const char* g_apikey = "API KEY";
const char* g_maxtoken = "100";
String g_question = "";
/*******************************************************************************
* Code
*******************************************************************************/
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  while (!Serial);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println("connected");
  Serial.print("IP addg_questions: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  Serial.println("");
  Serial.println("Question : ");
  while (!Serial.available())
    ;
  while (Serial.available()) {
    char add = Serial.read();
    g_question = g_question + add;
    delay(1);
  }
  int len = g_question.length();
  g_question = g_question.substring(0, (len - 1));
  g_question = "\"" + g_question + "\"";
  Serial.println("");
  Serial.print("Question : ");
  Serial.println(g_question);

  HTTPClient https;

  if (https.begin("https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key=" + (String)g_apikey)) { 

    https.addHeader("Content-Type", "application/json");
    String payload = String("{\"contents\": [{\"parts\":[{\"text\":" + g_question + "}]}],\"generationConfig\": {\"maxOutputTokens\": " + (String)g_maxtoken + "}}");
    
    int httpCode = https.POST(payload);

    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      String payload = https.getString();

      DynamicJsonDocument doc(1024);

      deserializeJson(doc, payload);
      String Answer = doc["candidates"][0]["content"]["parts"][0]["text"];

      Answer.trim();
      String filteredAnswer = "";
      for (size_t i = 0; i < Answer.length(); i++) {
        char c = Answer[i];
        if (isalnum(c) || isspace(c)) {
          filteredAnswer += c;
        } else {
          filteredAnswer += ' ';
        }
      }
      Answer = filteredAnswer;


      Serial.println("");
      Serial.println("Answer: ");
      Serial.println("");
      Serial.println(Answer);
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  g_question = "";
}
/*******************************************************************************
* EOF
*******************************************************************************/
