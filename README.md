#STEPS OF THE PROJECT :

##STEP 1 :
GET ALL THE ANALOG VALUES OF THE FLEX SENSORS USING THE 74HC4051 MULTIPLEXER

##STEP 2 :
 SET THE COMMUNICATION BETWEEN THE 2 ESP8266 (CLIENT-SERVER) WITH THE GLOVE AS THE SERVER AND THE 3D PRINTED HAND AS THE CLIENT :

##STEP 3 :
SEND THE VALUES FROM THE SERVER TO THE CLIENT

#THIS IS STEP 1 ONLY

####Components :

* ESP8266 Thing Dev (x1)
* Flex Sensors (x5)
* 74HC4051 multiplexer (x1)
* Wires

* Initially used :

####Schematic :

![Fritzing](img/fritzing.png "Schematic")


####files organization :

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




####NOTE :

- For pin declaration :

You can find all the Pin mux register and functions in the file
$PlatformIO_Folder/framework-esp8266-rtos-sdk/include/espressif/esp8266/pin_mux_register.h

- 
