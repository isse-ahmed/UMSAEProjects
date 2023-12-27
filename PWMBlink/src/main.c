
/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#define Pin1
#define Pin2
#define Pin3

#include "stm32f4xx.h"

/*
 * RCC stands for Reset and Clock Control.
 * It shows up everywhere because it controls the clocks that all the other
 * Peripherals depend on.
 * THE CLOCK THAT FEEDS INTO THE PERIPHERALS YOU ARE USING MUST BE ENABLED
 * IN THE RCC FOR YOUR PERIPHERALS TO WORK!
 *
 * A detailed clock tree diagram is available in the Reference Manual on page 115
 * For the clock bus associated with the peripheral you want see page 54-56
 * Remember, more than one peripheral depends on each clock bus, so only configure
 * each clock once, and take that configuration into account when using your peripheral
 */
#include "stm32f4xx_rcc.h"

/*
 * GPIO stands for General Purpose Input Output. If you need to use any hardware pin,
 * it will be set up using this file.
 */
#include "stm32f4xx_gpio.h"

/*
 * tim is short for Timers, This file is used to set up all the multi-purpose timers.
 * The timers on the STM32 chips are very powerful, They can be used for:
 * 		-PWM (At any frequency up to and including the CPU frequency!)
 * 		-Frequency outputs (Variable frequency instead of duty cycle)
 * 		-Variable Frequency and Duty
 * 		-Motor control (H-Bridge control, Complimentary PWM signals wiTH HARDWARE DEADBAND!!!)
 * 		-Arbitrary function generation (!!!!!)
 * 		-Interrupt generation
 * 		-Pulse counting
 * 		-Pulse timing
 * 		-PWM sensing
 * 		-Frequency sensing
 * 		-Both Frequency and Duty sensing
 * 		-ADC sampling triggering
 * 		-And so, so much more
 *
 * If you have ANYTHING that vaguely involves counting in your code, you can get a Timer to do it.
 * And you definitely should always use a Timer over your code, the timers are their totally own
 * hardware, they run asynchronous to your code, meaning you use no computing time doing repetitive,
 * dull tasks
 */
#include "stm32f4xx_tim.h"

#ifdef Pin1
#define PIN_NUM GPIO_Pin_6
#define PIN_SOURCE GPIO_PinSource6
#define RCC_PORT RCC_AHB1Periph_GPIOA
#define PORT GPIOA
#endif

#ifdef Pin2
#define RCC_PORT2 RCC_AHB1Periph_GPIOB
#define PORT2 GPIOB
#endif

#ifdef Pin3
#define PIN_NUM3 GPIO_Pin_0
#define PIN_SOURCE3 GPIO_PinSource0
#endif

int main(void)
{

	//Enable pin for output
	GPIO_InitTypeDef GPIOInitType;
	GPIO_InitTypeDef GPIOPin2;
	GPIO_InitTypeDef GPIOPin3;

	//Turn on ports
	RCC_AHB1PeriphClockCmd(RCC_PORT, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_PORT2, ENABLE);


	//Pin1
	GPIOInitType.GPIO_Mode = GPIO_Mode_AF;
	GPIOInitType.GPIO_OType = GPIO_OType_PP;
	GPIOInitType.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOInitType.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOInitType.GPIO_Pin = PIN_NUM;
	GPIO_Init(PORT, &GPIOInitType);

	//Pin2
	GPIOPin2.GPIO_Mode = GPIO_Mode_AF;
	GPIOPin2.GPIO_OType = GPIO_OType_PP;
	GPIOPin2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOPin2.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOPin2.GPIO_Pin = PIN_NUM;
	GPIO_Init(PORT2, &GPIOPin2);

	//Pin3
	GPIOPin3.GPIO_Mode = GPIO_Mode_AF;
	GPIOPin3.GPIO_OType = GPIO_OType_PP;
	GPIOPin3.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOPin3.GPIO_Speed = GPIO_Speed_100MHz;
	GPIOPin3.GPIO_Pin = PIN_NUM3;
	GPIO_Init(PORT, &GPIOPin3);


	//Enable clock for Timer3 (Pin1)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//Enable clock for Timer4 (Pin2)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	//Enable Clock for Timer4 (Pin2)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	//Getting System clocks and frequencies
	RCC_ClocksTypeDef clockSpeeds;
	RCC_GetClocksFreq(&clockSpeeds); //Pause here to see clock speeds
	                                 //SYSCLK is the CPU frequency
	                                 //HCLK is AHB
	                                 //PCLK1 is APB1
	                                 //PCLK2 is APB2

	//Setting the alternate function of the pins to the correct timer
	GPIO_PinAFConfig(PORT, PIN_SOURCE, GPIO_AF_TIM3);
	GPIO_Init(PORT,&GPIOInitType);

	GPIO_PinAFConfig(PORT2, PIN_SOURCE, GPIO_AF_TIM4);
	GPIO_Init(PORT2, &GPIOPin2);

	GPIO_PinAFConfig(PORT, PIN_SOURCE3, GPIO_AF_TIM5);
	GPIO_Init(PORT, &GPIOPin3);




	//Defining variable for the timer init struct
	//(again, definition of variables after code only works in fancy versions of c like C99 or gnu99)
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;


	//From the timer Information we see above, we know that APB1 runs at SystemCoreClock/4 by default.
	//We can change that but we will leave it for now. If you look at the clock tree we see that this
    //is actually multiplied by two before hitting the timer peripheral, so this timer will be running
	//at 90000000 Hz. To make the LED blink at exactly one second we will set the period to be half the
	//System core clock divided by a prescaler. Timer 2 is a 32 bit timer, so we don't have to worry about
	//maxing it out that easily, though I don't know if the capture compare register is 16 bit or 32 bit,
	//So we'll use a period that is below 16 bits just in case.
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;        //This divides the system clock to slow down the timer
	TIM_BaseStruct.TIM_CounterMode   = TIM_CounterMode_Up;  //This tells the timer to count up
	TIM_BaseStruct.TIM_Period        = 988;//This tells the timer when to reset
	TIM_BaseStruct.TIM_Prescaler     = 90;                //Sets another scaler to slow down the clock even more

	//Initializing Timers 3,4,5
	TIM_TimeBaseInit(TIM3, &TIM_BaseStruct);
	TIM_TimeBaseInit(TIM4,&TIM_BaseStruct);
	TIM_TimeBaseInit(TIM5, &TIM_BaseStruct);

	//Turning on Timers
	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM5, ENABLE);

	//Now we're going to set the mode of the timer
	//See the reference manual for all the modes
	TIM_OCInitTypeDef TIM_OCStruct;

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;             //Sets it to a PWM mode
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; //Tells the Timer to use the pin output
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //Sets the polarity of the PWM
	TIM_OCStruct.TIM_Pulse = TIM_BaseStruct.TIM_Period/1000;  //Sets the duty cycle (half of the reset counter)


	//Setup the mode of the timers

	//FULL BRIGHTNESS
	TIM_OC1Init(TIM3, &TIM_OCStruct);

	//HALF BRIGHTNESS
	TIM_OCStruct.TIM_Pulse = TIM_BaseStruct.TIM_Period/2;
	TIM_OC1Init(TIM4, &TIM_OCStruct);

	//QUARTER BRIGHTNESS
	TIM_OCStruct.TIM_Pulse = TIM_BaseStruct.TIM_Period/4;
	TIM_OC1Init(TIM5, &TIM_OCStruct);

	//No code in the infinite loop????
	//This is why timers are better!
	//Regardless of optimizations this led will blink at 1Hz forever! (even if your software freezes)

	for(;;);
}




