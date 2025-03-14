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
USBD_HandleTypeDef USBD_Device;             /* USB Device´¦Àí½á¹¹Ìå */
extern volatile uint8_t g_device_state;     /* USBÁ¬½Ó Çé¿ö */

/******************************************************************************************************/
/*FreeRTOSÅäÖÃ*/

/* START_TASK ÈÎÎñ ÅäÖÃ
 * °üÀ¨: ÈÎÎñ¾ä±ú ÈÎÎñÓÅÏÈ¼¶ ¶ÑÕ»´óĞ¡ ´´½¨ÈÎÎñ
 */
#define START_TASK_PRIO     1           /* ÈÎÎñÓÅÏÈ¼¶ */
#define START_STK_SIZE      128         /* ÈÎÎñ¶ÑÕ»´óĞ¡ */
TaskHandle_t StartTask_Handler;         /* ÈÎÎñ¾ä±ú */
void start_task(void *pvParameters);    /* ÈÎÎñº¯Êı */



/**
 * @brief       lvgl_demoÈë¿Úº¯Êı
 * @param       ÎŞ
 * @retval      ÎŞ
 */
void FreeRTOS_Start(void)
{
    lv_init();                                          /* lvglÏµÍ³³õÊ¼»¯ */
    lv_port_disp_init();                                /* lvglÏÔÊ¾½Ó¿Ú³õÊ¼»¯,·ÅÔÚlv_init()µÄºóÃæ */
    lv_port_indev_init();                               /* lvglÊäÈë½Ó¿Ú³õÊ¼»¯,·ÅÔÚlv_init()µÄºóÃæ */
    xTaskCreate((TaskFunction_t )start_task,(const char* )"start_task",(uint16_t)START_STK_SIZE,(void* )NULL,(UBaseType_t)START_TASK_PRIO,(TaskHandle_t* )&StartTask_Handler);   /* ÈÎÎñ¾ä±ú */
    vTaskStartScheduler();                              /* ¿ªÆôÈÎÎñµ÷¶È */
}


/**
 * @brief       start_task
 * @param       pvParameters : ´«Èë²ÎÊı(Î´ÓÃµ½)
 * @retval      ÎŞ
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    taskENTER_CRITICAL();           /* ½øÈëÁÙ½çÇø */
	LED_APP_Init();
	LVGL_APP_Init();
	RTC_APP_Init();
    taskEXIT_CRITICAL();            /* ÍË³öÁÙ½çÇø */
    vTaskDelete(StartTask_Handler); /* É¾³ı¿ªÊ¼ÈÎÎñ */
}

int main(void)
{
    HAL_Init();                         /* ³õÊ¼»¯HAL¿â */
    sys_stm32_clock_init(336, 8, 2, 7); /* ÉèÖÃÊ±ÖÓ,168Mhz */
    delay_init(168);                    /* ÑÓÊ±³õÊ¼»¯ */
    usart_init(115200);                 /* ´®¿Ú³õÊ¼»¯Îª115200 */
    led_init();                         /* ³õÊ¼»¯LED */
	while(1)
	{
		if (g_usart_rx_sta & 0x8000)         /* æ¥æ”¶åˆ°äº†æ•°æ®? */
        {
            len = g_usart_rx_sta & 0x3fff;  /* å¾—åˆ°æ­¤æ¬¡æ¥æ”¶åˆ°çš„æ•°æ®é•¿åº¦ */
            printf("æ”¶åˆ°ä¸²å£æ•°æ®å¦‚ä¸‹:\r\n");

            HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_usart_rx_buf,len,1000);    /* å‘é€æ¥æ”¶åˆ°çš„æ•°æ® */
            while(__HAL_UART_GET_FLAG(&g_uart1_handle,UART_FLAG_TC)!=SET);           /* ç­‰å¾…å‘é€ç»“æŸ */
            printf("\r\n\r\n");             /* æ’å…¥æ¢è¡Œ */
            g_usart_rx_sta = 0;
        }
		HAL_Delay(500);
	}
	#if 0
    key_init();                         /* ³õÊ¼»¯°´¼ü */
	rtc_init();
    sram_init();                        /* SRAM³õÊ¼»¯ */
    my_mem_init(SRAMIN);                /* ³õÊ¼»¯ÄÚ²¿SRAMÄÚ´æ³Ø */
   // my_mem_init(SRAMEX);                /* ³õÊ¼»¯Íâ²¿SRAMÄÚ´æ³Ø */
    USBD_Init(&USBD_Device, &VCP_Desc, 0);                      /* ³õÊ¼»¯USB */
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);           /* Ìí¼ÓÀà */
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);   /* ÎªMSCÀàÌí¼Ó»Øµ÷º¯Êı */
    USBD_Start(&USBD_Device);                                   /* ¿ªÆôUSB */
    FreeRTOS_Start();                        /* ÔËĞĞFreeRTOSÀı³Ì */
	#endif
}


