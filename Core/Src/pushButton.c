#include "pushButton.h"
#include "main.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "usart.h"
#include "stdio.h"
//有几个按钮就初始化几个
Button button1={1};
Button button2={2};
Button button3={3};
Button button4={4};
Button button5={5};

extern osMessageQId pushButtonQueueHandle;



/**
 * @brief 这个函数是用来检测按钮是否按下的，这个函数需要放到一个100hz的循环中，然后包含#include "pushButton.h"
 * @param button 传入实例化的按钮的结构体，在上面初始化的
 * @param GPIO_port 这个按钮对应的GPIOx
 * @param GPIO_Pin 这个按钮对应的GPIO_PIN

*/
void button_kernel(Button * button,GPIO_TypeDef *GPIO_port, uint16_t GPIO_Pin)
{
    button->button_this_time=HAL_GPIO_ReadPin(GPIO_port,GPIO_Pin);
    if((button->button_last_time==1)&&(button->button_this_time==0))//这个判断表示按钮按下
    {
        
        button->button_this_time=0;
        /* 在这个地方可以放你想干的事情   */
        //比如我在这里使用消息队列通知另一任务，来完成相应的事情
				
       if(osOK!=osMessagePut(pushButtonQueueHandle,((uint16_t)button->num << 8) | 1,0))
       {
					
            printf("erro!!erro!!,key queue is full!!\r\n");
            Error_Handler();//如果队列满了就执行errorhandler
       }
        

        
        /*end*/
    }
    else//其他情况
    {   
        button->button_status=0;
    }
    button->button_last_time=button->button_this_time;
}
/**
 * @brief 此函数用与从pushbutton中取数据出来，并读取数据，建议最少10hz循环执行这个函数
 * 
 * @brief 此函数会把按下的按钮打印出来
 * 
*/
void pushButtonQueueProcess()
{
  
    
      osEvent keyEvent=osMessageGet(pushButtonQueueHandle,0);//先取出来，取出来的是个结构体
	if(keyEvent.status==osEventMessage)//看看是不是有数据，有数据再打印
			{
					uint8_t temp=keyEvent.value.v>>8;
					printf("按钮%d被按下!\r\n",temp);
					
			}
    

    
}

