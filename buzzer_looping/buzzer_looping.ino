#include <Servo.h>
Servo servo;

//ubidots
#include "Ubidots.h"
#include <Wire.h>
#include <ESP8266WiFi.h>

const char* UBIDOTS_TOKEN = "BBFF-NHZWVT2avVZqQ8QBDxa9BXIYCukbGk";
const char* ssid     = "MAJOR PROJECT"; // Your ssid
const char* password = "12345678"; // Your Password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);

unsigned long init_millis = 0;
bool buzzer_blow;

int pos;
int Signal = D1;
int servopin = D2;


void setup() {

  Serial.begin(115200);

  ubidots.wifiConnect(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi is connected");



  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);

  pinMode(Signal, INPUT);
  servo.attach(servopin);
  servo.write(0);         //close cap on power on
  delay(1000);

  buzzer_blow = false;
  init_millis = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  //io.run();



  if (millis() - init_millis > 70000) {
    init_millis = millis();
    buzzer_blow = true;
  }

  if (buzzer_blow) {
    // turn buzzer on
    tone(D0, 200);
  } else {
    noTone(D0);
  }

  int buttonState = digitalRead(Signal);




  Serial.println(buttonState);
  if (buttonState == 0) {
    buzzer_blow = false;
    noTone(D0);

    //exceute next piece of servo code

    servo.write(90);
    delay(3000);       //wait 5 seconds
    servo.write(0);
    delay(1000);

    if (buzzer_blow == false) {

      ubidots.add("Pill Dispenser", buttonState ); // Change for your variable name
      Serial.print("bande tadi");
      bool bufferSent = false;
      bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id
      if (bufferSent) {
        Serial.println("Values sent by the device");
        buttonState = 1;
      }

    }

  }


}
