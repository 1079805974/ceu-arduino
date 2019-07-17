# I2C Library Test

This folder contains the I2C driver examples, and you can learn the basic usage of the driver.

## Hardware Requirements

Two Arduinos needed, connected as follows.

- A4 -> A4
- A5 -> A5
- GND -> GND

## Description

1. master_send.ceu  
Send the data in `buf` every second to the Arduino with I2C slave address 8.

2. master_recieve.ceu  
Every 1 second, requests 1 byte data from Arduino with I2C slave address 8 and print it to the serial port.

3. slave_recieve.ceu  
Declare that the I2C slave address is 8. When any data is received, the data is printed to the serial port.

4. slave_send.ceu  
Declare that the I2C slave address is 8. When the I2C master requests data, it sends the data in `str`.

5. master_recieve_send.ceu  
It has the functions of `master_recieve` and `master_send` and uses the `usart` module besides.

6. master_recieve_send.ceu  
It has the functions `slave_recieve` and `slave_send` and uses the `usart` module besides.