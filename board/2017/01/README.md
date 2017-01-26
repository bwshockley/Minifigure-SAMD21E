##Board 2017/01-001

This is a LEGO Minifigure sized development board using an Atmel SAMD21E 32-Bit ARM chip. I basically followed the recommended layout/setup from the SAMD21 spec sheets.

Included on Mini SAM is a WS2812B LED pad connected to PIN 28, a 0603 LED connected to PIN 14, a reset button with circuitry per datasheet, and a user programmable button that is active HIGH. The 0603 LED is connected in series with a 1Kohm resistor, part of R1, and care should be taken when selecting an 0603 LED such that at a supply of 3.3V, no more than 5mA is required.

Programming port is a basic 10-pin SWD for ARM based processors, which is a 2x5 1.27mm pitch connector. The SWD line is pulled up with a 1Kohm resistor, part of R1.

The SDA/SCL pins are pulled high with 4.7KOhm resistors, R2, per SDA/SCL recommendations.

The voltage regulator is an MCP1700 which is rated at 250mA at 3.3V. Plenty of current for the SAMD21E and a few other accessories such as the WS2812B.

This version has SMT layout for two single row 2.54mm pitch headers on the back. It is designed to be protoboard friendly and when populated the headers are the same width as a DIL package (7.62mm or .3") and span the center of a protoboard.

PINs 27 and 28 are not connected to headers - due to lack of space - but instead have two pads near the bottom of the board. PIN 27 is connected to the on-board WS2812B. PIN 28 is connected to the on-board BUTTON.

The AREF PIN - PIN AREF - has a pair of decoupling/filtering capacitors.

The micro-USB connector has an RC filter between the case/shield of the connector and GND. If left un-populated the case/shield is an open circuit - which is the next best option. Also included is space for a USBLC6-2 ESD protection circuit. You can bypass it by jumping PAD 1 to 6, and PAD 3 to 4, leaving the middle two PADs alone.

I have put together a sample BOM via Mouser.com which I’m including here. It can be used as a reference. Items that do not need to be populated to work: SW2812B 3535 LED, 0603 LED, SW2 (BUTTON). The SW2812B is not in the Mouser BOM - due to lack of parts. I would suggest not purchasing the 2.54mm pitch SMT male headers from Mouser, as you can find this much cheaper elsewhere. Mini SAM requires (2) 11-PIN headers. I suggest buying breakaway headers in 25-50 PIN lengths.

Arduino FIRMWARE/BOOTLOADER is complete, but I’m still testing all functions. So far all PINs work in digital setup and all analog PINs work accordingly. ANALOG and special function pins (MOSI, SDA, SCK, etc) can be accessed in Arduino by their name.
