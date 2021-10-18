#include "AD.h"

void AD_Config(void)
{
  ADC1_DeInit();
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,\
            ADC1_CHANNEL_3,\
            ADC1_PRESSEL_FCPU_D4,\
            ADC1_EXTTRIG_GPIO,DISABLE,\
            ADC1_ALIGN_LEFT,\
            ADC1_SCHMITTTRIG_CHANNEL3,DISABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC,ENABLE);
}

unsigned char AD_GetValue(void)
{
  unsigned char temp;
  ADC1_Cmd(ENABLE);
  ADC1_StartConversion();
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == RESET);
  ADC1_ClearFlag(ADC1_FLAG_EOC);
  temp = ADC1->DRH;;
  ADC1_Cmd(DISABLE);
  return temp;
}