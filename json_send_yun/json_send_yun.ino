#include <Process.h>
#include <Wire.h>
#define ADDR 0x40
int id;
float pmax, pmin, pF, tF;
int msg;

void setup() {
  Wire.begin();
  Bridge.begin();
  Console.begin();
  Serial.begin(115200);
  
  Serial.println("=========== Ready to Stream ===========");
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
    Serial.println("Posting Data!");
    postData(pressao(), temperatura(), 1); // the postData() function does all the work, 
    delay(100);
}

float postData(float pressao, float temp, int id){
  Process p;
  String jsonString = "{\"pressao\":\"";
  jsonString += pressao;
  jsonString +="\",\"temp\":\"";
  jsonString += temp;
  jsonString +="\",\"id\":\"";
  jsonString += id;
  jsonString +="\"}";
  String finalString = "curl -H \"Content-Type: application/json\" -X POST -d '"+jsonString+"' http://192.168.0.3/api2/";
  p.runShellCommand(finalString);
  Serial.println(finalString);
}
