
String serialdata="" ;
boolean stringComplete = false;  // whether the string is complete
char str_array[20];
int ldr_1, ldr_2, ldr_3, ldr_4;

void serialEvent();

void setup() {
  // initialize both serial ports:
  Serial.begin(19200);
  
  // reserve 200 bytes for the inputString:
  serialdata.reserve(200);
}



void loop() {
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    //Serial.println(serialdata);
    serialdata.toCharArray(str_array, 22);
    //Serial.print("str_array=");
    //Serial.print(str_array);
    sscanf(str_array, "<%d,%d,%d,%d>", &ldr_1, &ldr_2, &ldr_3, &ldr_4);
    //Serial.printf("\nA:%d | B:%d | C:%d | D:%d |",ldr_1,ldr_2,ldr_3,ldr_4);
    Serial.printf("\n%d,%d,%d,%d",ldr_1,ldr_2,ldr_3,ldr_4);
//    ldr_1=map(ldr_1, 225, 2325, 0, 100);
//    ldr_2=map(ldr_2, 456, 3911, 0, 100);
//    ldr_3=map(ldr_3, 44, 3447, 0, 100);
//    ldr_4=map(ldr_4, 24, 3711, 0, 100);
//    Serial.printf("\nA:%d | B:%d | C:%d | D:%d |",ldr_1,ldr_2,ldr_3,ldr_4);
    
    // clear the string:
    serialdata = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
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
