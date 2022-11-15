# Implement the given project successfully on PICSimLab !

##### In this folder, I have attached my attempt to create the firmware for the pressure and temperature measuring device for mountaineers.
Unfortunately, I have no prior knowledge with PicSimLAB, nor enough time to experement and dig into how it works especially the `qemu`  virtualization part.
In the project, I enabled `HSE oscillator` and enabled `I2C1` (also enabled `I2C2` later for testing), `USART1` for printing logging messages. 
I have added the `BMP280` as an input device (Modules ->  Spare parts) and connected it to `PB7`, `PB6`.

Also I have integrated two implementations of the `BMP280` sensor from [here](https://github.com/avislab/STM32F103/tree/master/Example_BMP280) and [here](https://github.com/ciastkolog/BMP280_STM32).
Sadly, the I2C **SDA** and **SCL** pins were always at **low level** and I couldn't pull them up, therefore, I couldn't get any response from the BMP280 sensor.
For that reason I didn't continue developement.
the `main.c` file is really dirty.
