/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"


#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "croutine.h"
#include "event_groups.h"
#include "led.h"
#include "portmacro.h"

void myFirstTask(void * pvParameters);
void myFirstTask2(void * pvParameters);
void myFirstTask3(void * pvParameters);


EventGroupHandle_t group;

void myFirstTask(void * pvParameters){
	for(;;){
		led_toggle();
		vTaskDelay(pdMS_TO_TICKS(250));
		xEventGroupSync(group,1 << 0, 1<<0 | 1 << 1 | 1 << 2,portMAX_DELAY);
	}
	vTaskDelete( NULL );
}

void myFirstTask2(void * pvParameters){
	for(;;){
		led_toggle2();
		vTaskDelay(pdMS_TO_TICKS(250));
		xEventGroupSync(group,1 << 1, 1<<0 | 1 << 1 | 1 << 2,portMAX_DELAY);
	}
	vTaskDelete( NULL );
}

void myFirstTask3(void * pvParameters){
	for(;;){
		led_toggle3();
		vTaskDelay(pdMS_TO_TICKS(250));
		xEventGroupSync(group,1 << 2, 1<<0 | 1 << 1 | 1 << 2,portMAX_DELAY);
	}
	vTaskDelete( NULL );
}

int main(void)
{
	led_init();
	BaseType_t ret = 0;
	group = xEventGroupCreate();

	if(group == NULL)
	{
		return 1;
	}

	ret += xTaskCreate(
			&myFirstTask, 		//pointer to the task function
			"myTask", 			//name for task
			100, 				//stack size, will not run if it is too small
			NULL, 				//pvparameters, what you pass to the task
			tskIDLE_PRIORITY+3, //task priority
			NULL 				//task handle
			);

	ret += xTaskCreate(
				&myFirstTask2, 		//pointer to the task function
				"myTask2", 			//name for task
				100, 				//stack size, will not run if it is too small
				NULL, 				//pvparameters, what you pass to the task
				tskIDLE_PRIORITY+3, //task priority
				NULL 				//task handle
				);

	ret += xTaskCreate(
				&myFirstTask3, 		//pointer to the task function
				"myTask3", 			//name for task
				100, 				//stack size, will not run if it is too small
				NULL, 				//pvparameters, what you pass to the task
				tskIDLE_PRIORITY+3, //task priority
				NULL 				//task handle
				);

	if(ret != pdPASS*3){
		for(;;); //loop forever
	}
	//starts the schedule, tasks start running
	vTaskStartScheduler();

	for(;;);
}

//will get called on stack overflow
void vApplicationStackOverflowHook(void)
{
	configASSERT(0);
}


