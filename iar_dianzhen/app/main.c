#include "stm8s.h"
#include "stdio.h"
#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long
#include "io_config.h"
#include "clk_config.h"
#include "hc595.h"
#include "hc138.h"
#include "AD.h"
#include "font.h"

#define DATA(a) do{if(a) GPIOC->ODR |= GPIO_PIN_6;else GPIOC->ODR &= ~GPIO_PIN_6;}while(0)

u8 BM_Flag = 0;
u16 AD_Value = 0;
u8 MODE = 0;
float AD_Value_f;

void WriteLed(u8 which);
void WriteLedBuffer(u8 which);
u8 BM_Read(void);
void SendDisplayData(u8 dat);
u8 GetDisplayData(void);
u8 SendFont(u8 which);
void RecFont(void);

u8 buffer[3][32];
u32 TEMP_SAVE;
u8 RecBit = 0;
u8 RecFlag = 0;
u8 StartRec = 0;

void delay_ms(u32 num)
{
  volatile u32 i;
  volatile u32 delay;
  for (i = 0; i < num; i++) {
    delay = 22;
    while(delay) {
      delay--;
    }
  }
}

void display_leftshift(u8 *display, u8 number, u8 roll_speed);

int main( void )
{
  CLK_Config();
  GPIO_Config();
  //UART_Config();
  HC595_Config();
  
  AD_Config();
  
  AD_Value = AD_GetValue();
  if(AD_Value < 130)
  {       //master
    MODE = 0;
    GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_IN_FL_NO_IT);        //MAX485 EN
  }
  else if(AD_Value > 140)
  {       //master
    MODE = 1;
    GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_OUT_PP_HIGH_FAST);        //MAX485 EN
  }
  
  u8 display_pool[8] = {13, 14, 18, 19, 15, 16, 17, 16};
  //u8 display_pool[8] = {0, 1, 2, 3};
      
  while(1)
  {
    BM_Flag = BM_Read();
    AD_Value = AD_GetValue();
    //AD_Value *= 4;
    //AD_Value_f = (((float)AD_Value)*5/1024);
#if 1
    display_leftshift(display_pool, sizeof(display_pool), 5);
#else
    switch(BM_Flag)
    {
    case 0:
      {
        //er ding
        WriteLed(13);
        WriteLed(14);
        //jie zhi
        WriteLed(18);
        WriteLed(19);
        //shou lian
        WriteLed(15);
        WriteLed(16);
        //jiao lian
        WriteLed(17);
        WriteLed(16);
      }break;
    case 1:
      {
        //ling du che shi
        WriteLed(0);
        WriteLed(1);
        WriteLed(2);
        WriteLed(3);
      }break;
    case 2:
      {
        //qiao yi qiao
        WriteLed(10);WriteLed(12);WriteLed(10);
        //kan yi kan
        WriteLed(11);WriteLed(12);WriteLed(11);
      }break;
    case 3:
      { 
        if(MODE == 1)        
        {
          SendFont(1);
          SendFont(5);
          SendFont(9);
          MODE = 2;
        }
        if(MODE == 0)        
        {
          RecFont();
          MODE = 2;
        }
        if(MODE == 2)
        {
          
          WriteLedBuffer(0);
          WriteLedBuffer(1);
          WriteLedBuffer(2);
          if(AD_Value < 130)
          {     
            MODE = 0;
          }
          else if(AD_Value > 140)
          {      
            MODE = 1;
          }
        }
      }break;
    default:;break;
    }
#endif
  }
}

void display_leftshift(u8 *display, u8 number, u8 roll_speed)
{
  u8 count;             //total display char number
  u8 i,j,m;
  u32 temp;                                     
  u8 curr_char = 0, last_char = 0;              //current char pos, last time char pos        
  u16 upline, downline;                         //the current time char line content
  u16 last_upline, last_downline;               //the last time char line content
  u32 move_up_line, move_down_line;             //finally to display line content
  for (count = 0; count < number; count++) {
    for (m = 0; m < 16; m++) {  //16 mean every char have 16 rows
      for(j = 0; j < roll_speed; j++) {       //display speed, make j<{flash times} little will display fast
        for(i = 0; i < 8; i++) {        //8 mean every char have 8 rows on each side(up and down)
          upline        = ((u32)FONT[display[curr_char]][(i*2)+ 0]<<8) | ((u32)FONT[display[curr_char]][(i*2)+ 1]<<0);
          downline      = ((u32)FONT[display[curr_char]][(i*2)+16]<<8) | ((u32)FONT[display[curr_char]][(i*2)+17]<<0);
          //实现从右到左的移动效果
          if ((curr_char == 0) && (last_char == 0)) {
            //第一次启动显示
            move_up_line        = ((upline   >> (16 - m)) & 0xffff);
            move_down_line      = ((downline >> (16 - m)) & 0xffff);
          } else {
            last_upline         = ((u32)FONT[display[last_char]][(i*2)+ 0]<<8) | ((u32)FONT[display[last_char]][(i*2)+ 1]<<0);
            last_downline       = ((u32)FONT[display[last_char]][(i*2)+16]<<8) | ((u32)FONT[display[last_char]][(i*2)+17]<<0);
            move_up_line        = (((last_upline   << m) | (upline   >> (16 - m))) & 0xffff);
            move_down_line      = (((last_downline << m) | (downline >> (16 - m))) & 0xffff);
          }  
          temp = (move_up_line << 16) | move_down_line;
          LightSomeBit((temp));
          hc138_send(i);
          delay_ms(1);
        }
        delay_ms(1);
      }
    }
    last_char = curr_char;
    curr_char++;
    if (curr_char >= number)
      curr_char = 0;
  }
}

u8 BM_Read(void)
{
  u8 temp = 0;
  if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_3) == SET)
  {
    temp |= 0x01;
  }
  if(GPIO_ReadInputPin(GPIOD,GPIO_PIN_4) == SET)
  {
    temp |= 0x02;
  }
  return temp;
}

void WriteLed(u8 which)
{
  u8 i,j;
  u32 temp;
  for(j=0;j<100;j++)
  {
    for(i=0;i<8;i++)
    {
      temp = (((u32)FONT[which][i*2]<<24)|((u32)FONT[which][(i*2)+1]<<16)|((u32)FONT[which][(i*2)+16]<<8)|((u32)FONT[which][(i*2)+17]));
      LightSomeBit((temp));
      hc138_send(i);
    }
  }
  GPIO_WriteReverse(GPIOA,GPIO_PIN_1);
}

void WriteLedBuffer(u8 which)
{
  u8 i,j;
  u32 temp;
  for(j=0;j<50;j++)
  {
    for(i=0;i<8;i++)
    {    
      temp = (((u32)buffer[which][i*2]<<24)|((u32)buffer[which][(i*2)+1]<<16)|((u32)buffer[which][(i*2)+16]<<8)|((u32)buffer[which][(i*2)+17]));
      LightSomeBit((temp));
      hc138_send(i);   
    }
  }
  GPIO_WriteReverse(GPIOA,GPIO_PIN_1);
}

void SendDisplayData(u8 dat)
{
  u8 i = 0;
  DATA(0);
  delay_ms(200);
  for(i=0;i<8;i++)
  {
    if((dat & 0x80) == 0x80)
    {
      DATA(1);
    }
    else
    {
      DATA(0);
    }
    delay_ms(50);
    dat <<= 1;
  }
  DATA(0);
  delay_ms(100);
  DATA(1);
  delay_ms(200);
}

u8 SendFont(u8 which)
{
  u8 i;
  u8 temp;
  for(i=0;i<8;i++)
  {
    SendDisplayData(0xAA);
    SendDisplayData(FONT[which][i*4+0]);
    SendDisplayData(FONT[which][i*4+1]);
    SendDisplayData(FONT[which][i*4+2]);
    SendDisplayData(FONT[which][i*4+3]);
    temp = 0xaa+FONT[which][i*4+0]+FONT[which][i*4+1]+FONT[which][i*4+2]+FONT[which][i*4+3];
    SendDisplayData(temp);
  } 
  return 0;
}

u8 GetDisplayData(void)
{
  u8 i = 0;
  u8 temp = 0;
  while(GPIO_ReadInputPin(GPIOC,GPIO_PIN_6) == SET);
  if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_6) == RESET)
    delay_ms(150);
  if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_6) == RESET)
  {
    delay_ms(50);
    for(i=0;i<8;i++)
    {
      temp <<= 1;
      if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_6) == SET) temp |= 0x01;      
      delay_ms(50);
    }
    if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_6) == RESET)
    {
      delay_ms(100);
      if(GPIO_ReadInputPin(GPIOC,GPIO_PIN_6) == SET)
        return temp;
      else
        return 0x55;
    }
  }
  else
  {
    temp = 0x55;                //返回错误码
  }
  return temp;
}

void RecFont(void)
{
  u8 temp;
  temp = GetDisplayData();  
  if(StartRec)
  {
    buffer[RecBit/32][RecBit%32] = temp;
    RecBit++;
    if(RecBit%4 == 0) 
    {
      RecBit--;
      temp = GetDisplayData();
      //GPIO_Init(GPIOC,GPIO_PIN_6,GPIO_MODE_OUT_PP_HIGH_FAST);
      if(temp == (buffer[RecBit/32][RecBit%32]+buffer[(RecBit-1)/32][(RecBit-1)%32]+buffer[(RecBit-2)/32][(RecBit-2)%32]+buffer[(RecBit-3)/32][(RecBit-3)%32]))
      {
        //DATA(1); 数据开始信号
        //delay(100);  
        RecFlag = 0;
      }
      else
      {
        //DATA(0); 数据开始信号
        //delay(100);
        RecFlag = 1;  //错误返回1
      }
    }
  }
  if(temp == 0xaa)
  {
    StartRec = 1;
  }
}
