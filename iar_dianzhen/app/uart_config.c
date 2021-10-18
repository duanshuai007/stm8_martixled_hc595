#include "uart_config.h"

void UART_Config(void)
{
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
  UART1_DeInit();
  UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE, DISABLE); 
  UART1_Cmd(ENABLE);
}

void UART_WriteData(unsigned char *dat)
{
  while((*dat) != '\0')
  {
    UART1_SendData8(*dat);                                      //发送八位数据
    while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);        //发送完成标志
    UART1_ClearFlag(UART1_FLAG_TXE);
    dat++;
  }
}
