/*
09/09/2019   rjw  original shell
09/10/2019   rjw  turn pump off before air solenoid(s) close to ensure air purge
 */

// add pump before solenoid end
// add max timer to limit the total time animal gets air 


const int ledPin     = 13;    // onboard LED
const int controlPin = 12;    // control pin to pump control Arduino
const int sol1       =  9;    // solenoid 1
const int sol2       = 10;    // solenoid 2
const int sol3       = 11;    // solenoid 3
int reading;
int lastreading;
unsigned long lastTime = millis();
unsigned long runTime  = millis();
int number            = 0;
bool disableSolenoids = false;

void setup() {

  pinMode(controlPin, INPUT);
  pinMode(sol1, OUTPUT); digitalWrite(sol1, LOW);
  pinMode(sol2, OUTPUT); digitalWrite(sol1, LOW);
  pinMode(sol3, OUTPUT); digitalWrite(sol1, LOW);
  pinMode(ledPin, OUTPUT); digitalWrite(LED_BUILTIN, LOW);
  reading     = digitalRead(controlPin);
  lastreading = reading;

  Serial.begin(9600);             // serial comms
  delay(3000);                    // wait to start
  Serial.println("test test test test . . . . . SLAVE ");
  Serial.print("ControlPin IS D"); Serial.println(controlPin);


} // of setup

void loop() {

  // read the signal from the master MCU
  reading = digitalRead(controlPin);


  // code below runs once when controlPin changes state ************************************************************************
  if (reading != lastreading) {  
    lastreading = reading;
    // turn everything on or off based on control input from master MCU
   
    if ((reading == LOW)) {
      Serial.println("Transition to LOW");
      disableSolenoids = false;
      digitalWrite(ledPin, LOW);
      digitalWrite(  sol1, LOW); delay(250);
      digitalWrite(  sol2, LOW);
      digitalWrite(  sol3, LOW);
    }    
    
    if (reading == HIGH) {
    Serial.println("Transition to HIGH");  
    lastTime = millis();
    runTime  = millis();
    disableSolenoids = false;
    digitalWrite(ledPin, HIGH);
    digitalWrite(  sol1, HIGH); delay(100);  // delay next solenoid to reduce surge current
    digitalWrite(  sol2, HIGH); delay(100);  // delay nex solenoid to reduce surge current
    digitalWrite(  sol3, HIGH); delay(1000); // initial delay before alternatine solenoids
    
     } 
  }
  // code above runs once when controlPin changes state ************************************************************************

  // check if system has been blowing air for more than 12 seconds
  if ((millis() - runTime) > 12000) {
    disableSolenoids = true;
  }
  
  // if longer than some interval into air cycle
  // alternate sides (500 msec)
  // this code override turning on all the solenoids on above
  // if runTime longer than some setting (12000 msec) turn off 
    if ((lastreading == HIGH) && ((millis() - lastTime) > 500) && !(disableSolenoids)) {
    Serial.println("In HIGH loop");  // nothing yet
    lastTime = millis();
    number++;
    if(number % 2 == 0) {
     digitalWrite(sol2, HIGH);
     digitalWrite(sol3, LOW );
    } else {
     digitalWrite(sol2, LOW);
     digitalWrite(sol3, HIGH);
    }
  }
  if ((disableSolenoids)) {
      digitalWrite(ledPin, LOW);
      digitalWrite(  sol1, LOW); delay(250);
      digitalWrite(  sol2, LOW);
      digitalWrite(  sol3, LOW);
  }
   
} // of loop
