<img src="img/icon_sipeed2.png" style="zoom:80%;" />

# SP_TOF Module User Guide

[中文](README_CN.md)

## Directory Structure

| Directory | Description                                   |
| :-------: | :-------------------------------------------- |
|    doc    | Reference documentation                       |
|    img    | Images                                        |
|  script   | Maixpy script example                         |
|    src    | C program example based on the standalone sdk |

## Introduce

<img src="img/sp_tof.png" style="padding-right:100px;" align="right" width="500"/>

The VL53L0X sensor used in this module is an I2C interface and a long distance single point flight time measurement (ToF) sensor. It has high performance and high reliability, with the longest distance of 4m and the highest refresh rate of 50Hz. With the red laser pointer, the laser is activated through XSHUT and connected by SP_MOD.

*See [模块规格书](doc/SP-ToF(1P)规格书V1.0.pdf) for more information.*

## Pin figure

<img src="img/back.png" height="300" />

## Mode of connection

<img src="img/connection.png" height="250">

|  MCU:FUN(IO)  | SP_RFID |
| :-----------: | :-----: |
| I2C:SDA(IO_7) |   SDA   |
|   NC(IO_15)   |   NC    |
| NC(IO_20)     |   IRQ   |
|   NC(IO_21)   |   NC    |
| GPIOHS(IO_8)  |   SHT   |
| I2C:SCL(IO_6) |   SCL   |
|   2.8~3.5V    |  3.3V   |
|      GND      |   GND   |

## MCU configuration

### IO Port configuration

Configure the IO port corresponding to the schematic diagram as I2C function

* C

  ```c
    //set io mux
    fpioa_set_function(VL53L0X_SCL, FUNC_I2C0_SCLK + VL53L0X_I2C_DEVICE * 2);
    fpioa_set_function(VL53L0X_SDA, FUNC_I2C0_SDA + VL53L0X_I2C_DEVICE * 2);
    fpioa_set_function(VL53L0X_SHT, FUNC_GPIOHS0 + VL53L0X_SHT);

    gpiohs_set_drive_mode(VL53L0X_SHT, GPIO_DM_OUTPUT);
  ```

* MaixPy

  ```python
    fm.register(VL53L0X_SHT, fm.fpioa.GPIOHS0, force=True)
    XSHUT = GPIO(GPIO.GPIOHS0, GPIO.OUT)  
  ```

### I2C initalization

* C

  ```c
   //i2c init
    maix_i2c_init(VL53L0X_I2C_DEVICE, 7, VL53L0X_I2C_FREQ_KHZ * 1000);
  ```

* MaixPy

  ```python
    i2c = I2C(VL53L0X_I2C_NUM, freq=VL53L0X_FREQ, scl=VL53L0X_SCL, sda=VL53L0X_SDA)
  ```

## Method of application

* Process

  1. Initializatin
  2. Adjust(option)
  3. Get distance

* C

  ```c
    while (vl53l0x_init(&vl53l0x_dev)) {
          printf("VL53L0X init error!!!\r\n");
          msleep(500);
    }

    printf("VL53L0X init success!\r\n");

    // adjusting
    printf("VL53L0X adjusting\r\n");
    vl53l0x_calibration_test(&vl53l0x_dev);

    // get distance
    printf("VL53L0X start work\r\n");
    vl53l0x_general_test(&vl53l0x_dev);
  ```
  
* MaixPy

  ```python
    # create obj and read distance
	  tof = VL53L0X(i2c)
	  while True:
      mm = tof.read()
      utime.sleep_ms(100)
      print(mm)
  ```

## Runtime enviroments

| Language |  Boards  |      SDK/firmware version      |
| :------: | :------: | :----------------------------: |
|    C     | MaixCube | kendryte-standalone-sdk v0.5.6 |
|  MaixPy  | MaixCube |         maixpy v0.5.1          |

## Result

* C

  <img src="img/log_c.png" height="200" />

* MaixPy

  <img src="img/.png"  height="200" />

## Transplant

Modify the following parameters to fit other K210 boards.

* C

```c
  // board_config.h
  #define VL53L0X_I2C_DEVICE 0 // i2c device number
  #define VL53L0X_I2C_FREQ_KHZ 100 // i2c frequence
  #define VL53L0X_SCL 6 // scl
  #define VL53L0X_SDA 7 // sda
  #define VL53L0X_SHT 8 // sht
```

* MaixPy

```python
  ################### config ###################
  VL53L0X_I2C_NUM = const(I2C.I2C0)
  VL53L0X_FREQ = const(100000)
  VL53L0X_SCL = const(6)
  VL53L0X_SDA = const(7)
  VL53L0X_SHT = const(8)
  ##############################################
```

## LICENSE

See [LICENSE](LICENSE.md) file

## Othre information

| Version |   Editor   |
| :-----: | :--------: |
|  v0.1   | vamoosebbf |