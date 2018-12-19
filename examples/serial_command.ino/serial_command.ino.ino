String serialdata="" ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(0,INPUT);
  
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.println("Start");
  
}

void loop() {
  
  while(Serial.available()>0){
    delay(15);
    serialdata=Serial.readString(); // Read All char inputed
    //Serial.println(serialdata);
    serialdata.trim(); //cut string for compare == " "
  }
  
  if(serialdata=="on"){
    Serial.println("LED ON");
    digitalWrite(LED_BUILTIN,LOW); //Led on
    serialdata="";
  }
  if(serialdata=="off"){
    Serial.println("LED OFF");
    digitalWrite(LED_BUILTIN,HIGH); //Led off
    serialdata="";
  }
  
  delay(100);
}
