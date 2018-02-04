#include "esp_common.h"
#include "Esp_system.h"
#include "gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//size of the array
#define arraySize 3
#define flexNumber 5
// S1, S2, S3
#define PIN_S0 5
#define PIN_GPIO_MUX5 PERIPHS_IO_MUX_GPIO5_U
#define PIN_GPIO_FUNC5 FUNC_GPIO5

#define PIN_S1 0
#define PIN_GPIO_MUX0 PERIPHS_IO_MUX_GPIO0_U
#define PIN_GPIO_FUNC0 FUNC_GPIO0

#define PIN_S2 4
#define PIN_GPIO_MUX4 PERIPHS_IO_MUX_GPIO4_U
#define PIN_GPIO_FUNC4 FUNC_GPIO4

const static uint8_t SxPinArray[3] = {PIN_S0,PIN_S1,PIN_S2};
const static char* SxPinFunction[3] = {PIN_GPIO_FUNC5,PIN_GPIO_FUNC0,PIN_GPIO_FUNC4};
uint16_t FlexValueArray [flexNumber] = {};

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR user_rf_pre_init(void)
{
}

void m_value_y0(void *pvParameters){
  GPIO_OUTPUT_SET(SxPinArray[0], 0);
  GPIO_OUTPUT_SET(SxPinArray[1], 0);
  GPIO_OUTPUT_SET(SxPinArray[2], 0);

  FlexValueArray[0] = system_adc_read();
}

void m_value_y1(void *pvParameters){
  GPIO_OUTPUT_SET(SxPinArray[0], 1);
  GPIO_OUTPUT_SET(SxPinArray[1], 0);
  GPIO_OUTPUT_SET(SxPinArray[2], 0);

  FlexValueArray[1] = system_adc_read();
}

void displayValue(){
  printf("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
  printf("---\t---\t---\t---\t---\t---\t---\t---");

  for(uint8_t i = 0; i < flexNumber; i++){
    printf("%d\t",FlexValueArray[i]);
    printf("\n");
  }
}

void ICACHE_FLASH_ATTR user_init(void)
{
  PIN_FUNC_SELECT(PIN_GPIO_MUX5, PIN_GPIO_FUNC5);
  PIN_FUNC_SELECT(PIN_GPIO_MUX0, PIN_GPIO_FUNC0);
  PIN_FUNC_SELECT(PIN_GPIO_MUX4, PIN_GPIO_FUNC4);

  xTaskCreate(&m_value_y0, "sen", 256, NULL, 2, NULL);
  vTaskDelay(20);
}
