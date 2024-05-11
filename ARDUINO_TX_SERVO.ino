//***********************************************THANG KU TO********************************************//
//************ARDUINO UNO R3 DIEU KHIEN SERVO, GUI GIA TRI CUONG DO ANH SANG CHO NODE MCU***************//
//******************************************************************************************************//
#include <Servo.h> 
#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Wire.h>

SoftwareSerial nodemcu(2,3);//tx-2, rx-3
Adafruit_SSD1306 display(128, 64, &Wire, -1);

String tx_data; //luu tru cac gia tri anh sang theo 4 diem

//dinh nghia gia tri goc max, min cho hai dong co ngang - doc
#define max_servo_ngang  180  
#define min_servo_ngang  0    
#define max_servo_doc    180  
#define min_servo_doc    0   

// khai bao 2 dong co servo dieu chinh chieu ngang va chieu doc
Servo dcservo_ngang; 
Servo dcservo_doc; 

 // gia tri goc quay dieu khien cua dong co servo theo chieu ngang va chieu doc
int servo_ngang = 90; 
int servo_doc = 45; 

// khai bao 4 quang tro dat o 4 goc
int ldrlt = A0; //LDR BEN TREN - BEN TRAI
int ldrrt = A1; //LDR BEN TREN - BEN PHAI
int ldrld = A2; //LDR BEN DUOI - BEN TRAI
int ldrrd = A3; //LDR BEN DUOI - BEN PHAI

int percent_light1 = 0;
int percent_light2 = 0;
int percent_light3 = 0;
int percent_light4 = 0;

void setup(){
  Serial.begin(9600); 
  nodemcu.begin(9600);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   
  display.setFont(&FreeSerif9pt7b);

  dcservo_ngang.attach(9);
  dcservo_doc.attach(10);
   
  dcservo_ngang.write(90);
  dcservo_doc.write(45);

  delay(3000);
}
void loop() {
// doc gia tri cuong do anh sang tu cac quang tro dat o bon goc
int light_lt = analogRead(ldrlt); // TREN TRAI
int light_rt = analogRead(ldrrt); // TREN PHAI
int light_ld = analogRead(ldrld); // DUOI TRAI
int light_rd = analogRead(ldrrd); // DUOI PHAI

//
percent_light1 = map(light_lt, 0, 1023, 1, 100);
percent_light2 = map(light_rt, 0, 1023, 1, 100);
percent_light3 = map(light_ld, 0, 1023, 1, 100);
percent_light4 = map(light_rd, 0, 1023, 1, 100);


int tolerance = 50; // sai so (tolerance) 
// tinh toan cuong do anh sang trung binh 
int alight_top = (light_lt + light_rt) / 2; // cuong do anh sang trung binh o tren
int alight_down = (light_ld + light_rd) / 2; // cuong do anh sang trung binh o duoi
int alight_left = (light_lt + light_ld) / 2; // cuong do anh sang trung binh ben trai
int alight_right = (light_rt + light_rd) / 2; // cuong do anh sang trung binh ben phai

// tinh toan do lech giua tren - duoi va trai - phai
int dif_ud = alight_top - alight_down; // tinh toan sai lech tren va duoi
int dif_lr = alight_left - alight_right;// tinh toan sai lech trai phai

// tinh toan de hieu chinh goc quay servo theo chieu doc
if (-1*tolerance > dif_ud || dif_ud > tolerance)
  {
    if(alight_top < alight_down)
    {
      servo_doc = servo_doc + 1;
      if(servo_doc > max_servo_doc)
      {
        servo_doc = max_servo_doc;
      }
      dcservo_doc.write(servo_doc);
      delay(20);
    }
    else if(alight_top > alight_down)
      {
        servo_doc = servo_doc - 1;
        if(servo_doc < min_servo_doc)
        {
          servo_doc = min_servo_doc;
        }
        dcservo_doc.write(servo_doc);
        delay(20);
      }
      else if(alight_top == alight_down)
        {

        }
  }
  //*************************************
  if (-1*tolerance >dif_lr || dif_lr > tolerance)
  {
    if(alight_left > alight_right)
    {
      servo_ngang = servo_ngang - 1;
      if(servo_ngang >  max_servo_ngang)
      {
        servo_ngang =  max_servo_ngang;
      }
      dcservo_ngang.write(servo_ngang);
      delay(20);
    }
    else if(alight_left  < alight_right)
      {
        servo_ngang = servo_ngang + 1;
        if(servo_ngang < min_servo_ngang)
        {
         servo_ngang = min_servo_ngang;
        }
        dcservo_ngang.write(servo_ngang);
        delay(20);
      }
      else if(alight_left == alight_right)
        {
          
        }
  }
  //gui du lieu 4 gia tri quang tro sang node mcu

   tx_data = tx_data + percent_light1 + "," + percent_light2 + "," + percent_light3 + "," + percent_light4; 
   Serial.println(tx_data); 
   nodemcu.println(tx_data);
   tx_data = ""; 

   oled_display();

   delay(100);
}
void oled_display()
{
  display.clearDisplay();
  display.setTextSize(1);             
  display.setTextColor(WHITE); 

  display.setCursor(0,20);             
  display.print("  NHOM 7 ");

  display.setCursor(0,40);             
  display.print("TL: ");
  display.print(percent_light1);
  display.println("%");

  display.setCursor(64,40);             
  display.print("BL: ");
  display.print(percent_light2);
  display.println("%");

  display.setCursor(0,60);             
  display.print("TR: ");
  display.print(percent_light3);
  display.println("%");

  display.setCursor(64,60);             
  display.print(": ");
  display.print(percent_light4);
  display.println("%");

  display.display();
}