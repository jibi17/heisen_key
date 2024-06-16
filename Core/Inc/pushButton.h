#ifndef PSUHBUTTON_H
#define PSUHBUTTON_H


#include "stdint.h"
#include "gpio.h"


#define button1_PORT GPIOE
#define button1_PIN GPIO_PIN_0

#define button2_PORT GPIOE
#define button2_PIN GPIO_PIN_1

#define button3_PORT GPIOE
#define button3_PIN GPIO_PIN_2

#define button4_PORT GPIOE
#define button4_PIN GPIO_PIN_3

#define button5_PORT GPIOE
#define button5_PIN GPIO_PIN_4
/**
 * @brief 每个按键的结构体，使用状态机方式实现
 * @param button_this_time 这次读取到的按键状态
 * @param button_last_time 上次读取到的按键的状态，每次读取后需要手动的把这次读到的值给button_last_time
 * @param button_status 预留的按下，抬起，等状态，还没想好， 1表示按下
*/
typedef struct pushButton
{
    /* data */
    uint8_t num;
    uint8_t button_this_time;
    uint8_t button_last_time;
    uint16_t button_status; 

}Button;


void button_kernel(Button * button,GPIO_TypeDef *GPIO_port, uint16_t GPIO_Pin);
void pushButtonQueueProcess(void);

extern Button button1;
extern Button button2;
extern Button button3;
extern Button button4;
extern Button button5;


#endif
