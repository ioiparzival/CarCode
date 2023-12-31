
#include <WiFi.h>

const char* ssid     = "CAR_WIFI";
const char* password = "12345678";

// Motor A
int motorAPin0 = 22; 
int motorAPin1 = 12; 
int enableAPin = 23;

// Motor B
int motorBPin0 = 18; 
int motorBPin1 = 19; 
int enableBPin = 5;

// Motor С
int motorCPin0 = 25; 
int motorCPin1 = 33; 
int enableCPin = 32;

// Motor D
int motorDPin0 = 26; 
int motorDPin1 = 27; 
int enableDPin = 14;

WiFiServer server(80);

void motor_write(int motor, int pwr){
  if(motor == 1){
    if(pwr > 0){
      digitalWrite(motorAPin0, 1);
      digitalWrite(motorAPin1, 0);
      ledcWrite(0, pwr);
    }
    else if(pwr < 0){
      digitalWrite(motorAPin0, 0);
      digitalWrite(motorAPin1, 1);
      ledcWrite(0, -pwr);
    }
    else{
      digitalWrite(motorAPin0, 0);
      digitalWrite(motorAPin1, 0);
      ledcWrite(0, 0);
    }
  }
  if(motor == 2){
    if(pwr > 0){
      digitalWrite(motorBPin0, 1);
      digitalWrite(motorBPin1, 0);
      ledcWrite(1, pwr);
    }
    else if(pwr < 0){
      digitalWrite(motorBPin0, 0);
      digitalWrite(motorBPin1, 1);
      ledcWrite(1, -pwr);
    }
    else{
      digitalWrite(motorBPin0, 0);
      digitalWrite(motorBPin1, 0);
      ledcWrite(1, 0);
    }
  }
  if(motor == 3){
    if(pwr > 0){
      digitalWrite(motorCPin0, 0);
      digitalWrite(motorCPin1, 1);
      ledcWrite(2, pwr);
    }
    else if(pwr < 0){
      digitalWrite(motorCPin0, 1);
      digitalWrite(motorCPin1, 0);
      ledcWrite(2, -pwr);
    }
    else{
      digitalWrite(motorCPin0, 0);
      digitalWrite(motorCPin1, 0);
      ledcWrite(2, 0);
    }
  }
  if(motor == 4){
    if(pwr > 0){
      digitalWrite(motorDPin0, 0);
      digitalWrite(motorDPin1, 1);
      ledcWrite(3, pwr);
    }
    else if(pwr < 0){
      digitalWrite(motorDPin0, 1);
      digitalWrite(motorDPin1, 0);
      ledcWrite(3, -pwr);
    }
    else{
      digitalWrite(motorDPin0, 0);
      digitalWrite(motorDPin1, 0);
      ledcWrite(3, 0);
    }
  }
}

void setup() {
  

  pinMode(motorAPin0, OUTPUT); digitalWrite(motorAPin0, 0);
  pinMode(motorAPin1, OUTPUT); digitalWrite(motorAPin1, 0);
  ledcSetup(0, 4000, 8);
  ledcAttachPin(enableAPin, 0);
  ledcWrite(0, 0);

  pinMode(motorBPin0, OUTPUT); digitalWrite(motorBPin0, 0);
  pinMode(motorBPin1, OUTPUT); digitalWrite(motorBPin1, 0);
  ledcSetup(1, 4000, 8);
  ledcAttachPin(enableBPin, 1);
  ledcWrite(1, 0);

  pinMode(motorCPin0, OUTPUT); digitalWrite(motorCPin0, 0);
  pinMode(motorCPin1, OUTPUT); digitalWrite(motorCPin1, 0);
  ledcSetup(2, 4000, 8);
  ledcAttachPin(enableCPin, 2);
  ledcWrite(2, 0);

  pinMode(motorDPin0, OUTPUT); digitalWrite(motorDPin0, 0);
  pinMode(motorDPin1, OUTPUT); digitalWrite(motorDPin1, 0);
  ledcSetup(3, 4000, 8);
  ledcAttachPin(enableDPin, 3);
  ledcWrite(3, 0);

  /*while(1){
    motor_write(1, 250);
    delay(1000);
    motor_write(1, 200);
    delay(1000);
    motor_write(1, 150);
    delay(1000);
    motor_write(1, 0);
    delay(1000);
  }*/

  Serial.begin(115200);
  Serial.println("Start!");

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();  

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) {           
      if (client.available()) {          
        char c = client.read();   
        //Serial.print(c);
        if(c == 'A' || c == 'B' || c == 'C' || c == 'D'){
          char c2 = '.';
          String str = "";
          while(client.connected()){
            if (client.available()) {          
              c2 = client.read();
              //Serial.print(c);
              if(c2 != ' '){
                str += c2;
              }
              else{
                int tmp = str.toInt();
                int motor = 1;
                if(c == 'B')
                  motor = 2;
                else if(c == 'C')
                  motor = 3;
                else if(c == 'D')
                  motor = 4;

/*Serial.print("Motor: ");
                Serial.print(motor);
                Serial.print(" pwr: ");
                Serial.println(tmp);*/
                motor_write(motor, tmp);
                break;
              }
            }
          }
        }
      }
    }
  }

  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
  delay(1000);
}