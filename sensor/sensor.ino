#include <Arduino.h>
#include <Servo.h>

//Til servo
Servo servo_lower;
Servo servo_upper;

const int servo_pin_lower = 10;
const int servo_pin_upper = 9;

const int top_left_pin = A1;
const int bottom_left_pin = A0;
const int top_right_pin = A3;
const int bottom_right_pin = A2;

int angle = 0;

//Til lyssensor
const int redLed = 5;
const int kraftSensorPin = A0;
float turnON = 200;

//Til sensor
#define trigPin 2  //Definerer trig og echo pinner
#define echoPin 3

long varighet; //Variabel varighet for tid mellom sending og mottak av lydbølger
int avstand; //Avstand variabelen skal kalkuleres til avstand ved bruk av varighet





////////////////////////////////////////////////////////////
////////////////////////Funksjons///////////////////////////
////////////////////////////////////////////////////////////
void avstandSensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  varighet = pulseIn(echoPin, HIGH);  //pulseIn er en funksjon tar tiden når pin går fra LOW til HIGH frem til den blir LOW igjen
  avstand = varighet * 0.034 / 2;  //Vi bruker 0.034 fordi det er lydens hastighet, og deler på to fordi vi får avstanden fram og tilbake, og vi vil bare ha avstanden fram
  Serial.print(avstand);
  Serial.println(" cm");

  delay(50);        
  //Verdiene vi hadde fått ut av denne kunne vi f.eks brukt til å regulere farten til Zumo bilen.
}

void lower_body_light_tracker(){
    int top_left_sensor = analogRead(top_left_pin);
    int top_right_sensor = analogRead(top_right_pin);
    int bottom_left_sensor = analogRead(bottom_left_pin);
    int bottom_right_sensor = analogRead(bottom_right_pin);

    int left_side = (top_left_sensor + bottom_left_sensor);
    int right_side =  (top_right_sensor + bottom_right_sensor);    
    
    if(left_side > right_side && 180 > angle > 0){
        angle++;
        servo_lower.write(angle);
    }
    else{
        angle--;
        servo_lower.write(angle);
    }
    //Next code is for stabilizing the servor
    if(left_side - right_side < 100 || right_side - left_side < 100){
        delay(10);
    }
}

void angle_reset(){
    if(angle < 0){
        angle = 0;
    }
    Serial.println(angle);
}

float readAvarage(){ // leser av sensoren 3 ganger og summerer verdiene og deler på 3 for å få gjennomsnittsmålingen over 3 avlesninger
  int sum = 0;
  for (int i = 0; i < 5; i++){
    	sum += analogRead(kraftSensorPin);
  }
  int avarage = sum/3; 
  return avarage;
}


void kraftSensor(){
     Serial.println(analogRead(kraftSensorPin));
  if (readAvarage() > turnON){
    digitalWrite(redLed, HIGH);
  }else {
    digitalWrite(redLed, LOW);
  }
}


////////////////////////////////////////////////////////////
//////////////////////////Setup/////////////////////////////
////////////////////////////////////////////////////////////
void setup(){
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(redLed, OUTPUT);

    pinMode(kraftSensorPin, INPUT);
    Serial.begin(9600);

    servo_lower.attach(servo_pin_lower);
    servo_upper.attach(servo_pin_upper);
    Serial.begin(9600);
    servo_lower.write(90);
    servo_upper.write(60);
}

////////////////////////////////////////////////////////////
///////////////////////////Loop/////////////////////////////
////////////////////////////////////////////////////////////
void loop(){
    avstandSensor();

    lower_body_light_tracker();
    angle_reset();

    kraftSensor();
}
