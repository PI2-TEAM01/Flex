/*
**********************************************************************************************************
Filename : flex.c
Description : you will find here all the #define, constant (static)
variable used for the flex sensors
Author : Maxime Rebillard
For project : PI2 (A4)
Team : 01
Year: 2017 / 2018
**********************************************************************************************************
*/

#include "flex.h"
#include "c_types.h"
#include "math.h"

// pointers declarations
int *FlexValueArray;
int *ServoValueArray;

void ICACHE_FLASH_ATTR displayValues(void *pvParameters) {
  int *array = NULL;
  int choice = (int)pvParameters;
  if (choice == 1)
    array = (int *)FlexValueArray;
  else
    array = (int *)ServoValueArray;

  while (1) {
    printf("Y0\tY1\tY2\tY3\tY4\t");
    printf("\n");
    printf("---\t---\t---\t---\t---");
    printf("\n");

    for (int i = 0; i < flexNumber; i++) {
      printf("%d\t", array[i]);
    }
    printf("\n");
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

void ICACHE_FLASH_ATTR displayValuesForProcesssing(void){
  int* array = NULL;
  //no choice the value that we are going to display is the final value (the PWM value)
  array = (int*)ServoValueArray;

  while(1){
    printf("data ");
    for (int i = 0; i < flexNumber; i++) {
      printf("%d ", array[i]);
    }
    printf("\n");
    vTaskDelay(10);
  }
}

int ICACHE_FLASH_ATTR convert_flex_to_servo(int input) {

  if (input >= 0 && input <= 1024) {
    // mapping from (1024 => 0) to (0 => 102)
    double slope =
        1.0 * (output_end - output_start) / (input_end - input_start);
    int output = output_start + slope * (input - input_start);
    return floor(output);
  }
  return -1;
}

void ICACHE_FLASH_ATTR value_yx(void *pvParameters) {
  // array that will point to the "code" in function of the pvParameters value
  int *tab = NULL;
  // switch with all the case (Y0, Y1, Y2, Y3, Y4)
  switch ((int)pvParameters) {

  case 0:
    tab = (int *)Code_y0;
    break;
  case 1:
    tab = (int *)Code_y1;
    break;
  case 2:
    tab = (int *)Code_y2;
    break;
  case 3:
    tab = (int *)Code_y3;
    break;
  case 4:
    tab = (int *)Code_y4;
    break;
  default:
    break;
  }

  // apply the combination in Pin 5, 0, 4
  GPIO_OUTPUT_SET(SxPinArray[0], tab[0]);
  GPIO_OUTPUT_SET(SxPinArray[1], tab[1]);
  GPIO_OUTPUT_SET(SxPinArray[2], tab[2]);

  // get the analogic value
  FlexValueArray[(int)pvParameters] = system_adc_read();

}

void ICACHE_FLASH_ATTR get_servo_values(void) {
  for (int i = 0; i < flexNumber; i++) {
    ServoValueArray[i] = convert_flex_to_servo(FlexValueArray[i]);
  }
}

void ICACHE_FLASH_ATTR get_flex_values(void) {
  while (1) {
    for (int i = 0; i < flexNumber; i++) {
      value_yx((void *)i);
    }
    get_servo_values();
  }
  vTaskDelete(NULL);
}


void ICACHE_FLASH_ATTR turn_servo(void) {
  while (1) {
    // set the duty cycle
    vTaskSuspend(xHandle_get_flex_values);
    vTaskSuspend(xHandle_display_values);
    printf("\ntask1 suspend\n");
    printf("task2 suspend\n");

    // void pwm_set_duty ( uint32 duty, uint8 channel )
    // the duty is the value inside ServoValueArray and the Pin declared in the
    // user_init is the corresponding channel
    pwm_set_duty(ServoValueArray[3], 1);
    pwm_set_duty(ServoValueArray[1], 0);
    // it is only where calling pwm_start() that the 2 line above are setted
    pwm_start();

    // wait 200ms
    vTaskDelay(200 / delay_time);

    printf("task1 resume\n");
    vTaskResume(xHandle_get_flex_values);
    printf("task2 resume\n");
    vTaskResume(xHandle_display_values);
  }
  vTaskDelete(NULL);
}
