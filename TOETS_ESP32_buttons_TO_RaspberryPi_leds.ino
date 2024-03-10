///////////////////////////////////////////////////////////////////////////////
////YOU MUST CHANGE THE WIFI PASSWORD AND SSID TO YOUR OWN FOR IT TO WORK./////
//Also, don't forget to change the 3 others, change in the python code aswell//
///////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "embed";                   //(change to YOUR) internet and password
const char* password = "weareincontrol";      //
const char* mqtt_server = "senne3030.local";  //(change to) Ip adress or the name of your raspberry pi
const int mqtt_port = 1883;                   // Port for MQTT communication
const char* mqtt_user = "senne";              //(change to) your MQTT username
const char* mqtt_password = "senne123";       //(change to) your MQTT password

const int YellowButton1 = 19;  //name the buttons (and give them)
const int BlueButton = 21;
const int YellowButton2 = 22;
const int RedButton = 23;

WiFiClient espClient;
PubSubClient client(espClient);

bool YellowButton1State = false;  //Set all the States to false for in the begining so it just starts off.
bool BlueButtonState = false;
bool YellowButton2State = false;
bool RedButtonState = false;

void setup() {
  Serial.begin(9600);                        //start serial comunication
  setup_wifi();                              //start wifi comunication
  client.setServer(mqtt_server, mqtt_port);  //set server for comunication

  pinMode(YellowButton1, INPUT_PULLUP);  // Set button pinmodes
  pinMode(BlueButton, INPUT_PULLUP);
  pinMode(YellowButton2, INPUT_PULLUP);
  pinMode(RedButton, INPUT_PULLUP);
}

void loop() {
  if (!client.connected()) {  //reconnect if it's not connected anymore
    reconnect();
  }
  client.loop();  //loop the client at all times

  if (digitalRead(YellowButton1) == LOW) {                                    // If this button is pressed turn the led on or off.
    YellowButton1State = !YellowButton1State;                                 //state to determene if the led has to be on or off.
    send_message("huis/woonkamer/licht", YellowButton1State ? "on" : "off");  //if topic is huis/woonkamer/licht check YellowButton1State, if high, on/ if low, off
    delay(500);                                                               // wait 0.5 seconds before pressing the button again.
  }

  if (digitalRead(BlueButton) == LOW) {                                 // If this button is pressed turn the led on or off.
    BlueButtonState = !BlueButtonState;                                 //state to determene if the led has to be on or off.
    send_message("huis/keuken/licht", BlueButtonState ? "on" : "off");  //if topic is huis/keuken/licht check BlueButtonState, if high, on/ if low, off
    delay(500);                                                         // wait 0.5 seconds before pressing the button again.
  }

  if (digitalRead(YellowButton2) == LOW) {                               // If this button is pressed turn the led on or off.
    YellowButton2State = !YellowButton2State;                            //state to determene if the led has to be on or off.
    send_message("huis/gang/licht", YellowButton2State ? "on" : "off");  //if topic is huis/gang/licht check YellowButton2State, if high, on/ if low, off
    delay(500);                                                          // wait 0.5 seconds before pressing the button again.
  }

  if (digitalRead(RedButton) == LOW) {                                     // If this button is pressed turn the led on or off.
    RedButtonState = !RedButtonState;                                      //state to determene if the led has to be on or off.
    send_message("huis/slaapkamer/licht", RedButtonState ? "on" : "off");  //if topic is huis/slaapkamer/licht check RedButtonState, if high, on/ if low, off
    delay(500);                                                            // wait 0.5 seconds before pressing the button again.
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  // connect the ESP32 to your wifi network

  while (WiFi.status() != WL_CONNECTED) {  // while it's not connected print "." in the serial monitor
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  //this is the IP adress, print it in serial monitor
}

void send_message(const char* topic, const char* message) {
  if (!client.publish(topic, message)) {  //if we failed to publish for some reason it says it in the serial monitor
    Serial.println("Failed to publish message.");
  }
}

void reconnect() {  //try to connect if the client isn't connected.
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {  //noemt de client ESP32Client en connecteerd met de username en password
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
