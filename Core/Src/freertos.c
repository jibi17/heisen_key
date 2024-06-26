/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pushButton.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId airHandle;
osThreadId movtionHandle;
osThreadId pushButtonHandle;
osMessageQId pushButtonQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void airTask(void const * argument);
void movtionTask(void const * argument);
void pushButtonTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of pushButtonQueue */
  osMessageQDef(pushButtonQueue, 256, uint32_t);
  pushButtonQueueHandle = osMessageCreate(osMessageQ(pushButtonQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of air */
  osThreadDef(air, airTask, osPriorityRealtime, 0, 128);
  airHandle = osThreadCreate(osThread(air), NULL);

  /* definition and creation of movtion */
  osThreadDef(movtion, movtionTask, osPriorityLow, 0, 128);
  movtionHandle = osThreadCreate(osThread(movtion), NULL);

  /* definition and creation of pushButton */
  osThreadDef(pushButton, pushButtonTask, osPriorityIdle, 0, 128);
  pushButtonHandle = osThreadCreate(osThread(pushButton), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_6);
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_7);
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_airTask */
/**
* @brief Function implementing the air thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_airTask */
void airTask(void const * argument)
{
  /* USER CODE BEGIN airTask */
  /* Infinite loop */
  for(;;)
  {
		//HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
		
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_SET);
		osDelay(1);
		HAL_UART_Transmit(&huart3,"123",3,3);
		osDelay(1);
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_10,GPIO_PIN_RESET);
    osDelay(10);
  }
  /* USER CODE END airTask */
}

/* USER CODE BEGIN Header_movtionTask */
/**
* @brief Function implementing the movtion thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_movtionTask */
void movtionTask(void const * argument)
{
  /* USER CODE BEGIN movtionTask */
  /* Infinite loop */
  for(;;)
  {
    
    

    pushButtonQueueProcess();


    osDelay(10);
  }
  /* USER CODE END movtionTask */
}

/* USER CODE BEGIN Header_pushButtonTask */
/**
* @brief Function implementing the pushButton thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_pushButtonTask */
void pushButtonTask(void const * argument)
{
  /* USER CODE BEGIN pushButtonTask */
  /* Infinite loop */
  for(;;)
  {
    button_kernel(&button1,button1_PORT,button1_PIN);
    button_kernel(&button2,button2_PORT,button2_PIN);
    button_kernel(&button3,button3_PORT,button3_PIN);
    button_kernel(&button4,button4_PORT,button4_PIN);
    button_kernel(&button5,button5_PORT,button5_PIN);
    osDelay(10);
  }
  /* USER CODE END pushButtonTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
