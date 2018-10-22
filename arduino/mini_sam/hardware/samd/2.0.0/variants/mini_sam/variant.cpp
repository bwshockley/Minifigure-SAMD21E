/*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
  Modified 17 January 2017 by Benjamin Shockley
     for Mini SAM D21E
*/


/*  Pins descriptions for Mini SAM
 *
 * | Arduino Pin Number |    Silkscreen    |  PIN   |   Alt. Function   | Comments (! means unavailable with this variant)
 * +--------------------+------------------+--------+-------------------+-------------------------------------------------
 * | --                 | --               |  PA00  | Xin32             | Xin32
 * | --                 | --               |  PA01  | Xout32            | Xout32
 * | 2                  | A0               |  PA02  |                   | EIC/EXTINT[2] ADC/AIN[0] PTC/Y[0] DAC/VOUT
 * | 3                  | AREF             |  PA03  | REFA              | EIC/EXTINT[3] REF/ADC/VREFA REF/DAC/VREFA ADC/AIN[1] PTC/Y[1]
 * | 4                  | ~A3              |  PA04  |                   | EIC/EXTINT[4] REF/ADC/VREFB ADC/AIN[4] AC/AIN[0] PTC/Y[2] !SERCOM0/PAD[0] TCC0/WO[0]
 * | 5                  | ~A4              |  PA05  |                   | EIC/EXTINT[5] ADC/AIN[5] AC/AIN[1] PTC/Y[3] !SERCOM0/PAD[1] TCC0/WO[1]
 * | 6                  | ~A1              |  PA06  |                   | !EIC/EXTINT[6] ADC/AIN[6] AC/AIN[2] PTC/Y[4] !SERCOM0/PAD[2] TCC1/WO[0]
 * | 7                  | ~A2              |  PA07  | Voltage Divider   | !EIC/EXTINT[7] ADC/AIN[7] AC/AIN[3] PTC/Y[5] !SERCOM0/PAD[3] TCC1/WO[1]
 * | 8                  | ~A5              |  PA08  |                   | EIC/NMI ADC/AIN[16] PTC/X[0] !SERCOM0/PAD[0] !SERCOM2/PAD[0] !TCC0/WO[0] TCC1/WO[2]
 * | 9                  | ~A6              |  PA09  |                   | EIC/EXTINT[9] ADC/AIN[17] PTC/X[1] !SERCOM0/PAD[1] !SERCOM2/PAD[1] !TCC0/WO[1] TCC1/WO[3]
 * | 10                 | TX               |  PA10  | TX                | !EIC/EXTINT[10] ADC/AIN[18] PTC/X[2] SERCOM0/PAD[2] !SERCOM2/PAD[2] !TCC1/WO[0] TCC0/WO[2]
 * | 11                 | RX               |  PA11  | RX                | !EIC/EXTINT[11] ADC/AIN[19] PTC/X[3] SERCOM0/PAD[3] !SERCOM2/PAD[3] !TCC1/WO[1] TCC0/WO[3]
 * | 14                 | ~14              |  PA14  |                   | EIC/EXTINT[14] SERCOM2/PAD[2] !TC3/WO[0] TCC0/WO[4]
 * | 15                 | ~15              |  PA15  | LED               | !EIC/EXTINT[15] SERCOM2/PAD[3] !TC3/WO[1] TCC0/WO[5]
 * | --                 | RST              |  ----  |                   | Reset
 * | --                 | Gnd              |  ----  |                   | Ground
 * | --                 | VDD              |  ----  |                   | VDD 3.3V
 * | --                 | Vin              |  ----  |                   | Vin 3.7V - 6.0V
 * | 16                 | ~MOSI            |  PA16  | SPI MOSI          | EIC/EXTINT[0] PTC/X[4] SERCOM1/PAD[0] !SERCOM3/PAD[0] TCC2/WO[0] !TCC0/WO[6]
 * | 17                 | ~SCK             |  PA17  | SPI SCK           | EIC/EXTINT[1] PTC/X[5] SERCOM1/PAD[1] !SERCOM3/PAD[1] TCC2/WO[1] !TCC0/WO[7]
 * | 18                 | ~SS              |  PA18  | SPI SS            | EIC/EXTINT[2] PTC/X[6] SERCOM1/PAD[2] !SERCOM3/PAD[2] TC3/WO[0] !TCC0/WO[2]
 * | 19                 | ~MISO            |  PA19  | SPI MISO          | EIC/EXTINT[3] PTC/X[7] SERCOM1/PAD[3] !SERCOM3/PAD[3] TC3/WO[1] !TCC0/WO[3]
 * | 22                 | ~SDA             |  PA22  | I2C/SDA w/pullup  | EIC/EXTINT[6] PTC/X[10] SERCOM3/PAD[0] !SERCOM5/PAD[0] TC4/WO[0] !TCC0/WO[4]
 * | 23                 | ~SCL             |  PA23  | I2C/SCL w/pullup  | EIC/EXTINT[7] PTC/X[11] SERCOM3/PAD[1] !SERCOM5/PAD[1] TC4/WO[1] !TCC0/WO[5]
 * | --                 | --               |  PA24  | USB Data -        | USB Connector
 * | --                 | --               |  PA25  | USB Data +        | USB Connector
 * | 27                 | 27               |  PA27  | RGB LED           | EIC/EXTINT[15] WS2812B LED
 * | 28                 | 28               |  PA28  |                   | EIC/EXTINT[15] Button
 * | 30                 | --               |  PA30  | SWD CLK           | EIC/EXTINT[10] !SERCOM1/PAD[2] TCC1/WO[0] SWD CLK
 * | 31                 | --               |  PA31  | SWD IO            | EIC/EXTINT[11] !SERCOM1/PAD[3] TCC1/WO[1] SWD IO
 * +--------------------+------------------+--------+-------------------+-------------------------------------------------
 *
 * You may use Arduino pin numbers ranging from 0 to 31.
 * For example, use pinMode(14, OUTPUT) to set the LED pin (marked as 14) as an output.
 * However, the following Arduino pin numbers are not mapped to a physical pin: 0, 1, 12, 13, 20, 21, 24, 25, 26, and 29.
 * Pins 0 and 1 are used by the 32.768KHz crystal which in turn is used by the Arduino core (the 16MHz crystal is unused by Arduino).
 * Pins 24 and 25 are in use by USB (USB_NEGATIVE and USB_POSITIVE).
 * Pin 27 is in use controlling the build in WS1812B RGB LED.
 * Pin 28 is not connected.
 * Pins 30 and 31 are in use by SWD Programming Port - Could eventually be utilized.
 */


#include "variant.h"

/*
 * Pins descriptions
 */
const PinDescription g_APinDescription[]=
{
	// 0..1 are unused (pins in use by 32.768KHz crystal, which in turn is used by the Arduino core)
	{ NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused
    { NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused

	// 2..9 - Analog capable pins (DAC available on 2)
	{ PORTA,  2, PIO_ANALOG, PIN_ATTR_ANALOG, ADC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // ADC/AIN[0]
	{ PORTA,  3, PIO_ANALOG, PIN_ATTR_ANALOG, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // DAC/VREFA
	{ PORTA,  4, PIO_ANALOG, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel4, PWM0_CH0, TCC0_CH0, EXTERNAL_INT_4 }, // ADC/AIN[4]
    { PORTA,  5, PIO_ANALOG, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel5, PWM0_CH1, TCC0_CH1, EXTERNAL_INT_5 }, // ADC/AIN[5]
	{ PORTA,  6, PIO_ANALOG, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel6, PWM1_CH0, TCC1_CH0, EXTERNAL_INT_6 }, // TCC1/WO[0]
    { PORTA,  7, PIO_ANALOG, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), ADC_Channel7, PWM1_CH1, TCC1_CH1, EXTERNAL_INT_7 }, // TCC1/WO[1]
	{ PORTA,  8, PIO_ANALOG, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), ADC_Channel16, PWM1_CH2, TCC1_CH2, EXTERNAL_INT_NMI },  // TCC0/WO[0]
	{ PORTA,  9, PIO_ANALOG, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), ADC_Channel17, PWM1_CH3, TCC1_CH3, EXTERNAL_INT_9 }, // TCC0/WO[1]

	// 10..11 - SERCOM/UART (TX/RX) (Serial1) or Analog or Digital functions
	{ PORTA, 10, PIO_SERCOM, (PIN_ATTR_ANALOG|PIN_ATTR_DIGITAL), ADC_Channel18, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_10 }, // TX: SERCOM0/PAD[2]
	{ PORTA, 11, PIO_SERCOM, (PIN_ATTR_ANALOG|PIN_ATTR_DIGITAL), ADC_Channel19, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11 }, // RX: SERCOM0/PAD[3]

	// 12..13 pins don't exist
	{ NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused
	{ NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused

	// 14..15 - Digital functions
	{ PORTA, 14, PIO_DIGITAL, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH4, TCC0_CH4, EXTERNAL_INT_14 }, // D14
	{ PORTA, 15, PIO_DIGITAL, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER_ALT), No_ADC_Channel, PWM0_CH5, TCC0_CH5, EXTERNAL_INT_15 }, // D15

	// 16..17 SERCOM/SPI (SPI) or Digital functions
	{ PORTA, 16, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH0, TCC2_CH0, EXTERNAL_INT_NONE }, // MOSI: SERCOM1/PAD[0]
	{ PORTA, 17, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM2_CH1, TCC2_CH1, EXTERNAL_INT_NONE }, // SCK: SERCOM1/PAD[1]
	{ PORTA, 18, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH0, TC3_CH0, EXTERNAL_INT_NONE }, // SS: SERCOM1/PAD[2]
	{ PORTA, 19, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM3_CH1, TC3_CH1, EXTERNAL_INT_NONE }, // MISO: SERCOM1/PAD[3]

  // 20..21 pins don't exist
	{ NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused
	{ NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused

	// 22..23 SERCOM/I2C (Wire) or Digital functions
	{ PORTA, 22, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM4_CH0, TC4_CH0, EXTERNAL_INT_NONE }, // SDA: SERCOM3/PAD[0]
    { PORTA, 23, PIO_SERCOM, (PIN_ATTR_DIGITAL|PIN_ATTR_PWM|PIN_ATTR_TIMER), No_ADC_Channel, PWM4_CH1, TC4_CH1, EXTERNAL_INT_NONE }, // SCL: SERCOM3/PAD[1]

	// 24..26 are unused (25 and 26 in use by USB_NEGATIVE and USB_POSITIVE, pin 26 does not exist)
	{ PORTA, 24, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DM
	{ PORTA, 25, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // USB/DP
	{ NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused

	// 27..29 RGB LED and Not Connected (pin 29 does not exist)
	{ PORTA, 27, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // D27
	{ PORTA, 28, PIO_DIGITAL, PIN_ATTR_DIGITAL, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // D28
	{ NOT_A_PORT,  0, PIO_NOT_A_PIN, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // Unused

	// 30..31 Digital functions / Debug interface (SWD CLK and SWD IO)
	{ PORTA, 30, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // SWD CLK
	{ PORTA, 31, PIO_COM, PIN_ATTR_NONE, No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_NONE }, // SWD IO

	// ----------------------
	// 32 - Alternate use of A0 (DAC output)
	{ PORTA,  2, PIO_ANALOG, PIN_ATTR_ANALOG, DAC_Channel0, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_2 }, // DAC/VOUT
} ;

const void* g_apTCInstances[TCC_INST_NUM+TC_INST_NUM]={ TCC0, TCC1, TCC2, TC3, TC4, TC5 } ;

// Multi-serial objects instantiation
SERCOM sercom0( SERCOM0 ) ;
SERCOM sercom1( SERCOM1 ) ;
SERCOM sercom2( SERCOM2 ) ;
SERCOM sercom3( SERCOM3 ) ;
SERCOM sercom4( SERCOM4 ) ;
SERCOM sercom5( SERCOM5 ) ;

Uart Serial( &sercom0, PIN_SERIAL_RX, PIN_SERIAL_TX, PAD_SERIAL_RX, PAD_SERIAL_TX ) ;

void SERCOM0_Handler()
{
  Serial1.IrqHandler();
}