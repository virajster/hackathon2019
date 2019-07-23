/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
char ssid[] = "MSFTGUEST";           // SSID of your home WiFi
char pass[] = "";            // password of your home WiFi

unsigned long askTimer = 0;

IPAddress server(10,106,169,142);       // the fix IP address of the server
WiFiClient client;

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "time1.google.com"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

int switchPin = 13;
int LedPin = 16;
int switchValue;


void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());*/
  pinMode(ledPin, OUTPUT);
  timeClient.begin();
  //pinMode(LedPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
}

void loop () {
  if (!digitalRead(switchPin)){
    timeClient.update();
    client.connect(server, 80);   // Connection to the server
    digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
    Serial.println(".");
    String time = timeClient.getFormattedTime();
    String id = "01";
    client.println("Hello server! I am Ultimaker "+id+" and I am distressed at "+time+"\r");  // sends the message to the server
    String answer = client.readStringUntil('\r');   // receives the answer from the sever
    Serial.println("from server: " + answer);
    client.flush();
    digitalWrite(ledPin, HIGH);
    delay(2000);                  // client will trigger the communication after two seconds
  }
}
