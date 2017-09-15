#include "TM1637.h"
#include "hx711.h"

// Hx711.DOUT - pin #A1
// Hx711.SCK - pin #A0
#define CLK 2//pins definitions for TM1637 and can be changed to other ports       
#define DIO 3

#define SHOW_MAX 2000

TM1637 dis7seg(CLK,DIO);
Hx711 scale(A1, A0);
byte times = 1;

float max_force = 0.0f;
bool applying_force = false;
bool printMax = false;
bool timing = false;
long startTime = 0;


static int8_t digitos[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x77,0x7c,
                           0x39,0x5e,0x79,0x71};//0~9,A,b,C,d,E,F

char lets[] = {'A','b','B','c','C','d','e','E','F','g','G','h','H','i','I','J','L','n','o','O','P','q','r','S','t','u','U','y','Z'};

uint8_t letras(char c);
void display_printgrams(float gramos);
void display_printString(char* str, int time=30);

void setup()
{
  dis7seg.init();
  dis7seg.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;

  Serial.begin(9600);
  //Scale, Offset:0.001199529647,-9740.830078125000
  scale.setReadTimes(1);
  scale.setScale(0.001199529647f);
  scale.setOffset(-9740.83007812f);
  display_printString("init", 1000);
}

void loop() {
  //Adq
  float force = scale.getGram();
  
  //Proc
  if (force > 50.0)
    applying_force = true;
  else{
      if (applying_force){
        applying_force = false;
        startTime = millis();
        timing = true;
      }
      else if(timing){
        long actualTime = millis();
        long elapsed = actualTime - startTime;
        if (elapsed > SHOW_MAX){
          printMax = true;
          startTime = actualTime;
          timing = false;
        }
      }
    }
    
  max_force = max(force, max_force);
  //Desp
  display_printgrams(force);
  if(printMax){
    printMax = false;
    for(int i = 0; i < 3; i++){
      display_printgrams(max_force);
      delay(500);
      dis7seg.clearDisplay();
      delay(500);
    }
    display_printgrams(max_force);
    delay(2000);
    max_force = 0.0f;
  }
  delay(50);
}

void display_printgrams(float gramos){
  int num = (int)gramos;
  ////////Obtener las unidades,decenas,centenas//////// 
  int miles=num/1000;
  int centenas=(num-(miles*1000))/100;
  int decenas=(num- (miles*1000 + centenas*100))/10;
  int unidades=num-(miles*1000 + centenas*100 + decenas*10 );

  bool neg = num < 0 ? true : false;
  //imprime
    if(!neg){
      dis7seg.point(false); dis7seg.clearDisplay();
      dis7seg.display(0,miles);
      dis7seg.display(1,centenas);
      dis7seg.display(2,decenas);
      dis7seg.display(3,unidades);
    }
    else{
      dis7seg.point(true); dis7seg.clearDisplay();
      dis7seg.display(0,-miles);
      dis7seg.display(1,-centenas);
      dis7seg.display(2,-decenas);
      dis7seg.display(3,-unidades);
    }
}

void display_printString(char* str, int time){
  int pos  = 0;
  while (pos < 4 && str[pos] != 0){
    dis7seg.display_raw(pos, letras(str[pos]));
    ++pos;
  }
  delay(time);
}

uint8_t letras(char c){
  uint8_t let = 0x00;
  switch(c){
    case 'A':
    let = 0b01110111;
    break;
    case 'b':
    let = 0b01111100;
    break;
    case 'B':
    let = 0b01111111;
    break;
    case 'c':
    let = 0b01011000;
    break;
    case 'C':
    let = 0b00111001;
    break;
    case 'd':
    let = 0b01011110;
    break;
    case 'e':
    let = 0b01111011;
    break;
    case 'E':
    let = 0b01111001;
    break;
    case 'F':
    let = 0b01110001;
    break;
    case 'g':
    let = 0b01101111;
    break;
    case 'G':
    let = 0b01111101;
    break;
    case 'h':
    let = 0b01110100;
    break;
    case 'H':
    let = 0b01110110;
    break;
    case 'i':
    let = 0b00000100;
    break;
    case 'I':
    let = 0b00000110;
    break;
    case 'J':
    let = 0b00001110;
    break;
    case 'L':
    let = 0b00111000;
    break;
    case 'n':
    let = 0b01010100;
    break;
    case 'o':
    let = 0b01011100;
    break;
    case 'O':
    let = 0b00111111;
    break;
    case 'P':
    let = 0b01110011;
    break;
    case 'q':
    let = 0b01100111;
    break;
    case 'r':
    let = 0b01010000;
    break;
    case 'S':
    let = 0b01101101;
    break;
    case 't':
    let = 0b01111000;
    break;
    case 'u':
    let = 0b00011100;
    break;
    case 'U':
    let = 0b00111110;
    break;
    case 'y':
    let = 0b01101110;
    break;
    case 'Z':
    let = 0b01011011;
    break;
    case '-':
    let = 0b01000000;
    break;
    case '=':
    let = 0b01001000;
    break;
    case ' ':
    let = 0b00000000;
    break;
    default:
    let = 0b01010011;
  }
  return let;
}
