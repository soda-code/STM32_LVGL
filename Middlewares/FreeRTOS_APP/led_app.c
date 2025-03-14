#include "./BSP/LED/led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led_app.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO       4           /* �������ȼ� */
#define LED_STK_SIZE        128         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;           /* ������ */
void led_task(void *pvParameters);      /* ������ */



void LED_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) 
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

void LED_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) 
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) 
{
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

void led_task(void *pvParameters)
{
    LED_Command_t cmd=LED_CMD_TOGGLE;
    while(1)
    {
        switch (cmd) 
		{
            case LED_CMD_ON:
                LED_On(LED0_GPIO_PORT, LED0_GPIO_PIN);
                break;
            case LED_CMD_OFF:
                LED_Off(LED0_GPIO_PORT, LED0_GPIO_PIN);
                break;
            case LED_CMD_TOGGLE:
                LED_Toggle(LED0_GPIO_PORT, LED0_GPIO_PIN);
                break;
        }
        vTaskDelay(1000);
    }
}


void LED_APP_Init(void)
{
	    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,(const char* )"led_task",(uint16_t )LED_STK_SIZE,(void* )NULL,(UBaseType_t )LED_TASK_PRIO,(TaskHandle_t*  )&LEDTask_Handler);
}
