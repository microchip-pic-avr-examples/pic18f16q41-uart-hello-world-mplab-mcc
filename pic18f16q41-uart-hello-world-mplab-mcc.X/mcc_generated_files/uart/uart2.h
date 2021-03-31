/**
  UART2 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    uart2.h

  @Summary
    This is the generated header file for the UART2 driver using CCL

  @Description
    This header file provides APIs for driver for UART2.
    Generation Information :
        Driver Version    :  2.4.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 v2.2
        MPLAB             :  Standalone
*/

/*
Copyright (c) [2012-2020] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/

#ifndef UART2_H
#define UART2_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "uart_interface.h"

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: Macro Declarations
*/

#define UART2_isDataReady  (UART2_IsRxReady())
extern const struct UART_INTERFACE UART2_Interface;

/**
  Section: Data Type Definitions
*/

typedef union {
    struct {
        unsigned perr : 1;
        unsigned ferr : 1;
        unsigned oerr : 1;
        unsigned reserved : 5;
    };
    uint8_t status;
}uart2_status_t;

//*********************************************************************************************************
/**
 * @deprecated
 * Deprecated APIs start
 */
//*********************************************************************************************************

bool __attribute__(( deprecated )) UART2_is_tx_ready(void);
bool __attribute__(( deprecated )) UART2_is_rx_ready(void);
bool __attribute__(( deprecated )) UART2_is_tx_done(void);
uart2_status_t __attribute__(( deprecated )) UART2_get_last_status(void);

/*************************************************************************************************************
 * Deprecated APIs end
 *************************************************************************************************************/


/**
  Section: UART2 APIs
*/

/**
  @Summary
    Initialization routine that takes inputs from the UART2 GUI.

  @Description
    This routine initializes the UART2 driver.
    This routine must be called before any other UART2 routine is called.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment

  @Example
*/
void UART2_Initialize(void);

/**
  @Summary
    Checks if the UART2 receiver ready for reading

  @Description
    This routine checks if UART2 receiver has received data 
    and ready to be read

  @Preconditions
    UART2_Initialize() function should be called
    before calling this function
    UART2 receiver should be enabled before calling this 
    function

  @Param
    None

  @Returns
    Status of UART2 receiver
    TRUE: UART2 receiver is ready for reading
    FALSE: UART2 receiver is not ready for reading
    
  @Example
    <code>
    void main(void)
    {
        volatile uint8_t rxData;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        while(1)
        {
            // Logic to echo received data
            if(UART2_IsRxReady())
            {
                rxData = UART2_Read();
                if(UART2_IsTxReady())
                {
                    UART2_Write(rxData);
                }
            }
        }
    }
    </code>
*/
bool UART2_IsRxReady(void);

/**
  @Summary
    Checks if the UART2 transmitter is ready to transmit data

  @Description
    This routine checks if UART2 transmitter is ready 
    to accept and transmit data byte

  @Preconditions
    UART2_Initialize() function should have been called
    before calling this function.
    UART2 transmitter should be enabled before calling 
    this function

  @Param
    None

  @Returns
    Status of UART2 transmitter
    TRUE: UART2 transmitter is ready
    FALSE: UART2 transmitter is not ready
    
  @Example
    <code>
    void main(void)
    {
        volatile uint8_t rxData;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        while(1)
        {
            // Logic to echo received data
            if(UART2_IsRxReady())
            {
                rxData = UART2_Read();
                if(UART2_IsTxReady())
                {
                    UART2_Write(rxData);
                }
            }
        }
    }
    </code>
*/
bool UART2_IsTxReady(void);

/**
  @Summary
    Checks if UART2 data is transmitted

  @Description
    This function return the status of transmit shift register

  @Preconditions
    UART2_Initialize() function should be called
    before calling this function
    UART2 transmitter should be enabled and UART2_Write
    should be called before calling this function

  @Param
    None

  @Returns
    Status of UART2 transmit shift register
    TRUE: Data completely shifted out if the UART shift register
    FALSE: Data is not completely shifted out of the shift register
    
  @Example
    <code>
    void main(void)
    {
        volatile uint8_t rxData;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        while(1)
        {
            if(UART2_IsTxReady())
            {
                LED_0_SetHigh();
                UART2Write(rxData);
            }
            if(UART2_IsTxDone()
            {
                LED_0_SetLow();
            }
        }
    }
    </code>
*/
bool UART2_IsTxDone(void);

/**
  @Summary
    Gets the error status of the last read byte.

  @Description
    This routine gets the error status of the last read byte.

  @Preconditions
    UART2_Initialize() function should have been called
    before calling this function. The returned value is only
    updated after a read is called.

  @Param
    None

  @Returns
    the status of the last read byte

  @Example
	<code>
    void main(void)
    {
        volatile uint8_t rxData;
        volatile uart2_status_t rxStatus;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        // Enable the Global Interrupts
        INTERRUPT_GlobalInterruptEnable();
        
        while(1)
        {
            // Logic to echo received data
            if(UART2_IsRxReady())
            {
                rxData = UART2_Read();
                rxStatus = UART2_GetLastStatus();
                if(rxStatus.ferr){
                    LED_0_SetHigh();
                }
            }
        }
    }
    </code>
 */
uart2_status_t UART2_GetLastStatus(void);

/**
  @Summary
    Read a byte of data from the UART2.

  @Description
    This routine reads a byte of data from the UART2.

  @Preconditions
    UART2_Initialize() function should have been called
    before calling this function. The transfer status should be checked to see
    if the receiver is not empty before calling this function.
	
	UART2_isDataReady is a macro which checks if any byte is received.
	Call this macro before using this function.

  @Param
    None

  @Returns
    A data byte received by the driver.
	
  @Example
	<code>
            void main(void) {
                            // initialize the device
                            SYSTEM_Initialize();
                            uint8_t data;

                            // Enable the Global Interrupts
                            INTERRUPT_GlobalInterruptEnable();

                            // Enable the Peripheral Interrupts
                            INTERRUPT_PeripheralInterruptEnable();

                            printf("\t\tTEST CODE\n\r");		//Enable redirect STDIO to USART before using printf statements
                            printf("\t\t---- ----\n\r");
                            printf("\t\tECHO TEST\n\r");
                            printf("\t\t---- ----\n\n\r");
                            printf("Enter any string: ");
                            do{
                            data = UART2_Read();		// Read data received
                            UART2_Write(data);			// Echo back the data received
                            }while(!UART2_isDataReady);		//check if any data is received

                    }
    </code>
*/
uint8_t UART2_Read(void);

 /**
  @Summary
    Writes a byte of data to the UART2.

  @Description
    This routine writes a byte of data to the UART2.

  @Preconditions
    UART2_Initialize() function should have been called
    before calling this function. The transfer status should be checked to see
    if transmitter is not busy before calling this function.

  @Param
    txData  - Data byte to write to the UART2

  @Returns
    None
  
  @Example
      <code>
          Refer to UART2_Read() for an example	
      </code>
*/
void UART2_Write(uint8_t txData);


/**
  @Summary
    Maintains the driver's transmitter state machine and implements its ISR.

  @Description
    This routine is used to maintain the driver's internal transmitter state
    machine.This interrupt service routine is called when the state of the
    transmitter needs to be maintained in a non polled manner.

  @Preconditions
    UART2_Initialize() function should have been called
    for the ISR to execute correctly.

  @Param
    None

  @Returns
    None
*/     
void UART2_Transmit_ISR(void);

/**
  @Summary
    Maintains the driver's receiver state machine and implements its ISR

  @Description
    This routine is used to maintain the driver's internal receiver state
    machine.This interrupt service routine is called when the state of the
    receiver needs to be maintained in a non polled manner.

  @Preconditions
    UART2_Initialize() function should have been called
    for the ISR to execute correctly.

  @Param
    None

  @Returns
    None
*/       
void UART2_Receive_ISR(void);

/**
  @Summary
    Maintains the driver's receiver state machine

  @Description
    This routine is called by the receive state routine and is used to maintain 
    the driver's internal receiver state machine. It should be called by a custom
    ISR to maintain normal behavior

  @Preconditions
    UART2_Initialize() function should have been called
    for the ISR to execute correctly.

  @Param
    None

  @Returns
    None
*/
void UART2_RxDataHandler(void);

/**
  @Summary
    Set UART2 Framing Error Handler

  @Description
    This API sets the function to be called upon UART2 framing error

  @Preconditions
    Initialize  the UART2 before calling this API

  @Param
    Address of function to be set as framing error handler

  @Returns
    None
*/
void UART2_SetFramingErrorHandler(void (* interruptHandler)(void));

/**
  @Summary
    Set UART2 Overrun Error Handler

  @Description
    This API sets the function to be called upon UART2 overrun error

  @Preconditions
    Initialize  the UART2 module before calling this API

  @Param
    Address of function to be set as overrun error handler

  @Returns
    None
*/
void UART2_SetOverrunErrorHandler(void (* interruptHandler)(void));

/**
  @Summary
    Set UART2 Error Handler

  @Description
    This API sets the function to be called upon UART2 error

  @Preconditions
    Initialize  the UART2 module before calling this API

  @Param
    Address of function to be set as error handler

  @Returns
    None
*/
void UART2_SetErrorHandler(void (* interruptHandler)(void));

/**
  @Summary
    Maintains the driver's error interrupt state machine and implements its ISR

  @Description
    This routine is used to maintain the driver's error interrupt state
    machine.This interrupt service routine is called when the state of the
    UART2 needs to be maintained in a non polled manner.

  @Preconditions
    UART2_Initialize() function should have been called
    for the ISR to execute correctly.

  @Param
    None

  @Returns
    None
*/
void UART2_FramingError_ISR(void);

/**
  @Summary
    Maintains the driver's interrupt state machine while in sleep and implements its ISR

  @Description
    This routine is used to maintain the driver's interrupt state
    machine when device is in sleep and UART2 operation is ceased. 
	This interrupt service routine is called when the state of the
    UART2 needs to be maintained in a non polled manner.

  @Preconditions
    UART2_Initialize() function should have been called
    for the ISR to execute correctly.

  @Param
    None

  @Returns
    None
*/
void UART2_UartInterrupt_ISR(void);

/**
  @Summary
    UART2 Receive Interrupt Handler

  @Description
    This is a pointer to the function that will be called upon UART2 receive interrupt

  @Preconditions
    Initialize  the UART2 module with receive interrupt enabled

  @Param
    None

  @Returns
    None
*/
void (*UART2_RxInterruptHandler)(void);

/**
  @Summary
    UART2 Transmit Interrupt Handler

  @Description
    This is a pointer to the function that will be called upon UART2 transmit interrupt

  @Preconditions
    Initialize  the UART2 module with transmit interrupt enabled

  @Param
    None

  @Returns
    None
*/
void (*UART2_TxInterruptHandler)(void);

/**
  @Summary
    UART1 Framing Error Interrupt Handler

  @Description
    This is a pointer to the function that will be called upon UART1 framing error interrupt

  @Preconditions
    Initialize  the UART1 module with UART1 error interrupt (UxEIE) enabled

  @Param
    None

  @Returns
    None
*/
void (*UART2_FramingErrorInterruptHandler)(void);

/**
  @Summary
    UART1 Interrupt Handler

  @Description
    This is a pointer to the function that will be called upon UART1 interrupt

  @Preconditions
    Initialize  the UART1 module with UART1 interrupt (UxIE) and Wake-Up Enabled

  @Param
    None

  @Returns
    None
*/
void (*UART2_UARTInterruptHandler)(void);

/**
  @Summary
    Set UART2 Receive Interrupt Handler

  @Description
    This API sets the function to be called upon UART2 receive interrupt

  @Preconditions
    Initialize  the UART2 module with receive interrupt enabled before calling this API

  @Param
    Address of function to be set as receive interrupt handler

  @Returns
    None
*/
void UART2_SetRxInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Set UART2 Transmit Interrupt Handler

  @Description
    This API sets the function to be called upon UART2 transmit interrupt

  @Preconditions
    Initialize  the UART2 module with transmit interrupt enabled before calling this API

  @Param
    Address of function to be set as transmit interrupt handler

  @Returns
    None
*/
void UART2_SetTxInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Set UART1 Interrupt Handler

  @Description
    This API sets the function to be called upon UART1 interrupt

  @Preconditions
    Initialize  the UART1 module with interrupt (UxIE) and Wake-Up enabled before calling this API

  @Param
    Address of function to be set as interrupt handler

  @Returns
    None
*/
void UART2_SetFramingErrorInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Set UART1 Framing Error Interrupt Handler

  @Description
    This API sets the function to be called upon UART1 framing error interrupt

  @Preconditions
    Initialize  the UART1 module with error interrupt (UxEIE) enabled before calling this API

  @Param
    Address of function to be set as framing error interrupt handler

  @Returns
    None
*/
void UART2_SetUartInterruptHandler(void (* InterruptHandler)(void));

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  // UART2_H
/**
 End of File
*/