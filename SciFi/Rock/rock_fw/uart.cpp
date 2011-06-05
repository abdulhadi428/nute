#include "uart.h"
#include "stm32f10x_usart.h"

UART_t Uart;

// ************** Implementation ***********************
void UART_t::Init() {
#ifdef UART_HARDWARE
    // ==== Clocks init ====
    RCC_APB2PeriphClockCmd(USART1_GPIO_CLK | USART1_CLK | RCC_APB2Periph_AFIO, ENABLE);
    // ==== GPIO init ====
    GPIO_InitTypeDef GPIO_InitStructure;
    // Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);
    // Configure USART Rx as input floating
    /*GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure); */

    // ==== USART configuration ====
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    // Enable USART
    USART_Cmd(USART1, ENABLE);
#endif
}
void UART_t::Print (uint8_t AByte) {
#ifdef UART_HARDWARE
    // Write a character to the USART
    USART_SendData(USART1, AByte);
    // Loop until the end of transmission
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
#else
    uint8_t i;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // send start bit
        CLEAR_TX_PIN ();
        _delay_us (UARTTxOneBit+1);
        // Send data bits
        for (i=0; i<8; i++, AByte >>=1) {
            if (AByte & 1) SET_TX_PIN ();  // send 1
            else CLEAR_TX_PIN ();          // send 0
            _delay_us (UARTTxOneBit);
        }
        // Send stop bit
        SET_TX_PIN ();	// send 1
        _delay_us (UARTTxOneBit);
    } //ATOMIC_BLOCK
#endif
}
void UART_t::PrintAsHex (uint8_t AByte){
    Print(UintToHexChar (AByte >> 4));
    Print(UintToHexChar (AByte & 0x0F));
}

char UART_t::UintToHexChar (uint8_t b) {
    return ((b<=0x09) ? (b+'0') : (b+'A'-10));
}


// ============================== High-level ===================================
// Send as digits
void UART_t::PrintUint (uint16_t ANumber) {
    uint8_t digit = '0';
    bool ShouldPrint = false;
    // >10000
    while (ANumber >= 10000) {
        digit++;
        ANumber -= 10000;
    }
    if (digit != '0'){
        Print (digit);
        ShouldPrint = true;
    }
    digit = '0';
    // >1000
    while (ANumber >= 1000){
        digit++;
        ANumber -= 1000;
    }
    if ((digit != '0') || ShouldPrint) {
        Print (digit);
        ShouldPrint = true;
    }
    digit = '0';

    // > 100
    while (ANumber >= 100) {		// Still larger than 100 ?
        digit++;			// Increment first digit
        ANumber -= 100;
    }
    if ((digit != '0') || ShouldPrint) {
            Print (digit);
            ShouldPrint = true;
    }
    digit = '0';
    // > 10
    while (ANumber >= 10) {		// Still larger than 10 ?
        digit++;			// Increment second digit
        ANumber -= 10;
    }
    Print (digit);
    Print ('0'+ANumber);
}
void UART_t::PrintInt (int16_t ANumber) {
    if (ANumber < 0) {
        Print ('-');
        ANumber = -ANumber;
    }
    else Print (' ');
    PrintUint (ANumber);
}

void UART_t::PrintString (const char *S) {
    while (*S != '\0') Print (*S++);
}
void UART_t::PrintStringLen (const char *S, const uint8_t ALength) {
    for (uint8_t i=0; i<ALength; i++) Print (*S++);
}

void UART_t::PrintArr (uint8_t *Arr, uint8_t ALength) {
    while (ALength--) Print(*(Arr++));
}

void UART_t::PrintArrAsString (uint8_t *Arr, uint8_t ALength) {
    while (ALength--) {
        PrintUint(*(Arr++));
        Print (' ');
    }
    NewLine();
}

