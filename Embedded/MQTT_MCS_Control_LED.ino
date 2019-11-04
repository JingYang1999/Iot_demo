#include <LWiFi.h>
#include "PubSubClient.h"

#define R_PIN 17    // P17
#define G_PIN 16    // P16
#define O0_PIN 15    // P15
#define O1_PIN 14    // P14
#define O2_PIN 13    // P13

char ssid[] = "JINGYANG";
char password[] = "jingyang";
char mqtt_server[] = "mqtt.iisjy.cn";
char sub_topic[] = "x5ssl431";
char client_Id[] = "linkit-7697-431";

int status = WL_IDLE_STATUS;

WiFiClient mtclient;
PubSubClient client(mtclient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  // wait for serial port to connect. Needed for native USB port only
  //}
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(O0_PIN, OUTPUT);
  pinMode(O1_PIN, OUTPUT);
  pinMode(O2_PIN, OUTPUT);
  digitalWrite(R_PIN, 1);
  digitalWrite(G_PIN, 1);
  digitalWrite(O0_PIN, 1);
  digitalWrite(O1_PIN, 1);
  digitalWrite(O2_PIN, 1);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}


void printWifiStatus() {                     //print Wifi status
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup_wifi() {                       //setup Wifi
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    bling(R_PIN);
    Serial.print(".");

  }
  LED_on(R_PIN);
  randomSeed(micros());
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void bling(int col) {
  digitalWrite(col, LOW);
  delay(250);
  digitalWrite(col, HIGH);
  delay(250);
}
void LED_on(int col) {
  digitalWrite(col, LOW);
}
void LED_off(int col) {
  digitalWrite(col, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) {   //MQTT sub
  if ((char)payload[0] == '0') {
    LED_off(O0_PIN);

  } else if ((char)payload[0] == '1') {
    LED_on(O0_PIN);

  } else if ((char)payload[0] == '2') {
    LED_off(O1_PIN);

  } else if ((char)payload[0] == '3') {
    LED_on(O1_PIN);

  } else if ((char)payload[0] == '4') {
    LED_off(O2_PIN);

  } else if ((char)payload[0] == '5') {
    LED_on(O2_PIN);

  } else {

    for (int i = 0; i < 4; i++) {
      bling(G_PIN);
    }
  }
}

void reconnect() {  //reconnect MQTT
  // Loop until we're reconnected
  while (!client.connected()) {
    // Create a random client ID
    String clientId = client_Id;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      // Once connected, publish an announcement...
      // ... and resubscribe
      LED_on(G_PIN);
      client.subscribe(sub_topic);
    } else {
      // Wait 5 seconds before retrying
      for (int i = 0; i < 10; i++) {
        bling(G_PIN);
      }
    }
  }
}
