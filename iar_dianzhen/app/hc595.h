#ifndef __HC595_H
#define __HC595_H

#include "stm8s.h"

void HC595_Config(void);
void WriteTo595(unsigned long dat);
void LightSomeBit(unsigned long which);
void DarkAll(unsigned char status);

#endif