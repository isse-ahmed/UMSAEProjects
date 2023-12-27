
#define RE
#define BUTTON

/**
  ******************************************************************************
  * @file    main.c
  * @author  Matthew Kehler for UMSAE Formula Electric
  * @version V1.0
  * @date    2016-August-02
  * @brief   Default main function.
  ******************************************************************************
*/
// Learning Branch
/*
 * This header file contains all the system definitions from ISR handlers
 * to register mappings. It's a big file, but if you need to find
 * some system level stuff it's more than likely in there.
 * DON'T CHANGE ANYTHING IN HERE UNLESS YOU'RE ABSOLUTLY SURE ON WHAT YOU'RE DOING!
 */
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
			
/*
 * Boring old software blocking LED blink
 */
int main(void)
{
#ifdef RE
#define PIN_NUM GPIO_Pin_5
#define PIN_SOURCE GPIO_PinSource5
#define RCC_PORT RCC_AHB1Periph_GPIOA
#define PORT GPIOA
#endif

#ifdef BUTTON
#define BUTTON_PIN GPIO_Pin_13
#define BUTTON_SOURCE GPIO_PinSource13
#define BUTTON_RCC_PORT RCC_AHB1Periph_GPIOC
#define BUTTON_PORT GPIOC
#endif


#ifdef ZE

#define PIN_NUM GPIO_Pin_0
#define RCC_PORT RCC_AHB1Periph_GPIOB
#define PORT GPIOB
#define PIN_SOURCE GPIO_PinSource0

#endif




	//Variable declaration for GPIO Initialization struct
	GPIO_InitTypeDef GPIOInitType;
	GPIO_InitTypeDef GPIOButton;

	//The LED is on GPIOA, which is connected to the AHB1 clock, so we must turn it on.
	//We don't give a hoot about the configuration of the clock for GPIO, so all we have to do is turn it on
	//This is the case for most things
	RCC_AHB1PeriphClockCmd(RCC_PORT, ENABLE);
	RCC_AHB1PeriphClockCmd(BUTTON_RCC_PORT,ENABLE);

	//Filling out the details of the GPIO Init struct


	//For GPIO mode you have OUT, IN, AF (Alternate Function), and AN (analog mode),
	GPIOInitType.GPIO_Mode = GPIO_Mode_OUT;
	GPIOButton.GPIO_Mode = GPIO_Mode_IN;

	//For output type you have PP(Push pull) or OD(open drain (switches between ground and high impedance))
	GPIOInitType.GPIO_OType = GPIO_OType_PP;
	GPIOButton.GPIO_OType = GPIO_OType_PP;

	//Choose between the internal pull ups or pull downs, more applicable to inputs
	//(Not even sure if they work on output mode
	GPIOInitType.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOButton.GPIO_PuPd = GPIO_PuPd_UP;

	//This is the speed the IO switches at. The higher the speed the more noise you will get
	//Some applications have sensitive switching speeds, this is where this setting is most useful
	GPIOButton.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInitType.GPIO_Speed = GPIO_Speed_100MHz;

	//Finally this is the pin(s) you intend to use. You have to use the defines for them, as they
	//are not mapped to the pin number. Because of this though you can OR them together to initialize
	//More than one pin on an output register at a time.
	GPIOInitType.GPIO_Pin = PIN_NUM;
	GPIOButton.GPIO_Pin = BUTTON_PIN;

	//This is the actual initialization function for the GPIOs. the first parameter is the GPIO bus
	//The second parameter is a pointer to the struct we filled out
	//(if you're in eclipse you can hover over the function for more information)
	GPIO_Init(PORT, &GPIOInitType);
	GPIO_Init(PORT, &GPIOButton);



	//comment out the following line for much, much better implementation
//#define BAD_OLD_BORING_LED_BLINK

#ifdef BAD_OLD_BORING_LED_BLINK
	//Infinite loop
	for(;;)
	{
		//Function to toggle the Pin we want
		GPIO_ToggleBits(PORT, PIN_NUM);

		//A very, very, very poor blocking loop.
		//(note you must be using the C standard gnu99 to declare variables in for loops and to use in line assembly)
		//Also ask Matt to show you fun trick about why counting instruction cycles doesn't work (relating to optimizations)
		for(uint32_t i = 0; i<SystemCoreClock/60; i++) __NOP();
	}
#endif

	//Push Button
	for(;;)
	{
		if(GPIO_ReadInputData(BUTTON_PORT))
		{

			GPIO_ResetBits(PORT,PIN_NUM);
		}
		else
		{

			GPIO_SetBits(PORT,PIN_NUM);
		}

		for(int i = 0; i < 1000000; i++);
	}


	//Enable clock for Timer2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	RCC_ClocksTypeDef clockSpeeds;
	RCC_GetClocksFreq(&clockSpeeds); //Pause here to see clock speeds
	                                 //SYSCLK is the CPU frequency
	                                 //HCLK is AHB
	                                 //PCLK1 is APB1
	                                 //PCLK2 is APB2


	GPIO_PinAFConfig(PORT, PIN_SOURCE, GPIO_AF_TIM2);
	//We now need the pin to be attached to the timer

	GPIOInitType.GPIO_Mode = GPIO_Mode_AF;
	//Set the alternate function of pin 5 to timer 2

	GPIO_Init(PORT, &GPIOInitType);



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
	TIM_BaseStruct.TIM_Period        = SystemCoreClock/20000;//This tells the timer when to reset
	TIM_BaseStruct.TIM_Prescaler     = 10000;                //Sets another scaler to slow down the clock even more

	TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);   //Initializes the clock
	TIM_Cmd(TIM2, ENABLE);                     //Turns on the clock

	//Now we're going to set the mode of the timer
	//See the reference manual for all the modes
	TIM_OCInitTypeDef TIM_OCStruct;

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;             //Sets it to a PWM mode
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; //Tells the Timer to use the pin output
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //Sets the polarity of the PWM
	TIM_OCStruct.TIM_Pulse = TIM_BaseStruct.TIM_Period/2;  //Sets the duty cycle (half of the reset counter)


	//Setup the mode of the timer
	TIM_OC1Init(TIM2, &TIM_OCStruct);

	//No code in the infinite loop????
	//This is why timers are better!
	//Regardless of optimizations this led will blink at 1Hz forever! (even if your software freezes)
	for(;;);

}




