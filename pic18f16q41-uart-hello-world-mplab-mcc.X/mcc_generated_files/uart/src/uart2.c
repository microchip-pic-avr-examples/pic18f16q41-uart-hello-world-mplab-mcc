/**
  UART2 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    uart2.c

  @Summary
    This is the generated driver implementation file for the UART2 driver using CCL

  @Description
    This source file provides APIs for UART2.
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

/**
  Section: Included Files
*/
#include <xc.h>
#include "../uart2.h"

const struct UART_INTERFACE UART2_Interface = {
  .Initialize = UART2_Initialize,
  .Write = UART2_Write,
  .Read = UART2_Read,
  .RxCompleteCallbackRegister = NULL,
  .TxCompleteCallbackRegister = NULL,
  .ErrorCallbackRegister = UART2_SetErrorHandler,
  .FramingErrorCallbackRegister = UART2_SetFramingErrorHandler,
  .OverrunErrorCallbackRegister = UART2_SetOverrunErrorHandler,
  .ParityErrorCallbackRegister = NULL,
  .ChecksumErrorCallbackRegister = NULL,
  .IsRxReady = UART2_IsRxReady,
  .IsTxReady = UART2_IsTxReady,
  .IsTxDone = UART2_IsTxDone
};

/**
  Section: Macro Declarations
*/
#define UART2_TX_BUFFER_SIZE 8
#define UART2_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
*/

static volatile uint8_t uart2TxHead = 0;
static volatile uint8_t uart2TxTail = 0;
static volatile uint8_t uart2TxBuffer[UART2_TX_BUFFER_SIZE];
volatile uint8_t uart2TxBufferRemaining;

static volatile uint8_t uart2RxHead = 0;
static volatile uint8_t uart2RxTail = 0;
static volatile uint8_t uart2RxBuffer[UART2_RX_BUFFER_SIZE];
static volatile uart2_status_t uart2RxStatusBuffer[UART2_RX_BUFFER_SIZE];
volatile uint8_t uart2RxCount;
static volatile uart2_status_t uart2RxLastError;

/**
  Section: UART2 APIs
*/
void (*UART2_FramingErrorHandler)(void);
void (*UART2_OverrunErrorHandler)(void);
void (*UART2_ErrorHandler)(void);

void UART2_DefaultFramingErrorHandler(void);
void UART2_DefaultOverrunErrorHandler(void);
void UART2_DefaultErrorHandler(void);

void UART2_Initialize(void)
{
    // Disable interrupts before changing states
    PIE8bits.U2RXIE = 0;
    UART2_SetRxInterruptHandler(UART2_Receive_ISR);
    PIE8bits.U2TXIE = 0;
    UART2_SetTxInterruptHandler(UART2_Transmit_ISR);
    PIE8bits.U2EIE = 0;
    UART2_SetFramingErrorInterruptHandler(UART2_FramingError_ISR);
    PIE8bits.U2IE = 0;
    UART2_SetUartInterruptHandler(UART2_UartInterrupt_ISR);

    // Set the UART2 module to the options selected in the user interface.
    // RXB disabled; 
    U2RXB = 0x0;
    // TXB disabled; 
    U2TXB = 0x0;
    // P1L 0x0; 
    U2P1L = 0x0;
    // P2L 0x0; 
    U2P2L = 0x0;
    // P3L 0x0; 
    U2P3L = 0x0;
    // MODE Asynchronous 8-bit mode; RXEN disabled; TXEN enabled; ABDEN disabled; BRGS normal speed; 
    U2CON0 = 0x20;
    // SENDB disabled; BRKOVR disabled; RXBIMD Set RXBKIF on rising RX input; WUE disabled; ON enabled; 
    U2CON1 = 0x80;
    // FLO off; TXPOL not inverted; STP Transmit 1Stop bit, receiver verifies first Stop bit; RXPOL not inverted; RUNOVF RX input shifter stops all activity; 
    U2CON2 = 0x0;
    // BRGL 12; 
    U2BRGL = 0xC;
    // BRGH 0; 
    U2BRGH = 0x0;
    // STPMD in middle of first Stop bit; TXWRE No error; 
    U2FIFO = 0x0;
    // ABDIE disabled; ABDIF Auto-baud not enabled or not complete; WUIF WUE not enabled by software; 
    U2UIR = 0x0;
    // TXCIF 0x0; RXFOIF not overflowed; RXBKIF No Break detected; CERIF No Checksum error; ABDOVF Not overflowed; 
    U2ERRIR = 0x0;
    // TXCIE disabled; RXFOIE disabled; RXBKIE disabled; FERIE disabled; CERIE disabled; ABDOVE disabled; PERIE disabled; TXMTIE disabled; 
    U2ERRIE = 0x0;

    UART2_SetFramingErrorHandler(UART2_DefaultFramingErrorHandler);
    UART2_SetOverrunErrorHandler(UART2_DefaultOverrunErrorHandler);
    UART2_SetErrorHandler(UART2_DefaultErrorHandler);

    uart2RxLastError.status = 0;

}

bool UART2_IsRxReady(void)
{
    return (bool)(PIR8bits.U2RXIF);
}

bool UART2_is_rx_ready(void)
{    
    return UART2_IsRxReady();
}

bool UART2_IsTxReady(void)
{
    return (bool)(PIR8bits.U2TXIF && U2CON0bits.TXEN);
}

bool UART2_is_tx_ready(void)
{
    return UART2_IsTxReady();
}

bool UART2_IsTxDone(void)
{
    return U2ERRIRbits.TXMTIF;
}

bool UART2_is_tx_done(void)
{
    return UART2_IsTxDone();
}

uart2_status_t UART2_GetLastStatus(void){
    return uart2RxLastError;
}

uart2_status_t UART2_get_last_status(void){
    return UART2_GetLastStatus();
}


uint8_t UART2_Read(void)
{
    while(!PIR8bits.U2RXIF)
    {
    }

    uart2RxLastError.status = 0;

    if(U2ERRIRbits.FERIF){
        uart2RxLastError.ferr = 1;
        UART2_FramingErrorHandler();
    }

    if(U2ERRIRbits.RXFOIF){
        uart2RxLastError.oerr = 1;
        UART2_OverrunErrorHandler();
    }

    if(uart2RxLastError.status){
        UART2_ErrorHandler();
    }

    return U2RXB;
}

void UART2_Write(uint8_t txData)
{
    while(0 == PIR8bits.U2TXIF)
    {
    }

    U2TXB = txData;    // Write the data byte to the USART.
}

char getch(void)
{
    return UART2_Read();
}

void putch(char txData)
{
    UART2_Write(txData);
}

void UART2_Transmit_ISR(void)
{
    // use this default transmit interrupt handler code
    if(sizeof(uart2TxBuffer) > uart2TxBufferRemaining)
    {
        U2TXB = uart2TxBuffer[uart2TxTail++];
       if(sizeof(uart2TxBuffer) <= uart2TxTail)
        {
            uart2TxTail = 0;
        }
        uart2TxBufferRemaining++;
    }
    else
    {
        PIE8bits.U2TXIE = 0;
    }
    
    // or set custom function using UART2_SetTxInterruptHandler()
}

void UART2_Receive_ISR(void)
{
    // use this default receive interrupt handler code
    uart2RxStatusBuffer[uart2RxHead].status = 0;

    if(U2ERRIRbits.FERIF){
        uart2RxStatusBuffer[uart2RxHead].ferr = 1;
        UART2_FramingErrorHandler();
    }
    
    if(U2ERRIRbits.RXFOIF){
        uart2RxStatusBuffer[uart2RxHead].oerr = 1;
        UART2_OverrunErrorHandler();
    }
    
    if(uart2RxStatusBuffer[uart2RxHead].status){
        UART2_ErrorHandler();
    } else {
        UART2_RxDataHandler();
    }

    // or set custom function using UART2_SetRxInterruptHandler()
}

void UART2_RxDataHandler(void){
    // use this default receive interrupt handler code
    uart2RxBuffer[uart2RxHead++] = U2RXB;
    if(sizeof(uart2RxBuffer) <= uart2RxHead)
    {
        uart2RxHead = 0;
    }
    uart2RxCount++;
}

void UART2_DefaultFramingErrorHandler(void){}

void UART2_DefaultOverrunErrorHandler(void){}

void UART2_DefaultErrorHandler(void){
}

void UART2_SetFramingErrorHandler(void (* interruptHandler)(void)){
    UART2_FramingErrorHandler = interruptHandler;
}

void UART2_SetOverrunErrorHandler(void (* interruptHandler)(void)){
    UART2_OverrunErrorHandler = interruptHandler;
}

void UART2_SetErrorHandler(void (* interruptHandler)(void)){
    UART2_ErrorHandler = interruptHandler;
}

void UART2_FramingError_ISR(void)
{
    // To clear the interrupt condition, all bits in the UxERRIR register must be cleared
    U2ERRIR = 0;
    
    // add your UART2 error interrupt custom code

}

void UART2_UartInterrupt_ISR(void)
{
    // WUIF must be cleared by software to clear UxIF
    U2UIRbits.WUIF = 0;
    
    // add your UART2 interrupt custom code
}

void UART2_SetRxInterruptHandler(void (* InterruptHandler)(void)){
    UART2_RxInterruptHandler = InterruptHandler;
}

void UART2_SetTxInterruptHandler(void (* InterruptHandler)(void)){
    UART2_TxInterruptHandler = InterruptHandler;
}

void UART2_SetFramingErrorInterruptHandler(void (* InterruptHandler)(void)){
    UART2_FramingErrorInterruptHandler = InterruptHandler;
}

void UART2_SetUartInterruptHandler(void (* InterruptHandler)(void)){
    UART2_UARTInterruptHandler = InterruptHandler;
}
/**
  End of File
*/