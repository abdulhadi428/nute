#include "stm32f10x.h"
#include "UARTClass.h"
#include "tiny_sprintf.h"


void UART_Class :: UART_Init(USART_TypeDef* UART)
{
	if (FIFO_TxData.Init(UART2_BUF_SIZE)!=FIFO_NO_ERROR) return;
	if (FIFO_RxData.Init(UART2_BUF_SIZE)!=FIFO_NO_ERROR) return;
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    pUART=UART;
    eco_in_flag=0;
    eco_out_flag=0;
    TxCompleteFlag=1;
    switch ((uint32_t)UART)
    {
      case ((uint32_t)USART1_BASE):
          pUART1=this; // глобальный указатель - для обработки прерываний именно этого обьекта
          break;

      case ((uint32_t)USART2_BASE):  /* Initialize USART2  для связи PC*/

		  /* Enable GPIO &  USARTx clock */
		  RCC_APB2PeriphClockCmd (USART2_GPIO_CLK, ENABLE );
		  RCC_APB1PeriphClockCmd( USART2_CLK, ENABLE );

		  /* Configure pin: RX */
		  GPIO_InitStructure.GPIO_Pin = USART2_RxPin;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		  GPIO_Init(USART2_GPIO, &GPIO_InitStructure);

		  /* Configure pin: TX */
		  GPIO_InitStructure.GPIO_Pin = USART2_TxPin;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		  GPIO_Init(USART2_GPIO, &GPIO_InitStructure);

		  /* настройка порта */
		  USART_InitStructure.USART_BaudRate = UART2_BAUD_RATE;
		  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		  USART_InitStructure.USART_StopBits = UART2_BITS;
		  USART_InitStructure.USART_Parity = UART2_PARITY;
		  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
		  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
		  USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
		  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

		   /* Enable the USARTy Interrupt */
		  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
          pUART2=this; // глобальный указатель - для обработки прерываний именно этого обьекта
          break;

      case ((uint32_t)USART3_BASE):
          pUART3=this; // глобальный указатель - для обработки прерываний именно этого обьекта
          break;

    case ((uint32_t)UART4_BASE):
          pUART4=this; // глобальный указатель - для обработки прерываний именно этого обьекта
          break;

    case ((uint32_t)UART5_BASE):
          pUART5=this; // глобальный указатель - для обработки прерываний именно этого обьекта
          break;

    default:
        return;
        break;
    }

    USART_Init(UART, &USART_InitStructure);
    USART_ClockInit( UART, &USART_ClockInitStructure );

    /* Disable USART Receive and Transmit interrupts */
    USART_ITConfig(UART, USART_IT_RXNE, DISABLE);
    USART_ITConfig(UART, USART_IT_TXE, DISABLE);

	/* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    NVIC_Init(&NVIC_InitStructure);

    /* Enable USART Receive interrupt */
    USART_ITConfig( UART, USART_IT_RXNE, ENABLE );

    /* Enable the USART */
    USART_Cmd(UART, ENABLE);
}

void  UART_Class ::SendByte(char chData)
{
	FIFO_TxData.WriteByte(chData);
	UART_StartTx();
}

uint16_t UART_Class ::SendDataBuf(uint16_t iDataSize,uint8_t* pchDataBuf)
{
	uint16_t i;
	i=FIFO_TxData.WriteData(iDataSize,pchDataBuf);
	UART_StartTx();
	return i;
}

void UART_Class :: UART_StartTx(void){TxCompleteFlag=0;USART_ITConfig( pUART, USART_IT_TXE, ENABLE );}
void UART_Class :: UART_StartRx(void){USART_ITConfig( pUART, USART_IT_RXNE, ENABLE );}
void UART_Class :: UART_StopTx(void){USART_ITConfig( pUART, USART_IT_TXE, DISABLE );}
void UART_Class :: UART_StopRx(void){USART_ITConfig( pUART, USART_IT_RXNE, DISABLE );}
void UART_Class :: UART_Enable(void) {USART_Cmd(pUART, ENABLE);}
void UART_Class :: UART_Disable(void) {USART_Cmd(pUART, DISABLE);}

void UART_Class :: UART_InterruptHandler(void)
{
	/* обработка прерывания по опустошению буфера TX   */
	if( USART_GetITStatus( pUART, USART_IT_TXE ) == SET )
	{
		/* The interrupt was caused by the THR becoming empty.  Are there any
		more characters to transmit? */
		if (FIFO_TxData.IsEmpty())
		{
			USART_ITConfig( pUART, USART_IT_TXE, DISABLE );// последний байт из THR отправлен на передачу.
			USART_ITConfig(pUART, USART_IT_TC, ENABLE);  // будем ждать конца передачи
		}
		else
		{
			/* A character was retrieved from the FIFO so can be sent to the THR now. */
			cChar=FIFO_TxData.SimpleReadByte();
			USART_SendData( pUART,cChar);
		    if (eco_out_flag) FIFO_RxData.WriteByte(cChar); // эхо-флаг.
		}
	}
	/* обработка прерывания по приему байта   */
	if( USART_GetITStatus( pUART, USART_IT_RXNE ) == SET )
	{
	      cChar = USART_ReceiveData( pUART );
	      FIFO_RxData.WriteByte(cChar);

          if (eco_in_flag)  // эхо-флаг. Отправляем на TX то, что пришло по RX.
          {
        	  FIFO_TxData.WriteByte(cChar);
              UART_StartTx();
          }
	}

	  /* обработка прерывания по окончанию передачи   */
	  if(USART_GetITStatus(pUART, USART_IT_TC) != RESET)
	  {
	    USART_ITConfig(pUART, USART_IT_TC, DISABLE);
	    TxCompleteFlag=1;

	    /*FUEL_SENSOR_CONTROL_PORT->BRR =FUEL_SENSOR_CONTROL__Pin;*/  // переключаем контроллер 485 на прием
	  }
}

void USART2_IRQHandler(void)
{
	pUART2->UART_InterruptHandler();
}
