#include "stm8s.h"
#include "stdio.h"
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#include "io_config.h"
#include "clk_config.h"
#include "uart_config.h"

#define MAX485_EN(a) if(a) GPIOC->ODR |= GPIO_PIN_6;\
                      else GPIOC->ODR &= ~GPIO_PIN_6;       
u8 UART1_Buffer[100];
u8 UART1_RecBit = 0;
u8 UART1_RecFlag = 0;

void delay(u8 num);

u32 TEMP_SAVE;

int main( void )
{
  CLK_Config();
  GPIO_Config();
  //UART_Config();
  
  while(1)
  {
    //MAX485_EN(1);
    //UART_WriteData("Hello World!\n");
    //UART1_SendData8(0X55);
    //while(UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);        //发送完成标志
    //UART1_ClearFlag(UART1_FLAG_TXE);
    GPIOD->ODR |= GPIO_PIN_5;
    delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);
    //MAX485_EN(0);
    GPIOD->ODR &= ~GPIO_PIN_5;
    delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);delay(200);
  }
}

void delay(u8 num)
{
  u8 i;
  while(num)
  {
    for(i=0;i<200;i++)
    {
      nop();
    }
    num--;
  }
}
