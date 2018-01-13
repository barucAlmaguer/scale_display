#include "hx711.h"
#include <Deadband.h>

#define TARA_TIME 10000 //milisegundos entre cada posible tara

// Hx711.DOUT - pin #A1
// Hx711.SCK - pin #A0
Hx711 scale(A1, A0);
DeadbandFilter *filter = NULL;
byte times = 1;
int tara;
long last_tara = 0;
void setup()
{
  Serial.begin(9600);
  int widthOfDeadbandInBits = 1;
  filter = createDeadbandFilter(widthOfDeadbandInBits);
  scale.setReadTimes(1);
  //calibracion para 20kg
  scale.setScale(0.005572824);
  scale.setOffset(-46282.92765);
  tara = scale.getGram();
  last_tara = millis();
}

void loop()
{
  long actual_time = millis();
  long elapsed = millis() - last_tara;
  //Logica de auto-tarificacion
  if (elapsed >= TARA_TIME){
    last_tara = actual_time;
    int grams = scale.getGram() - tara;
    int possible_tara = grams + tara;
    //Aplica Tara si subió o bajó un poco
    if (grams > 0 && grams < 5 || grams < 0 && grams > -5){
      tara = possible_tara;
    }
  }
  int grams = scale.getGram();
  int grams_tara = grams - tara; //gramos con tara
  int grams_db = deadband(filter, grams_tara); //gramos con filtro 
  int printable_grams = grams_db - (grams_db % 5);
  Serial.print(grams);
  Serial.print(" ");
  Serial.print(grams_tara);
  Serial.print(" ");
  Serial.print(grams_db);
  Serial.print(" ");
  Serial.println(printable_grams);

  delay(250);
}


