# I2C (TWI)
This driver can be used to communicate via TWI between microcontrollers, compatible with I2C.

## Pins

```
Board	              I2C / TWI pins
Uno, Ethernet	      A4 (SDA), A5 (SCL)
Mega2560	      20 (SDA), 21 (SCL)
Leonardo	      2 (SDA), 3 (SCL)
Due	              20 (SDA), 21 (SCL), SDA1, SCL1
```

## API

### Includes

```ceu
#include "i2c.ceu"
```

### Code Abstractions

#### I2C_Start

Start I2C as master (without address) or slave (with address)

```ceu
code/call I2C_Start (var u8? address) -> none
```

Parameters:

- `u8? address`: I2C slave address

#### I2C_Stop

Stop I2C transmit
```ceu
code/call I2C_Stop () -> none
```

#### I2C_Read

Read specific length data form slave (specified by address
) or master

```ceu
code/await I2C_Read  (var&[] byte buf, var u8? address, var usize? len) -> none
```
Parameters:

- `byte[] buf` : buffer of data
- `u8? address`: I2C slave address
- `usize? len` : data length wish to read, will read data length of `len` or less


#### I2C_Write

Write data in `buf` to slave (specified by address
) or master
```ceu
code/await I2C_Write (var&[] byte buf, var u8? address) -> none;
```
Parameters:

- `byte[] buf` : buffer of data
- `u8? address`: I2C slave address

#### I2C_Set_Clock

Set twi bit rate
```ceu
code/call I2C_Set_Clock (var u32 clock) -> none;
```
Parameters:

- `u32 clock` : Clock Frequency
