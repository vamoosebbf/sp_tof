<img src="img/icon_sipeed2.png" style="zoom: 80%;" />

# SP_TOF 模块使用说明
    
[English](README.md)

## 1. 目录结构

|  目录  | 描述           |
| :----: | :------------- |
|  doc   | 参考文档       |
|  img   | 图片           |
| script | MaixPy脚本示例 |
|  src   | C裸机程序示例  |

## 2. 介绍

<img src="img/sp_tof.png" style="padding-right:100px;" align="right" width="" height="500" />

本模块所采用的 VL53L0X 传感器是一个 I2C 接口，长距离单点飞行时间测定（ToF）传感器，它具有高性能和高可靠性, 最远距离4m, 最高50hz的刷新率, 搭配红色激光瞄头, 通过 XSHUT 控制激光开光, 使用 SP_MOD 进行连接.

*查看[模块规格书](doc/SP-ToF(1P)规格书V1.0.pdf)获取更多特性信息*

## 3. 引脚图

<img src="img/back.png" height="300" />

## 4. 接线方式

<img src="img/connection.png" height="250">

|  MCU:FUN(IO)  | SP_RFID |
| :-----------: | :-----: |
| I2C:SDA(IO_7) |   SDA   |
|   NC(IO_15)   |   NC    |
| GPIOHS(IO_20) |   IRQ   |
|   NC(IO_21)   |   NC    |
| GPIOHS(IO_8)  |   SHT   |
| I2C:SCL(IO_6) |   SCL   |
|   2.8~3.5V    |  3.3V   |
|      GND      |   GND   |

## 5. MCU 配置

### 5.1. IO 配置

将 MCU 原理图对应的 IO 口配置为 SPI 功能引脚。

* C 示例
  
  此示例使用的是软件模拟 SPI，所以在此需将设置对应引脚为 GPIOHS 功能而不是 SPI 功能，具体实现请查看完整代码。
  ```c
  fpioa_set_function(RFID_CS_PIN, FUNC_GPIOHS0 + RFID_CS_HSNUM); // RFID_CS_PIN: 20;
  fpioa_set_function(RFID_CK_PIN, FUNC_GPIOHS0 + RFID_CK_HSNUM); // RFID_CK_PIN: 21;
  fpioa_set_function(RFID_MO_PIN, FUNC_GPIOHS0 + RFID_MO_HSNUM); // RFID_MO_PIN: 8;
  fpioa_set_function(RFID_MI_PIN, FUNC_GPIOHS0 + RFID_MI_HSNUM); // RFID_MI_PIN: 15;

  gpiohs_set_drive_mode(spi_io_cfg.hs_cs, GPIO_DM_OUTPUT);
  gpiohs_set_drive_mode(spi_io_cfg.hs_clk, GPIO_DM_OUTPUT);
  gpiohs_set_drive_mode(spi_io_cfg.hs_mosi, GPIO_DM_OUTPUT);
  gpiohs_set_drive_mode(spi_io_cfg.hs_miso, GPIO_DM_INPUT);
  ```

* MaixPy 示例
  ```python
  # 20: CS_NUM;
  fm.register(20, fm.fpioa.GPIOHS20, force=True)
  # set gpiohs work mode to output mode
  cs = GPIO(GPIO.GPIOHS20, GPIO.OUT)
  ```

### 5.2. SPI 初始化

* C 示例
  
  软件 SPI 只需要配置对应引脚，并没有 SPI 的初始化。

* MaixPy 示例
  ```python
  # RFID_SCK: 21; RFID_SI:8; RFID_SO: 15;
  spi1 = SPI(SPI.SPI1, mode=SPI.MODE_MASTER, baudrate=600 * 1000,
          polarity=0, phase=0, bits=8, firstbit=SPI.MSB, sck=21, mosi=8, miso=15)
  ```

## 6. 使用方式

* 流程
  1. 初始化
  2. 扫描并绑定卡片
  3. 读写数据

* C 示例
  ```c
  // detected card
  PcdRequest(0x52, type)

  // auth and bind...

  // read or write 16 bytes data from sector 0x11
  PcdWrite(0x11, w_buf)
  PcdRead(0x11, &r_buf)
  ```
  
* MaixPy 示例
  ```python
  # Create an object of the class MFRC522
  MIFAREReader = MFRC522(spi1, cs)
  
  # detected and auth, bind...
  
  # read or write 16 bytes data from sector 0x11
  MIFAREReader.MFRC522_Write(0x11, data)
  MIFAREReader.MFRC522_Read(0x11)
  ```

## 7. 运行环境

|  语言  | 开发板   | SDK/固件版本                   |
| :----: | :------- | :----------------------------- |
|   C    | MaixCube | kendryte-standalone-sdk v0.5.6 |
| MaixPy | MaixCube | maixpy v0.5.1                  |

## 8. 运行结果

* C

  <img src="img/c_log.png" height="200" />

* MaixPy

  <img src="img/maixpy_log.png" height="200" />

## 10. 许可

请查看 [LICENSE](LICENSE.md) 文件.

## 11. 相关信息

| 版本号 |   编辑人   |
| :----: | :--------: |
|  v0.1  | vamoosebbf |
