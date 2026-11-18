/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
volatile uint8_t hours = 12;   // Начальное время для теста
volatile uint8_t minutes = 34;
volatile uint8_t seconds = 0;

volatile uint8_t digits[4] = {1, 2, 3, 4}; // Массив для хранения цифр на 4 лампах
volatile uint8_t current_lamp = 0;         // Текущая активная лампа (0-3)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Set_Lamp(uint8_t lamp);
void Set_Digit(uint8_t digit);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // Запуск ШИМ (100 кГц)
  HAL_TIM_Base_Start_IT(&htim3);            // Запуск таймера времени (1 Гц)
  HAL_TIM_Base_Start_IT(&htim4);            // Запуск таймера экрана (2 мс)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// Функция включения нужной лампы (анода)
// Принимает номер лампы (0, 1, 2, 3). Если передать 255, выключает все.
// Функция подачи кода цифры на дешифратор (P0-P3 на пинах PB0-PB3)
void Set_Digit(uint8_t digit) {
    // Сначала сбрасываем все пины в 0
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
    
    // Выставляем биты в зависимости от цифры
    if (digit & 0x01) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // P0
    if (digit & 0x02) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // P1
    if (digit & 0x04) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET); // P2
    if (digit & 0x08) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // P3
}

// Функция включения нужной лампы (аноды на пинах PB4-PB7)
void Set_Lamp(uint8_t lamp) {
    // Сначала выключаем все аноды
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
    
    // Инвертируем номер лампы (0 станет 3, 1 станет 2 и т.д.)
    uint8_t real_lamp = 3 - lamp;

    // Включаем нужную лампу
    if (real_lamp == 0) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
    if (real_lamp == 1) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    if (real_lamp == 2) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    if (real_lamp == 3) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}
// Функция, которая будет вызываться при срабатывании любого таймера
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    // --- ТАЙМЕР 3: Счёт времени (срабатывает раз в секунду) ---
    if (htim->Instance == TIM3) {
        seconds++;
        if (seconds >= 60) {
            seconds = 0;
            minutes++;
            if (minutes >= 60) {
                minutes = 0;
                hours++;
                if (hours >= 24) {
                    hours = 0;
                }
            }
        }

        // Раскладываем время на цифры для индикаторов
        digits[0] = hours / 10;       // Десятки часов
        digits[1] = hours % 10;       // Единицы часов
        digits[2] = minutes / 10;     // Десятки минут
        digits[3] = minutes % 10;     // Единицы минут
    }

    // --- ТАЙМЕР 4: Мультиплексирование (срабатывает каждые 2 мс) ---
    if (htim->Instance == TIM4) {
    // 1. Выключаем ВСЕ лампы
    Set_Lamp(255);
    
    // 2. КОРОТКАЯ ПАУЗА (Слепое окно)
    // Даем транзисторам время закрыться, а дешифратору переключиться.
    // Обычно достаточно 10-50 микросекунд. 
    // В цикле используем __NOP() (пустая операция), чтобы не тормозить весь процесс.
    for (volatile uint16_t i = 0; i < 50; i++) { __NOP(); } 

    // 3. Подаем код текущей цифры на дешифратор
    Set_Digit(digits[current_lamp]);

    // 4. Включаем нужную лампу
    Set_Lamp(current_lamp);

    // 5. Переходим к следующей лампе
    current_lamp++;
    if (current_lamp >= 4) {
        current_lamp = 0;
    }

  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
