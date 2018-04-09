<center> <h1>STEPS OF THE PROJECT :</h1> </center>

## STEP 1 :
Get all the analog values of the flex sensors using the 74HC4051 multiplexer

## STEP 2 :
 Set the communication between the 2 esp8266 (client-server) with the glove as the server and the 3d printed hand as the client :

## STEP 3 :
Send the values from the server to the client

<center>
 <h1>THIS IS STEP 1 ONLY</h1>
 <img src="https://media.giphy.com/media/B0uJ6d5OXb50k/giphy.gif" width=10% title="Github Logo">
 </center>



#### Components :
* used :
  * ESP8266 Thing Dev (x1)
  * Flex Sensors (x5)
  * 47 kΩ resistors (x5)
  * 74HC4051 multiplexer (x1)
  * Wires


* not used :
  * MPU6050 6 DOF IMU (x2) :  
Because we encountered some problems :
the I2C bus on the ESP8266 is software not hardware, it means that the driver for I2C  will  be responsible of the delay regulations between the ESP8266 and the I2C sensors. (to regulate the communication between the two). We have Tested the I2C driver APPROVED by Espressif (because there is no official I2C driver from Espressif) and other drivers made by internet programers


* to do :
 * Change the ESP8266 to the ESP32
 * Change all the resistors
 * Add OP Amp(s)

#### Schematic :

<p align=center>
<img src="img/fritzing.png" title="Github Logo">
</p>



#### Files Organization :

* flex final
* img
  * fritzing.png
* lib
  * flex
    * flex.c
    * flex.h
* src
    * main.c
* .gitignore
* .travis.yml
* platformio.ini



#### NOTE :

* For pin declaration : You can find all the Pin mux register and functions in the file :

 ```
/home/$Your_Personal_Folder/.platformio/packages/framework-esp8266-rtos-sdk/include/espressif/esp8266/pin_mux_register.h
 ```

* If you used ESPCONN objects and that you uploaded a new piece of code to the ESP8266 (that don't own ESPCONN objects at all), you will see that informations about network are still printed in the serial monitor ...
So in order to delete these parameters you need to execute in the user_init() the single line :

 ```
void user_init(void) {
   system_restore();
  }
  ```
  It will restore the configuration of the ESP including the ESPCONN objects.
* If you have UPLOAD ERROR(S) :
  * connect the Pin 0 of the ESP8266 to the Gnd
  * Turn off
  * Turn on
  * retry (it should work now)
