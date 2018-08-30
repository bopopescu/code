/************************************************************************
* Copyright � 2010 Altera Corporation, San Jose, California, USA.
* All rights reserved. All use of this software and documentation is
* subject to the License Agreement located at the end of this file below.
*************************************************************************
* Author:       S. O'Reilly
* Date:         1/8/10
*************************************************************************
* Description:
*
* Note: Use of this source file is explained further in the application
* note #459, titled, "Guidelines for Developing a Nios II HAL Device Driver".
*
* The following is a uart polled mode test program.
* Change the STDIO values for your bsp to uart1 (or the name of your hardware
* design's uart peripheral).
*
* In alt_sys_init.c, comment out the line
* to INIT the Uart.  We will initialize the uart here instead, simulating
* the way you would initialize a new device for which the INIT macro does
* not yet exist.
*
* Requirements:
*   -Supported Example Hardware Platforms
*     Full Featured
*   -Supported Development Boards
*     Nios II Embedded Development Kit
*     (To port this test code to other boards, just replace the uart
*     hard-coded base address value of 0x80 used in main()
*     with the uart register base address of the target board.
*     If the uart has a name other than 'uart1', the UART1_BASE
*     macro usage must be changed to reflect the new device name.
*   -Board Support Package Settings
*     STDOUT, STDIN, STDERR:  uart1
*   -Know Issues
*     None
*
**************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include "alt_types.h"
#include "system.h"
#include <io.h>

/* Include alt_log logging facility for debugging. MACROS are used to bypass
 * the driver and access the peripheral directly, so that messages can be
 * printed during boot before devices get initialized.
 *
 * Set hal.log_port to the desired logging device, using the name as specified
 * in SOPC Builder, such as jtag_uart or uart1 for the AN459 example HW design.
 * system.h will be generated to
 * set the following parameters to use alt_log with jtag_uart:
 *
 *          alt_log_port_base = "JTAG_UART_BASE";
 *          alt_log_port_type = "altera_avalon_jtag_uart";
 *
 * The SOF image does not need to be regenerated in SOPC Builder or
 * recompiled in Quartus.  Only the application and board support package
 * need to have their makefiles modified and to be recompiled.  This can
 * be accomplished by using the following
 * switches to the nios2-bsp command in a create-this-bsp script:
 *
 * --set hal.log_flags 3
 * --set hal.log_port jtag_uart
 *
 * The hal.log_port and hal.log_flags settings will cause the following
 * build flags to be specified in the public.mk makefile fragment shared by
 * app and bsp projects:
 *
 *          -DALT_LOG_ENABLE
 *          -DALT_LOG_FLAGS=3
 *
 * ALT_LOG_FLAGS valid value range is -1 to 3.
 *    (3 is the most verbose, 0 is boot messages only, -1 is silent.)
 *
 * The writes are blocking by default, so a terminal (such as the
 * nios2-terminal command) needs to accept the alt_log output in order for
 * the Nios II application to complete boot initialization.
 *
 * You can add alt_log diagnostic messages to your code by invoking
 * ALT_LOG_PRINTF(), a macro that handles most printf options except for
 * floating point (%f or %g).
 */

#include "sys/alt_log_printf.h"

/* The following definitions from altera_nios2\HAL\inc\io.h for macros IORD()
 * and IOWR() translate into ldwio and stwio Nios II assembler instructions
 * which bypass the cache, reading and writing values directly to peripheral
 * memory mapped registers.
 *
#define __IO_CALC_ADDRESS_NATIVE(BASE, REGNUM) \
  ((void *)(((alt_u8*)BASE) + ((REGNUM) * (SYSTEM_BUS_WIDTH/8))))

#define IORD(BASE, REGNUM) \
  __builtin_ldwio (__IO_CALC_ADDRESS_NATIVE ((BASE), (REGNUM)))
#define IOWR(BASE, REGNUM, DATA) \
  __builtin_stwio (__IO_CALC_ADDRESS_NATIVE ((BASE), (REGNUM)), (DATA))

 * The above macros implement native addressing, an addressing mode that has
 * been deprecated, since Altera is moving to a direct addressing model.
 * New peripherals should be written to use byte enable signals.
 * New device drivers for these new peripherals should use direct
 * addressing macros instead, such as IORD_32DIRECT().  Offsets for direct
 * address macros are always in bytes.
 *
 * The bit_bang_uart example application uses native addressing.
 * The my_uart device driver also uses native addressing, as do all Altera
 * device drivers in version 9.1.
 *
 * For example, use of a native addressing macro:
 *   IOWR(UART1_BASE, 2, 0);
 *
 * translates to the following direct addressing macro:
 *   IOWR_32DIRECT(UART1_BASE, 8, 0);
 *
 * Notice that the offset specified is now 8 bytes, instead of 2 long words.
 *
 * For more details on direct addressing macros, see the "Nios II Software
 * Developer's Handbook", chapter 9 - "Cache and Tightly-Coupled Memory",
 * Table 9-1, "HAL I/O macros to Bypass the Data Cache".
 */


/* First, try direct manipulation of the peripheral registers.
 * The UART1_BASE address comes from system.h, generated from the definition
 * in SOPC Builder for the base address of the Altera_Avalon_Uart peripheral
 * named uart1. For the Nios II Embedded Evaluation Kit, a
 * hardware reference design in included with this application note #459.
 * In this hardware design, this uart's memory mapped register base address
 * is 0x80.
 *
 * These initial device tests assume no flow control for simplicity.
 * Configure the terminal connected to the other end of the UART
 * to not use flow control.  Flow control is not possible on the Nios II
 * Embedded Evaluation Kit, as only the UART's receive and transmit pins are
 * actually connected to the FPGA by routing them through the HSMC connector.
 */

/*
 * Test UART reception via direct UART peripheral register manipulation.
 */
void BitBangUartReceive()
{
  /* uart_status is used to hold the value read from the UART status register.
   */
  int uart_status;

  /* Define an array of characters used to test transmission over the UART.
   */
  char input_request[30] = "\nPress any key: \0";

  /* index is used to select the next character from the input_request array.
   */
  int index;

  /* incoming_character is used to hold the value of a character received on
   * the UART.
   */
   char incoming_character;

  /* Clear any errors on the UART by zeroing the status register.
   */
  IOWR(UART1_BASE, 2, 0);

  /* Transmit the input_request characters over the UART.
   */
  for (index = 0; input_request[index] != '\0'; index++)
  {
    /* Wait for TRDY transmit ready bit to go high (bit 6 of status register).
     */
    while (!(uart_status = IORD(UART1_BASE, 2) & 0x40));

    /* Write the next input_request[] character to the txdata register.
     */
    IOWR(UART1_BASE, 1, input_request[index]);
  }

  /* Wait for incoming data by polling the RRDY bit of the UART status
   * register.  RRDY (bit 7) will go high when the uart's rxdata register
   * (offset 0x1 from the uart's memory mapped register base) receives
   * a character.
   */
  while (!(uart_status = IORD(UART1_BASE, 2) & 0x80));

  /* A receive character is ready, so read it from the rxdata register.
   */
  incoming_character = IORD(UART1_BASE, 0);

  /* Echo the key pressed on the UART.
   */

  /* Wait for TRDY transmit ready bit to go high (bit 6 of status register).
   */
  while (!(uart_status = IORD(UART1_BASE, 2) & 0x40));

  /* Write the pressed input character to the txdata register.
   */
  IOWR(UART1_BASE, 1, incoming_character);
}

/* Test UART transmission via direct peripheral register manipulation. */
void BitBangUartTransmit()
{
  /* uart_status is used to hold the value read from the UART status register.
   */
  int uart_status;

  /* Define an array of characters used to test transmission over the UART.
   */
  char bitbang[10] = "BIT BANG\0";

  /* index is used to select the next character from the bitbang array.
   */
  int index;

  /* Clear any old existing errors on the UART by zeroing the status register.
   */
  IOWR(UART1_BASE, 2, 0);

  /* First, try direct manipulation of the peripheral registers.
   * The UART1_BASE address comes from system.h, generated from the definition
   * in SOPC Builder for the base address of the Altera_Avalon_Uart peripheral
   * named uart1. For the Nios II Embedded Evaluation Kit, hardware reference
   * design included with this application note #459, this UART's memory mapped
   * register base address is 0x80.
   */
  for (index = 0; bitbang[index] != '\0'; index++)
  {
    /* Wait for TRDY transmit ready bit to go high (bit 6 of status register).
     * This initial device test assumes no flow control for simplicity.
     * Configure the terminal connected to the other end of the UART
     * to not use flow control.
     */
    while (!(uart_status = IORD(UART1_BASE, 2) & 0x40));

    /* Write the next bitbang[] character to the txdata register.
     */
    IOWR(UART1_BASE, 1, bitbang[index]);
  }

  /* The following UART txdata register writes (offset 1 from the UART's
   * memory mapped register base) will cause a transmitter overrun
   * by successive writes of an 'A', 'S', and 'H' to the txdata register
   * immediately after writing a 'B' to the txdata register, without checking
   * that the UART has had time to move the 'B' into the transmitter shift
   * register. This results in a UART status register value of 0x0170.
   */

  IOWR(UART1_BASE, 1, 'B');
  IOWR(UART1_BASE, 1, 'A');
  IOWR(UART1_BASE, 1, 'S');
  IOWR(UART1_BASE, 1, 'H');

  /* Place a breakpoint here after the line which writes an 'H' to the txdata
   * register.  When this breakpoint is hit, view the value of the UART status
   * register in the Nios II Software Build Tools for Eclipse memory window at
   * address 0x88.  (register offset 2, byte offset 8, from uart1's memory
   * mapped register base).
   * The exception bit (bit 8) and toe bit (bit 4) of the UART status register
   * are set by the UART to indicate transmission overrun.
   */

  uart_status = IORD(UART1_BASE, 2);
  /* At this point, the Nios II Software Build Tools for Eclipse Variables window
   * will also show the transmitter overrun via the variable named uart_status.
   */

  /* NOTE: Characters can be transmitted over the UART by directly writing
   * to txdata register via the Nios II Software Build Tools for Eclipse
   * memory view.
   */

   /* NOTE: Peripheral memory mapped registers bypass the data cache.
    * Therefore, the status register value displayed in the memory window will
    * reflect any changes to the status bits made by the peripheral. IOWR()
    * and IORD() macros always bypass the cache also.
    */
}

/* The main function tests the UART transmit and receive in polled mode
 * (no interrupts).
 */
int main(void)
{
  /* Write a single asterisk character to the UART by writing directly to the
   * UART peripheral's memory-mapped register.
   */
  IOWR(0x80,1,'*');

  /* Test polled mode UART transmit and receive.
   */
  ALT_LOG_PRINTF("Calling BitBangUartTransmit.\n");
  BitBangUartTransmit();
  ALT_LOG_PRINTF("Calling BitBangUartReceive.\n");
  BitBangUartReceive();
  ALT_LOG_PRINTF("Done.  Looping forever.\n");

  /* Loop forever when testing has completed.
   */
  while(1);

  return 0;

  /* The first IOWR() above, at the beginning of main(), shows that you can
   * write to a hard-coded base address for uart1 of 0x80.  If you are
   * not using a Nios II Embedded Evaluation Kit, change this hard-coded
   * address value to that of the UART register base address that matches your
   * SOPC Builder design.  This example usage of a hard-coded address value is
   * intended to demonstrate that it may be convenient when first verifying
   * hardware functionality.  Once the communication link to the hardware from
   * software is established, change the hard-coded address to UART1_BASE.  Use
   * of a hard-coded address initially can be useful when first bringing up new
   * hardware to rule out any software errors in obtaining this peripheral
   * memory-mapped registers base address.  Once hard-coded register addresses
   * are replaced with symbolic definitions based on the component name, such
   * as UART1_BASE, any SOPC Builder changes to the peripheral register base
   * address (and SOPC Builder regeneration and Quartus II recompile ) will also
   * require regeneration of the BSP.  One method of accomplishing this is to
   * use the Generate button in BSP Editor.  The changed UART1_BASE is passed
   * via system.h to the bit_bang_uart.c source file.
   */
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2010 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
