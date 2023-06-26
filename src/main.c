/*


Team Members:


Member 1: Habiba Fathallah, Section: 1, Bench Number: 27
Member 2: Rawan Mohamed Fekry, Section: 1, Bench Number: 34
Member 3: Sara Ayman Mohamed Abdullah, Section: 1, Bench Number: 39
Member 4: Mariam Wael, Section: 2, Bench Number: 36



*/



/* Include Header Files */
#include "Gpt.h"
#include "Gpio.h"
#include "Rcc.h"
#include "Utils.h"
#include "Std_Types.h"
#include "Bit_Operations.h"
#include <math.h>



/* Initially Both the Door & Handle are Locked*/
uint8 VEHICLE_DOOR_LOCK = 0;
uint8 DOOR_CLOSED = 0;

/* Global Variable to alert us that a button has been pressed */
uint8 flag = 0;


/* Cases' Functions Declaration */
void Case1(void);
void Case2(void);
void Case3(void);
void Case4(void);
void Case5(void);
void Case6(void);
void Cases(void);


/*
	LEDs Representations:

	R-LED: GPIO_D , PIN 0 ( Simulates the Vehicle Lock )
	G-LED: GPIO_D , PIN 1 ( Simulates the Hazards Light )
	B-LED: GPIO_D , PIN 2 ( Simulates the Vehicle Ambient Light )



*/





/******************************************************* Main ***************************************************/
int main() {


	Rcc_Init();
	GPT_Init();
	Rcc_Enable(RCC_GPIOB);           // Enable clock of GPIOB
	Rcc_Enable(RCC_GPIOD);          // Enable clock of GPIOD



	Gpio_ConfigPin(GPIO_B, 0, GPIO_INPUT, GPIO_PULL_UP);		// Set Push button as input (Door Lock)
	Gpio_ConfigPin(GPIO_B, 2, GPIO_INPUT, GPIO_PULL_UP);		// Set Push button as input (Handle Lock)


	uint8 led;
	for(led=0; led<3; led++){
		// Set all 3 LEDs as output
		Gpio_ConfigPin(GPIO_D, led, GPIO_OUTPUT, GPIO_PUSH_PULL);
	}


	/* Default State When System is powered on */
	Case1();

	while (1) {


		flag = 0;	// Clear the flag

		Cases();

		/* If no button was pressed for 10 seconds Perform case 4 */
		if(flag == 0){
			Case4();
		}


	}	// Loop Infinitely

	return 0;
}







/********************************************************************** Use Cases ************************************************************************/


// When System is powered on (Default State)
void Case1(void){


	VEHICLE_DOOR_LOCK = 0;   // Vehicle Door is locked

    // The 3 LEDS are off
	Gpio_WritePin(GPIO_D, 0, 0);
	Gpio_WritePin(GPIO_D, 2, 0);
	Gpio_WritePin(GPIO_D, 1, 0);

	return;

}






// When Door Handle Button is Pressed & the Vehicle Door is Locked (Door Unlock)
void Case2(void){

	VEHICLE_DOOR_LOCK = 1;	// Vehicle Door is Unlocked but stays closed
	DOOR_CLOSED = 0;

	Gpio_WritePin(GPIO_D, 0, 1);    // The Vehicle Lock LED is on
	Gpio_WritePin(GPIO_D, 2, 1);  	// The Vehicle Ambient Light LED is on for 2 seconds
	Gpio_WritePin(GPIO_D, 1, 1); 	// The Hazards Light LED is on for 0.5 seconds


	uint32 required_time_2sec = (GPT_GetElapsedTime() + 320);		/* 2(seconds) x 160(cycle/second) = 320(cycles) */
	uint32 required_time_halfsec = (GPT_GetElapsedTime() + 80);		/* 0.5(second) x 160(cycle/second) = 80(cycles) */


	/* Loop for 2 seconds */
	while(GPT_GetElapsedTime() <= required_time_2sec){

		/* Loop for 0.5 second */
		while(GPT_GetElapsedTime() <= required_time_halfsec){  // While remaining time is Less than or equal 0.5 second

		}

		Gpio_WritePin(GPIO_D, 1, 0);   // The Hazards Light LED is off
	}

	Gpio_WritePin(GPIO_D, 2, 0);      // The Vehicle Ambient Light LED is off

	return;
}







// When Door is Closed, Vehicle Door is Unlocked & Door Unlock Button is Pressed (Door is opened)
void Case3(void){

	VEHICLE_DOOR_LOCK = 1;  // Vehicle Door is Unlocked
	DOOR_CLOSED = 1;  // Door is opened

	Gpio_WritePin(GPIO_D, 2, 1);  	// The Vehicle Ambient Light LED is on

	return;
}







// When Door is Closed, Vehicle Door is Unlocked & No Buttons Pressed for 10 Seconds (Anti Theft Vehicle Lock)
void Case4(void){


	VEHICLE_DOOR_LOCK = 0;		// Vehicle Door is Locked

	Gpio_WritePin(GPIO_D, 0, 0);    // The Vehicle Lock LED is off
	Gpio_WritePin(GPIO_D, 2, 0);    // The Vehicle Ambient Light LED is off
	Gpio_WritePin(GPIO_D, 1, 1);   // The Hazards Light LED is on


	GPT_StartTimer(240);		// Count 1.5 seconds, 1.5(seconds) x 160(cycle/second) = 240(cycles)
	uint32 required_time_one_halfsec = (GPT_GetElapsedTime() + 240);     /* 1.5(seconds) x 160(cycle/second) = 240(cycles) */
	uint32 required_time_1sec = (GPT_GetElapsedTime() + 160);            /* 1(second) x 160(cycle/second) = 160(cycles) */
	uint32 required_time_halfsec = (GPT_GetElapsedTime() + 80);			/*  0.5(second) x 160(cycle/second) = 80(cycles) */


	/* Loop for 1.5 seconds */
	while(!GPT_CheckTimeIsElapsed()){

		/* Loop for 0.5 second */
		while(GPT_GetElapsedTime() <= required_time_halfsec){  // While remaining time is Less than or equal 0.5 second
		}

		Gpio_WritePin(GPIO_D, 1, 0);   // The Hazards Light LED is off
		/* Loop for 0.5 second */
		while(GPT_GetElapsedTime() <= required_time_1sec){  // While remaining time is Less than or equal 0.5 second
		}

		Gpio_WritePin(GPIO_D, 1, 1);   // The Hazards Light LED is on
		/* Loop for 0.5 second */
		while(GPT_GetElapsedTime() <= required_time_one_halfsec){  // While remaining time is Less than or equal 0.5 second
		}

		break;
	}



   Gpio_WritePin(GPIO_D, 1, 0);   // The Hazards Light LED is off

   return;
}








// When Door is Open, Vehicle Door is Unlocked & Door Lock Button is Pressed (Closing the Door)
void Case5(void){

	VEHICLE_DOOR_LOCK = 1;    // Vehicle Door is Unlocked
	DOOR_CLOSED = 0;   // but is closed

	Gpio_WritePin(GPIO_D, 0, 0); // The Vehicle Lock LED is off
	Gpio_WritePin(GPIO_D, 1, 0); // The Hazards Light LED is off
	Gpio_WritePin(GPIO_D, 2, 1); // The Vehicle Ambient Light LED is on for 1 second then off

	uint32 required_time_1sec = (GPT_GetElapsedTime() + 160);		/* 1(second) x 160(cycle/second) = 160(cycles) */

	while(GPT_GetElapsedTime() <= required_time_1sec){
	}

	Gpio_WritePin(GPIO_D, 2, 0); // The Vehicle Ambient Light LED is off after 1 second

	return;
}







// When Door is Closed, Vehicle Door is Unlocked & Door Handle Button is Pressed (Locking the Door)
void Case6(void){

	VEHICLE_DOOR_LOCK = 0;    // Vehicle Door is locked
	DOOR_CLOSED = 0;   // but is closed

	Gpio_WritePin(GPIO_D, 0, 0); // The Vehicle Lock LED is off
	Gpio_WritePin(GPIO_D, 2, 0); // The Vehicle Ambient Light LED is off
	Gpio_WritePin(GPIO_D, 1, 1); // The Hazards Light LED is on for 0.5 second


	uint32 required_time_one_halfsec = (GPT_GetElapsedTime() + 240);     /* 1.5(seconds) x 160(cycle/second) = 240(cycles) */
	uint32 required_time_1sec = (GPT_GetElapsedTime() + 160);            /* 1(second) x 160(cycle/second) = 160(cycles) */
	uint32 required_time_halfsec = (GPT_GetElapsedTime() + 80);			/*  0.5(second) x 160(cycle/second) = 80(cycles) */


	/* Loop for 1.5 seconds */
	while(GPT_GetElapsedTime() <= required_time_one_halfsec){

		 /* Wait for 0.5 second */
		 while (GPT_GetElapsedTime() <= required_time_halfsec){
		 }

		 Gpio_WritePin(GPIO_D, 1, 0); // The Hazards Light LED is off for 0.5 second
		 /* Wait for another 0.5 second */
		 while ((GPT_GetElapsedTime() >= required_time_halfsec) && (GPT_GetElapsedTime() <= required_time_1sec)){
		 }

		 Gpio_WritePin(GPIO_D, 1, 1); // The Hazards Light LED is on for 0.5 second
		 while ((GPT_GetElapsedTime() >= required_time_1sec) && (GPT_GetElapsedTime() <= required_time_one_halfsec)){
		 }
	}

	Gpio_WritePin(GPIO_D, 1, 0); // The Hazards Light LED is off

	return;

}







/* Combined Cases */
void Cases(void){


	/* Count 10 seconds */
	GPT_StartTimer(1600);	// 0x0A0 = 1600 cycle ( since 1 second = 160 cycle, thus 10 seconds = 1600 cycle)

	// While the 10 seconds are being counted, check for any activity from the buttons
	while(!GPT_CheckTimeIsElapsed()){


		/* If the Door Handle Button is Pressed Perform case 2 or case 6 */
		if(!Gpio_ReadPin(GPIO_B, 2)){


			// Wait for 2 seconds to overcome de-bounce effect
			while(GPT_GetElapsedTime()<= 320){  		/* 2(seconds) x 160(cycle/second) = 320(cycles) */
			}


			if(!Gpio_ReadPin(GPIO_B, 2)){

				flag = 1;

				if(!VEHICLE_DOOR_LOCK){
					Case2();
				}

				else if((VEHICLE_DOOR_LOCK) && (!DOOR_CLOSED)){
					Case6();
				}

				return;		// Exit the while loop to start counting 10 seconds again
		  }

	   }	// End of condition on the Door Handle Button



		/* If the Door Lock/Unlock Button is Pressed Perform case 3 or case 5 */
		if(!Gpio_ReadPin(GPIO_B, 0)){

			// Wait for 2 seconds to overcome de-bounce effect
			while(GPT_GetElapsedTime()<= 320){		/* 2(seconds) x 160(cycle/second) = 320(cycles) */
			}

			// Check if the button is pressed again
			if(!Gpio_ReadPin(GPIO_B, 0)){

				flag = 1;

				if((VEHICLE_DOOR_LOCK) && (!DOOR_CLOSED)){
					Case3();
				}

				else if((VEHICLE_DOOR_LOCK) && (DOOR_CLOSED)){
					Case5();
				}

				return;	// Exit the while loop to start counting 10 seconds again
			}

		}	// End of condition on the Door Lock/Unlock Button


	}		// End of Looping for 10 seconds

	return;
}










