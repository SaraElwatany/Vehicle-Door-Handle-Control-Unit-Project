#include "Gpt.h"
#include "RCC.h"
#include "Gpio.h"




/*  Create TIMxTYPE variable */
TIMxTYPE *TIM2_BASE_ADDR = 0x40000000;






/* Initialisation Function For The GPT Driver */
void GPT_Init(void)
{
     /* Enable the clock for the TIMER2 peripheral */
	 Rcc_Enable(RCC_TIM2);

	 /* Set the Pre-scaler register to get 1000 cycle / 1sec */
	 TIM2_BASE_ADDR->gpt_PSC = 99999;	// Set the Pre-scaler to 99999 (100000 - 1), 99999 = 160 cycle/second

	 /*	Up-counting Mode (DIR) Bit */
	 TIM2_BASE_ADDR->gpt_CR1 &= ~(0x01 << 4);	// Reset bit 4 (DIR) , for Up-counting mode
	 /* Set the timer Pre-scaler register to divide the clock by 1 */
	 TIM2_BASE_ADDR->gpt_CR1 &= ~(0x07 << 8);	// Reset bit 8 & 9 (CKD)
	 /* Set the Edge Aligned Mode in the CMS Position */
	 TIM2_BASE_ADDR->gpt_CR1 &= ~(0x07 << 5);	// Reset bit 5 & 6 (CMS)
	 /* Disable the TIM2 in the CEN Position */
	 TIM2_BASE_ADDR->gpt_CR1 &= ~(0x01 << 0);

	 /* Set the UG bit in the Event Generation Register */
	 TIM2_BASE_ADDR->gpt_EGR &= ~(0x01 << 0);
	 TIM2_BASE_ADDR->gpt_EGR |= (0x01 << 0);

	 /* Reset the Update interrupt flag of the Status Register */
	 TIM2_BASE_ADDR->gpt_SR &= ~(0x01 << 0);

}









/* Start Timer Function For The GPT Driver */
void GPT_StartTimer(uint32 OverFlowTicks)
{

	/* Disable the update interrupt by clearing the CEN bit in the Control Register */
	TIM2_BASE_ADDR->gpt_CR1 &= ~(0x01 << 0);

	/* Reset the Control Register to start/initial value of 1 */
	TIM2_BASE_ADDR->gpt_CNT = 0x00;

	/* Reset the Update interrupt flag of the Status Register */
	TIM2_BASE_ADDR->gpt_SR &= ~(0x01 << 0);

	/* Set the Capture Compare Register to the value of required overflow ticks */
	TIM2_BASE_ADDR->gpt_ARR = OverFlowTicks;

    /* Enable the update interrupt by setting the CEN bit in the Control Register */
	TIM2_BASE_ADDR->gpt_CR1 |= (0x01 << 0);

}







/* Check if an overflow occurs Function For The GPT Driver */
uint8 GPT_CheckTimeIsElapsed(void)
{

    uint8 overflow_occurred = 0;

    if ((TIM2_BASE_ADDR->gpt_SR) & (0x01 << 0)){	// An overflow occurred
        overflow_occurred = 1;
    }


    return overflow_occurred;
}







/* Get Time Elapsed Function For The GPT Driver */
uint32 GPT_GetElapsedTime(void)
{
    if (!(TIM2_BASE_ADDR->gpt_CR1)) {
        return 0;			// GPT_StartTimer has not been called yet
    }

    if ((TIM2_BASE_ADDR->gpt_SR) & (0x01 << 0)) {
        return 0xffffffff;		// An overflow occurred
    }

    return (TIM2_BASE_ADDR->gpt_CNT);		// return elapsed time
}







/* Get Remaining Time Function For The GPT Driver */
uint32 GPT_GetRemainingTime(void)
{

    if (!(TIM2_BASE_ADDR->gpt_CR1)) {
        return 0xffffffff;   // GPT_StartTimer has not been called yet
    }

    if ((TIM2_BASE_ADDR->gpt_SR) & (0x01 << 0)) {
        return 0;       // An overflow occurred
    }

    uint32 remaining_ticks = (TIM2_BASE_ADDR->gpt_ARR) - (TIM2_BASE_ADDR->gpt_CNT);   // Calculate the remaining ticks

    return remaining_ticks;		// return remaining ticks
}
