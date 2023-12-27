/**
  ******************************************************************************
  * @file    main.c
  * @author  Huizhe Zheng
  * @version V1.0
  * @date    05-October-2019
  * @brief   UART intro project.
  ******************************************************************************
*/

#define RE
#define TX
#define RX
#define LED

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include <string.h>

#define INTERRUPT_METHOD
void USART2_IRQHandler(void);
void USART_PutChar(char c);
uint16_t USART_GetChar();
void USART_PutString(char *s);

#ifdef RE

#define USB_USART USART2
#endif

#ifdef TX
#define TX_PIN_NUM GPIO_Pin_2
#define TX_PIN_SOURCE GPIO_PinSource2
#endif

#ifdef RX
#define RX_PIN_NUM GPIO_Pin_3
#define RX_PIN_SOURCE GPIO_PinSource3
#endif

#ifdef LED
#define PIN_NUM GPIO_Pin_5
#define PIN_SOURCE GPIO_PinSource5
#endif

#ifdef ZE

#define USB_USART USART3
#endif

void System_Init(void) {
	/* Enable clock for GPIOA and USART2  (for RE) use function below (x = 1 in this case).
	 * Enable clock for GPIOD and USART3  (for ZE)
     * RCC_AHBxPeriphClockCmd();
     * RCC_APBxPeriphClockCmd();
     */

    /* Write your code here */

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	/**
	 * Tell pins PA2 and PA3 (for Tx and Rx for RE) which alternating function you will use
	 * Tell pins PD8 and PD9 (for Tx and Rx for ZE) which alternating function you will use
	 * @important Make sure, these lines are before pins configuration!
     * Use GPIO_PinAFConfig();
	 */

    /* Write your code here */
	GPIO_PinAFConfig(GPIOA,TX_PIN_SOURCE,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,RX_PIN_SOURCE,GPIO_AF_USART2);

	// Initialize GPIO pins as alternating function, same process as LED BLINK.
    GPIO_InitTypeDef GPIO_Tx;
    GPIO_InitTypeDef GPIO_Rx;

    GPIO_Tx.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Rx.GPIO_Mode = GPIO_Mode_AF;

    GPIO_Tx.GPIO_Pin = TX_PIN_NUM;
    GPIO_Rx.GPIO_Pin = RX_PIN_NUM;

    GPIO_Tx.GPIO_OType = GPIO_OType_PP;
    GPIO_Rx.GPIO_OType = GPIO_OType_PP;

    /* Write your code here */
    GPIO_Tx.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Rx.GPIO_PuPd = GPIO_PuPd_NOPULL;


    GPIO_Tx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Rx.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_Tx);
    GPIO_Init(GPIOA, &GPIO_Rx);

    GPIO_InitTypeDef GPIO_LED;

    GPIO_LED.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_LED.GPIO_Pin = PIN_NUM;
    GPIO_LED.GPIO_OType = GPIO_OType_PP;
    GPIO_LED.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_LED.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_LED);


	/**
	 * Now we have to enable the USART line
	 * Set Baudrate to value you pass to function
	 * Disable Hardware Flow control
	 * Set Mode To TX and RX, so USART will work in full-duplex mode
	 * Disable parity bit
	 * Set 1 stop bit
	 * Set Data bits to 8
	 *
	 * Initialize USART2 Below
	 * Activate USART2
	 */
    USART_InitTypeDef USART_InitStruct;


    USART_InitStruct.USART_BaudRate = 9600;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = (USART_Mode_Tx | USART_Mode_Rx);
    USART_Init(USART2,&USART_InitStruct);
    USART_Cmd(USART2,ENABLE);

    /* Write your code here */



    /*
     * Enable the USART line
     * USART_Cmd();
     */

#ifdef INTERRUPT_METHOD
	/** If you wanna more challenge, initialize the NVCR interrupt,
	 * The correspond function for this has been finished at the end of main.c, USART2_IRQHandler(void) or USART3_IRQHandler(void) .
	 * Is just simply toggle the LED by send UART message from your laptop.
	 *
	 *
	 *
	 * Set Channel to USART2 or 3
	 * Set Channel Cmd to enable. That will enable USART2 or 3 channel in NVIC
	 * Set Both priorities to 0. This means high priority
	 * Initialize NVIC
	 * Enable RX interrupt
	 */

    /* Write your code here. USART_ITConfig */


	NVIC_InitTypeDef NVIC_InitStruct;
    /* Write your code here, write your init here*/

	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);



#endif

}

int main(void) {
	uint16_t c;
	uint16_t d;
	System_Init();

#ifdef NO_INTERRUPT
	USART_PutString("Welcome to the UMSAE Software System\n\r");
	while (1) {
		/* Get character from internal buffer */
		c = USART_GetChar();
		if (c) {
			/* If anything received, put it back to terminal */
			USART_SendData(USB_USART,'\n');
			USART_SendData(USB_USART, c);
		}

	}

#endif

#ifdef INTERRUPT_METHOD

while(1)
{
}

#endif

}

void USART_PutChar(char c) {
// Wait until transmit data register is empty, use USART_GetFlagStatus to get register status
	while(USART_GetFlagStatus(USB_USART, USART_FLAG_TXE) == RESET);


	USART_SendData(USB_USART, c);

// Send a char using USART2 use USART_SendData()

}

uint16_t USART_GetChar() {
	// Wait until data is received use USART_GetFlagStatus();
	while(USART_GetFlagStatus(USB_USART, USART_FLAG_RXNE) == RESET);

	// Read received char use USART_ReceiveData
	USART_ReceiveData(USB_USART);

}

/*
 * This is just a simple Comp2160 function how to concat a bunch of chars to string
 */
void USART_PutString(char *s) {
	while (*s) {
		USART_PutChar(*s++);
	}
}


#ifdef INTERRUPT_METHOD
/**********************************************************
 * USART2 interrupt request handler: on reception of a
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART2_IRQHandler(void)
{
	/* RXNE handler */
	uint16_t c;;
	if(USART_GetITStatus(USB_USART, USART_IT_RXNE) != RESET)
	{
		c = USART_GetChar();
		if (c == 't') {
			/* If anything received, put it back to terminal */
			USART_PutChar(c);
			GPIO_SetBits(GPIOA,PIN_NUM);
		}
		else
		{

			USART_PutChar(c);
			GPIO_ResetBits(GPIOA,PIN_NUM);
		}
	}
}


/**********************************************************
 * USART3 interrupt request handler: on reception of a
 * character 't', toggle LED and transmit a character 'T'
 *********************************************************/
void USART3_IRQHandler(void)
{
	/* RXNE handler */
	uint16_t c;;
	if(USART_GetITStatus(USB_USART, USART_IT_RXNE) != RESET)
	{
		c = USART_GetChar();
		if (c) {
			/* If anything received, put it back to terminal */
			USART_SendData(USB_USART, c);
		}
	}
}

#endif

