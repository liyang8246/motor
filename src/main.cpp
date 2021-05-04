#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire);

int n = 0;
int sp1[59];
long num = -1;
int tim0 = 0;
float sp0 = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("hello world");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  pinMode(A0,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  pinMode(10,OUTPUT);
}

int speed(){
  int dtim = 0;
  if(digitalRead(A0)==0 && n==0){
    tim0 = micros();
    n = 1;
  }
  if (digitalRead(A0)==1 && n==1){
    dtim = micros() - tim0;
    n = 0;
  }
  return dtim;
}

void screen(){
  display.clearDisplay();
  display.drawLine(12,63,127,63,WHITE);
  display.drawLine(12,63,12,14,WHITE);
  display.setCursor(0,0);
  display.print("speed:");
  display.setCursor(36,0);
  display.print(sp0);
  display.setCursor(86,0);
  display.print("rad/min");
  display.setCursor(0,14);
  display.print("20");
  display.setCursor(6,56);
  display.print("0");
  for(int i=0;i<60;i++){
    display.drawLine(12+i*2,map(sp1[i],0,20000,63,14),12+(i+1)*2,map(sp1[i+1],0,20000,63,14),WHITE);
  }
  display.display();
}

void loop() {
  num = num + 1;
  if(num == 262144){
    num = 0;
    screen();
  }
  if(num%16384 == 0){
    sp1[59] = sp0;
    for(int i=0;i<59;i++){
      sp1[i] = sp1[i+1];
    }
  }
  if(digitalRead(A0)==0){
    digitalWrite(13,1);
    digitalWrite(10,1);
  }else{
    digitalWrite(13,0);
    digitalWrite(10,0);
  }
  float sp = speed();
  if(sp != 0){
    sp = 1000000/sp;
    sp = sp * 20 * 2 * 3.14;
    if(sp >= 0){
      sp0 = sp;
    }
  }
}