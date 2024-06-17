# heisen_key
To make a Libraries that are easy to port for esp32 or stm32 and so on, that run with a rtos

what you need to do is 5 steps.
first:
new a struch in pushButton.c like this 
```
Button button1={1};
Button button2={2};
Button button3={3};
Button button4={4};
Button button5={5};

```
second:
and then extern them in pushButton.h like this
```
extern Button button1;
extern Button button2;
extern Button button3;
extern Button button4;
extern Button button5;
```
third: 
define the port and pin of the key
```
#define button1_PORT GPIOE
#define button1_PIN GPIO_PIN_0

```
fourth:
invoke the button_kernel(Button * button,GPIO_TypeDef *GPIO_port, uint16_t GPIO_Pin) in a thread, that the thread need to be called in 100hz

```
    button_kernel(&button1,button1_PORT,button1_PIN);
    button_kernel(&button2,button2_PORT,button2_PIN);
    button_kernel(&button3,button3_PORT,button3_PIN);
    button_kernel(&button4,button4_PORT,button4_PIN);
    button_kernel(&button5,button5_PORT,button5_PIN);
```

fifth:
call the pushButtonQueueProcess() in another thread to do what you want to do after the keys is been pushed .

finally use pushButtonQueueProcess() to handle push down 、push up and long push.
