/*HD(HC585-DATA),HE(HC595-SCLK),HU(HC595-UPDATA),HS(HC595-EN)*/
/*EN为高电平是输入数据，为低电平时将收到的数据输出到总线*/
/*74HC595具有8位移位寄存器和一个存储器，三态输出功能。 移位寄存器和存储器是分别的时钟。 数据
在SCLK（移位寄存器时钟输入）的上升沿输入到移位寄存器中，在UPDATA（存储器时钟输入）的上升沿输入到存储
寄存器中去。
如果两个时钟连在一起，则移位寄存器总是比存储寄存器早一个脉冲。 移位寄存器有一个串行移位
输入（Ds），和一个串行输出（Q7’）,和一个异步的低电平复位，存储寄存器有一个并行8位的，具备三态的总线
输出，当使能OE时（为低电平），存储寄存器的数据输出到总线。*/
#include "hc595.h"

#define HC595_EN(a) do{if(a) GPIOA->ODR |= GPIO_PIN_3;else GPIOA->ODR &= ~GPIO_PIN_3;}while(0)
#define HC595_Data(a) do{if(a) GPIOC->ODR |= GPIO_PIN_7;else GPIOC->ODR &= ~GPIO_PIN_7;}while(0)
#define HC595_UPDATA(a) do{if(a) GPIOB->ODR |= GPIO_PIN_5;else GPIOB->ODR &= ~GPIO_PIN_5;}while(0)
#define HC595_SCLK(a) do{if(a) GPIOB->ODR |= GPIO_PIN_4;else GPIOB->ODR &= ~GPIO_PIN_4;}while(0)

void Delay(unsigned char delay)
{
  volatile unsigned char i;
  while(delay)
  {
    for(i=0;i<10;i++)
    {
      nop();
    }
    delay--;
  }
}

void HC595_Config(void)
{
  HC595_EN(1);
  HC595_UPDATA(0);
  HC595_SCLK(0);
}

void HC595UPDATA(void)
{
  //	HC595_EN(1);	//EN为高电平，不把存储中的数据发送到总线
  //	Delay(10);
  HC595_UPDATA(1);
  nop();
  HC595_EN(0);
  HC595_UPDATA(0);	
}
void WriteTo595(unsigned long dat)      //直接写入32位数据
{       //0x12345678    需要将1，2，3，4，5，6，7，8分别发送，并且高位在前
#if 0
  unsigned char i,j;
  unsigned long temp;
  unsigned long soso = 0x0000000f;
  for(i=0;i<8;i++)
  {
    //temp = ((dat & (soso << (i*4)))>>(i*4)); 
    temp = (data >> (i*4)) & 0xf;
    for(j=0;j<4;j++)
    {
      if((temp&0x1) == 0x1)
      {
        HC595_Data(1);
      }
      else
      {
        HC595_Data(0);
      }
      HC595_SCLK(1);	//sclk上升沿将数据写入移位寄存器中
      nop();		
      HC595_SCLK(0);	
      temp>>=1;              
    }
  }
#else
  u8 i;
  for (i = 0; i < 32; i++) {
    HC595_Data((u8)(dat & 0x1));
    HC595_SCLK(1);	//sclk上升沿将数据写入移位寄存器中
    nop();		
    HC595_SCLK(0);
    dat >>= 1;
  }
#endif
}

void LightSomeBit(unsigned long which)
{
  WriteTo595(which);	
  HC595UPDATA();
}

void DarkAll(unsigned char status)
{
  HC595_EN(status);
  //WriteTo595(0);
  //HC595UPDATA();
}