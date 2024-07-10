#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6GFSDlfez"
#define BLYNK_TEMPLATE_NAME "project"
#define BLYNK_AUTH_TOKEN "RloP1XaMqEsNykan1PSY7R1q1XmfnAd6"

BlynkTimer timer;
Servo myServo;

char auth[] = "RloP1XaMqEsNykan1PSY7R1q1XmfnAd6";
char ssid[] = "Tuan";
char pass[] = "tuan@6789";

#define rainsensor D0
#define ledr D1
#define ledg D3
#define ledy D2
#define analogPin A0
#define relay1 D7
#define relay2 D8
#define servo D4

int mood,analogVal,pump,door,Ref1,Ref2,rain;
int phantram;
WidgetLED appledr(V1);
WidgetLED appledg(V3);
WidgetLED appledy(V2);
WidgetLED appledrain(V15);

void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  myServo.attach(servo); 

  pinMode(ledr,OUTPUT);
  pinMode(ledy,OUTPUT);
  pinMode(ledg,OUTPUT);
  pinMode(relay1,OUTPUT);

  Blynk.syncVirtual(V14);
  Blynk.syncVirtual(V11);
  Blynk.syncVirtual(V10);
  Blynk.syncVirtual(V12);
  Blynk.syncVirtual(V13);
}

BLYNK_WRITE(V14) { // điều chỉnh chế độ
 mood = param.asInt();
}

BLYNK_WRITE(V11) { // Lấy giá trị ngưỡng trên
 Ref2=param.asInt();
}

BLYNK_WRITE(V10) { // Lấy giá trị ngưỡng duoi
 Ref1=param.asInt();
}

BLYNK_WRITE(V12) { // Nut nhan bat tat relay bom nuoc
 pump=param.asInt();
}

BLYNK_WRITE(V13) {
 door=param.asInt();
}

void loop(){
  analogVal = analogRead(analogPin); // 0 --> 1023
  phantram=map(analogVal, 0, 1023, 100, 0); // convert to %
  rain = digitalRead(rainsensor);
  Blynk.virtualWrite(V0,phantram);
  Blynk.run();
  timer.run();

  if (mood == 1){
   if(rain == 1){
     appledrain.off();
     if (phantram > Ref2){
       appledr.off();
       appledg.off();
       appledy.on();
       digitalWrite(ledr,LOW);
       digitalWrite(ledg,LOW);
       digitalWrite(ledy,HIGH);
       digitalWrite(relay1,LOW);
       Blynk.logEvent("Độ ẩm cao");
       Blynk.virtualWrite(V6,"Độ ẩm cao"); //hiển thị trên LCD dòng 1
      }
     else if (phantram < Ref1){
       appledr.on();
       appledg.off();
       appledy.off();
       digitalWrite(ledr,HIGH);
       digitalWrite(ledg,LOW);
       digitalWrite(ledy,LOW);
       digitalWrite(relay1,HIGH);
       Blynk.logEvent("Độ ẩm thấp");
       Blynk.virtualWrite(V6,"Đang tưới nước");
      }
     else {
       appledr.off();
       appledg.on();
       appledy.off();
       digitalWrite(ledr,LOW);
       digitalWrite(ledg,HIGH);
       digitalWrite(ledy,LOW);
       digitalWrite(relay1,LOW);
       Blynk.virtualWrite(V6,"Độ ẩm BT");
      }
    }

  else{
    digitalWrite(ledr,HIGH);
    digitalWrite(ledg,HIGH);
    digitalWrite(ledy,HIGH);
    appledrain.on();
    appledr.off();
    appledg.off();
    appledy.on();
    if (phantram > Ref2){
      myServo.write(180);
      Blynk.virtualWrite(V13,0);
      appledr.off();
      appledg.off();
      appledy.on();
    }
    else {
      myServo.write(0);
      Blynk.virtualWrite(V13,1);
      appledr.on();
      appledg.on();
      appledy.off();
    }
  }
}


 else{
  if (pump == 1){
    digitalWrite(relay1,HIGH);
  }
  else {
   digitalWrite(relay1,LOW);
  } 

  if (door == 1){
   myServo.write(0);
  }
  else{
   myServo.write(180);
  }
}
}




