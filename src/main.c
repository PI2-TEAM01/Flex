#include "esp_common.h"
#include "gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uart.h"
#include "i2c_master.h"

//size of the array that contain S0 S1 S2
#define arraySize 3
//number of flex sensor
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
const static uint SxPinMux[3] = {PERIPHS_IO_MUX_GPIO5_U,PERIPHS_IO_MUX_GPIO0_U,PERIPHS_IO_MUX_GPIO4_U};
const static char* SxPinFunction[3] = {PIN_GPIO_FUNC5,PIN_GPIO_FUNC0,PIN_GPIO_FUNC4};
int *FlexValueArray = NULL;

//all the code in order for the multiplexer (that correspond to the code for S0 S1 S2)
const static int Code_y0 [3] = {0,0,0};
const static int Code_y1 [3] = {0,0,1};
const static int Code_y2 [3] = {0,1,0};
const static int Code_y3 [3] = {0,1,1};
const static int Code_y4 [3] = {1,0,0};

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

void ICACHE_FLASH_ATTR displayValues(void){

  printf("Y0\tY1\tY2\tY3\tY4\tY5\tY6\tY7");
  printf("\n");
  printf("---\t---\t---\t---\t---\t---\t---\t---");
  printf("\n");

  for(int i = 0; i < flexNumber; i++){
    printf("%d\t",FlexValueArray[i]);
  }
  printf("\n");
  vTaskDelay(5000);

  }

//according to the value of pvParameters (4th parametr in xTaskCreate()), get the correct the value on the correct pin
void ICACHE_FLASH_ATTR value_yx(void *pvParameters){

  int *tab = NULL;
  switch((int)pvParameters){

    case 0 : tab = (int*)Code_y0;
    break;
    case 1 : tab = (int*)Code_y1;
    break;
    case 2 : tab = (int*)Code_y2;
    break;
    case 3:  tab = (int*)Code_y3;
    break;
    case 4:  tab = (int*)Code_y4;
    break;
    default:
    break;
  }

  //modify S0 -> S2
  GPIO_OUTPUT_SET(SxPinArray[0], tab[0]);
  GPIO_OUTPUT_SET(SxPinArray[1], tab[1]);
  GPIO_OUTPUT_SET(SxPinArray[2], tab[2]);

  //get the analogic value for the correpsonding output
  FlexValueArray[(int)pvParameters] = system_adc_read();
  printf("analog value : %d", FlexValueArray[(int)pvParameters]);
}

void ICACHE_FLASH_ATTR user_init(void)
{

  //flex array declaration
  FlexValueArray = (int*)malloc(flexNumber*sizeof(int));

//definition
  PIN_FUNC_SELECT(PIN_GPIO_MUX5, PIN_GPIO_FUNC5);
  PIN_FUNC_SELECT(PIN_GPIO_MUX0, PIN_GPIO_FUNC0);
  PIN_FUNC_SELECT(PIN_GPIO_MUX4, PIN_GPIO_FUNC4);


//create all the tasks for each
  xTaskCreate(&value_yx, (signed char*)"y0", 256, (void*) 0, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y1", 256, (void*) 1, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y2", 256, (void*) 2, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y3", 256, (void*) 3, 2, NULL);
  xTaskCreate(&value_yx, (signed char*)"y4", 256, (void*) 4, 2, NULL);
//xTaskCreate(&displayValues, (signed char*)"display",256,NULL,6,NULL);


//delay de 500ms
//  xTaskCreate(&displayValues ,(signed char*)"display" ,256 , NULL ,2 ,NULL);

}
