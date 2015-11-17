#include <Wire.h>
#define ADDR 0x40
float pmax, pmin, pF, tF;
int msg;

void enviarDados(int id, float pressao, float temperatura, int teste) {
    Serial.print("SENSOR_MTR_V1:");
    Serial.print(pressao,3);
    Serial.print("#");
    Serial.print(temperatura,1);
    Serial.println(":FIM");
    delay(10000);
}

void setup(){
  Wire.begin();
  Serial.begin(115200);
}

float pressao(){
  Wire.beginTransmission(ADDR);
  Wire.write(0xAC);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(ADDR, 5);
  
  int st = Wire.read();
  int za = Wire.read()<<8;
  int zb = Wire.read();
  int zc = Wire.read()<<8;
  int zd = Wire.read();
  
  int pre = za + zb;
  int tem = zc + zd;
  float pref = ((pre - 16384) * 1.0 / 32769);
  float dif = pref * (0.016 - 0.019) / 1.0 - 0.0;
  return pref + dif;
}
float temperatura(){
  Wire.beginTransmission(ADDR);
  Wire.write(0xAC);
  Wire.endTransmission();
  delay(10);
  Wire.requestFrom(ADDR, 5);
  
  int st = Wire.read();
  int za = Wire.read()<<8;
  int zb = Wire.read();
  int zc = Wire.read()<<8;
  int zd = Wire.read();
  
  int pre = za + zb;
  int tem = zc + zd;
  
  return ((tem/16)-24)*0.05-50;
}
void loop(){
pF = pressao();
tF = temperatura();
msg = 0; //leitura
enviarDados(00001, pF, tF, msg);
}

