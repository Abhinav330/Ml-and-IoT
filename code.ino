#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
String readString;
#include <DHT.h>
const char* ssid = "";
const char* password = "";
const char* host = "script.google.com";
const int httpsPort = 443;
WiFiClientSecure client;
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
String GAS_ID = "";  // Replace by your GAS service id
#define DHTTYPE DHT11 
const int DHTPin = D1; 
DHT dht(DHTPin, DHTTYPE); 

void setup()
{ pinMode(D0, OUTPUT);
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }       
}
void loop()
{ digitalWrite(D0,HIGH);
  delay(500);
  digitalWrite(D0,LOW);
  int h = dht.readHumidity();
  float t = dht.readTemperature();
  float hic = dht.computeHeatIndex(t, h, false);
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor !");
    delay(500);
  }
  String Temp = "Temperature : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";
  String hici = "HeatIndex : " + String(hic) + "°C";
  Serial.println(Temp);
  Serial.println(Humi);
  Serial.println(hici);
  
   sendData(t,h,hic);
   delay(300000);
}
void sendData(float tem, int hum , float hic)
{
 client.setInsecure();
 Serial.print("connecting to ");
 Serial.println(host);
 if (!client.connect(host, httpsPort)) {
   Serial.println("connection failed");
   return;
 }
// if (client.verify(fingerprint, host)) {
// Serial.println("certificate matches");
// } else {
// Serial.println("certificate doesn't match");
// }
 String string_temp     =  String(tem);
 String string_hum      =  String(hum, DEC);
 String string_hicc      =  String(hic);
 
 String url = "/macros/s/" + GAS_ID + "/exec?temp=" + string_temp + "&hum=" + string_hum + "&Fl="+ string_hicc;
 Serial.print("requesting URL: ");
 Serial.println(url);
 client.print(String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "User-Agent: BuildFailureDetectorESP8266\r\n" +
        "Connection: close\r\n\r\n");
 Serial.println("request sent");
 while (client.connected()) {
 String line = client.readStringUntil('\n');
 if (line == "\r") {
   Serial.println("headers received");
   break;
 }
 }
 String line = client.readStringUntil('\n');
 if (line.startsWith("{\"state\":\"success\"")) {
 Serial.println("esp8266/Arduino CI successfull!");
 } else {
 Serial.println("esp8266/Arduino CI has failed");
 }
 Serial.println("reply was:");
 Serial.println("==========");
 Serial.println(line);
 Serial.println("==========");
 Serial.println("closing connection");
}
