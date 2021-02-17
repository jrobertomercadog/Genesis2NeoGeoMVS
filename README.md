# Genesis2NeoGeoMVS #

Read Sega Genesis (Mega Drive) controller and use it on a Neo Geo MVS.
This project uses Jon Thysell "SegaController" library in order to function.
Check it out at: https://github.com/jonthysell/SegaController

## Preparation ##

1. Download the SegaController.zip Library from https://github.com/jonthysell/SegaController/releases/latest
2. Open the Arduino IDE
3. Open "Sketch > Include Library > Add .ZIP Library..."
4. Select the zip file you downloaded in step 1

## Assembly ##

The SEGA Controller pins are as follows, looking face on to the (female) DB9 plug:

    5 4 3 2 1
     9 8 7 6

1. Connect pin 5 to Arduino +5V 
2. Connect pin 8 to Arduino GND
3. DB9 Pin 7 -> D8
4. DB9 Pin 1 -> D2 
5. DB9 Pin 2 -> D3 
6. DB9 Pin 3 -> D4 
7. DB9 Pin 4 -> D5 
8. DB9 Pin 6 -> D6 
9. DB9 Pin 9 -> D7

The Neo Geo MVS pins are as follows, looking face on to the (female) DB15 plug:

    8   7   6  5  4  3  2   1
    15  14  13  12  11  10 9

Connect the Arduino to the NeoGeo MVS plug as follows:
1. Connect Arduino +5V to MVS pin 1 
2. Connect Arduino GND to MVS pin 8
3. D9  -> MVS 11
4. D10 -> MVS 04
5. D11 -> MVS 12
6. D12 -> MVS 05
7. D14 -> MVS 09
8. D15 -> MVS 02
9. D16 -> MVS 10
10. D17 -> MVS 03
11. D18 -> MVS 13

Finally, Solder a 0.1uF 0805 SMD capacitor (or tantalum one but leaving the leads very short) between GND and 5V at the Arduino pins.
This is known as a "bypass" capacitor, it is not compulsory but it is highly advised.
