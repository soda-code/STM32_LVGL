#include "LVGL/GUI_APP/lv_mainstart.h"
#include "lvgl.h"
#include <stdio.h>
#include "./SYSTEM/delay/delay.h"
#include "RTC_Read_app.h"


/* 获取当前活动屏幕的宽高 */
#define scr_act_width()  lv_obj_get_width(lv_scr_act())
#define scr_act_height() lv_obj_get_height(lv_scr_act())

LV_IMG_DECLARE(img_gear);                     /* 声明图片 */

static lv_obj_t *win;                                               /* 定义窗口 */
static lv_obj_t *title; 
/* 定义图片部件、滑块部件 */
static lv_obj_t *img;


uint8_t time_buf[50]={0}; 



void update_title(lv_obj_t * label)
{
	sprintf((char*)time_buf,"%02d:%02d:%02d",RTC_Info.hour,RTC_Info.min,RTC_Info.sec);
    lv_label_set_text(label, (char*)time_buf);
}

// 定时器任务回调函数
static void  timer_task( lv_event_t *e) 
{
	update_title(title);
    //usb_printf("Timer task called: %d\n", count++);
}

/**
 * @brief  按钮事件回调
 * @param  *e ：事件相关参数的集合，它包含了该事件的所有数据
 * @return 无
 */
static void btn_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);                    /* 获取事件类型 */

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("Toggled");
    }

}

/**
 * @brief  窗口实例
 * @param  无
 * @return 无
 */

static void LV_Main_Win(void)
{


    const lv_font_t *font = &lv_font_montserrat_30;


    /* 窗口整体 */
    win = lv_win_create(lv_scr_act(), scr_act_height()/12);                                     /* 创建窗口 */
    lv_obj_set_size(win, scr_act_width(), scr_act_height());                         			 /* 设置大小 */
    lv_obj_center(win);                                                                         /* 设置位置 */
    lv_obj_set_style_border_width(win, 1, LV_STATE_DEFAULT);                                    /* 设置边框宽度 */
    lv_obj_set_style_border_color(win, lv_color_hex(0x8a8a8a), LV_STATE_DEFAULT);               /* 设置边框颜色 */
    lv_obj_set_style_border_opa(win, 100, LV_STATE_DEFAULT);                                    /* 设置边框透明度 */
	lv_obj_set_height(lv_win_get_header(win), 30);  // 将标题栏高度设置为 50 像素
     title = lv_win_add_title(win, ""); 
    lv_obj_set_style_text_font(title, font, LV_STATE_DEFAULT);                                  /* 设置字体 */

    /* 主体背景 */
    lv_obj_t *content = lv_win_get_content(win);                                                /* 获取主体 */
    lv_obj_set_style_bg_color(content, lv_color_hex(0xffffff),LV_STATE_DEFAULT);                /* 设置背景颜色 */
    /* 右侧按钮 */
    lv_obj_t *btn_close = lv_btn_create(content);                              /* 添加按钮 */
	lv_obj_set_size(btn_close, 30, 30);
	lv_obj_align(btn_close,LV_ALIGN_DEFAULT,0,0);
	lv_obj_add_flag(btn_close, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_add_event_cb(btn_close, btn_event_cb, LV_EVENT_CLICKED, NULL);                       /* 添加事件 */

    
}

/**
 * @brief  LVGL演示
 * @param  无
 * @return 无
 */
void HOME_DISPLAY(void)
{
	img = lv_img_create(lv_scr_act());																/* 创建图片部件 */
	lv_img_set_src(img, &img_gear);																  /* 设置图片源 */
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);                                    /* 设置图片位置 */
    lv_obj_update_layout(img);                                                                      /* 更新图片参数 */

	lv_refr_now(NULL); 
	delay_ms(3000);
    lv_obj_clean(img);  // 清空当前屏幕上的所有对象
    
    LV_Main_Win();   //**************创建主界面

}
