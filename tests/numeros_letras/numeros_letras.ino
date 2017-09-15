//  Author:Frankie.Chu
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#include "TM1637.h"
#define CLK 2//pins definitions for TM1637 and can be changed to other ports       
#define DIO 3
TM1637 dis7seg(CLK,DIO);

static int8_t digitos[] = {0x3f,0x06,0x5b,0x4f,
                           0x66,0x6d,0x7d,0x07,
                           0x7f,0x6f,0x77,0x7c,
                           0x39,0x5e,0x79,0x71};//0~9,A,b,C,d,E,F

char lets[] = {'A','b','B','c','C','d','e','E','F','g','G','h','H','i','I','J','L','n','o','O','P','q','r','S','t','u','U','y','Z'};

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
    default:
    let = 0b01010011;
  }
  return let;
}
void setup()
{
  dis7seg.init();
  dis7seg.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
}

void loop()
{
  for (int i = 0; i < sizeof(lets); i+=4){
    //"init"
    dis7seg.display_raw(0,letras(lets[i]));
    dis7seg.display_raw(1,letras(lets[i+1]));
    dis7seg.display_raw(2,letras(lets[i+2]));
    dis7seg.display_raw(3,letras(lets[i+3]));
    delay(2000);
  }
}
