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


void led_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while(1)
    {
        LED0_TOGGLE();
		usb_printf("\r\n123\r\n\r\n");
        vTaskDelay(1000);
    }
}


void LED_APP_Init(void)
{
	    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,(const char* )"led_task",(uint16_t )LED_STK_SIZE,(void* )NULL,(UBaseType_t )LED_TASK_PRIO,(TaskHandle_t*  )&LEDTask_Handler);
}
