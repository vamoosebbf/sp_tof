
## Instructions

Modification of board_config.h can be adapted to other models

```c
    #define VL53L0X_I2C_DEVICE 0 // i2c device number
    #define VL53L0X_I2C_FREQ_KHZ 100 // i2c frequence
    #define VL53L0X_SCL 6 // scl
    #define VL53L0X_SDA 7 // sda
    #define VL53L0X_SHT 8 // sht
```

## Log

```shell
    Core 0 Hello world
    USE_I2C_2V8 set
    Set I2C standard mode start status: 0 
    VL53L0X_DataInit status: 0
    function: VL53L0X_get_info_from_device, line: 239, status: 0
    VL53L0X_get_info_from_device status: 0 
    VL53L0X_check_part_used status: 0
    VL53L0X_GetDeviceInfo status: 0
    VL53L0X init success!
    VL53L0X adjusting
    need a white target,and the distance keep 100mm.
    Start calibration...
    function: VL53L0X_get_info_from_device, line: 239, status: 0
    The SPADS Calibration Start...
    refSpadCount = 9
    isApertureSpads = 1
    The SPADS Calibration Finish...

    The Ref Calibration Start...
    VhvSettings = 28
    PhaseCal = 1
    The Ref Calibration Finish...

    Offset Calibration:need a white target,in black space,and the distance keep 100mm!
    The Offset Calibration Start...
    CalDistanceMilliMeter = 6553600 mm
    OffsetMicroMeter = 26000 mm
    The Offset Calibration Finish...

    Cross Talk Calibration:need a grey target
    The Cross Talk Calibration Start...
    XTalkCalDistance = 100 mm
    XTalkCompensationRateMegaCps = 0
    The Cross Talk Calibration Finish...

    All the Calibration has Finished!
    Calibration is successful!!
    Calibration is complete!
    VL53L0X start work
    USE_I2C_2V8 set
    Set I2C standard mode start status: 0 
    function: VL53L0X_get_info_from_device, line: 239, status: 0
    mode : Default 
    distance:  125mm
    distance:  124mm
    distance:  125mm
    distance:  125mm
    distance:  122mm
    distance:  124mm
```