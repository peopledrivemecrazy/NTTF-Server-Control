#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Sange & Yasha";
const char* password = "awesomegod321";

ESP8266WebServer server(80);
int motorIN1 = D1;
int motorIN2 = D2;
int motorDir;
int motorState = 0;

unsigned long previousMillis = 0;
const long interval = 5000;


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


void forward() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, HIGH);
  delay(5000);
  motorstop();
}

void backward() {
  digitalWrite(motorIN1, HIGH);
  digitalWrite(motorIN2, LOW);
  delay(5000);
  motorstop();
}

void motorstop() {
  digitalWrite(motorIN1, LOW);
  digitalWrite(motorIN2, LOW);
}
void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(motorIN1, OUTPUT);
  pinMode(motorIN2, OUTPUT);
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

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/open", []() {
    server.send(200, "text/plain", "this works as well");
    forward();
  });
  server.on("/close", []() {
    server.send(200, "text/plain", "this works as well");
    backward();
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
