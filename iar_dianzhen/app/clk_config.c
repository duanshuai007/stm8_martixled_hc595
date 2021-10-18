#include "clk_config.h"

void CLK_Config(void)
{
    CLK_HSICmd(ENABLE);/* Set HSIEN bit */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4); /* Fmaster = 4MHz */
}
