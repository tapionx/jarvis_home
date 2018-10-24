#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "XXXXXXX";
const char* password = "XXXXXXX";

#define RELAY_GATE_PIN 14
#define RELAY_GARAGE_PIN 4 
#define RELAY_DOOR_PIN 5 

IPAddress ip(192, 168, 1, 3);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

const int led = 13;

void openGate(int gate, int timeout) {
  digitalWrite(led, 1);
  digitalWrite(gate, HIGH);
  server.send(200, "text/plain", "YES\n");
  delay(timeout);
  digitalWrite(gate, LOW);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(RELAY_GARAGE_PIN, OUTPUT);
  pinMode(RELAY_DOOR_PIN, OUTPUT);
  pinMode(RELAY_GATE_PIN, OUTPUT);
  digitalWrite(RELAY_GARAGE_PIN, LOW);
  digitalWrite(RELAY_DOOR_PIN, LOW);
  digitalWrite(RELAY_GATE_PIN, LOW);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/door", HTTP_POST, [](){
    openGate(RELAY_DOOR_PIN, 500);
  });

  server.on("/gate", HTTP_POST, [](){
    openGate(RELAY_GATE_PIN, 500);
  });
  
  server.on("/garage", HTTP_POST, [](){
    openGate(RELAY_GARAGE_PIN, 1000);
  });

  server.on("/house", HTTP_POST, [](){
    openGate(RELAY_GATE_PIN, 500);
    openGate(RELAY_DOOR_PIN, 500);
  });

  server.onNotFound([](){
    server.send(200, "text/plain", "404");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
