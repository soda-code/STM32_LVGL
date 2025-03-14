#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"

#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "LVGL/GUI_APP/lv_mainstart.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led_app.h"
#include "lvgl_app.h"
#include "RTC_Read_app.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"
USBD_HandleTypeDef USBD_Device;             /* USB Device����ṹ�� */
extern volatile uint8_t g_device_state;     /* USB���� ��� */

/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;         /* ������ */
void start_task(void *pvParameters);    /* ������ */



/**
 * @brief       lvgl_demo��ں���
 * @param       ��
 * @retval      ��
 */
void FreeRTOS_Start(void)
{
    lv_init();                                          /* lvglϵͳ��ʼ�� */
    lv_port_disp_init();                                /* lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ��� */
    lv_port_indev_init();                               /* lvgl����ӿڳ�ʼ��,����lv_init()�ĺ��� */
    xTaskCreate((TaskFunction_t )start_task,(const char* )"start_task",(uint16_t)START_STK_SIZE,(void* )NULL,(UBaseType_t)START_TASK_PRIO,(TaskHandle_t* )&StartTask_Handler);   /* ������ */
    vTaskStartScheduler();                              /* ����������� */
}


/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    taskENTER_CRITICAL();           /* �����ٽ��� */
	LED_APP_Init();
	LVGL_APP_Init();
	RTC_APP_Init();
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
}

int main(void)
{
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��,168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
	while(1)
	{
		if (g_usart_rx_sta & 0x8000)         /* 接收到了数据? */
        {
            len = g_usart_rx_sta & 0x3fff;  /* 得到此次接收到的数据长度 */
            printf("收到串口数据如下:\r\n");

            HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart_rx_buf,len,1000);    /* 发送接收到的数据 */
            while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);           /* 等待发送结束 */
            printf("\r\n\r\n");             /* 插入换行 */
            g_usart_rx_sta = 0;
        }
		HAL_Delay(500);
	}
	#if 0
    key_init();                         /* ��ʼ������ */
	rtc_init();
    sram_init();                        /* SRAM��ʼ�� */
    my_mem_init(SRAMIN);                /* ��ʼ���ڲ�SRAM�ڴ�� */
   // my_mem_init(SRAMEX);                /* ��ʼ���ⲿSRAM�ڴ�� */
    USBD_Init(&USBD_Device, &VCP_Desc, 0);                      /* ��ʼ��USB */
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);           /* ����� */
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);   /* ΪMSC����ӻص����� */
    USBD_Start(&USBD_Device);                                   /* ����USB */
    FreeRTOS_Start();                        /* ����FreeRTOS���� */
	#endif
}


