#include "hc138.h"

#define HC138_A0        GPIO_PIN_5
#define HC138_A1        GPIO_PIN_4
#define HC138_A2        GPIO_PIN_3

void hc138_send(unsigned char num)
{
  switch(num)
  {
  case 0:
    {
      GPIOC->ODR &= ~HC138_A2;        //A2
      GPIOC->ODR &= ~HC138_A1;        //A1
      GPIOC->ODR &= ~HC138_A0;        //A0
    }break;
  case 1:
    {
      GPIOC->ODR &= ~HC138_A2;        //A2
      GPIOC->ODR &= ~HC138_A1;        //A1
      GPIOC->ODR |= HC138_A0;        //A0
    }break;
  case 2:
    {
      GPIOC->ODR &= ~HC138_A2;        //A2
      GPIOC->ODR |= HC138_A1;        //A1
      GPIOC->ODR &= ~HC138_A0;        //A0
    }break;
  case 3:
    {
      GPIOC->ODR &= ~HC138_A2;        //A2
      GPIOC->ODR |= HC138_A1;        //A1
      GPIOC->ODR |= HC138_A0;        //A0
    }break;
  case 4:
    {
      GPIOC->ODR |= HC138_A2;        //A2
      GPIOC->ODR &= ~HC138_A1;        //A1
      GPIOC->ODR &= ~HC138_A0;        //A0
    }break;
  case 5:
    {
      GPIOC->ODR |= HC138_A2;        //A2
      GPIOC->ODR &= ~HC138_A1;        //A1
      GPIOC->ODR |= HC138_A0;        //A0
    }break;
  case 6:
    {
      GPIOC->ODR |= HC138_A2;        //A2
      GPIOC->ODR |= HC138_A1;        //A1
      GPIOC->ODR &= ~HC138_A0;        //A0
    }break;
  case 7:
    {
      GPIOC->ODR |= GPIO_PIN_3;        //A2
      GPIOC->ODR |= GPIO_PIN_4;        //A1
      GPIOC->ODR |= GPIO_PIN_5;        //A0
    }break;
  }
}