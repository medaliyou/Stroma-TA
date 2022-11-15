# QA !

#### 1. What are the main features of Real-Time Operating Systems and how do they differ from regular OS’s?
 The difference between an OS (Operating System) and an RTOS (Real Time Operating System) is:
 -  Response Time
    OS’s typically provide a non-deterministic, soft real time response, where there are no guarantees as to when each task will complete.
    RTOS provides a hard real time response, with a fast, highly deterministic reaction to external events.
- Scheduling
  RTOS mostly use a preemptive scheduling algorithm while OS uses Round Robin or Co-operative or Hybrid scheduling
 - Tasks and Priorities
  preemptive scheduling assigns a priority value to each task (thread),
  the faster the required response, the higher the priority level assigned.

#### 2. When is it preferable to use STM32 over Arduino, ESP32, PIC or other comparable embedded system?
  This depends on the application ,use case requirementes and product life cycle:
- For example if we are developing a huge application dedicated for industrial usage, 
  then probably it embeds some complicated logic and computations, it should be certified
  and it should met industrial requirements (severe working conditions, temperature, product availiablity, mass production, cost ...)
  for such cases, STM32 MCU are ideal meeting these requiements and in terms of cost .
- If the industrial application is restricted and not complicated, then PIC MCUs can meet requirements.
- If the use case requires connectivity (IIoT, Iot, BLE, WiFi ...) then ESP32 is a perfect match for it as it provides sophisticated SDK and documentation.
- Arduino based MCUs are for learning, PoC and restricted projects.

#### 3. Let's assume that a few of the peripherals you use have the same hardcoded I2C address. 
What solution would you use to solve this problem?
There are few workarounds for such case:
- we can unselect or turn off one/multple devices.
- some devices provides programmable addresses( volatile ), if so , we can assign different addresses each time.
- we can use a HW I2C multiplexer.

#### 4. Let's assume that there is a hardware module that is attached to the motherboard with a mezzanine connector. Due to the small form factor, the number of pins on the mezzanine connector is much less than the number of pins you need.  Assuming that the communication speed is not important, how would you solve this problem?
  - Reduce the number of data pins because speed is not important.

#### 5. Let's say you want about 30 hardware modules to communicate with each other.
  There is one STM32 microcontroller on each module, and the modules can be removed and installed instantly.
  Which communication standard would you use for these modules to communicate effectively with each other? Why?
- CAN communication standard can be used in this case. 
  Because CAN can support multiple devices and Flexible for dynamic removal / installation of devices.

#### 6. 3 devices with identical embedded software and hardware are required to communicate over I2C.
 What kind of solution would you develop so that devices can be dynamically addressed and recognize each other?
 - Well the question is not so clear for me (the part of same software)
 - We can use multiple I2C IP (I2C1, I2C2, I2C3) if supported.
 - Or we can add and use an I2C multiplexer.
 - We can add addition pin for every slave device 
  (SlaveSelect1 : Master -> Slave1,SlaveSelect2 : Master -> Slave2) and choose which device we are selecting at a time.
 - Also We can create a protocol upon the I2C communication where we can pre-assign addresses on the slave,  The master will send normal I2C messages but add a Header before the data, containing the preassigned device address,  that way only concerned I2C devices will respond (on the slave side ignore all I2C messages that don't have the same slave address )
