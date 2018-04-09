/*
**********************************************************************************************************
  Filename : main.c
  Description : you will find here how to get all the flex sensors value by using an
  analogic multiplexer by programming with the ESP8266 using the RTOS API from Espressif
  Author : Maxime Rebillard
  For project : PI2 (A4)
  Year : 2017 / 2018
**********************************************************************************************************
*/
#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"
#include "flex.h"

// the user_rf_cal_sector_set is used to allocate the memory of the esp8266
//declared in all program
uint32 user_rf_cal_sector_set(void) {
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

// do not modified the function below too
void ICACHE_FLASH_ATTR user_rf_pre_init(void) {}

void ICACHE_FLASH_ATTR user_init(void) {

  // Pin declaration used for pwm_init
  uint32 io_info[][3] = {
      {PWM_2_OUT_IO_MUX, PWM_2_OUT_IO_FUNC, PWM_2_OUT_IO_NUM},
      {PWM_14_OUT_IO_MUX, PWM_14_OUT_IO_FUNC, PWM_14_OUT_IO_NUM}};

  // doesn't matter the value below
  u32 duty[1] = {15};

  /*we initalise the PWM for a period of 20ms (so 20 000 micro-seconds),
    we specify the duty (irrelevant here because we will change it multiple
    times when we will set the value for the servo) we specify the number of
    channel (here only one) and all the information about the pin (that is an
    array of array of size n (the number of channel used and a constant[3])
  */
  pwm_init(20000, duty, PWM_NUM_CHANNEL_NUM, io_info);

  // dynamic allocation of arrays

  // FlexValueArray used to save the flex sensors values (input)
  FlexValueArray = (int *)malloc(flexNumber * sizeof(int));

  // ServoValueArray used to save the servo values (output)
  ServoValueArray = (int *)malloc(flexNumber * sizeof(int));

  // change this value will change values displayed in the Serial Monitor
  // 1 for flex sensors
  // otherwise servo sensors

  // Pin definition (we use Digital Pin 5, 0, 4)
  PIN_FUNC_SELECT(PIN_GPIO_MUX5, PIN_GPIO_FUNC5);
  PIN_FUNC_SELECT(PIN_GPIO_MUX0, PIN_GPIO_FUNC0);
  PIN_FUNC_SELECT(PIN_GPIO_MUX4, PIN_GPIO_FUNC4);

  // Tasks

  // one task for all  the flex sensors values
  xTaskCreate(get_flex_values, (signed char *)"updateflexValues", 256, NULL, 2,
              &xHandle_get_flex_values);
              vTaskDelay(10);
  xTaskCreate(displayValuesForProcesssing, (signed char *)"display", 256, (void *)mode, 2,
             &xHandle_display_values);
  //  xTaskCreate(ex_servo,(signed char*)"servo",256,NULL,2,NULL);

  //enable the function below to "test" the servo
  //  xTaskCreate(turn_servo,(signed char*)"turnservo",256,NULL,2,NULL);

  /*
//create all the tasks for each
  xTaskCreate(&value_yx, (signed char*)"y0", 256, (void*) 0, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y1", 256, (void*) 1, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y2", 256, (void*) 2, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y3", 256, (void*) 3, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y4", 256, (void*) 4, 2, NULL);

  */

  // delay de 500ms
  //  xTaskCreate(&displayValues ,(signed char*)"display" ,256 , NULL ,2 ,NULL);
}
