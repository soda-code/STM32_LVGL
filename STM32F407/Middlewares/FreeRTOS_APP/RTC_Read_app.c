#include "./BSP/LED/led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "RTC_Read_app.h"


/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define RTC_TASK_PRIO       4           /* �������ȼ� */
#define RTC_STK_SIZE        128         /* �����ջ��С */
TaskHandle_t RTCTask_Handler;           /* ������ */
void RTC_task(void *pvParameters);      /* ������ */

RTC_STRUCT RTC_Info={0};

void RTC_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while(1)
    {
		rtc_get_time(&RTC_Info);
		rtc_get_date(&RTC_Info);

        vTaskDelay(500);
    }
}


void RTC_APP_Init(void)
{
	    /* LED�������� */
    xTaskCreate((TaskFunction_t )RTC_task,(const char* )"RTC_task",(uint16_t )RTC_STK_SIZE,(void* )NULL,(UBaseType_t )RTC_TASK_PRIO,(TaskHandle_t*  )&RTCTask_Handler);
}
