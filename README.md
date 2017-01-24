# Minifigure-SAMD21E
SAMD21E based Minifigure shaped development board.

## About
This circuit board was designed as a fun project for a SAMD21E microcontroller based development board.  The circuit design follows the recommended design from the SAMD Datasheet - including section 39. Schematic Checklist.  The USB protection may be overkill, but it is complete.  I made this just for fun - I am not going to provide any special support, but always welcome recommendations, suggestions, and ideas for improvements.  You are free to take it and run with it, but do not sell this design.

Order boards here:
Board 1: Stand alone - haven't tested, but should soon. [OSHPark](https://oshpark.com/shared_projects/Wh7NwFnv)

Board 2: This does not have a native USB or programming port - I am developing a backpack for it.  Will share backpack. [OSHPark](https://oshpark.com/shared_projects/W2bUXtFr)

MINI SAM TQFP SIMPLE - This version is more friendly to hand soldering with a TQFP SAMD21E chip.  USB protection and filtering removed, reset debounce removed.  [OSHPARK](https://oshpark.com/shared_projects/yFTXsDW0)

MINI SAM TQFP 1.27 SIMPLE - This version retains the TQFP SAMD21E chip, but on the back instead of the tiny 8-pin connector - I've replaced it with a 1.27mm pitch SMT connector.  Still trying to retain a "backpack" idea for add-on boards.  [OSHPARK](https://oshpark.com/shared_projects/vt6FGicv)

MINI SAM TQFP 2.54 BB (BreadBoard or Protoboard) - This version changes the rear once more to a 2.54mm pitch SMT setup for a protoboard friendly design. [OSHPARK](https://oshpark.com/shared_projects/SMD5UkUu)

Mini SAM - Protoboard - TQFP - 2017-01-17-00 - This version is similar to the other BB version, but has some refinements to layout and includes options for the onboard LED (Basic LED or the WS2812B), a plain button, and other enhancements.  [OSHPARK](https://oshpark.com/shared_projects/IhRoccbe)

## Etc
I have not yet tested the design.  As it sits, it should pass DRC for OSHPark manufacture with two major unknowns.  1) Wether or not the cutouts for the hands will be manufactured as intended and 2) The exact outline of the board - there may be some corners, nooks, and crannies that are not manufactured by OSHPark and will require additional filing/work by me.  I will update once I get a set manufactured.

## BOM
*The BOM listed at mouser is for the Mini SAM - Protoboard - TQFP - 2017-01-17-004 board.  It is a good place to start, but keep in mind you may want to change or leave out items you don't want.*

The schematic has all items listed and values and should be easily sourced.  Additionally, I have setup a [project on mouser.com that can be found here.](http://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=328cc4307e)  The BOM through mouser is roughly 12 USD at this time (Oct 2016).  It does not include the WS8212B 3535 LED - these can be sourced from [Adafruit](https://www.adafruit.com/products/2659) or eBay among others.  The LED does not need to be populated for the board to function, but it is fun.

## Firmware
I plan on trying to implement both Arduino and possibly Micropython on this board, but need some significant help with this.  I will update as I can.

In order to test my prototype board, I loaded the Arduino Zero bootloader, but changes the PortA pin 14 for PostA Pin 27.  Nothing else is fucntional yet - it was a bad hack to get access to the WS2812 LED for the time being and to prove to myself the board will work in theory.

(01-20-2017) FIRMWARE has been updated and almost complete.  I'm working through the last bit of verification and validation now.  Will post asap.

## License
CC BY SA 4.0
