

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
int temp_refresh_rate = 20000 ; // Set Data Refreshrate to MQTT Broker in ms
int light_refresh_rate = 100 ; // Set Data Refreshrate to MQTT Broker in ms
const int led = LED_BUILTIN;
String serialdata="" ;
boolean stringComplete = false;  // whether the string is complete
char str_array[20];
int ldr_1, ldr_2, ldr_3, ldr_4;
char ldr_a[5],ldr_b[5],ldr_c[5],ldr_d[5];

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "DESKTOP-RR394AO 1491";
const char* password = "fipasgay";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.137.50";

// Timers auxiliar variables
long now_temp = millis();
long lastMeasure_temp = 0;

long now_light = millis();
long lastMeasure_light = 0;


//======== Functions ========//
void setup_wifi(); // Connect ESP8266 to Router
void reconnect();
void callback(String topic, byte* message, unsigned int length) ;
void serialEvent(); // Receive ADC values from PIC24


void setup() {
  myHumidity.begin(); // Inicia Class do sensor de HTU21D
  
  pinMode(led, OUTPUT); // Feedback LED
  digitalWrite(led, HIGH); // Write to (HIGH IS OFF)
  
  Serial.begin(19200); // Begin Serial Comms
  setup_wifi(); // Call Start Wifi
  client.setServer(mqtt_server, 1883); // Start Client Server at @Port
  client.setCallback(callback); // ??? No idea #REVER#

  serialdata.reserve(200);
}

void loop() {
  // Trying to Connect Client to MQTT Broker
  if (!client.connected()) {
    reconnect();
  }

  // Connected With Name
  if(!client.loop())
    client.connect("ESP8266Client");

  now_light = millis();
  if (now_light - lastMeasure_light > light_refresh_rate) {
    lastMeasure_light = now_light;
    serialEvent();
    if (stringComplete) {
      //Serial.println(serialdata);
      serialdata.toCharArray(str_array, 22);
      //Serial.print("str_array=");
      //Serial.print(str_array);
      sscanf(str_array, "<%d,%d,%d,%d>", &ldr_1, &ldr_2, &ldr_3, &ldr_4);
      //Serial.printf("\nA:%d | B:%d | C:%d | D:%d |",ldr_1,ldr_2,ldr_3,ldr_4);
      //Serial.printf("\n%d,%d,%d,%d",ldr_1,ldr_2,ldr_3,ldr_4);
      // Linear Mapping Fix This for Exp Mapping
      ldr_1=map(ldr_1, 225, 2325, 0, 2000);
      ldr_2=map(ldr_2, 456, 3911, 0, 2000);
      ldr_3=map(ldr_3, 44, 3447, 0, 2000);
      ldr_4=map(ldr_4, 24, 3711, 0, 2000);
      //Serial.printf("\nA:%d | B:%d | C:%d | D:%d |",ldr_1,ldr_2,ldr_3,ldr_4);
      
      // clear the string:
      serialdata = "";
      stringComplete = false;
    }
    
    itoa(ldr_1,ldr_a,10);
    itoa(ldr_2,ldr_b,10);
    itoa(ldr_3,ldr_c,10);
    itoa(ldr_4,ldr_d,10);
//    Serial.print(" %\t Up: ");
//    Serial.print(ldr_a);
//    Serial.print(" | ");
//    Serial.print(ldr_b);
//    Serial.print(" | ");
//    Serial.print(ldr_c);
//    Serial.print(" | ");
//    Serial.print(ldr_d);
//    Serial.print(" | ");
    client.publish("room/light", ldr_a);
    client.publish("room/light2", ldr_b);
    client.publish("room/light3", ldr_c);
    client.publish("room/light4", ldr_d);
    
  }
  
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
    
//    Serial.print("\nHumidity: ");
//    Serial.print(h);
//    Serial.print(" %\t Temperature: ");
//    Serial.print(t);
//    Serial.print(" ºC ");
    
    // Publishes Temperature and Humidity values
    client.publish("room/temp", temperatureTemp);
    client.publish("room/humd", humidityTemp);
  }
}


//======== Functions ========//
//==== Serial Receive ====//
void serialEvent(){
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    serialdata += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

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
