#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Servo.h>

const char* ssid = "Sange & Yasha";
const char* password = "awesomegod321";

ESP8266WebServer server(80);
Servo myservo;
int sensorPin = A0;
int sensorPinValue;
int countB, countW;

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  myservo.attach(2);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/count", []() {
    String message = "Object Counts \n";
    message += "White: ";
    message += countW;
    message += "n";
    message += "Black:";
    message += countB;

    server.send(200, "text/plain", message);
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setdefault() {
  myservo.write(90);
  delay(15);
}

void loop() {
  server.handleClient();
  sensorPinValue = analogRead(sensorPin);
  delay(1000);
  if (sensorPinValue >= 500) {
    countW += 1;
    Serial.println("White color");
    myservo.write(0);
    delay(15);
    setdefault();
  }
  if (sensorPinValue <= 100) {
    countB += 1;
    Serial.println("Black color");
    myservo.write(180);
    delay(15);
    setdefault();
  }

}
