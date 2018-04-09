/*
**********************************************************************************************************
Filename : flex.h
Description : contains C function declarations and macro definitions to be shared with flex.c
variable used for the flex sensors
Author : Maxime Rebillard
For project : PI2 (A4)
Team : 01
Year: 2017 / 2018
**********************************************************************************************************
*/

#include "c_types.h"
#include "esp_common.h"
#include "gpio.h"

// size of the array (the array that contains S2 S1 S0)
#define arraySize 3

// number of flex sensor
#define flexNumber 5

/*
  Pins definition
  all the pin used to control :
  S0 => Pin 5
  S1 => Pin 0
  S2 => Pin 4
*/

#define PIN_S0 5
#define PIN_GPIO_MUX5 PERIPHS_IO_MUX_GPIO5_U
#define PIN_GPIO_FUNC5 FUNC_GPIO5

#define PIN_S1 0
#define PIN_GPIO_MUX0 PERIPHS_IO_MUX_GPIO0_U
#define PIN_GPIO_FUNC0 FUNC_GPIO0

#define PIN_S2 4
#define PIN_GPIO_MUX4 PERIPHS_IO_MUX_GPIO4_U
#define PIN_GPIO_FUNC4 FUNC_GPIO4

// Pin Number 2 and 14 (only used for testing Servo with PWM.h)

#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_GPIO2_U
#define PWM_2_OUT_IO_NUM 2
#define PWM_2_OUT_IO_FUNC FUNC_GPIO2

#define PWM_14_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define PWM_14_OUT_IO_NUM 14
#define PWM_14_OUT_IO_FUNC FUNC_GPIO14

// Number of channel that need to be initialized when calling "pwm_init"

#define PWM_NUM_CHANNEL_NUM 2

/*
input of the ESP8266 :
analog values from 1024 to 0 :
  if not pressed => value 1024
  if pressed at the maximum => value 0
*/

#define input_start 1024
#define input_end 0

/*
output of the ESP8266
values that will be sent to the other ESP8266
  need to map the input values to scale between 0 and the maximum value taken by the servo
  here from 0 to 152
*/

#define output_start 0
#define output_end 152

#define delay_time portTICK_RATE_MS

// Define some array to organize the code (not mandatory)

const static uint8_t SxPinArray[3] = {PIN_S0, PIN_S1, PIN_S2};
const static uint SxPinMux[3] = {PERIPHS_IO_MUX_GPIO5_U, PERIPHS_IO_MUX_GPIO0_U,
                                 PERIPHS_IO_MUX_GPIO4_U};
const static char *SxPinFunction[3] = {PIN_GPIO_FUNC5, PIN_GPIO_FUNC0,
                                       PIN_GPIO_FUNC4};

/*
Display Mode :
  note : update the values of the resistor in the voltage divider because at the minimum value when flexed will not be closed to 0
  values : 1 to display all the flex sensors values : you'll see all the Yn with the corresponding value for each sensor : 1024 => 0,
  other values :  servo motors values (from 0 to 152)
*/

const static  int mode = 1;

// all the S0 S1 S2 code for the multiplexer (that correspond to the code for S2 S1 S0)

const static int Code_y0[3] = {0, 0, 0};
const static int Code_y1[3] = {1, 0, 0};
const static int Code_y2[3] = {0, 1, 0};
const static int Code_y3[3] = {1, 1, 0};
const static int Code_y4[3] = {0, 0, 1};

// Task Handlers (used for the example with the servo)
xTaskHandle xHandle_get_flex_values;
xTaskHandle xHandle_display_values;

extern int *FlexValueArray;
extern int *ServoValueArray;

/*
  don't forget to declare the Pin in the user_init() FunctionName
  for example : if you need to use Pin 5 :
  PIN_FUNC_SELECT(PIN_GPIO_MUX5, PIN_GPIO_FUNC5);
*/

//prototype of the functions declared in the flex.c file

// display all the analogic value (use of a task)
void ICACHE_FLASH_ATTR displayValues(void *pvParameters);

// display all the values in order to be used with processing ide :
// like "data v1 v2 V3 v4 v5" with v1, the value of the first flex sensors
void ICACHE_FLASH_ATTR displayValuesForProcesssing(void);

// convert the values of the flex (between 1024 => 0 to 0 152) in order to be used by the servo
int ICACHE_FLASH_ATTR convert_flex_to_servo(int input);

// get the correct analogic value from the multiplexer in function of the parameter
void ICACHE_FLASH_ATTR value_yx(void *pvParameters);

// function that get the final servo values : Part2
void ICACHE_FLASH_ATTR get_servo_values(void);

// function that get the final servo values : Part1
void ICACHE_FLASH_ATTR get_flex_values(void);

// an example of servo application (to be deleted)
void ICACHE_FLASH_ATTR turn_servo(void);
