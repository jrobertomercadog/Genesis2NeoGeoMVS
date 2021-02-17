//
// Genesis2NeoGeoMVS
//
// This project requires the Arduino library "SegaController" from Jon Thysell,
// Please follow the instructions at https://github.com/jonthysell/SegaController 
//
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
//   8   7   6  5  4  3  2   1
//   15  14  13  12  11  10 9
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
// 
// Solder a 0.1uF SMD capacitor (or tantalum) between GND and 5V at the Arduino pins.
// This is known as a "bypass" capacitor, it is not compulsory but it is highly advised.

const byte MVS_A = 9;
const byte MVS_B = 10;
const byte MVS_C = 11;
const byte MVS_D = 12;
const byte MVS_UP = 14;
const byte MVS_DOWN = 15;
const byte MVS_LEFT = 16;
const byte MVS_RIGHT = 17;
const byte MVS_START = 18;

SegaController controller(8, 2, 3, 4, 5, 6, 7);

const unsigned long TOGGLE_TIME_WINDOW = 23; //don't use any value lower than 20 millis, the default "23 millis" is the "sweet-spot" I've tested for auto-firing.

// Controller states
word currentState = 0;
word lastState = 0;
byte bToggle = 0;
unsigned long lastReadToggle = 0;

void setup()
{
    pinMode(MVS_A, OUTPUT); //A
    pinMode(MVS_B, OUTPUT); //B
    pinMode(MVS_C, OUTPUT); //C
    pinMode(MVS_D, OUTPUT); //D

    pinMode(MVS_UP, OUTPUT); //UP
    pinMode(MVS_DOWN, OUTPUT); //DOWN
    pinMode(MVS_LEFT, OUTPUT); //LEFT
    pinMode(MVS_RIGHT, OUTPUT); //RIGHT
    pinMode(MVS_START, OUTPUT); //START

    digitalWrite(MVS_A, HIGH); // MVS controllers seems to be pulled-up by default and a "press" is detected when transitions from HIGH to LOW.
    digitalWrite(MVS_B, HIGH);
    digitalWrite(MVS_C, HIGH);
    digitalWrite(MVS_D, HIGH);

    digitalWrite(MVS_UP, HIGH);
    digitalWrite(MVS_DOWN, HIGH);
    digitalWrite(MVS_LEFT, HIGH);
    digitalWrite(MVS_RIGHT, HIGH);
    digitalWrite(MVS_START, HIGH);
    lastReadToggle = millis();
}

void loop()
{
    currentState = controller.getState();
    processGenesis();
}

void processGenesis()
{
    if ((currentState != lastState)&&(currentState & SC_CTL_ON)) //processing for other buttons ocurrs only if there's new button presses (one or many) and also if the SEGA controller is detected.
    {
      for(byte idx = 0; idx < 9; idx++) 
      {
        byte bThisInput=0;
        switch(idx)
        {          
          case 0:
            bThisInput = ((currentState & SC_BTN_A) ? LOW : HIGH);
            digitalWrite(MVS_A, bThisInput);
          break;
          case 1:
            bThisInput = ((currentState & SC_BTN_B) ? LOW : HIGH);
            digitalWrite(MVS_B, bThisInput);
          break;              
          case 2:
            bThisInput = ((currentState & SC_BTN_C) ? LOW : HIGH);
            digitalWrite(MVS_C, bThisInput);
          break;
          case 3:
            bThisInput = ((currentState & SC_BTN_Z) ? LOW : HIGH);
            digitalWrite(MVS_D, bThisInput);
          break;
          case 4:
            bThisInput = ((currentState & SC_BTN_UP) ? LOW : HIGH);
            digitalWrite(MVS_UP, bThisInput);
          break;
          case 5:
            bThisInput = ((currentState & SC_BTN_DOWN) ? LOW : HIGH);
            digitalWrite(MVS_DOWN, bThisInput);
            break;
          case 6:
            bThisInput = ((currentState & SC_BTN_LEFT) ? LOW : HIGH);
            digitalWrite(MVS_LEFT, bThisInput);
          break;
          case 7:
            bThisInput = ((currentState & SC_BTN_RIGHT) ? LOW : HIGH);
            digitalWrite(MVS_RIGHT, bThisInput);
          break;
          case 8:
            bThisInput = ((currentState & SC_BTN_START) ? LOW : HIGH);
            digitalWrite(MVS_START, bThisInput);
          break;                                                                                                
        }
      }
    }
    //The "auto-fire" function, by default, is assigned to the "SEGA Y BUTTON"
    //This function triggers every X amount of time toggling the "MVS A BUTTON"
    //the "window time" is determined by the TOGGLE_TIME_WINDOW variable at the top.
    //
    //if you want to use the "SEGA X BUTTON" for auto-fire instead of "SEGA Y BUTTON",
    //uncomment the following two lines and comment the next two:
    //word bCheckTrigger = currentState&SC_BTN_X;
    //bCheckTrigger = bCheckTrigger >> 9;
    word bCheckTrigger = currentState&SC_BTN_Y; //START COMMENTING FROM HERE you want to disable "auto-fire"
    bCheckTrigger = bCheckTrigger >> 10;
    if(bCheckTrigger) 
    {
      unsigned long timeDelta = millis() -lastReadToggle;
      if (timeDelta > TOGGLE_TIME_WINDOW) 
      {
        digitalWrite(MVS_A, !bToggle); //you may change the toggle button if you like, MVS_A is assigned by default.
        bToggle = !bToggle;
        lastReadToggle = millis();        
      }
    }
    //COMMENT UP TO HERE if you want to disable "auto-fire"
      
    if(!(currentState & SC_CTL_ON)) //if the SEGA controller has been disconnected, pull-up all MVS buttons back. This avoids the issue of "stuck buttons".
    {
      digitalWrite(MVS_A, HIGH);
      digitalWrite(MVS_B, HIGH);
      digitalWrite(MVS_C, HIGH);
      digitalWrite(MVS_D, HIGH);
  
      digitalWrite(MVS_UP, HIGH);
      digitalWrite(MVS_DOWN, HIGH);
      digitalWrite(MVS_LEFT, HIGH);
      digitalWrite(MVS_RIGHT, HIGH);
      digitalWrite(MVS_START, HIGH);
    }      

    lastState = currentState;
}
