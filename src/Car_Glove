/*
--Это код машинки

Проект Glove состоит из перчатки(манипулятора) и машинки (которым управляют)

Машинка подключается к серверу , которая находится в перчатке
Машинка получает строку типа "A0 B0 C0 D0 " где на месте 0 стоит число ШИМ которае подается на двигатель : A,B,C или D 

*/


#include <WiFi.h>

const char* ssid = "CAR_WIFI";
const char* password = "12345678";

int motorPin0[4] = { 22, 18, 25, 26 };
int motorPin1[4] = { 12, 19, 33, 27 };
int enablePin[4] = { 23, 5, 32, 14 };

WiFiServer server(80);
WiFiClient client;

void motor_write(int motor, int pwr) {

  Serial.printf("motor=%d  pwr=%d\n", motor, pwr);
  if (motor == 1 || motor == 0) pwr = (-1) * pwr;
  if (pwr > 0) {
    digitalWrite(motorPin0[motor], 1);  // Ставим двигатель на поездку вперед
    digitalWrite(motorPin1[motor], 0);
  } else if (pwr < 0) {
    digitalWrite(motorPin0[motor], 0);  // Ставим двигатель на поездку назад
    digitalWrite(motorPin1[motor], 1);
  } else {
    digitalWrite(motorPin0[motor], 0);  // Ставим двигатель на тормоз
    digitalWrite(motorPin1[motor], 0);
  }

  if (pwr < 0) pwr = abs(pwr);

  ledcWrite(motor, pwr);
}

void setup() {

  Serial.begin(115200);
  Serial.println("Start!");

  for (int i = 0; i < 4; i++) {
    pinMode(motorPin0[i], OUTPUT);  // Настраиваем пин управления направлением (вперед,назад,тормоз)
    digitalWrite(motorPin0[i], 0);  // Ставим двигатель на тормоз

    pinMode(motorPin1[i], OUTPUT);  // Настраиваем пин управления направлением (вперед,назад,тормоз)
    digitalWrite(motorPin1[i], 0);  // Ставим двигатель на тормоз

    ledcSetup(i, 4000, 8);           // Настройка шим для светодиодов (i-канал;40000-частота;8(біт)-разрешение)
    ledcAttachPin(enablePin[i], i);  // Настройка связи пинов с их каналами
    ledcWrite(i, 0);                 // Ставим ШИМ сигнал в 0 на всех двигателях
  }

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  client = server.available();  // Узнаем если куда подключится
  Serial.print(client);

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) Data_Processing();  // Если есть связь с перчаткой то переходим в функцию расшифровки данных
    }
  }

  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
  delay(1000);
}

void Data_Processing()  // Расшифровуем данные с перчатки и полученными данными включает соответствующие двигатели
{
  char c = client.read();

  if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {
    char c2 = '.';
    String str = "";
    while (client.connected()) {
      if (client.available()) {
        c2 = client.read();
        if (c2 != ' ') {
          str += c2;
        } else {
          int tmp = str.toInt();
          int motor;
          if (c == 'A') motor = 0;
          if (c == 'B') motor = 1;
          if (c == 'C') motor = 2;
          if (c == 'D') motor = 3;

          motor_write(motor, tmp);
          break;
        }
      }
    }
  }
}
