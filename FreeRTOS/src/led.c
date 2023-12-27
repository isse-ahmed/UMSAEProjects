#include "stm32f4xx.h"
#include "led.h"


static uint8_t init_status = 0;

void led_init(){
		//Variable declaration for GPIO Initialization struct
		GPIO_InitTypeDef GPIOInitType;

		//The LED is on GPIOA, which is connected to the AHB1 clock, so we must turn it on.
		//We don't give a hoot about the configuration of the clock for GPIO, so all we have to do is turn it on
		//This is the case for most things
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		//Filling out the details of the GPIO Init struct

		//For GPIO mode you have OUT, IN, AF (Alternate Function), and AN (analog mode),
		GPIOInitType.GPIO_Mode = GPIO_Mode_OUT;

		//For output type you have PP(Push pull) or OD(open drain (switches between ground and high impedance))
		GPIOInitType.GPIO_OType = GPIO_OType_PP;

		//Choose between the internal pull ups or pull downs, more applicable to inputs
		//(Not even sure if they work on output mode
		GPIOInitType.GPIO_PuPd = GPIO_PuPd_NOPULL;

		//This is the speed the IO switches at. The higher the speed the more noise you will get
		//Some applications have sensitive switching speeds, this is where this setting is most useful
		GPIOInitType.GPIO_Speed = GPIO_Speed_100MHz;

		//Finally this is the pin(s) you intend to use. You have to use the defines for them, as they
		//are not mapped to the pin number. Because of this though you can OR them together to initialize
		//More than one pin on an output register at a time.
		GPIOInitType.GPIO_Pin = NUCLEO_LED_PIN | PIN_2;

		//This is the actual initialization function for the GPIOs. the first parameter is the GPIO bus
		//The second parameter is a pointer to the struct we filled out
		//(if you're in eclipse you can hover over the function for more information)
		GPIO_Init(NUCLEO_LED_PORT, &GPIOInitType);

		GPIOInitType.GPIO_Pin = NUCLEO_LED_PIN;

		GPIO_Init(PIN_3_PORT,&GPIOInitType);

		init_status = 1;
}

void led_toggle(){
	if(init_status == 1){
		GPIO_ToggleBits(NUCLEO_LED_PORT, NUCLEO_LED_PIN);
	}
}

void led_toggle2(){
	if(init_status == 1){
		GPIO_ToggleBits(NUCLEO_LED_PORT,PIN_2);
	}
}

void led_toggle3(){
	if(init_status == 1){
		GPIO_ToggleBits(PIN_3_PORT,NUCLEO_LED_PIN);
	}
}
