#ifndef LED_H
#define LED_h

#define NUCLEO_LED_PIN 		GPIO_Pin_6
#define NUCLEO_LED_PORT 	GPIOA
#define PIN_2 GPIO_Pin_0
#define PIN_3_PORT GPIOB

void led_init();
void led_toggle();
void led_toggle2();
void led_toggle3();

#endif
