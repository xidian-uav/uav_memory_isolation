#ifndef _DMA_H
#define _DMA_H

#include "sys.h"

void DMA_init(DMA_Channel_TypeDef *DMA_Channel, u32 seq);
void DMA_printf(char *fmt, ...);

#endif
