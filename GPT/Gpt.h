#include <stdio.h>
#include "Rcc.h"
#include "Gpio.h"
#include "Std_Types.h"
#include "Bit_Operations.h"

#ifndef GPT_H
#define GPT_H



/* Define a User-Defined Variable(structure) for TIMER2 Peripheral */
typedef struct {

	uint32 gpt_CR1;
	uint32 gpt_CR2;
	uint32 gpt_SMCR;
	uint32 gpt_DIER;
	uint32 gpt_SR;
	uint32 gpt_EGR;
	uint32 gpt_CCMR1;
	uint32 gpt_CCMR2;
	uint32 gpt_CCER;
	uint32 gpt_CNT;
	uint32 gpt_PSC;
	uint32 gpt_ARR;
	uint32 gpt_CCR1;
	uint32 gpt_CCR2;
	uint32 gpt_CCR3;
	uint32 gpt_CCR4;
	uint32 gpt_BDTR;
	uint32 gpt_DCR;
	uint32 gpt_DMAR;
	uint32 gpt_OR;

}TIMxTYPE;




/* Driver Functions' Declaration for TIMER2 Peripheral */
void GPT_Init(void);
void GPT_StartTimer(unsigned long int OverFlowTicks);
uint8 GPT_CheckTimeIsElapsed(void);
uint32 GPT_GetElapsedTime(void);
uint32 GPT_GetRemainingTime(void);



#endif
