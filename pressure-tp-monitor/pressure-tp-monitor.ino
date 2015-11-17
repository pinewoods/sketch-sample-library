#include <Wire.h>
#include <U8glib.h>
#include <elapsedMillis.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#define ADDR 0x40
float pmax, pmin, pF, tF;
int vermelho = 2;
int amarelo = 3;
int verde =  4;
elapsedMillis count;
int msg;
int testado = 0;

void enviarDados(int id, float pressao, float temperatura, int teste) {
  
  Serial.print("c");
  Serial.print("00000000001");
  Serial.print("i");
  Serial.print(id);
  Serial.print("t");
  Serial.print(temperatura);
  Serial.print("p");
  Serial.print(pressao,6);
  Serial.print("s");
  Serial.print(teste);
  Serial.println("e");
  
}

void setup(){
  Wire.begin();
  Serial.begin(9600);
  pinMode (vermelho, OUTPUT);
  pinMode (amarelo, OUTPUT);
  pinMode (verde, OUTPUT);
  u8g.sleepOn();
  delay(1000);
  u8g.sleepOff();
  delay(500);
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
  float pref = ((pre - 16384) * 10.0 / 32768);
  float dif = pref * (0.05 - 0.09) / 10.0 + 0.04;
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
delay(500);
}
