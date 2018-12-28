

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SparkFunHTU21D.h"

//Create an instance of the object
HTU21D myHumidity;

// Initializes the ESP_Client.
WiFiClient espClient; //Change This name if you use multiple ESP's
PubSubClient client(espClient);


//======== Global Variables ========//
const int led = LED_BUILTIN;
int temp_refresh_rate = 1000 ; // Set Data Refreshrate to MQTT Broker in ms

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "DESKTOP-RR394AO 1491";
const char* password = "fipasgay";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.137.64";

// Timers auxiliar variables
long now_temp = millis();
long lastMeasure_temp = 0;


//======== Functions ========//
void setup_wifi(); // Connect ESP8266 to Router
void reconnect();
void callback(String topic, byte* message, unsigned int length) ;



void setup() {
  myHumidity.begin(); // Inicia Class do sensor de HTU21D
  
  pinMode(led, OUTPUT); // Feedback LED
  digitalWrite(led, HIGH); // Write to (HIGH IS OFF)
  
  Serial.begin(115200); // Begin Serial Comms
  setup_wifi(); // Call Start Wifi
  client.setServer(mqtt_server, 1883); // Start Client Server at @Port
  client.setCallback(callback); // ??? No idea #REVER#

}

void loop() {
  // Trying to Connect Client to MQTT Broker
  if (!client.connected()) {
    reconnect();
  }

  // Connected With Name
  if(!client.loop())
    client.connect("ESP8266Client");

  now_temp = millis();
  // Publishes new temperature and humidity every 30 seconds
  if (now_temp - lastMeasure_temp > temp_refresh_rate) {
    lastMeasure_temp = now_temp;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = myHumidity.readHumidity();
    // Read temperature as Celsius (the default)
    float t = myHumidity.readTemperature();
    
    unsigned int acq = analogRead(A0);
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
    static char temperatureTemp[7];
    dtostrf(t, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    static char adc_value[5];
    dtostrf(acq,6,2,adc_value);
    
    Serial.print("\nHumidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" ºC ");
    Serial.print(" %\t LDR Value: ");
    Serial.print(acq);
    
    // Publishes Temperature and Humidity values
    client.publish("room/temp", temperatureTemp);
    client.publish("room/humd", humidityTemp);
    client.publish("room/light", adc_value);
    
    
    
    // Serial.print(hif);
    // Serial.println(" *F");
  }
}


//======== Functions ========//

//==== Connect ESP8266 to Router ====//
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}


// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("\nMessage arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/led, you check if the message is either on or off. Turns the led GPIO according to the message
  if(topic=="room/led"){
      Serial.print("Changing Room led to ");
      if(messageTemp == "on"){
        digitalWrite(led, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "off"){
        digitalWrite(led, HIGH);
        Serial.print("Off");
      }
  }
  Serial.println();
}



// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("room/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
