/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.

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

#include <sam.h>
#include "board_definitions.h"

/**
 * \brief system_init() configures the needed clocks and according Flash Read Wait States.
 * At reset:
 * - OSC8M clock source is enabled with a divider by 8 (1MHz).
 * - Generic Clock Generator 0 (GCLKMAIN) is using OSC8M as source.
 * We need to:
 * 1) Enable OSC32K clock (External on-board 32.768Hz oscillator), will be used as DFLL48M reference.
 * 2) Put OSC32K as source of Generic Clock Generator 1
 * 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
 * 4) Enable DFLL48M clock
 * 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
 * 6) Modify PRESCaler value of OSCM to have 8MHz
 * 7) Put OSC8M as source for Generic Clock Generator 3
 */
// Constants for Clock generators
#define GENERIC_CLOCK_GENERATOR_MAIN      (0u)

#define GENERIC_CLOCK_GENERATOR_XOSC32K   (1u)
#define GENERIC_CLOCK_GENERATOR_OSC32K    (1u)
#define GENERIC_CLOCK_GENERATOR_XOSC      (1u)

#define GENERIC_CLOCK_GENERATOR_OSCULP32K (2u) /* Initialized at reset for WDT */
#define GENERIC_CLOCK_GENERATOR_OSC8M     (3u)

// Constants for Clock multiplexers
#define GENERIC_CLOCK_MULTIPLEXER_DFLL48M (0u)

void board_init(void)
{
  /* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
  NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;

  /* Turn on the digital interface clock */
  PM->APBAMASK.reg |= PM_APBAMASK_GCLK; //| PM_APBAMASK_RTC;

  //board_init_osc32k(); // This is too troublesome to use for DFLL48M source)
  //board_init_osc8m(); // This gets set up by Arduino core later on.
  //board_init_xosc(); // We don't actually need this clock during bootloader mode.

  board_init_dfll48_closed( GCLK_CLKCTRL_GEN_GCLK2 ); // Clock 2 is internal OSCULP32K (32.768KHz)

  board_init_set_dfll48_as_master(); // Run CPU at 48MHz!


  /*
   * Now that all system clocks are configured, we can set CPU and APBx BUS clocks.
   * These values are normally the ones present after Reset.
   */
  PM->CPUSEL.reg  = PM_CPUSEL_CPUDIV_DIV1;
  PM->APBASEL.reg = PM_APBASEL_APBADIV_DIV1_Val;
  PM->APBBSEL.reg = PM_APBBSEL_APBBDIV_DIV1_Val;
  PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1_Val;

  //board_init_usb_clock();
}

void board_init_osc32k(void)
{
    /* ----------------------------------------------------------------------------------------------
     * 1) Enable OSC32K clock (Internal 32.768Hz oscillator)
     */
    SYSCTRL->OSC32K.reg = SYSCTRL_OSC32K_STARTUP( 0x7u ) | /* cf table 15.10 of product datasheet in chapter 15.8.6 */
                           SYSCTRL_XOSC32K_ENABLE | 
                           SYSCTRL_XOSC32K_EN32K ; // |
                           // SYSCTRL_OSC32K_CALIB( 0x12u ) ;
    SYSCTRL->OSC32K.bit.ENABLE = 1; /* separate call, as described in chapter 15.6.3 */

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC32KRDY) == 0 )
    {
      /* Wait for oscillator stabilization */
    }

    /* Software reset the module to ensure it is re-initialized correctly */
    /* Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
     * CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete, as described in chapter 13.8.1
     */
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;

    while ( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) )
    {
      /* Wait for reset to complete */
    }

    /* ----------------------------------------------------------------------------------------------
     * 2) Put OSC32K as source of Generic Clock Generator 1
     */
    GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_OSC32K ); // Generic Clock Generator 1

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }

    /* Write Generic Clock Generator 1 configuration */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_OSC32K ) | // Generic Clock Generator 1
                        GCLK_GENCTRL_SRC_OSC32K | // Selected source is External 32KHz Oscillator
  //                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
                        GCLK_GENCTRL_GENEN;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }
}

void board_init_osc8m(void)
{
    /* ----------------------------------------------------------------------------------------------
     * 1) Enable internal OSC8M clock
     */
    SYSCTRL->OSC8M.bit.ENABLE = 1;
    SYSCTRL->OSC8M.bit.PRESC = 0x0; // Prescale of 0x0 = 1, 8MHz
    SYSCTRL->OSC8M.bit.ONDEMAND = 0;


    GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_OSC8M );
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_OSC8M ) | // Generic Clock Generator 3
		      GCLK_GENCTRL_SRC_OSC8M | // Selected source is RC OSC 8MHz (already enabled at reset)
    //                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
		      GCLK_GENCTRL_GENEN ;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for oscillator stabilization */
    }

    /* Software reset the module to ensure it is re-initialized correctly */
    /* Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
     * CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete, as described in chapter 13.8.1
     */
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;

    while ( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) )
    {
      /* Wait for reset to complete */
    }

    /* Write Generic Clock Generator 3 configuration */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_OSC8M ) | // Generic Clock Generator 3
		      GCLK_GENCTRL_SRC_OSC8M | // Selected source is Internal 32KHz Oscillator
    //                    GCLK_GENCTRL_OE | // Output clock to a pin for tests
		      GCLK_GENCTRL_GENEN ;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }
}

void board_init_xosc(void)
{
    /* ----------------------------------------------------------------------------------------------
     * 1) Enable external XOSC clock
     */
    SYSCTRL->XOSC.reg = SYSCTRL_XOSC_STARTUP( 0x6u ) | /* cf table 15.10 of product datasheet in chapter 15.8.6 */
                         SYSCTRL_XOSC_XTALEN;
    SYSCTRL->XOSC.bit.ENABLE = 1; /* separate call, as described in chapter 15.6.3 */
    SYSCTRL->XOSC.bit.AMPGC = 1;

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSCRDY) == 0 )
    {
      /* Wait for oscillator stabilization */
    }

    /* Software reset the module to ensure it is re-initialized correctly */
    /* Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
     * CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete, as described in chapter 13.8.1
     */
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;

    while ( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) )
    {
      /* Wait for reset to complete */
    }

    /* Write Generic Clock Generator # configuration */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_XOSC ) | // Generic Clock Generator #
		    GCLK_GENCTRL_SRC_XOSC | // Selected source is External XOSC (12MHz) Oscillator
    //                    GCLK_GENCTRL_OE | // Output clock to a pin for tests
		    GCLK_GENCTRL_GENEN ;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }
}

void board_init_dfll48(unsigned int SOURCE)
{
    /* ----------------------------------------------------------------------------------------------
     * 3) Put Generic Clock Generator 3 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
     */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GENERIC_CLOCK_MULTIPLEXER_DFLL48M ) | // Generic Clock Multiplexer 0
			SOURCE | 
			GCLK_CLKCTRL_CLKEN ;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }

    /* ----------------------------------------------------------------------------------------------
     * 4) Enable DFLL48M clock
     */

    /* DFLL Configuration in Closed Loop mode, cf product datasheet chapter 15.6.7.1 - Closed-Loop Operation */

    /* Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905 */
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }

    /* get the coarse and fine values stored in NVM */
    uint32_t coarse = (*(uint32_t *)(0x806024) >> 26);
    uint32_t fine = (*(uint32_t *)(0x806028) & 0x3FF);

    SYSCTRL->DFLLVAL.bit.COARSE = coarse;
    SYSCTRL->DFLLVAL.bit.FINE = fine;


    // This is required when using closed-loop mode. I'm not sure if it's needed in open loop mode...
    SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP( (0x1f / 4) ) | // Coarse step
			   SYSCTRL_DFLLMUL_FSTEP( (0xff / 4) ) | // Fine step
			   SYSCTRL_DFLLMUL_MUL( (VARIANT_MCK/VARIANT_MAINOSC) );

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }

    /* Write full configuration to DFLL control register */
    SYSCTRL->DFLLCTRL.reg |= //SYSCTRL_DFLLCTRL_MODE | /* Enable the closed loop mode */
			     //SYSCTRL_DFLLCTRL_WAITLOCK |
			     SYSCTRL_DFLLCTRL_QLDIS ; /* Disable Quick lock */

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }

    /* Enable the DFLL */
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE ;

    // This is only used in closed loop mode.
    // I couldn't get execution past this when attempting to use closed loop mode.
    //  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 ||
    //          (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0 )
    //  {
    //    /* Wait for locks flags */
    //  }

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }
}

void board_init_dfll48_closed(unsigned int SOURCE)
{
    /* ----------------------------------------------------------------------------------------------
     * 3) Put Generic Clock Generator SOURCE as source for Generic Clock Multiplexer 0 (DFLL48M reference)
     */
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GENERIC_CLOCK_MULTIPLEXER_DFLL48M ) | // Generic Clock Multiplexer 0
			SOURCE |
			GCLK_CLKCTRL_CLKEN ;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }

    /* ----------------------------------------------------------------------------------------------
     * 4) Enable DFLL48M clock
     */

    /* DFLL Configuration in Closed Loop mode, cf product datasheet chapter 15.6.7.1 - Closed-Loop Operation */

    /* Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905 */
    SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }

    /* get the coarse and fine values stored in NVM */
    uint32_t coarse = 0x1f; //(*(uint32_t *)(0x806024) >> 26); // Chip has 0x18 (24), SMART says 0x1f (31)
    uint32_t fine = 0x0200; //(*(uint32_t *)(0x806028) & 0x3FF); // Chip has 0x0200 (512), SMART says 0xff (512)

    SYSCTRL->DFLLVAL.bit.COARSE = coarse;
    SYSCTRL->DFLLVAL.bit.FINE = fine;


    // This is required when using closed-loop mode. I'm not sure if it's needed in open loop mode...
    // SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP( (0x1f / 4) ) | // Coarse step
			 //   SYSCTRL_DFLLMUL_FSTEP( (0xff / 4) ) | // Fine step
			 //   SYSCTRL_DFLLMUL_MUL( (VARIANT_MCK/VARIANT_MAINOSC) );
    SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP( (8ul) ) | // Coarse step
         SYSCTRL_DFLLMUL_FSTEP( (64ul) ) | // Fine step
         SYSCTRL_DFLLMUL_MUL( (1465ul) );


    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }

    /* Write full configuration to DFLL control register */
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | /* Enable the closed loop mode */
			     SYSCTRL_DFLLCTRL_WAITLOCK |
			     SYSCTRL_DFLLCTRL_QLDIS ; /* Disable Quick lock */

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }

    /* Enable the DFLL */
    SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE ;

    // This is only used in closed loop mode.
    // I couldn't get execution past this when attempting to use closed loop mode.
    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 ||
	    (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0 )
    {
      /* Wait for locks flags */
    }

    while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
    {
      /* Wait for synchronization */
    }
}

void board_init_set_dfll48_as_master(void)
{
    /* ----------------------------------------------------------------------------------------------
     * 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
     */
    GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_MAIN ) ; // Generic Clock Generator 0

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }

    /* Write Generic Clock Generator 0 configuration */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_MAIN ) | // Generic Clock Generator 0
			GCLK_GENCTRL_SRC_DFLL48M | // Selected source is DFLL 48MHz
    //                    GCLK_GENCTRL_OE | // Output clock to a pin for tests
			GCLK_GENCTRL_IDC | // Set 50/50 duty cycle
			GCLK_GENCTRL_GENEN ;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }
}

void board_init_usb_clock(void)
{
  /* ----------------------------------------------------------------------------------------------
   * Put Generic Clock Generator 0 as source for Generic Clock Multiplexer 6 (USB reference)
   */
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( 6 ) | // Generic Clock Multiplexer 6
              GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
              GCLK_CLKCTRL_CLKEN ;

  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
  {
    /* Wait for synchronization */
  }

  /* Reset */
  USB->DEVICE.CTRLA.bit.SWRST = 1;
  while (USB->DEVICE.SYNCBUSY.bit.SWRST)
  {
    /* Sync wait */
  }
}

void board_init_set_osc8m_as_master(void)
{
    /* ----------------------------------------------------------------------------------------------
     * 5) Switch Generic Clock Generator 0 to OSC8M. CPU will run at 8MHz.
     */
    GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_MAIN ) ; // Generic Clock Generator 0

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }

    /* Write Generic Clock Generator 0 configuration */
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_MAIN ) | // Generic Clock Generator 0
				GCLK_GENCTRL_SRC_OSC8M | // Selected source is OSC8M
    //                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
		      GCLK_GENCTRL_IDC | // Set 50/50 duty cycle
		      GCLK_GENCTRL_GENEN ;

    while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
    {
      /* Wait for synchronization */
    }
}
