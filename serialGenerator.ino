/*********************************************************
 * Janky sketch to general data on serial line
 * mainly for use as a test tool for other projects in flight
 * BTN1 (pin2) momentary switch: causes the sketch to switch serial port baud rates (for testing audodetect)
 * BTN2 (pin3) flip switch: high causes RX to check if recieve matches what was sent, low skips
 */
const int BUTTON_PIN1 = 2;
const int BUTTON_PIN2 = 3;

int bauds[] = {4800, 9600, 19200, 38400, 115200, 230400, 460800, 921600};
int currentBaud = bauds[1];

volatile bool changeBaud = false;
volatile bool checkRX = false;

void INT_BUTTON2_RXCHECK(){
  checkRX = digitalRead(BUTTON_PIN2);
  
}
void INT_BUTTON1_CHANGE_BAUD(){
  changeBaud = true;
}

void strobeLED(int len) {
  digitalWrite(LED_BUILTIN,HIGH);
  delay(len);
  digitalWrite(LED_BUILTIN,LOW);
}

void flashLED(int count) {
  for(int i=0;i<count;i++) {
    digitalWrite(LED_BUILTIN,HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN,LOW);
    delay(250);
    digitalWrite(LED_BUILTIN,HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN,LOW);
    delay(125);
  }
}

void setup() {
 Serial.begin(currentBaud);
 Serial.setTimeout(2500);
 pinMode(LED_BUILTIN, OUTPUT);
 pinMode(BUTTON_PIN1, INPUT);
 pinMode(BUTTON_PIN2, INPUT);
 attachInterrupt(digitalPinToInterrupt(BUTTON_PIN1), INT_BUTTON1_CHANGE_BAUD, RISING);
 attachInterrupt(digitalPinToInterrupt(BUTTON_PIN2), INT_BUTTON2_RXCHECK, CHANGE);
 flashLED(1);
 delay(1000);
 flashLED(1);
 delay(1000);
}

void loop() {
 if(changeBaud) {
  currentBaud++;
  if(currentBaud == 8)
   currentBaud = 0;
  Serial.flush();
  Serial.begin(currentBaud);
  flashLED(2);
  changeBaud = false;
 }
 String testStr(currentBaud);
 Serial.print(testStr + ":");
 if(checkRX){
   while (Serial.available() == 0){}
   String rxStr = Serial.readString();
   rxStr.trim();
   if(rxStr.equals(testStr))
    flashLED(3);
   else strobeLED(3000);
 }
}
