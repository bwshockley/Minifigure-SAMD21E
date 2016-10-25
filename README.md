# Minifigure-SAMD21E
SAMD21E based Minifigure shaped development board.

## About
This circuit board was designed as a fun project for a SAMD21E microcontroller based development board.  The circuit design follows the recommended design from the SAMD Datasheet - including section 39. Schematic Checklist.  The USB protection may be overkill, but it is complete.  I made this just for fun - I am not going to provide any special support, but always welcome recommendations, suggestions, and ideas for improvements.  You are free to take it and run with it, but do not sell this design.

Order boards here:
Board 1: Stand alone - haven't tested, but should soon. [OSHPark](https://oshpark.com/shared_projects/Wh7NwFnv)

Board 2: This does not have a native USB or programming port - I am developing a backpack for it.  Will share backpack. [OSHPark](https://oshpark.com/shared_projects/W2bUXtFr)

## Etc
I have not yet tested the design.  As it sits, it should pass DRC for OSHPark manufacture with two major unknowns.  1) Wether or not the cutouts for the hands will be manufactured as intended and 2) The exact outline of the board - there may be some corners, nooks, and crannies that are not manufactured by OSHPark and will require additional filing/work by me.  I will update once I get a set manufactured.

## BOM
The schematic has all items listed and values and should be easily sourced.  Additionally, I have setup a [project on mouser.com that can be found here.](http://www.mouser.com/ProjectManager/ProjectDetail.aspx?AccessID=328cc4307e)  The BOM through mouser is roughly 12 USD at this time (Oct 2016).  It does not include the WS8212B 3535 LED - these can be sourced from [Adafruit](https://www.adafruit.com/products/2659) or eBay among others.  The LED does not need to be populated for the board to function, but it is fun.

## Firmware
I plan on trying to implement both Arduino and possibly Micropython on this board, but need some significant help with this.  I will update as I can.

## License
For personal use only - do not sell.  You may modify as you wish.
