#include <Wire.h>
#include <U8glib.h>
#include <elapsedMillis.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#define ADDR 0x40
float pmax, pmin, pF, tF;
int vermelho = 4;
int amarelo = 3;
int verde =  2;
elapsedMillis count;
int msg;

void enviarDados(int id, float pressao, float temperatura, int teste) {
    Serial.print(pressao,3);
    Serial.print("S");  
    Serial.print(temperatura,1);
    Serial.print("S");
    Serial.print(id);
    Serial.println("SEND");
    delay(700);

  /*
  Serial.print("@");
  Serial.print(id);
  Serial.print("#");
  Serial.print(pressao,6);
  Serial.print("#");
  Serial.print(temperatura);
  Serial.print("#");
  Serial.print(teste);
  Serial.print("!\n");
  */
}

void draw(float pressao, float temperatura) {  
  u8g.setFont(u8g_font_helvB24);
  u8g.setFontPosTop();
  u8g.setPrintPos(4, 0);
  u8g.print(pressao,3);
  u8g.setPrintPos(4, 38);
  u8g.print(temperatura);
  u8g.setFont(u8g_font_courB14);
  u8g.setFontPosTop();
  u8g.drawStr270(112, 31, "bar");
  u8g.drawCircle(110, 53, 2);
  u8g.drawStr(114, 50, "C");
  u8g.drawHLine(0, 33, 128);
}

void draw_teste(float pressao, int t) {  
  u8g.setFont(u8g_font_helvB24);
  u8g.setFontPosTop();
  u8g.setPrintPos(4, 0);
  u8g.print(pressao,3);
  u8g.setFont(u8g_font_courB14);
  u8g.setFontPosTop();
  u8g.drawStr270(112, 31, "bar");
  if(t == 1) {
    u8g.drawStr(10, 36, "Executando");
    u8g.drawStr(10, 50, "Teste 0001");
  }else if(t == 2) {
    u8g.drawStr(10, 36, "Executando");
    u8g.drawStr(10, 50, "Teste 0002");
  }else if(t == 3) {
    u8g.drawStr(10, 44, "Reprovado!");
  }else if(t == 4) {
    u8g.drawStr(10, 44, "Aprovado!!");
  }  
  u8g.drawHLine(0, 33, 128);
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

void leds(int vd, int am, int ve){
  digitalWrite(verde, vd);
  digitalWrite(amarelo, am);
  digitalWrite(vermelho, ve);
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
  float pref = ((pre - 16384) * 30.0 / 32768);
  float dif = pref * (0.33 - 0.09) / 30.0 + 0.09;
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
/*
if(pF >= 2.0){
  if(pF < 8.0){
    msg = 10; //testando_0001
    u8g.firstPage();  
    do {
      draw_teste(pF, 1);
    } while( u8g.nextPage() );
  }
  enviarDados(1, pF, tF, msg);
  if(count > 10000 && pF < 8.0){
    msg = 11; //reprovado_teste_0001
    enviarDados(00001, pF, tF, msg);
    u8g.firstPage();  
    do {
      draw_teste(pF, 3);
    } while( u8g.nextPage() );
    leds(0,0,1);
    delay(2000);
    count = 0;
  }else if(pF >= 8.0){
    msg = 20; //testando_0002
    enviarDados(00001, pF, tF, msg);
    u8g.firstPage();  
    do {
      draw_teste(pF, 2);
    } while( u8g.nextPage() );
    float ta = pF;
    if(count > 4000){
      float tb = ta - pressao();
      if(tb > 0.2){
        msg = 21; //reprovado_teste_0002
        enviarDados(00001, pF, tF, msg);
        u8g.firstPage();  
        do {
          draw_teste(pF, 3);
        } while( u8g.nextPage() );
        leds(0,0,1);
        delay(500);
        leds(0,0,0);
        delay(500);
        leds(0,0,1);
        delay(500);
        leds(0,0,0);
        delay(500);
        count = 0;
      }else{
        msg = 1; //aprovado
        enviarDados(00001, pF, tF, msg);
        u8g.firstPage();  
        do {
          draw_teste(pF, 4);
        } while( u8g.nextPage() );
        leds(1,0,0);
        delay(500);
        leds(0,0,0);
        delay(500);
        leds(1,0,0);
        delay(500);
        leds(0,0,0);
        delay(500);
        count = 0;
      }
    }
  }
}else{
  count = 0;
}
*/
//if(pF < 2.0){
  u8g.firstPage();  
  do {
    draw(pF, tF);
  } while( u8g.nextPage() );
//}

  enviarDados(00001, pF, tF, msg);
  
  leds(0,1,0);
}

