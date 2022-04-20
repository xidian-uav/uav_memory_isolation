#include <stdarg.h>
#include "dma.h"
#include "usart.h"

DMA_HandleTypeDef  UART1TxDMA_Handler;

#define SEND_BUF_SIZE (8800)
u8 SendBuff[SEND_BUF_SIZE];

volatile uint8_t usart_dma_tx_over = 1;

void DMA_init(DMA_Channel_TypeDef *DMA_Channel, u32 seq)
{
	 if((u32)DMA_Channel > (u32)DMA2) //????stream???DMA2??DMA1
    {
        __HAL_RCC_DMA2_CLK_ENABLE();//DMA2????
    }

    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();//DMA1????
    }

    __HAL_LINKDMA(&UART1_Handler, hdmatx, UART1TxDMA_Handler);  //?DMA?USART1????(??DMA)

    //Tx DMA??
    UART1TxDMA_Handler.Instance = DMA_Channel;                          //????
    UART1TxDMA_Handler.Init.Request = seq;                              //????
    UART1TxDMA_Handler.Init.Direction = DMA_MEMORY_TO_PERIPH;           //??????
    UART1TxDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;               //???????
    UART1TxDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;                   //???????
    UART1TxDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;  //??????:8?
    UART1TxDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;     //???????:8?
    UART1TxDMA_Handler.Init.Mode = DMA_NORMAL;                          //??????
    UART1TxDMA_Handler.Init.Priority = DMA_PRIORITY_HIGH;             	//?????

    HAL_DMA_Init(&UART1TxDMA_Handler);
}

void DMA_printf(char *fmt, ...)
{
	va_list arg;
	int rv;
	
	while(!usart_dma_tx_over)
	{
		if(__HAL_DMA_GET_FLAG(&UART1TxDMA_Handler, DMA_FLAG_TC4))
		{
			__HAL_DMA_CLEAR_FLAG(&UART1TxDMA_Handler, DMA_FLAG_TC4);
			HAL_UART_DMAStop(&UART1_Handler);
			usart_dma_tx_over = 1;
		}
	}
	
//	while (!usart_dma_tx_over);
	
	va_start(arg, fmt);
	rv = vsnprintf((char *)SendBuff, sizeof(SendBuff) - 1, (char *)fmt, arg);
	va_end(arg);
	
	HAL_UART_Transmit_DMA(&UART1_Handler, (u8*)SendBuff, rv);
	usart_dma_tx_over = 0;
}
