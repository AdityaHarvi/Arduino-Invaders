# Arduino Invaders

## Materials
- 1 Arduino
- 36 Jumper Wires to Connect the Arduino to buttons, LEDs, and the speaker
- 5 Jumper Wires to Connect breadboard grounds together 
- 2 Buttons
- 1 Speaker
- 20 Red LEDs
- 7 Green LEDs
- 30 Resistors
- 3 Breadboards
- 1 Connecting Wire for Power (Either connecting to computer or wall)
- 1 Computer, or 1 Wall Socket

## Wiring Instructions
There are 27 LEDs in total, 7 of which are green and 20 of which are red. 
There are four LEDs on each row of the breadboards, with a total of 6 rows
spread over 3 breadboards. Four green LEDs should be placed in the row
closest to the Arduino, wired(from left to right) using ports 2 through 5 
as well as a resistor attached in series with each LED; the LED turned on 
represents the position of the player's spacecraft. Two buttons should also 
be placed on either side of the first row and wired using ports 8 and 9 
as well as 2 resistors. In the next row, on the right hand side, place the
three remaining green LEDs as close as possible to each other, wiring them 
using 3 resistors and ports 11 to 13 from left to right. These represent the 
player's lives; when all three are on, the player has three lives remaing, when 
two are on, they have two lives, and so on. Then, directly above the leftmost 
green LED, place 5 red LEDs, each in a different row of the three breadboards 
(in a straight line). Wire each LED using a resistor, and then, from top to 
bottom, wire the LEDs using ports 53-49. Do the same above the second green 
LED, wiring the red LEDs from ports 48-44 (again from top to bottom). Do the 
same thing once again for the third column of red LEDs, placing them directly 
above the third green LED and wiring them using ports 43, 42, 41, 40 and A0 
(from top to bottom). For the final column, place 5 more red LEDs and wire them 
using ports A1 through A5. Then, connect the speaker to port 31 and ground it 
using a resistor. Finally, connect all of the grounds together and connect the 
breadboard ground to the ground on the arduino.

## Wiring Diagram (the ports are listed in brackets)
Breadboard 3:

            Red(53)         Red(48)         Red(43)         Red(A1)


            Red(52)         Red(47)         Red(42)         Red(A2)


Breadboard 2:          

            Red(51)         Red(46)         Red(41)         Red(A3)


Speaker(31) Red(50)         Red(45)         Red(40)         Red(A4)


Breadboard 1:

            Red(49)         Red(44)         Red(A0)         Red(A5)     Green(11) Green(12) Green(13)

Button(8)                                                               Button(9)
            Green(2)        Green(3)        Green(4)        Green(5)

## Running Instructions
To run 'asteroids.cpp' with 1 Computer:
1) After wiring your arduino as stated above and connecting it to your 
computer, open terminal and navigate to the directory where "asteroids.cpp" 
is located,making sure your Makefile is in the same directory.
2) Enter "make", then "make upload" to upload the code to the Arduino.
3) Once the code has been uploaded, press and hold both side buttons
for three seconds until the game starts.
4) Move left and right to avoid the asteroids, and enjoy playing
"Arduino Asteroids"!

## Team
- Aditya Harvi
- Micheal Antifaoff

## Referemces
Used this to learn how to make sounds on the arduino:
https://learn.adafruit.com/adafruit-arduino-lesson-10-making-sounds/overview

Went through this guide to understand concurrency and the millis() function:
https://forum.arduino.cc/index.php?topic=503368.0

For further instructions, please see the wiring photos included in the asteroids
file, called asteroidsWiring1.jpg, asteroidsWiring2.jpg, and asteroidsWiring3.jpg.
