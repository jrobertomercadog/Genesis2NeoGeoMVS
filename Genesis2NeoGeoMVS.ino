//
// Genesis2NeoGeoMVS
// Inspired by the library "SegaController" 
//  from Jon Thysell <thysell@gmail.com> <http://jonthysell.com>
//
// Authors:
//       Robert Mercado <jrobertomercadog@gmail.com>
//       Jon Thysell <thysell@gmail.com> <http://jonthysell.com>
//
// Copyright (c) 2021 Robert Mercado <https://blog.lilmonika.info>
// Copyright (c) 2017 Jon Thysell <http://jonthysell.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <SegaController.h>

// Genesis Controller DB9 pins (looking face-on to the end of the plug):
//
// 5 4 3 2 1
//  9 8 7 6
//
// Connect pin 5 to +5V and pin 8 to GND
// Connect the remaining pins to digital I/O pins as follows:
// DB9 Pin 7 -> D8
// DB9 Pin 1 -> D2 
// DB9 Pin 2 -> D3 
// DB9 Pin 3 -> D4 
// DB9 Pin 4 -> D5 
// DB9 Pin 6 -> D6 
// DB9 Pin 9 -> D7

// NeoGeo MVS  DB15 pins (looking face-on to the end of the plug):
//
//   8    7   6  5  4  3  2   1
//    15  14  13  12  11 10  9
//
// Connect the Arduino to the NeoGeo MVS as follows:
// D9  -> MVS_11
// D10 -> MVS_04
// D11 -> MVS_12
// D12 -> MVS_05
// D14 -> MVS_09
// D15 -> MVS_02
// D16 -> MVS_10
// D17 -> MVS_03
// D18 -> MVS_13
// +5V -> MVS_01
// GND -> MVS_08

SegaController controller(8, 2, 3, 4, 5, 6, 7);

const unsigned long TOGGLE_TIME_WINDOW = 23;

// Controller states
word currentState = 0;
word lastState = 0;
byte bToggle = 0;
unsigned long lastReadToggle = 0;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(9, OUTPUT); //A
    pinMode(10, OUTPUT); //B
    pinMode(11, OUTPUT); //C
    pinMode(12, OUTPUT); //D

    pinMode(14, OUTPUT); //UP
    pinMode(15, OUTPUT); //DOWN
    pinMode(16, OUTPUT); //LEFT
    pinMode(17, OUTPUT); //RIGHT
    pinMode(18, OUTPUT); //START

    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);

    digitalWrite(14, HIGH);
    digitalWrite(15, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(17, HIGH);
    digitalWrite(18, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    lastReadToggle = millis();
}

void loop()
{
    currentState = controller.getState();
    processGenesis();
}

void processGenesis()
{
    if ((currentState != lastState)&&(currentState & SC_CTL_ON))
    {
      for(byte idx = 0; idx < 9; idx++) 
      {
        byte bThisInput=0;
        switch(idx)
        {          
          case 0:
            bThisInput = ((currentState & SC_BTN_A) ? LOW : HIGH);
            digitalWrite(9, bThisInput);
          break;
          case 1:
            bThisInput = ((currentState & SC_BTN_B) ? LOW : HIGH);
            digitalWrite(10, bThisInput);
          break;              
          case 2:
            bThisInput = ((currentState & SC_BTN_C) ? LOW : HIGH);
            digitalWrite(11, bThisInput);
          break;
          case 3:
            bThisInput = ((currentState & SC_BTN_Z) ? LOW : HIGH);
            digitalWrite(12, bThisInput);
          break;
          case 4:
            bThisInput = ((currentState & SC_BTN_UP) ? LOW : HIGH);
            digitalWrite(14, bThisInput);
          break;
          case 5:
            bThisInput = ((currentState & SC_BTN_DOWN) ? LOW : HIGH);
            digitalWrite(15, bThisInput);
            break;
          case 6:
            bThisInput = ((currentState & SC_BTN_LEFT) ? LOW : HIGH);
            digitalWrite(16, bThisInput);
          break;
          case 7:
            bThisInput = ((currentState & SC_BTN_RIGHT) ? LOW : HIGH);
            digitalWrite(17, bThisInput);
          break;
          case 8:
            bThisInput = ((currentState & SC_BTN_START) ? LOW : HIGH);
            digitalWrite(18, bThisInput);
          break;                                                                                                
        }
      }
    }
    
    word bCheckY = currentState&SC_BTN_Y;
    bCheckY = bCheckY >> 10;
    if(bCheckY) 
    {
      unsigned long timeDelta = millis() -lastReadToggle;
      if (timeDelta > TOGGLE_TIME_WINDOW) 
      {
        digitalWrite(9, !bToggle);
        bToggle = !bToggle;
        lastReadToggle = millis();        
      }
    }    
      
    if(!(currentState & SC_CTL_ON)) 
    {
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
  
      digitalWrite(14, HIGH);
      digitalWrite(15, HIGH);
      digitalWrite(16, HIGH);
      digitalWrite(17, HIGH);
      digitalWrite(18, HIGH);
    }      

    lastState = currentState;
}
