#include "SoftwareSerial.h"
SoftwareSerial serial_connection(10, 11);//Create a serial connection with TX and RX on these pins

const int trigPin = 8;
const int echoPin = 9;
// defines variables
long duration;
int distance;

const int IN1 = 2, IN2 = 3, IN3 = 4, IN4 = 5;
const int enable12 = 3 , enable34 = 9;

#define BUFFER_SIZE 64
char inData[BUFFER_SIZE];
char inChar=-1;
int count=0;
int i=0;
void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);//Initialize communications to the serial monitor in the Arduino IDE
  serial_connection.begin(9600);//Initialize communications with the bluetooth module
  serial_connection.println("Ready!!!");//Send something to just start comms. This will never be seen.
  Serial.println("Started");//Tell the serial monitor that the sketch has started.
}
void loop()
{
  while(calDistance() <= 15){
//    Serial.println("Too close");
    brake();
//    reverse();
    delay(100);
    brake();
  }
  //This will prevent bufferoverrun errors
  byte byte_count=serial_connection.available();//This gets the number of bytes that were sent by the python script
  if(byte_count)//If there are any bytes then deal with them
  {
    Serial.println("Incoming Data");//Signal to the monitor that something is happening
    int first_bytes=byte_count;//initialize the number of bytes that we might handle. 
    int remaining_bytes=0;//Initialize the bytes that we may have to burn off to prevent a buffer overrun
    if(first_bytes>=BUFFER_SIZE-1)//If the incoming byte count is more than our buffer...
    {
      remaining_bytes=byte_count-(BUFFER_SIZE-1);//Reduce the bytes that we plan on handleing to below the buffer size
    }
    for(i=0;i<first_bytes;i++)//Handle the number of incoming bytes
    {
      inChar=serial_connection.read();//Read one byte
      inData[i]=inChar;//Put it into a character string(array)
    }
    inData[i]='\0';
    char command = inData[i - 1];
    Serial.println(inData);

    if(command == '1' || command == '2'){
      Serial.println("anticlockwiseA");
      anticlockwiseA();
    }
    else if(command == '4' || command == '5'){
      Serial.println("anticlockwiseB");
      anticlockwiseB();
    }
    else if(command == '3'){
      Serial.println("forward");
      forward();
    }
    else{
      Serial.println("brake");
      brake();
    }
    
    for(i=0;i<remaining_bytes;i++)//This burns off any remaining bytes that the buffer can't handle.
    {
      inChar=serial_connection.read();
    }
  }
  delay(100);//Pause for a moment 
}

void reverse()
{
  Serial.print("reverse");
  digitalWrite(13, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void forward()
{
  Serial.print("forward");
  digitalWrite(13, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void anticlockwiseA()
{
  Serial.print("A");
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void anticlockwiseB()
{
  Serial.print("B");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

//Motor Brake
void brake()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


//Ultrasonic sensor
int calDistance(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(1000000);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(1000000);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
//    Serial.print("Distance: ");
//    Serial.println(distance);
    return distance;
}
