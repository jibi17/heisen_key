#include "pushButton.h"
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
//有几个按钮就初始化几个
Button button1={1};
Button button2={2};
Button button3={3};
Button button4={4};
Button button5={5};

extern osMessageQId pushButtonQueueHandle;//这个是把在freertos中



/**
 * @brief 这个函数是用来检测按钮是否按下的，这个函数需要放到一个100hz的循环中，然后包含#include "pushButton.h" 并且需要一个uint32_t的消息队列，
 * @brief 在这32位中及【31：0】 ：【31：23】代表第几号按钮被按下；【23：15】表示状态，0表示无事发生，1按下，2真正被长按，3，弹起；【15：0】表示被长按的时间，单位10ms，最大长安时间10min。
 * @brief 如果是放到freertos线程里面的话记得算一下栈空间够不够 一个函数算20byte！！！
 * @param button 传入实例化的按钮的结构体，在上面初始化的
 * @param GPIO_port 这个按钮对应的GPIOx
 * @param GPIO_Pin 这个按钮对应的GPIO_PIN

*/
//先检测按钮是不是被按下，在看是不是在被长按，最后再看是不是被放开及按钮弹起。
void button_kernel(Button * button,GPIO_TypeDef *GPIO_port, uint16_t GPIO_Pin)
{
    button->button_this_time=HAL_GPIO_ReadPin(GPIO_port,GPIO_Pin);
    //下面是按钮的三个状态 按下 长安 抬起
    uint32_t info=0;
    if((button->button_last_time==1)&&(button->button_this_time==0))//这个判断表示按钮按下
    {
        button->button_status=BTN_PUSH_DOWN;
        button->time=0;//被按下了即代表有可能要开始长按了，先把时间给初始化为0
        //在这里使用消息队列通知另一任务，在另一个任务中来完成相应的事情
		info=button->num << 24;//几号按钮
        info=info|0x00010000;//0x00010000表示按下
       if(osOK!=osMessagePut(pushButtonQueueHandle,info,0))//在这32位中及【31：0】 ：【31：23】代表第几号按钮被按下；【23：15】表示状态，0表示无事发生，1按下，2真正被长按，3，弹起；【15：0】表示被长按的时间，单位10ms，最大长安时间10min。
       {		
            printf("erro!!erro!!,key queue is full!!\r\n");
            Error_Handler();//如果队列满了就执行errorhandler 几个按钮狂按就有可能队列满了溢出，但是这个基本是不可能的
       }
    }
    else if(button->button_this_time==0&&button->button_last_time==0)//这个代表在被长按
    {
        button->button_status=BTN_HOLD_ON;
        button->time++;
        info=button->num << 24;//几号按钮
        info=info|0x00020000;//0x00020000表示正在被长按
        info=info|button->time;//把长按的时间也给放到info里面去
        if(osOK!=osMessagePut(pushButtonQueueHandle,info,0))//在这32位中及【31：0】 ：【31：23】代表第几号按钮被按下；【23：15】表示状态，0表示无事发生，1按下，2真正被长按，3，弹起；【15：0】表示被长按的时间，单位10ms，最大长安时间10min。
       {		
            printf("erro!!erro!!,key queue is full!!\r\n");
            Error_Handler();//如果队列满了就执行errorhandler 几个按钮狂按就有可能队列满了溢出，但是这个基本是不可能的
       }
    }
    else if (button->button_this_time==1&&button->button_last_time==0)//代表按键被弹起
    {
        button->button_status=BTN_RELEASE;
        button->time=0;
        info=button->num << 24;//几号按钮
        info=info|0x00030000;//0x00030000表示被弹起
        if(osOK!=osMessagePut(pushButtonQueueHandle,info,0))//在这32位中及【31：0】 ：【31：23】代表第几号按钮被按下；【23：15】表示状态，0表示无事发生，1按下，2真正被长按，3，弹起；【15：0】表示被长按的时间，单位10ms，最大长安时间10min。
       {		
            printf("erro!!erro!!,key queue is full!!\r\n");
            Error_Handler();//如果队列满了就执行errorhandler 几个按钮狂按就有可能队列满了溢出，但是这个基本是不可能的
       }
    }
    
    else//其他情况
    {   
        button->button_status=0;
    }
    button->button_last_time=button->button_this_time;
}
/**
 * @brief 此函数用与从pushbutton中取数据出来，并读取数据，建议最少10hz循环执行这个函数
 * @brief 放进去的是一个uint16_t的数 高8位代表几号按钮被按下，
 * @brief 此函数会把按下的按钮打印出来
 * 
*/

//多个按钮同时长安你怎么解决！！！
void pushButtonQueueProcess()
{
  //记录最大长按时间！
    uint32_t info=0;
    Button tempButton;
    static uint16_t max_time=0;
    memset(&tempButton,0,sizeof(Button));//初始化变量
    osEvent keyEvent=osMessageGet(pushButtonQueueHandle,0);//先取出来，取出来的是个结构体
	if(keyEvent.status==osEventMessage)//看看是不是有数据，有数据再打印
			{
					
                    info=keyEvent.value.v;
                    tempButton.num=info>>24;//num取出来
                    tempButton.button_status= (info >> 16) & 0xFF;//状态取出来
                    tempButton.time=info&0x0000ffff;//长按时间取出来
                    //下面开始打印检测
                    if(tempButton.button_status==BTN_PUSH_DOWN)
                    {
                        printf("按钮%d被按下",tempButton.num);//按下
                    }

                    if(tempButton.button_status==BTN_HOLD_ON)
                    {
                        uint32_t ms_time=tempButton.time*10;//变成单位为ms
                        if(ms_time>2000)
                        {
                            //长按时间大于两秒，开始搞事情
                            printf("长安时间：%d\r\n",ms_time);//长安
                        }
                        
                    }
                    if(tempButton.button_status==BTN_RELEASE)
                    {
                        printf("按钮%d弹起\r\n",tempButton.num);//弹起
                    }

					
					


                    
			}
    

    
}

